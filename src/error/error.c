// src/error.c - Professional diagnostic emitter
//
// Rust/Clang-style: error label, source line with ^^^ underline,
// annotated notes and help. Color-aware for TTY, auto-strips
// when piping. All diagnostics go through diag_emit().

#include "error/error.h"
#include "core/source.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// ── TTY detection for color ─────────────────────────────────────────

static bool use_color(void) {
  static int cached = -1;
  if (cached < 0) {
    cached = isatty(STDERR_FILENO) ? 1 : 0;
  }
  return cached == 1;
}

// ── Color codes ─────────────────────────────────────────────────────

#define ANSI_RED     "\033[1;31m"
#define ANSI_GREEN   "\033[32m"
#define ANSI_YELLOW  "\033[1;33m"
#define ANSI_BLUE    "\033[34m"
#define ANSI_CYAN    "\033[36m"
#define ANSI_BOLD    "\033[1m"
#define ANSI_DIM     "\033[90m"
#define ANSI_RESET   "\033[0m"

// Conditionally wrap text in ANSI if color is enabled
static const char *c(const char *code) {
  return use_color() ? code : "";
}

// ── Level label ─────────────────────────────────────────────────────

static const char *level_label(ErrorLevel level) {
  switch (level) {
    case LEVEL_ERROR:   return "error";
    case LEVEL_WARNING: return "warning";
    case LEVEL_NOTE:    return "note";
    case LEVEL_HELP:    return "help";
  }
  return "?";
}

static const char *level_color(ErrorLevel level) {
  switch (level) {
    case LEVEL_ERROR:   return ANSI_RED;
    case LEVEL_WARNING: return ANSI_YELLOW;
    case LEVEL_NOTE:    return ANSI_BLUE;
    case LEVEL_HELP:    return ANSI_CYAN;
  }
  return "";
}

// ── Primary emitter ─────────────────────────────────────────────────

void diag_emit(ErrorLevel level,
               const char *file,
               int line, int column, int span_length,
               const char *message,
               const SourceInfo *source,
               const char **notes, int note_count) {
  // ── Header: "error: message" or "error[FILE]:LINE:COL: message" ──
  // Format:
  //   error: message                     (no location)
  //   error: file:line:col: message      (with location, no source)
  //   error: message                     (with source — line header follows)
  //     --> file:line:col

  bool have_location = (file != NULL && line > 0 && column > -1);
  bool have_source  = (source != NULL && have_location);

  if (have_source) {
    // Two-line header:
    //   error: message
    //    --> file.sat:line:col
    fprintf(stderr, "%s%s:%s %s%s%s",
            c(level_color(level)), level_label(level), c(ANSI_RESET),
            c(ANSI_BOLD), message, c(ANSI_RESET));
    fprintf(stderr, "\n%*s%s-->%s %s:%d:%d\n",
            4, "",
            c(ANSI_DIM), c(ANSI_RESET),
            file, line, column);
  } else if (have_location) {
    // Single-line: error: file:line:col: message
    fprintf(stderr, "%s%s:%s %s%s:%d:%d:%s %s%s%s\n",
            c(level_color(level)), level_label(level), c(ANSI_RESET),
            c(ANSI_DIM), file, line, column, c(ANSI_RESET),
            c(ANSI_BOLD), message, c(ANSI_RESET));
    return;
  } else {
    // No location: error: message
    fprintf(stderr, "%s%s:%s %s%s%s\n",
            c(level_color(level)), level_label(level), c(ANSI_RESET),
            c(ANSI_BOLD), message, c(ANSI_RESET));
    return;
  }

  // ── Source line display ───────────────────────────────────────────
  //    |
  //  3 | let x :=
  //    |         ^ expected expression
  //    |

  // Blank gutter line (no line number) — just spaces + "|"
  char blank_gutter[64];
  snprintf(blank_gutter, sizeof(blank_gutter), " %*s ", 4, "");

  // Line above the source (blank gutter + separator)
  //     |
  fprintf(stderr, "%s%s|%s\n",
          blank_gutter, c(ANSI_DIM), c(ANSI_RESET));

  // The source line itself
  //   3 | let x :=
  int line_len = 0;
  const char *line_text = source_get_line((SourceInfo *)source, line, &line_len);

  // Print line number, separator, and line text
  fprintf(stderr, "%s%4d %s|%s", c(ANSI_DIM), line, c(ANSI_DIM), c(ANSI_RESET));
  if (line_text && line_len > 0) {
    fprintf(stderr, " %.*s", line_len, line_text);
  }
  fprintf(stderr, "\n");

  // Span underline
  //     |         ^^^^^^^^^^^^^^^^^^^
  // (indent to column, then print ^^^ for span_length)
  fprintf(stderr, "%s%s|%s ", blank_gutter, c(ANSI_DIM), c(ANSI_RESET));
  if (have_source && column > 0) {
    int indent = column - 1;
    for (int i = 0; i < indent; i++) {
      fputc(' ', stderr);
    }
    int len = (span_length > 0) ? span_length : 1;
    fprintf(stderr, "%s", c(level_color(level)));
    for (int i = 0; i < len; i++) {
      fputc('^', stderr);
    }
    fprintf(stderr, "%s %s", c(ANSI_RESET), message);
  }
  fprintf(stderr, "\n");

  // Blank line for spacing
  fprintf(stderr, "%s%s|%s\n",
          blank_gutter, c(ANSI_DIM), c(ANSI_RESET));

  // ── Secondary labels (note/help) ──────────────────────────────────
  //     = note: this is additional context
  //     = help: try this instead
  if (notes && note_count > 0) {
    for (int i = 0; i < note_count; i++) {
      // Determine level from prefix
      const char *content = notes[i];
      const char *label = "note";
      const char *color = ANSI_BLUE;
      if (strncmp(notes[i], "help: ", 6) == 0) {
        label = "help";
        color = ANSI_CYAN;
        content = notes[i] + 6;
      } else if (strncmp(notes[i], "note: ", 6) == 0) {
        label = "note";
        color = ANSI_BLUE;
        content = notes[i] + 6;
      }

      fprintf(stderr, "%s%s=%s %s%s:%s %s\n",
              blank_gutter,
              c(ANSI_DIM), c(ANSI_RESET),
              c(color), label, c(ANSI_RESET),
              content);
    }
  }
}

// ── Convenience wrappers ────────────────────────────────────────────

void error_report(const char *file, int line, int column,
                  const char *format, ...) {
  va_list args;
  va_start(args, format);
  char buf[4096];
  vsnprintf(buf, sizeof(buf), format, args);
  va_end(args);
  diag_emit(LEVEL_ERROR, file, line, column, 0, buf, NULL, NULL, 0);
}

void error_report_simple(const char *format, ...) {
  va_list args;
  va_start(args, format);
  char buf[4096];
  vsnprintf(buf, sizeof(buf), format, args);
  va_end(args);
  diag_emit(LEVEL_ERROR, NULL, 0, 0, 0, buf, NULL, NULL, 0);
}

void error_fatal(const char *file, int line,
                 const char *format, ...) {
  va_list args;
  va_start(args, format);
  char buf[4096];
  vsnprintf(buf, sizeof(buf), format, args);
  va_end(args);
  diag_emit(LEVEL_ERROR, file, line, 0, 0, buf, NULL, NULL, 0);
  exit(1);
}

void warning_report(const char *file, int line, int column,
                    const char *format, ...) {
  va_list args;
  va_start(args, format);
  char buf[4096];
  vsnprintf(buf, sizeof(buf), format, args);
  va_end(args);
  diag_emit(LEVEL_WARNING, file, line, column, 0, buf, NULL, NULL, 0);
}

void note_label(const char *file, int line, int column,
                const char *format, ...) {
  va_list args;
  va_start(args, format);
  char buf[4096];
  vsnprintf(buf, sizeof(buf), format, args);
  va_end(args);
  diag_emit(LEVEL_NOTE, file, line, column, 0, buf, NULL, NULL, 0);
}

void help_label(const char *format, ...) {
  va_list args;
  va_start(args, format);
  char buf[4096];
  vsnprintf(buf, sizeof(buf), format, args);
  va_end(args);
  diag_emit(LEVEL_HELP, NULL, 0, 0, 0, buf, NULL, NULL, 0);
}
