// src/vm.c - The executor

#define _POSIX_C_SOURCE 200809L

#include "runtime/vm.h"
#include "core/value.h"
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
}

void vm_free(VM *vm) { chunk_free(&vm->chunk); }

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

    case OP_PRINT: {
      // Built-in print - for now just call println
      int arg_count = READ_BYTE();
      Value args[256];
      for (int i = arg_count - 1; i >= 0; i--) {
        args[i] = stack_pop(vm);
      }
      builtin_println(arg_count, args);
      stack_push(vm, value_make_nil());
      break;
    }

    case OP_IMPORT: {
      // For now, just skip - we'll handle modules properly later
      READ_BYTE(); // module name constant index
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
}
