// tests/test_module_bytecode.c - Test module system via bytecode execution
//
// Manually generates bytecode that:
// 1. Imports io module
// 2. Calls io.println with a string
// 3. Calls io.println with format string

#include "runtime/vm.h"
#include "runtime/module.h"
#include "core/value.h"
#include <stdio.h>

int main(void) {
  VM vm;
  vm_init(&vm);
  
  printf("=== Bytecode Module Test ===\n\n");
  
  // Build bytecode manually:
  // import io
  // io.println "Hello, World!"
  // io.println "Number: {}", 42
  
  Chunk *chunk = &vm.chunk;
  
  // Instruction 1: OP_IMPORT "io"
  chunk_write(chunk, OP_IMPORT);
  int io_name_idx = chunk_add_constant(chunk, value_make_string("io"));
  chunk_write(chunk, io_name_idx);
  
  // Instruction 2: Get io.println function
  chunk_write(chunk, OP_GET_GLOBAL);
  int println_name_idx = chunk_add_constant(chunk, value_make_string("io.println"));
  chunk_write(chunk, println_name_idx);
  
  // Instruction 3: Push argument "Hello, World!"
  chunk_write(chunk, OP_CONSTANT);
  int str_idx = chunk_add_constant(chunk, value_make_string("Hello, World!"));
  chunk_write(chunk, str_idx);
  
  // Instruction 4: Call native function with 1 arg
  chunk_write(chunk, OP_CALL_NATIVE);
  chunk_write(chunk, 1);  // 1 argument
  
  // Instruction 5: Pop the return value (nil)
  chunk_write(chunk, OP_POP);
  
  // Instruction 6: Get io.println again
  chunk_write(chunk, OP_GET_GLOBAL);
  chunk_write(chunk, println_name_idx);  // Reuse index
  
  // Instruction 7: Push format string
  chunk_write(chunk, OP_CONSTANT);
  int fmt_idx = chunk_add_constant(chunk, value_make_string("Number: {}"));
  chunk_write(chunk, fmt_idx);
  
  // Instruction 8: Push number argument
  chunk_write(chunk, OP_CONSTANT);
  int num_idx = chunk_add_constant(chunk, value_make_int(42));
  chunk_write(chunk, num_idx);
  
  // Instruction 9: Call with 2 args
  chunk_write(chunk, OP_CALL_NATIVE);
  chunk_write(chunk, 2);
  
  // Instruction 10: Pop return value
  chunk_write(chunk, OP_POP);
  
  // Instruction 11: Halt
  chunk_write(chunk, OP_HALT);
  
  // Run the bytecode
  printf("Executing bytecode:\n");
  printf("-------------------\n");
  bool success = vm_run(&vm);
  printf("-------------------\n");
  
  if (success) {
    printf("\n✓ Bytecode execution successful!\n");
  } else {
    printf("\n✗ Bytecode execution failed!\n");
    vm_free(&vm);
    return 1;
  }
  
  vm_free(&vm);
  return 0;
}
