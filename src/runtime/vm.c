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
  chunk->lines = NULL;
  chunk->count = 0;
  chunk->capacity = 0;
  chunk->constants = NULL;
  chunk->constant_count = 0;
  chunk->constant_capacity = 0;
}

void chunk_free(Chunk *chunk) {
  free(chunk->code);
  free(chunk->lines);
  for (int i = 0; i < chunk->constant_count; i++) {
    value_free(chunk->constants[i]);
  }
  free(chunk->constants);
  chunk_init(chunk);
}

void chunk_write(Chunk *chunk, u8 byte, int line) {
  if (chunk->capacity < chunk->count + 1) {
    int old_capacity = chunk->capacity;
    chunk->capacity = old_capacity < 8 ? 8 : old_capacity * 2;
    chunk->code = realloc(chunk->code, (size_t)chunk->capacity);
    chunk->lines = realloc(chunk->lines, (size_t)chunk->capacity * sizeof(int));
  }
  chunk->code[chunk->count] = byte;
  if (chunk->lines) {
    chunk->lines[chunk->count] = line;
  }
  chunk->count++;
}

int chunk_add_constant(Chunk *chunk, Value value) {
  if (chunk->constant_capacity < chunk->constant_count + 1) {
    int old_capacity = chunk->constant_capacity;
    chunk->constant_capacity = old_capacity < 8 ? 8 : old_capacity * 2;
    chunk->constants =
        realloc(chunk->constants,
                (size_t)chunk->constant_capacity * sizeof(Value));
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
  vm->has_source = false;
  module_system_init(vm);
}

void vm_free(VM *vm) {
  chunk_free(&vm->chunk);
  if (vm->has_source) {
    source_free(&vm->source_info);
    vm->has_source = false;
  }
  module_system_free(vm);
}

void vm_set_source(VM *vm, SourceInfo *source_info) {
  vm->source_info = *source_info;
  vm->has_source = true;
}

int vm_current_line(VM *vm) {
  if (!vm->chunk.lines) return 0;
  int offset = (int)(vm->ip - vm->chunk.code);
  if (offset >= 0 && offset < vm->chunk.count) {
    return vm->chunk.lines[offset];
  }
  return 0;
}

static void runtime_error(VM *vm, int line, const char *message) {
  if (vm->has_source && line > 0) {
    const char *notes[] = {
      "note: the program stopped here"
    };
    diag_emit(LEVEL_ERROR, vm->source_info.file_path,
              line, 0, 0, message,
              &vm->source_info,
              notes, 1);
  } else {
    error_report_simple(message);
  }
  exit(1);
}

static void stack_push(VM *vm, Value value) {
  if (vm->stack_top >= SATORI_STACK_MAX) {
    runtime_error(vm, vm_current_line(vm), "Stack overflow");
  }
  vm->stack[vm->stack_top++] = value;
}

static Value stack_pop(VM *vm) {
  if (vm->stack_top <= 0) {
    runtime_error(vm, vm_current_line(vm), "Stack underflow");
  }
  return vm->stack[--vm->stack_top];
}

static Value stack_peek(VM *vm, int distance) {
  return vm->stack[vm->stack_top - 1 - distance];
}

bool vm_run(VM *vm) {
  vm->ip = vm->chunk.code;

#define READ_BYTE() (*vm->ip++)
#define READ_SHORT() \
  (vm->ip += 2, (u16)((vm->ip[-2] << 8) | vm->ip[-1]))
#define READ_CONSTANT() (vm->chunk.constants[READ_BYTE()])

  for (;;) {
    u8 instruction;
    int line = vm_current_line(vm);

#ifdef SATORI_DEBUG_TRACE_EXECUTION
    // Disassemble current instruction
    printf("[%04d] ", (int)(vm->ip - vm->chunk.code));
    disassemble_instruction(&vm->chunk, (int)(vm->ip - vm->chunk.code));
    printf("  [stack: ");
    for (int i = 0; i < vm->stack_top; i++) {
      value_print(vm->stack[i]);
      printf(" ");
    }
    printf("]\n");
#endif

    switch (instruction = READ_BYTE()) {
    case OP_CONSTANT: {
      Value value = READ_CONSTANT();
      stack_push(vm, value);
      break;
    }

    case OP_POP: {
      stack_pop(vm);
      break;
    }

    case OP_GET_LOCAL: {
      u8 slot = READ_BYTE();
      if (slot >= SATORI_MAX_LOCALS) {
        runtime_error(vm, line, "Local variable slot out of bounds");
      }
      stack_push(vm, vm->locals[slot]);
      break;
    }

    case OP_SET_LOCAL: {
      u8 slot = READ_BYTE();
      if (slot >= SATORI_MAX_LOCALS) {
        runtime_error(vm, line, "Local variable slot out of bounds");
      }
      vm->locals[slot] = stack_peek(vm, 0);
      // Track the highest local slot seen
      if (slot + 1 > vm->local_count) {
        vm->local_count = slot + 1;
      }
      break;
    }

    case OP_GET_GLOBAL: {
      Value name = READ_CONSTANT();
      const char *name_str = AS_STRING(name);
      Value value;
      if (table_get(&vm->globals, name_str, &value)) {
        stack_push(vm, value);
      } else {
        char buf[512];
        snprintf(buf, sizeof(buf), "Undefined global '%s'", name_str);
        runtime_error(vm, line, buf);
      }
      break;
    }

    case OP_CALL_NATIVE: {
      u8 arg_count = READ_BYTE();
      // Stack layout: [func, arg1, arg2, ..., argN]
      // Func is below the args: at stack_top - arg_count - 1
      int func_idx = vm->stack_top - arg_count - 1;
      if (func_idx < 0) {
        runtime_error(vm, line, "Stack error in function call");
      }

      Value callee = vm->stack[func_idx];
      if (!IS_NATIVE_FN(callee)) {
        runtime_error(vm, line, "Can only call native functions");
      }

      NativeFn fn = AS_NATIVE_FN(callee);
      // Args start right after the function
      Value result = fn(arg_count, vm->stack + func_idx + 1);

      // Pop all args and func, push result
      vm->stack_top -= arg_count + 1;
      stack_push(vm, result);
      break;
    }

    case OP_IMPORT: {
      Value module_name_val = READ_CONSTANT();
      const char *module_name = AS_STRING(module_name_val);

      // Load the module
      if (!module_load(vm, module_name)) {
        char buf[512];
        snprintf(buf, sizeof(buf), "Failed to load module '%s'", module_name);
        runtime_error(vm, line, buf);
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
        runtime_error(vm, line, "Division by zero");
      }
      stack_push(vm, value_make_float(a_val / b_val));
      break;
    }

    case OP_MODULO: {
      Value b = stack_pop(vm);
      Value a = stack_pop(vm);
      if (!IS_INT(a) || !IS_INT(b)) {
        runtime_error(vm, line, "Modulo requires integer operands");
      }
      if (AS_INT(b) == 0) {
        runtime_error(vm, line, "Modulo by zero");
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
        runtime_error(vm, line, "Cannot negate non-numeric value");
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
      f64 a_val = IS_INT(a) ? (f64)AS_INT(a) : AS_FLOAT(a);
      f64 b_val = IS_INT(b) ? (f64)AS_INT(b) : AS_FLOAT(b);
      stack_push(vm, value_make_bool(a_val < b_val));
      break;
    }

    case OP_LESS_EQUAL: {
      Value b = stack_pop(vm);
      Value a = stack_pop(vm);
      f64 a_val = IS_INT(a) ? (f64)AS_INT(a) : AS_FLOAT(a);
      f64 b_val = IS_INT(b) ? (f64)AS_INT(b) : AS_FLOAT(b);
      stack_push(vm, value_make_bool(a_val <= b_val));
      break;
    }

    case OP_GREATER: {
      Value b = stack_pop(vm);
      Value a = stack_pop(vm);
      f64 a_val = IS_INT(a) ? (f64)AS_INT(a) : AS_FLOAT(a);
      f64 b_val = IS_INT(b) ? (f64)AS_INT(b) : AS_FLOAT(b);
      stack_push(vm, value_make_bool(a_val > b_val));
      break;
    }

    case OP_GREATER_EQUAL: {
      Value b = stack_pop(vm);
      Value a = stack_pop(vm);
      f64 a_val = IS_INT(a) ? (f64)AS_INT(a) : AS_FLOAT(a);
      f64 b_val = IS_INT(b) ? (f64)AS_INT(b) : AS_FLOAT(b);
      stack_push(vm, value_make_bool(a_val >= b_val));
      break;
    }

    case OP_NOT: {
      Value a = stack_pop(vm);
      stack_push(vm, value_make_bool(!value_is_truthy(a)));
      break;
    }

    // Control flow operations
    case OP_JUMP: {
      u16 offset = READ_SHORT();
      vm->ip += offset;
      break;
    }

    case OP_JUMP_IF_FALSE: {
      u16 offset = READ_SHORT();
      Value condition = stack_peek(vm, 0);
      if (!value_is_truthy(condition)) {
        vm->ip += offset;
      }
      break;
    }

    case OP_LOOP: {
      u16 offset = READ_SHORT();
      vm->ip -= offset;
      break;
    }

    // Built-in operations
    case OP_PRINT: {
      Value value = stack_pop(vm);
      value_print(value);
      printf("\n");
      break;
    }

    case OP_RETURN: {
      // Not yet implemented
      runtime_error(vm, line, "Return not yet implemented");
      break;
    }

    case OP_HALT: {
      return true;
    }

    default: {
      char buf[512];
      snprintf(buf, sizeof(buf), "Unknown opcode: %d", instruction);
      runtime_error(vm, line, buf);
      return false;
    }
    }
  }

#undef READ_BYTE
#undef READ_SHORT
#undef READ_CONSTANT
}
