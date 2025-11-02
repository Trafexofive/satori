// src/error.c - Clean error reporting

#include "error/error.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void error_report(const char *file, int line, int column, const char *format,
                  ...) {
  fprintf(stderr, "\033[1;31merror:\033[0m %s:%d:%d: ", file, line, column);

  va_list args;
  va_start(args, format);
  vfprintf(stderr, format, args);
  va_end(args);

  fprintf(stderr, "\n");
}

void error_report_simple(const char *format, ...) {
  fprintf(stderr, "\033[1;31merror:\033[0m ");

  va_list args;
  va_start(args, format);
  vfprintf(stderr, format, args);
  va_end(args);

  fprintf(stderr, "\n");
}

void error_fatal(const char *format, ...) {
  fprintf(stderr, "\033[1;31mfatal error:\033[0m ");

  va_list args;
  va_start(args, format);
  vfprintf(stderr, format, args);
  va_end(args);

  fprintf(stderr, "\n");
  exit(1);
}

void warning_report(const char *file, int line, int column, const char *format,
                    ...) {
  fprintf(stderr, "\033[1;33mwarning:\033[0m %s:%d:%d: ", file, line, column);

  va_list args;
  va_start(args, format);
  vfprintf(stderr, format, args);
  va_end(args);

  fprintf(stderr, "\n");
}
