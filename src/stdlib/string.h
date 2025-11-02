// src/stdlib/string.h - String module interface

#ifndef SATORI_STDLIB_STRING_H
#define SATORI_STDLIB_STRING_H

#include "core/value.h"
#include "runtime/vm.h"

// Module initialization
void string_module_init(VM *vm);

// Native functions
Value native_string_to_upper(int arg_count, Value *args);
Value native_string_to_lower(int arg_count, Value *args);

#endif // SATORI_STDLIB_STRING_H
