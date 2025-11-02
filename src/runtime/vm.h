// src/vm.h - Bytecode interpreter

#ifndef SATORI_VM_H
#define SATORI_VM_H

#include "core/common.h"
#include "core/value.h"

typedef enum {
  OP_CONSTANT,   // Load constant
  OP_POP,        // Pop from stack
  OP_CALL,       // Call function
  OP_IMPORT,     // Import module
  OP_GET_MEMBER, // Get member from object
  OP_PRINT,      // Built-in print
  OP_RETURN,     // Return from function
  OP_HALT,       // Stop execution
} OpCode;

typedef struct {
  u8 *code;
  int count;
  int capacity;
  Value *constants;
  int constant_count;
  int constant_capacity;
} Chunk;

typedef struct {
  Chunk chunk;
  u8 *ip; // Instruction pointer
  Value stack[SATORI_STACK_MAX];
  int stack_top;
} VM;

// Chunk operations
void chunk_init(Chunk *chunk);
void chunk_free(Chunk *chunk);
void chunk_write(Chunk *chunk, u8 byte);
int chunk_add_constant(Chunk *chunk, Value value);

// VM operations
void vm_init(VM *vm);
void vm_free(VM *vm);
bool vm_run(VM *vm);

#endif // SATORI_VM_H
