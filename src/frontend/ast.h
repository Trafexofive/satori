// src/ast.h - Abstract Syntax Tree

#ifndef SATORI_AST_H
#define SATORI_AST_H

#include "core/common.h"
#include "frontend/lexer.h"

typedef enum {
  AST_PROGRAM,
  AST_IMPORT,
  AST_LET,           // Variable declaration
  AST_ASSIGNMENT,    // Variable assignment (for future)
  AST_CALL,
  AST_MEMBER_ACCESS,
  AST_IDENTIFIER,
  AST_STRING_LITERAL,
  AST_INT_LITERAL,
  AST_FLOAT_LITERAL,
} AstNodeType;

typedef struct AstNode AstNode;

typedef struct {
  char *module_name;
} AstImport;

typedef struct {
  AstNode *object; // Left side (e.g., "io")
  char *member;    // Right side (e.g., "println")
} AstMemberAccess;

typedef struct {
  AstNode *callee; // Function being called
  AstNode **args;  // Arguments
  int arg_count;
} AstCall;

typedef struct {
  char *name;
} AstIdentifier;

typedef struct {
  char *name;        // Variable name
  AstNode *value;    // Initial value expression
} AstLet;

typedef struct {
  char *value;
} AstStringLiteral;

typedef struct {
  i64 value;
} AstIntLiteral;

typedef struct {
  f64 value;
} AstFloatLiteral;

typedef struct {
  AstNode **statements;
  int statement_count;
} AstProgram;

struct AstNode {
  AstNodeType type;
  int line;
  int column;

  union {
    AstProgram program;
    AstImport import;
    AstLet let;
    AstCall call;
    AstMemberAccess member_access;
    AstIdentifier identifier;
    AstStringLiteral string_literal;
    AstIntLiteral int_literal;
    AstFloatLiteral float_literal;
  } as;
};

// Constructors
AstNode *ast_make_program(void);
AstNode *ast_make_import(char *module_name, int line, int column);
AstNode *ast_make_let(char *name, AstNode *value, int line, int column);
AstNode *ast_make_call(AstNode *callee, AstNode **args, int arg_count, int line,
                       int column);
AstNode *ast_make_member_access(AstNode *object, char *member, int line,
                                int column);
AstNode *ast_make_identifier(char *name, int line, int column);
AstNode *ast_make_string_literal(char *value, int line, int column);
AstNode *ast_make_int_literal(i64 value, int line, int column);
AstNode *ast_make_float_literal(f64 value, int line, int column);

// Add statement to program
void ast_program_add_statement(AstNode *program, AstNode *statement);

// Cleanup
void ast_free(AstNode *node);

// Debug
void ast_print(AstNode *node, int indent);

#endif // SATORI_AST_H
