// src/parser.h - Parse tokens into AST

#ifndef SATORI_PARSER_H
#define SATORI_PARSER_H

#include "frontend/ast.h"
#include "core/common.h"
#include "frontend/lexer.h"

typedef struct {
  Lexer *lexer;
  Token current;
  Token previous;
  bool had_error;
  bool panic_mode;
  const char *file_path;
} Parser;

void parser_init(Parser *parser, Lexer *lexer, const char *file_path);
AstNode *parser_parse(Parser *parser);

#endif // SATORI_PARSER_H
