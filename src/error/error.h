// src/error.h

#ifndef SATORI_ERROR_H
#define SATORI_ERROR_H

// Report error with file location
void error_report(const char *file, int line, int column, const char *format,
                  ...);

// Report error without location
void error_report_simple(const char *format, ...);

// Fatal error - exits program
void error_fatal(const char *format, ...);

// Warning with location
void warning_report(const char *file, int line, int column, const char *format,
                    ...);

#endif // SATORI_ERROR_H
