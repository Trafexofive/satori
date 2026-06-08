// src/error/error.h - Professional diagnostic system
//
// Rust/Clang-style error output with source line display,
// span underlines, note/help labels, and automatic color
// stripping for non-TTY output.
//
// Architecture:
//   All diagnostics funnel through diag_emit(). The old
//   error_report / error_report_simple / error_fatal are
//   reimplemented as thin wrappers. Raw fprintf to stderr
//   for diagnostics is discouraged — use this system.

#ifndef SATORI_ERROR_H
#define SATORI_ERROR_H

#include <stdbool.h>
#include "core/source.h"

// Error level for labeling
typedef enum {
  LEVEL_ERROR,   // Red "error"
  LEVEL_WARNING, // Yellow "warning"
  LEVEL_NOTE,    // Blue "note"
  LEVEL_HELP,    // Cyan "help"
} ErrorLevel;

// ── Primary diagnostic emitter ──────────────────────────────────────
//
//   error[E0001]: expected expression
//     --> foo.sat:3:10
//      |
//    3 | let x :=
//      |         ^ expected expression
//      |
//      = note: a variable declaration needs a value
//      = help: try `let x := 5`
//
// Parameters:
//   level       — ERROR, WARNING, NOTE, or HELP
//   file        — source file path (NULL = omit)
//   line        — 1-based line number (<=0 = omit span)
//   column      — 1-based column number (<=0 = omit column pointer)
//   span_length — length of offending span in characters (0 = just point)
//   message     — primary diagnostic message
//   source      — SourceInfo for line display (NULL = skip line display)
//   notes       — array of secondary label strings, or NULL
//   note_count  — number of notes
//
void diag_emit(ErrorLevel level,
               const char *file,
               int line, int column, int span_length,
               const char *message,
               const SourceInfo *source,
               const char **notes, int note_count);

// ── Convenience wrappers ───────────────────────────────────────────

// Standard error with location (replaces old error_report)
void error_report(const char *file, int line, int column,
                  const char *format, ...);

// Error without location (replaces old error_report_simple)
void error_report_simple(const char *format, ...);

// Fatal error (replaces old error_fatal — still exits)
void error_fatal(const char *file, int line,
                 const char *format, ...);

// Warning with location (replaces old warning_report)
void warning_report(const char *file, int line, int column,
                    const char *format, ...);

// Note label (new — for secondary messages)
void note_label(const char *file, int line, int column,
                const char *format, ...);

// Help label (new — for suggestions)
void help_label(const char *format, ...);

#endif // SATORI_ERROR_H
