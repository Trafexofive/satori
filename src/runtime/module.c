// src/runtime/module.c - Module system implementation

#define _POSIX_C_SOURCE 200809L

#include "module.h"
#include "vm.h"
#include "core/table.h"
#include <stdio.h>
#include <string.h>

// Registry of all built-in modules
static ModuleDescriptor builtin_modules[] = {
  {"io", io_module_init},
  {"string", string_module_init},
  {NULL, NULL}  // Sentinel
};

void module_system_init(VM *vm) {
  table_init(&vm->globals);
  table_init(&vm->loaded_modules);
}

void module_system_free(VM *vm) {
  table_free(&vm->globals);
  table_free(&vm->loaded_modules);
}

bool module_load(VM *vm, const char *name) {
  // Check if already loaded
  Value dummy;
  if (table_get(&vm->loaded_modules, name, &dummy)) {
    return true;  // Already loaded, success
  }
  
  // Find module in registry
  for (int i = 0; builtin_modules[i].name != NULL; i++) {
    if (strcmp(builtin_modules[i].name, name) == 0) {
      // Initialize module
      builtin_modules[i].init(vm);
      
      // Mark as loaded
      table_set(&vm->loaded_modules, name, value_make_bool(true));
      
      return true;
    }
  }
  
  // Module not found
  fprintf(stderr, "Error: Unknown module '%s'\n", name);
  return false;
}

void module_register_native(VM *vm, const char *name, NativeFn function) {
  Value fn_value = value_make_native_fn(function);
  table_set(&vm->globals, name, fn_value);
}
