// src/parser.h - Parse tokens into AST

#ifndef SATORI_PARSER_H
#define SATORI_PARSER_H

#include "frontend/ast.h"
#include "core/common.h"
#include "core/source.h"
#include "frontend/lexer.h"

typedef struct {
  Lexer *lexer;
  Token current;
  Token previous;
  bool had_error;
  bool panic_mode;
  const char *file_path;
  SourceInfo *source;  // For source-line error display
} Parser;

void parser_init(Parser *parser, Lexer *lexer, const char *file_path);
void parser_set_source(Parser *parser, SourceInfo *source);
AstNode *parser_parse(Parser *parser);

#endif // SATORI_PARSER_H
