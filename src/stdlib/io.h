// src/stdlib/io.h - I/O module interface

#ifndef SATORI_STDLIB_IO_H
#define SATORI_STDLIB_IO_H

#include "core/value.h"
#include "runtime/vm.h"

// Module initialization
void io_module_init(VM *vm);

// Native functions (exported for potential direct use)
Value native_io_println(int arg_count, Value *args);
Value native_io_print(int arg_count, Value *args);

#endif // SATORI_STDLIB_IO_H
