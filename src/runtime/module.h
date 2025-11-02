// src/runtime/module.h - Module system interface
//
// Handles module registration, loading, and native function binding.
// Clean separation: modules define what they export, this handles how.

#ifndef SATORI_MODULE_H
#define SATORI_MODULE_H

#include "core/common.h"
#include "core/value.h"
#include "vm.h"

// Module initialization function type
typedef void (*ModuleInitFn)(VM *vm);

// Module descriptor
typedef struct {
  const char *name;     // Module name (e.g., "io", "math")
  ModuleInitFn init;    // Initialization function
} ModuleDescriptor;

// Module system operations
void module_system_init(VM *vm);
void module_system_free(VM *vm);
bool module_load(VM *vm, const char *name);
void module_register_native(VM *vm, const char *name, NativeFn function);

// Built-in module declarations
void io_module_init(VM *vm);
void string_module_init(VM *vm);

#endif // SATORI_MODULE_H
