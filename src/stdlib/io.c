// src/stdlib/io.c - I/O module implementation
//
// Provides basic input/output operations:
// - io.println: Print with newline
// - io.print: Print without newline

#define _POSIX_C_SOURCE 200809L

#include "io.h"
#include "runtime/module.h"
#include "core/value.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Helper: Process format string with {} placeholders
static void print_formatted(const char *format, int arg_count, Value *args) {
  int arg_index = 1;  // Skip format string itself
  
  for (const char *p = format; *p; p++) {
    if (*p == '{' && *(p + 1) == '}') {
      // Found placeholder
      if (arg_index < arg_count) {
        value_print(args[arg_index]);
        arg_index++;
      }
      p++;  // Skip the '}'
    } else {
      putchar(*p);
    }
  }
}

// io.println - Print with newline
// Supports:
//   io.println "text"
//   io.println "x={}", value
//   io.println "x={}, y={}", x, y
Value native_io_println(int arg_count, Value *args) {
  if (arg_count == 0) {
    printf("\n");
    return value_make_nil();
  }
  
  // First argument should be a string (format or plain text)
  if (!IS_STRING(args[0])) {
    // Fallback: just print the value
    value_print(args[0]);
    printf("\n");
    return value_make_nil();
  }
  
  const char *format = AS_STRING(args[0]);
  
  if (arg_count == 1) {
    // No placeholders, just print the string
    printf("%s\n", format);
  } else {
    // Format string with arguments
    print_formatted(format, arg_count, args);
    printf("\n");
  }
  
  return value_make_nil();
}

// io.print - Print without newline
Value native_io_print(int arg_count, Value *args) {
  if (arg_count == 0) {
    return value_make_nil();
  }
  
  if (!IS_STRING(args[0])) {
    value_print(args[0]);
    return value_make_nil();
  }
  
  const char *format = AS_STRING(args[0]);
  
  if (arg_count == 1) {
    printf("%s", format);
  } else {
    print_formatted(format, arg_count, args);
  }
  
  return value_make_nil();
}

// Module initialization - registers all io functions
void io_module_init(VM *vm) {
  module_register_native(vm, "io.println", native_io_println);
  module_register_native(vm, "io.print", native_io_print);
}
