// src/codegen.c - Emit bytecode

#include "backend/codegen.h"
#include "error/error.h"
#include <string.h>
#include <stdio.h>

typedef struct {
  Chunk *chunk;
  bool had_error;
} Compiler;

static void emit_byte(Compiler *c, u8 byte) { chunk_write(c->chunk, byte); }

static void emit_bytes(Compiler *c, u8 byte1, u8 byte2) {
  emit_byte(c, byte1);
  emit_byte(c, byte2);
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
    // For now, just emit a no-op import
    int constant =
        make_constant(c, value_make_string(node->as.import.module_name));
    emit_bytes(c, OP_IMPORT, constant);
    break;
  }

  case AST_CALL: {
    compile_call(c, node);
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

  compile_node(&compiler, ast);
  emit_byte(&compiler, OP_HALT);

  return !compiler.had_error;
}
