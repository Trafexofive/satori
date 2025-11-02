// src/stdlib/string.c - String module implementation
//
// Provides string manipulation operations.

#define _POSIX_C_SOURCE 200809L

#include "string.h"
#include "runtime/module.h"
#include "core/value.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// string.to_upper - Convert string to uppercase
Value native_string_to_upper(int arg_count, Value *args) {
  if (arg_count != 1) {
    fprintf(stderr, "Error: to_upper expects 1 argument, got %d\n", arg_count);
    return value_make_nil();
  }
  
  if (!IS_STRING(args[0])) {
    fprintf(stderr, "Error: to_upper expects string argument\n");
    return value_make_nil();
  }
  
  const char *input = AS_STRING(args[0]);
  int len = strlen(input);
  char *result = malloc(len + 1);
  
  for (int i = 0; i < len; i++) {
    result[i] = toupper((unsigned char)input[i]);
  }
  result[len] = '\0';
  
  Value str_val = value_make_string(result);
  free(result);  // value_make_string does strdup
  
  return str_val;
}

// string.to_lower - Convert string to lowercase
Value native_string_to_lower(int arg_count, Value *args) {
  if (arg_count != 1) {
    fprintf(stderr, "Error: to_lower expects 1 argument, got %d\n", arg_count);
    return value_make_nil();
  }
  
  if (!IS_STRING(args[0])) {
    fprintf(stderr, "Error: to_lower expects string argument\n");
    return value_make_nil();
  }
  
  const char *input = AS_STRING(args[0]);
  int len = strlen(input);
  char *result = malloc(len + 1);
  
  for (int i = 0; i < len; i++) {
    result[i] = tolower((unsigned char)input[i]);
  }
  result[len] = '\0';
  
  Value str_val = value_make_string(result);
  free(result);
  
  return str_val;
}

// Module initialization
void string_module_init(VM *vm) {
  module_register_native(vm, "string.to_upper", native_string_to_upper);
  module_register_native(vm, "string.to_lower", native_string_to_lower);
}
