// src/ast.c - AST construction

#define _POSIX_C_SOURCE 200809L

#include "frontend/ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

AstNode *ast_make_program(void) {
  AstNode *node = malloc(sizeof(AstNode));
  node->type = AST_PROGRAM;
  node->line = 0;
  node->column = 0;
  node->as.program.statements = NULL;
  node->as.program.statement_count = 0;
  return node;
}

AstNode *ast_make_import(char *module_name, int line, int column) {
  AstNode *node = malloc(sizeof(AstNode));
  node->type = AST_IMPORT;
  node->line = line;
  node->column = column;
  node->as.import.module_name = strdup(module_name);
  return node;
}

AstNode *ast_make_let(char *name, AstNode *value, int line, int column) {
  AstNode *node = malloc(sizeof(AstNode));
  node->type = AST_LET;
  node->line = line;
  node->column = column;
  node->as.let.name = strdup(name);
  node->as.let.value = value;
  return node;
}

AstNode *ast_make_call(AstNode *callee, AstNode **args, int arg_count, int line,
                       int column) {
  AstNode *node = malloc(sizeof(AstNode));
  node->type = AST_CALL;
  node->line = line;
  node->column = column;
  node->as.call.callee = callee;
  node->as.call.args = args;
  node->as.call.arg_count = arg_count;
  return node;
}

AstNode *ast_make_member_access(AstNode *object, char *member, int line,
                                int column) {
  AstNode *node = malloc(sizeof(AstNode));
  node->type = AST_MEMBER_ACCESS;
  node->line = line;
  node->column = column;
  node->as.member_access.object = object;
  node->as.member_access.member = strdup(member);
  return node;
}

AstNode *ast_make_identifier(char *name, int line, int column) {
  AstNode *node = malloc(sizeof(AstNode));
  node->type = AST_IDENTIFIER;
  node->line = line;
  node->column = column;
  node->as.identifier.name = strdup(name);
  return node;
}

AstNode *ast_make_string_literal(char *value, int line, int column) {
  AstNode *node = malloc(sizeof(AstNode));
  node->type = AST_STRING_LITERAL;
  node->line = line;
  node->column = column;
  node->as.string_literal.value = strdup(value);
  return node;
}

AstNode *ast_make_int_literal(i64 value, int line, int column) {
  AstNode *node = malloc(sizeof(AstNode));
  node->type = AST_INT_LITERAL;
  node->line = line;
  node->column = column;
  node->as.int_literal.value = value;
  return node;
}

AstNode *ast_make_float_literal(f64 value, int line, int column) {
  AstNode *node = malloc(sizeof(AstNode));
  node->type = AST_FLOAT_LITERAL;
  node->line = line;
  node->column = column;
  node->as.float_literal.value = value;
  return node;
}

void ast_program_add_statement(AstNode *program, AstNode *statement) {
  ASSERT(program->type == AST_PROGRAM, "not a program node");

  program->as.program.statement_count++;
  program->as.program.statements =
      realloc(program->as.program.statements,
              sizeof(AstNode *) * program->as.program.statement_count);
  program->as.program.statements[program->as.program.statement_count - 1] =
      statement;
}

void ast_free(AstNode *node) {
  if (!node)
    return;

  switch (node->type) {
  case AST_PROGRAM:
    for (int i = 0; i < node->as.program.statement_count; i++) {
      ast_free(node->as.program.statements[i]);
    }
    free(node->as.program.statements);
    break;
  case AST_IMPORT:
    free(node->as.import.module_name);
    break;
  case AST_LET:
    free(node->as.let.name);
    ast_free(node->as.let.value);
    break;
  case AST_CALL:
    ast_free(node->as.call.callee);
    for (int i = 0; i < node->as.call.arg_count; i++) {
      ast_free(node->as.call.args[i]);
    }
    free(node->as.call.args);
    break;
  case AST_MEMBER_ACCESS:
    ast_free(node->as.member_access.object);
    free(node->as.member_access.member);
    break;
  case AST_IDENTIFIER:
    free(node->as.identifier.name);
    break;
  case AST_STRING_LITERAL:
    free(node->as.string_literal.value);
    break;
  default:
    break;
  }

  free(node);
}

void ast_print(AstNode *node, int indent) {
  if (!node)
    return;

  for (int i = 0; i < indent; i++)
    printf("  ");

  switch (node->type) {
  case AST_PROGRAM:
    printf("Program\n");
    for (int i = 0; i < node->as.program.statement_count; i++) {
      ast_print(node->as.program.statements[i], indent + 1);
    }
    break;
  case AST_IMPORT:
    printf("Import: %s\n", node->as.import.module_name);
    break;
  case AST_LET:
    printf("Let: %s :=\n", node->as.let.name);
    ast_print(node->as.let.value, indent + 1);
    break;
  case AST_CALL:
    printf("Call\n");
    ast_print(node->as.call.callee, indent + 1);
    for (int i = 0; i < node->as.call.arg_count; i++) {
      ast_print(node->as.call.args[i], indent + 1);
    }
    break;
  case AST_MEMBER_ACCESS:
    printf("MemberAccess: .%s\n", node->as.member_access.member);
    ast_print(node->as.member_access.object, indent + 1);
    break;
  case AST_IDENTIFIER:
    printf("Identifier: %s\n", node->as.identifier.name);
    break;
  case AST_STRING_LITERAL:
    printf("String: %s\n", node->as.string_literal.value);
    break;
  case AST_INT_LITERAL:
    printf("Int: %ld\n", node->as.int_literal.value);
    break;
  case AST_FLOAT_LITERAL:
    printf("Float: %f\n", node->as.float_literal.value);
    break;
  case AST_ASSIGNMENT:
    // Not yet implemented
    break;
  }
}
