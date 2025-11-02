// src/codegen.c - Emit bytecode

#define _POSIX_C_SOURCE 200809L

#include "backend/codegen.h"
#include "error/error.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Local variable tracking
typedef struct {
  char *name;
  int slot;
} Local;

typedef struct {
  Chunk *chunk;
  bool had_error;
  
  // Local variables
  Local locals[SATORI_MAX_LOCALS];
  int local_count;
} Compiler;

static void emit_byte(Compiler *c, u8 byte) { chunk_write(c->chunk, byte); }

static void emit_bytes(Compiler *c, u8 byte1, u8 byte2) {
  emit_byte(c, byte1);
  emit_byte(c, byte2);
}

static int emit_jump(Compiler *c, u8 instruction) {
  emit_byte(c, instruction);
  emit_byte(c, 0xff);  // Placeholder
  emit_byte(c, 0xff);  // Placeholder
  return c->chunk->count - 2;  // Return offset of jump address
}

static void patch_jump(Compiler *c, int offset) {
  // -2 to adjust for the jump offset itself
  int jump = c->chunk->count - offset - 2;
  
  if (jump > 0xffff) {
    error_report_simple("Too much code to jump over");
    c->had_error = true;
    return;
  }
  
  c->chunk->code[offset] = (jump >> 8) & 0xff;
  c->chunk->code[offset + 1] = jump & 0xff;
}

static void emit_loop(Compiler *c, int loop_start) {
  emit_byte(c, OP_LOOP);
  
  int offset = c->chunk->count - loop_start + 2;
  if (offset > 0xffff) {
    error_report_simple("Loop body too large");
    c->had_error = true;
    return;
  }
  
  emit_byte(c, (offset >> 8) & 0xff);
  emit_byte(c, offset & 0xff);
}

static int make_constant(Compiler *c, Value value) {
  int constant = chunk_add_constant(c->chunk, value);
  if (constant > 255) {
    error_report_simple("Too many constants in one chunk");
    c->had_error = true;
    return 0;
  }
  return constant;
}

// Add a local variable
static int add_local(Compiler *c, const char *name) {
  if (c->local_count >= SATORI_MAX_LOCALS) {
    error_report_simple("Too many local variables");
    c->had_error = true;
    return -1;
  }
  
  Local *local = &c->locals[c->local_count];
  local->name = strdup(name);
  local->slot = c->local_count;
  return c->local_count++;
}

// Find a local variable by name
static int resolve_local(Compiler *c, const char *name) {
  for (int i = c->local_count - 1; i >= 0; i--) {
    if (strcmp(c->locals[i].name, name) == 0) {
      return c->locals[i].slot;
    }
  }
  return -1;  // Not found
}

static void compile_node(Compiler *c, AstNode *node);

static void compile_call(Compiler *c, AstNode *node) {
  AstCall *call = &node->as.call;

  // Handle module.function() calls
  if (call->callee->type == AST_MEMBER_ACCESS) {
    AstMemberAccess *member = &call->callee->as.member_access;
    if (member->object->type == AST_IDENTIFIER) {
      AstIdentifier *obj = &member->object->as.identifier;
      
      // Build the full function name: "module.function"
      char full_name[256];
      snprintf(full_name, sizeof(full_name), "%s.%s", obj->name, member->member);
      
      // Emit OP_GET_GLOBAL to get the native function
      int name_idx = make_constant(c, value_make_string(full_name));
      emit_bytes(c, OP_GET_GLOBAL, name_idx);
      
      // Compile arguments (push them on stack)
      for (int i = 0; i < call->arg_count; i++) {
        compile_node(c, call->args[i]);
      }
      
      // Emit OP_CALL_NATIVE with argument count
      emit_bytes(c, OP_CALL_NATIVE, call->arg_count);
      
      // The result is now on the stack
      // For expression statements, we need to pop it
      // For now, we'll always pop (statements only)
      emit_byte(c, OP_POP);
      return;
    }
  }

  error_report_simple("Unknown function call");
  c->had_error = true;
}

static void compile_node(Compiler *c, AstNode *node) {
  if (!node)
    return;

  switch (node->type) {
  case AST_PROGRAM: {
    for (int i = 0; i < node->as.program.statement_count; i++) {
      compile_node(c, node->as.program.statements[i]);
    }
    break;
  }

  case AST_IMPORT: {
    int constant =
        make_constant(c, value_make_string(node->as.import.module_name));
    emit_bytes(c, OP_IMPORT, constant);
    break;
  }

  case AST_LET: {
    // let name := value
    // Compile the value expression first (puts it on stack)
    compile_node(c, node->as.let.value);
    
    // Add local variable and emit OP_SET_LOCAL
    int slot = add_local(c, node->as.let.name);
    if (slot >= 0) {
      emit_bytes(c, OP_SET_LOCAL, slot);
    }
    break;
  }
  
  case AST_ASSIGNMENT: {
    // name = value
    compile_node(c, node->as.assignment.value);
    int slot = resolve_local(c, node->as.assignment.name);
    if (slot >= 0) {
      emit_bytes(c, OP_SET_LOCAL, slot);
    } else {
      error_report_simple("Undefined variable in assignment");
      c->had_error = true;
    }
    break;
  }
  
  case AST_IDENTIFIER: {
    // Look up the identifier as a local variable
    int slot = resolve_local(c, node->as.identifier.name);
    if (slot >= 0) {
      // It's a local variable
      emit_bytes(c, OP_GET_LOCAL, slot);
    } else {
      // Not found - this is an error for now
      error_report_simple("Undefined variable");
      c->had_error = true;
    }
    break;
  }
  
  case AST_BINARY_OP: {
    // Compile left and right operands
    compile_node(c, node->as.binary_op.left);
    compile_node(c, node->as.binary_op.right);
    
    // Emit the operation
    switch (node->as.binary_op.op) {
      case BIN_ADD: emit_byte(c, OP_ADD); break;
      case BIN_SUB: emit_byte(c, OP_SUBTRACT); break;
      case BIN_MUL: emit_byte(c, OP_MULTIPLY); break;
      case BIN_DIV: emit_byte(c, OP_DIVIDE); break;
      case BIN_MOD: emit_byte(c, OP_MODULO); break;
      case BIN_EQ:  emit_byte(c, OP_EQUAL); break;
      case BIN_NEQ: emit_byte(c, OP_NOT_EQUAL); break;
      case BIN_LT:  emit_byte(c, OP_LESS); break;
      case BIN_LTE: emit_byte(c, OP_LESS_EQUAL); break;
      case BIN_GT:  emit_byte(c, OP_GREATER); break;
      case BIN_GTE: emit_byte(c, OP_GREATER_EQUAL); break;
    }
    break;
  }
  
  case AST_UNARY_OP: {
    // Compile the operand
    compile_node(c, node->as.unary_op.operand);
    
    // Emit the operation
    switch (node->as.unary_op.op) {
      case UNARY_NEG: emit_byte(c, OP_NEGATE); break;
      case UNARY_NOT: emit_byte(c, OP_NOT); break;
    }
    break;
  }
  
  case AST_IF: {
    // Compile condition
    compile_node(c, node->as.if_stmt.condition);
    
    // Jump to else branch if condition is false
    int else_jump = emit_jump(c, OP_JUMP_IF_FALSE);
    emit_byte(c, OP_POP);  // Pop condition
    
    // Compile then branch
    compile_node(c, node->as.if_stmt.then_branch);
    
    // Jump over else branch
    int end_jump = emit_jump(c, OP_JUMP);
    
    // Patch else jump
    patch_jump(c, else_jump);
    emit_byte(c, OP_POP);  // Pop condition
    
    // Compile else branch if it exists
    if (node->as.if_stmt.else_branch) {
      compile_node(c, node->as.if_stmt.else_branch);
    }
    
    // Patch end jump
    patch_jump(c, end_jump);
    break;
  }
  
  case AST_WHILE: {
    int loop_start = c->chunk->count;
    
    // Compile condition
    compile_node(c, node->as.while_loop.condition);
    
    // Jump out of loop if condition is false
    int exit_jump = emit_jump(c, OP_JUMP_IF_FALSE);
    emit_byte(c, OP_POP);  // Pop condition
    
    // Compile body
    compile_node(c, node->as.while_loop.body);
    
    // Loop back to start
    emit_loop(c, loop_start);
    
    // Patch exit jump
    patch_jump(c, exit_jump);
    emit_byte(c, OP_POP);  // Pop condition
    break;
  }
  
  case AST_LOOP: {
    int loop_start = c->chunk->count;
    
    // Compile body
    compile_node(c, node->as.loop.body);
    
    // Loop back to start
    emit_loop(c, loop_start);
    break;
  }
  
  case AST_BREAK:
    // For now, break is not implemented (needs loop context tracking)
    error_report_simple("break not yet implemented");
    c->had_error = true;
    break;
  
  case AST_CONTINUE:
    // For now, continue is not implemented (needs loop context tracking)
    error_report_simple("continue not yet implemented");
    c->had_error = true;
    break;
  
  case AST_BLOCK: {
    for (int i = 0; i < node->as.block.statement_count; i++) {
      compile_node(c, node->as.block.statements[i]);
    }
    break;
  }

  case AST_CALL: {
    compile_call(c, node);
    break;
  }
  
  case AST_MEMBER_ACCESS: {
    // Member access is only valid as part of a call
    // If we get here, it's an error (e.g., just `io.println` without calling it)
    error_report_simple("Member access must be used in a call");
    c->had_error = true;
    break;
  }

  case AST_STRING_LITERAL: {
    int constant =
        make_constant(c, value_make_string(node->as.string_literal.value));
    emit_bytes(c, OP_CONSTANT, constant);
    break;
  }

  case AST_INT_LITERAL: {
    int constant = make_constant(c, value_make_int(node->as.int_literal.value));
    emit_bytes(c, OP_CONSTANT, constant);
    break;
  }

  case AST_FLOAT_LITERAL: {
    int constant =
        make_constant(c, value_make_float(node->as.float_literal.value));
    emit_bytes(c, OP_CONSTANT, constant);
    break;
  }

  default:
    error_report_simple("Unknown AST node type in codegen");
    c->had_error = true;
    break;
  }
}

bool codegen_compile(AstNode *ast, Chunk *chunk) {
  Compiler compiler;
  compiler.chunk = chunk;
  compiler.had_error = false;
  compiler.local_count = 0;

  compile_node(&compiler, ast);
  emit_byte(&compiler, OP_HALT);
  
  // Free local variable names
  for (int i = 0; i < compiler.local_count; i++) {
    free(compiler.locals[i].name);
  }

  return !compiler.had_error;
}
