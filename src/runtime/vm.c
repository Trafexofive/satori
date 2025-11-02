// src/vm.c - The executor

#define _POSIX_C_SOURCE 200809L

#include "runtime/vm.h"
#include "runtime/module.h"
#include "core/value.h"
#include "core/table.h"
#include "error/error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Chunk operations
void chunk_init(Chunk *chunk) {
  chunk->code = NULL;
  chunk->count = 0;
  chunk->capacity = 0;
  chunk->constants = NULL;
  chunk->constant_count = 0;
  chunk->constant_capacity = 0;
}

void chunk_free(Chunk *chunk) {
  free(chunk->code);
  for (int i = 0; i < chunk->constant_count; i++) {
    value_free(chunk->constants[i]);
  }
  free(chunk->constants);
  chunk_init(chunk);
}

void chunk_write(Chunk *chunk, u8 byte) {
  if (chunk->capacity < chunk->count + 1) {
    int old_capacity = chunk->capacity;
    chunk->capacity = old_capacity < 8 ? 8 : old_capacity * 2;
    chunk->code = realloc(chunk->code, chunk->capacity);
  }
  chunk->code[chunk->count] = byte;
  chunk->count++;
}

int chunk_add_constant(Chunk *chunk, Value value) {
  if (chunk->constant_capacity < chunk->constant_count + 1) {
    int old_capacity = chunk->constant_capacity;
    chunk->constant_capacity = old_capacity < 8 ? 8 : old_capacity * 2;
    chunk->constants =
        realloc(chunk->constants, chunk->constant_capacity * sizeof(Value));
  }
  chunk->constants[chunk->constant_count] = value;
  return chunk->constant_count++;
}

// VM operations (value functions now in core/value.c)
void vm_init(VM *vm) {
  chunk_init(&vm->chunk);
  vm->ip = vm->chunk.code;
  vm->stack_top = 0;
  vm->local_count = 0;
  module_system_init(vm);
}

void vm_free(VM *vm) {
  chunk_free(&vm->chunk);
  module_system_free(vm);
}

static void stack_push(VM *vm, Value value) {
  if (vm->stack_top >= SATORI_STACK_MAX) {
    error_fatal("Stack overflow");
  }
  vm->stack[vm->stack_top++] = value;
}

static Value stack_pop(VM *vm) {
  if (vm->stack_top <= 0) {
    error_fatal("Stack underflow");
  }
  return vm->stack[--vm->stack_top];
}

static Value stack_peek(VM *vm, int distance) {
  return vm->stack[vm->stack_top - 1 - distance];
}

// Built-in println function
static Value builtin_println(int arg_count, Value *args) {
  for (int i = 0; i < arg_count; i++) {
    value_print(args[i]);
    if (i < arg_count - 1)
      printf(" ");
  }
  printf("\n");
  return value_make_nil();
}

bool vm_run(VM *vm) {
  vm->ip = vm->chunk.code;

#define READ_BYTE() (*vm->ip++)
#define READ_CONSTANT() (vm->chunk.constants[READ_BYTE()])
#define READ_STRING() AS_STRING(READ_CONSTANT())
#define READ_SHORT() (vm->ip += 2, (u16)((vm->ip[-2] << 8) | vm->ip[-1]))

  for (;;) {
#ifdef SATORI_DEBUG_TRACE_EXECUTION
    printf("Stack: ");
    for (int i = 0; i < vm->stack_top; i++) {
      printf("[ ");
      value_print(vm->stack[i]);
      printf(" ]");
    }
    printf("\n");
#endif

    u8 instruction = READ_BYTE();
    switch (instruction) {
    case OP_CONSTANT: {
      Value constant = READ_CONSTANT();
      stack_push(vm, constant);
      break;
    }

    case OP_POP: {
      stack_pop(vm);
      break;
    }
    
    case OP_SET_LOCAL: {
      u8 slot = READ_BYTE();
      if (slot >= SATORI_MAX_LOCALS) {
        error_fatal("Local variable slot %d out of bounds", slot);
        return false;
      }
      vm->locals[slot] = stack_pop(vm);  // Pop the value from stack
      if (slot >= vm->local_count) {
        vm->local_count = slot + 1;
      }
      break;
    }
    
    case OP_GET_LOCAL: {
      u8 slot = READ_BYTE();
      if (slot >= vm->local_count) {
        error_fatal("Undefined local variable at slot %d", slot);
        return false;
      }
      stack_push(vm, vm->locals[slot]);
      break;
    }
    
    case OP_GET_GLOBAL: {
      const char *name = READ_STRING();
      Value value;
      if (!table_get(&vm->globals, name, &value)) {
        error_fatal("Undefined global '%s'", name);
        return false;
      }
      stack_push(vm, value);
      break;
    }
    
    case OP_CALL_NATIVE: {
      u8 arg_count = READ_BYTE();
      
      // Get the function from the stack
      Value callee = stack_peek(vm, arg_count);
      
      if (!IS_NATIVE_FN(callee)) {
        error_fatal("Can only call native functions");
        return false;
      }
      
      // Prepare arguments (they're already on the stack)
      Value *args = &vm->stack[vm->stack_top - arg_count];
      
      // Call the native function
      NativeFn native = AS_NATIVE_FN(callee);
      Value result = native(arg_count, args);
      
      // Pop arguments and function from stack
      vm->stack_top -= arg_count + 1;
      
      // Push result
      stack_push(vm, result);
      break;
    }

    case OP_IMPORT: {
      const char *module_name = READ_STRING();
      if (!module_load(vm, module_name)) {
        error_fatal("Failed to load module '%s'", module_name);
        return false;
      }
      break;
    }
    
    // Arithmetic operations
    case OP_ADD: {
      Value b = stack_pop(vm);
      Value a = stack_pop(vm);
      if (IS_INT(a) && IS_INT(b)) {
        stack_push(vm, value_make_int(AS_INT(a) + AS_INT(b)));
      } else {
        f64 a_val = IS_INT(a) ? (f64)AS_INT(a) : AS_FLOAT(a);
        f64 b_val = IS_INT(b) ? (f64)AS_INT(b) : AS_FLOAT(b);
        stack_push(vm, value_make_float(a_val + b_val));
      }
      break;
    }
    
    case OP_SUBTRACT: {
      Value b = stack_pop(vm);
      Value a = stack_pop(vm);
      if (IS_INT(a) && IS_INT(b)) {
        stack_push(vm, value_make_int(AS_INT(a) - AS_INT(b)));
      } else {
        f64 a_val = IS_INT(a) ? (f64)AS_INT(a) : AS_FLOAT(a);
        f64 b_val = IS_INT(b) ? (f64)AS_INT(b) : AS_FLOAT(b);
        stack_push(vm, value_make_float(a_val - b_val));
      }
      break;
    }
    
    case OP_MULTIPLY: {
      Value b = stack_pop(vm);
      Value a = stack_pop(vm);
      if (IS_INT(a) && IS_INT(b)) {
        stack_push(vm, value_make_int(AS_INT(a) * AS_INT(b)));
      } else {
        f64 a_val = IS_INT(a) ? (f64)AS_INT(a) : AS_FLOAT(a);
        f64 b_val = IS_INT(b) ? (f64)AS_INT(b) : AS_FLOAT(b);
        stack_push(vm, value_make_float(a_val * b_val));
      }
      break;
    }
    
    case OP_DIVIDE: {
      Value b = stack_pop(vm);
      Value a = stack_pop(vm);
      f64 a_val = IS_INT(a) ? (f64)AS_INT(a) : AS_FLOAT(a);
      f64 b_val = IS_INT(b) ? (f64)AS_INT(b) : AS_FLOAT(b);
      if (b_val == 0.0) {
        error_fatal("Division by zero");
        return false;
      }
      stack_push(vm, value_make_float(a_val / b_val));
      break;
    }
    
    case OP_MODULO: {
      Value b = stack_pop(vm);
      Value a = stack_pop(vm);
      if (!IS_INT(a) || !IS_INT(b)) {
        error_fatal("Modulo requires integer operands");
        return false;
      }
      if (AS_INT(b) == 0) {
        error_fatal("Modulo by zero");
        return false;
      }
      stack_push(vm, value_make_int(AS_INT(a) % AS_INT(b)));
      break;
    }
    
    case OP_NEGATE: {
      Value a = stack_pop(vm);
      if (IS_INT(a)) {
        stack_push(vm, value_make_int(-AS_INT(a)));
      } else if (IS_FLOAT(a)) {
        stack_push(vm, value_make_float(-AS_FLOAT(a)));
      } else {
        error_fatal("Cannot negate non-numeric value");
        return false;
      }
      break;
    }
    
    // Comparison operations
    case OP_EQUAL: {
      Value b = stack_pop(vm);
      Value a = stack_pop(vm);
      stack_push(vm, value_make_bool(value_equal(a, b)));
      break;
    }
    
    case OP_NOT_EQUAL: {
      Value b = stack_pop(vm);
      Value a = stack_pop(vm);
      stack_push(vm, value_make_bool(!value_equal(a, b)));
      break;
    }
    
    case OP_LESS: {
      Value b = stack_pop(vm);
      Value a = stack_pop(vm);
      f64 a_val = value_to_float(a);
      f64 b_val = value_to_float(b);
      stack_push(vm, value_make_bool(a_val < b_val));
      break;
    }
    
    case OP_LESS_EQUAL: {
      Value b = stack_pop(vm);
      Value a = stack_pop(vm);
      f64 a_val = value_to_float(a);
      f64 b_val = value_to_float(b);
      stack_push(vm, value_make_bool(a_val <= b_val));
      break;
    }
    
    case OP_GREATER: {
      Value b = stack_pop(vm);
      Value a = stack_pop(vm);
      f64 a_val = value_to_float(a);
      f64 b_val = value_to_float(b);
      stack_push(vm, value_make_bool(a_val > b_val));
      break;
    }
    
    case OP_GREATER_EQUAL: {
      Value b = stack_pop(vm);
      Value a = stack_pop(vm);
      f64 a_val = value_to_float(a);
      f64 b_val = value_to_float(b);
      stack_push(vm, value_make_bool(a_val >= b_val));
      break;
    }
    
    case OP_NOT: {
      Value a = stack_pop(vm);
      // In Satori, only false and nil are falsy
      bool is_truthy = !(IS_NIL(a) || (IS_BOOL(a) && !AS_BOOL(a)));
      stack_push(vm, value_make_bool(!is_truthy));
      break;
    }
    
    // Control flow
    case OP_JUMP: {
      u16 offset = READ_SHORT();
      vm->ip += offset;
      break;
    }
    
    case OP_JUMP_IF_FALSE: {
      u16 offset = READ_SHORT();
      Value condition = stack_peek(vm, 0);
      // Check if falsy (nil or false)
      bool is_falsy = IS_NIL(condition) || (IS_BOOL(condition) && !AS_BOOL(condition));
      if (is_falsy) {
        vm->ip += offset;
      }
      break;
    }
    
    case OP_LOOP: {
      u16 offset = READ_SHORT();
      vm->ip -= offset;
      break;
    }

    case OP_PRINT: {
      // Deprecated built-in print - for backwards compatibility
      int arg_count = READ_BYTE();
      Value args[256];
      for (int i = arg_count - 1; i >= 0; i--) {
        args[i] = stack_pop(vm);
      }
      builtin_println(arg_count, args);
      stack_push(vm, value_make_nil());
      break;
    }

    case OP_HALT: {
      return true;
    }

    default:
      error_fatal("Unknown opcode: %d", instruction);
      return false;
    }
  }

#undef READ_BYTE
#undef READ_CONSTANT
#undef READ_STRING
}
