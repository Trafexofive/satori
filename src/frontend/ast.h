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
  AST_BINARY_OP,     // Binary operation (a + b, a * b, etc.)
  AST_UNARY_OP,      // Unary operation (-x, !x)
  AST_IF,            // If statement
  AST_WHILE,         // While loop
  AST_LOOP,          // Infinite loop
  AST_BREAK,         // Break statement
  AST_CONTINUE,      // Continue statement
  AST_BLOCK,         // Block of statements
  AST_CALL,
  AST_MEMBER_ACCESS,
  AST_IDENTIFIER,
  AST_STRING_LITERAL,
  AST_INT_LITERAL,
  AST_FLOAT_LITERAL,
} AstNodeType;

typedef enum {
  BIN_ADD,      // +
  BIN_SUB,      // -
  BIN_MUL,      // *
  BIN_DIV,      // /
  BIN_MOD,      // %
  BIN_EQ,       // ==
  BIN_NEQ,      // !=
  BIN_LT,       // <
  BIN_LTE,      // <=
  BIN_GT,       // >
  BIN_GTE,      // >=
} BinaryOperator;

typedef enum {
  UNARY_NEG,   // -
  UNARY_NOT,   // !
} UnaryOperator;

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
  char *name;        // Variable name
  AstNode *value;    // New value expression
} AstAssignment;

typedef struct {
  BinaryOperator op;
  AstNode *left;
  AstNode *right;
} AstBinaryOp;

typedef struct {
  UnaryOperator op;
  AstNode *operand;
} AstUnaryOp;

typedef struct {
  AstNode *condition;
  AstNode *then_branch;
  AstNode *else_branch;  // NULL if no else
} AstIf;

typedef struct {
  AstNode *condition;
  AstNode *body;
} AstWhile;

typedef struct {
  AstNode *body;
} AstLoop;

typedef struct {
  AstNode **statements;
  int statement_count;
  int capacity;
} AstBlock;

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
    AstAssignment assignment;
    AstBinaryOp binary_op;
    AstUnaryOp unary_op;
    AstIf if_stmt;
    AstWhile while_loop;
    AstLoop loop;
    AstBlock block;
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
AstNode *ast_make_assignment(char *name, AstNode *value, int line, int column);
AstNode *ast_make_binary_op(BinaryOperator op, AstNode *left, AstNode *right, int line, int column);
AstNode *ast_make_unary_op(UnaryOperator op, AstNode *operand, int line, int column);
AstNode *ast_make_if(AstNode *condition, AstNode *then_branch, AstNode *else_branch, int line, int column);
AstNode *ast_make_while(AstNode *condition, AstNode *body, int line, int column);
AstNode *ast_make_loop(AstNode *body, int line, int column);
AstNode *ast_make_break(int line, int column);
AstNode *ast_make_continue(int line, int column);
AstNode *ast_make_block(int line, int column);
void ast_block_add_statement(AstNode *block, AstNode *statement);
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
