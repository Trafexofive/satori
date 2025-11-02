// src/vm.h - Bytecode interpreter

#ifndef SATORI_VM_H
#define SATORI_VM_H

#include "core/common.h"
#include "core/value.h"
#include "core/table.h"

typedef enum {
  OP_CONSTANT,      // Load constant
  OP_POP,           // Pop from stack
  OP_GET_LOCAL,     // Get local variable
  OP_SET_LOCAL,     // Set local variable
  OP_GET_GLOBAL,    // Get global variable/function
  OP_CALL_NATIVE,   // Call native function
  OP_IMPORT,        // Import module
  OP_GET_MEMBER,    // Get member from object
  
  // Arithmetic operations
  OP_ADD,           // +
  OP_SUBTRACT,      // -
  OP_MULTIPLY,      // *
  OP_DIVIDE,        // /
  OP_MODULO,        // %
  OP_NEGATE,        // unary -
  
  // Comparison operations
  OP_EQUAL,         // ==
  OP_NOT_EQUAL,     // !=
  OP_LESS,          // <
  OP_LESS_EQUAL,    // <=
  OP_GREATER,       // >
  OP_GREATER_EQUAL, // >=
  OP_NOT,           // unary !
  
  // Control flow
  OP_JUMP,          // Unconditional jump
  OP_JUMP_IF_FALSE, // Jump if top of stack is false
  OP_LOOP,          // Jump backwards (for loops)
  
  OP_PRINT,         // Built-in print (deprecated, use io.println)
  OP_RETURN,        // Return from function
  OP_HALT,          // Stop execution
} OpCode;

typedef struct {
  u8 *code;
  int count;
  int capacity;
  Value *constants;
  int constant_count;
  int constant_capacity;
} Chunk;

typedef struct VM {
  Chunk chunk;
  u8 *ip;                          // Instruction pointer
  Value stack[SATORI_STACK_MAX];
  int stack_top;
  
  // Local variables (simple array for now, no scoping)
  Value locals[SATORI_MAX_LOCALS];
  int local_count;
  
  // Module system
  Table globals;                   // Global functions and variables
  Table loaded_modules;            // Tracking loaded modules
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
