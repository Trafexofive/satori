// src/parser.c - Recursive descent parser

#include "frontend/parser.h"
#include "error/error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void advance(Parser *p) {
  p->previous = p->current;

  for (;;) {
    p->current = lexer_next_token(p->lexer);
    if (p->current.type != TOKEN_ERROR)
      break;

    error_report(p->file_path, p->current.line, p->current.column, "%.*s",
                 p->current.length, p->current.start);
    p->had_error = true;
  }
}

static void skip_newlines(Parser *p) {
  while (p->current.type == TOKEN_NEWLINE) {
    advance(p);
  }
}

static bool check(Parser *p, TokenType type) { return p->current.type == type; }

static bool match(Parser *p, TokenType type) {
  if (!check(p, type))
    return false;
  advance(p);
  return true;
}

static void consume(Parser *p, TokenType type, const char *message) {
  if (p->current.type == type) {
    advance(p);
    return;
  }

  error_report(p->file_path, p->current.line, p->current.column, "%s", message);
  p->had_error = true;
}

static char *token_to_string(Token token) {
  char *str = malloc(token.length + 1);
  memcpy(str, token.start, token.length);
  str[token.length] = '\0';
  return str;
}

static AstNode *parse_primary(Parser *p) {
  if (match(p, TOKEN_STRING)) {
    // Strip quotes
    char *value = malloc(p->previous.length - 1);
    memcpy(value, p->previous.start + 1, p->previous.length - 2);
    value[p->previous.length - 2] = '\0';
    AstNode *node =
        ast_make_string_literal(value, p->previous.line, p->previous.column);
    free(value);
    return node;
  }

  if (match(p, TOKEN_INT)) {
    char *str = token_to_string(p->previous);
    i64 value = atoll(str);
    free(str);
    return ast_make_int_literal(value, p->previous.line, p->previous.column);
  }

  if (match(p, TOKEN_FLOAT)) {
    char *str = token_to_string(p->previous);
    f64 value = atof(str);
    free(str);
    return ast_make_float_literal(value, p->previous.line, p->previous.column);
  }

  if (match(p, TOKEN_IDENTIFIER)) {
    char *name = token_to_string(p->previous);
    AstNode *node =
        ast_make_identifier(name, p->previous.line, p->previous.column);
    free(name);
    return node;
  }

  error_report(p->file_path, p->current.line, p->current.column,
               "expected expression");
  p->had_error = true;
  return NULL;
}

static AstNode *parse_call(Parser *p) {
  AstNode *expr = parse_primary(p);

  while (true) {
    if (match(p, TOKEN_DOT)) {
      // Member access
      consume(p, TOKEN_IDENTIFIER, "expected member name after '.'");
      char *member = token_to_string(p->previous);
      expr = ast_make_member_access(expr, member, p->previous.line,
                                    p->previous.column);
      free(member);
    } else if (check(p, TOKEN_STRING) || check(p, TOKEN_INT) ||
               check(p, TOKEN_FLOAT) || check(p, TOKEN_IDENTIFIER)) {
      // Function call with arguments (no parens needed for single arg)
      AstNode **args = malloc(sizeof(AstNode *));
      args[0] = parse_primary(p);
      expr = ast_make_call(expr, args, 1, p->previous.line, p->previous.column);
    } else {
      break;
    }
  }

  return expr;
}

static AstNode *parse_statement(Parser *p) {
  skip_newlines(p);

  if (match(p, TOKEN_IMPORT)) {
    consume(p, TOKEN_IDENTIFIER, "expected module name after 'import'");
    char *module = token_to_string(p->previous);
    AstNode *node =
        ast_make_import(module, p->previous.line, p->previous.column);
    free(module);
    return node;
  }

  // Expression statement
  return parse_call(p);
}

void parser_init(Parser *parser, Lexer *lexer, const char *file_path) {
  parser->lexer = lexer;
  parser->had_error = false;
  parser->panic_mode = false;
  parser->file_path = file_path;
  advance(parser);
}

AstNode *parser_parse(Parser *parser) {
  AstNode *program = ast_make_program();

  skip_newlines(parser);

  while (!check(parser, TOKEN_EOF)) {
    AstNode *stmt = parse_statement(parser);
    if (stmt) {
      ast_program_add_statement(program, stmt);
    }

    skip_newlines(parser);

    if (parser->had_error) {
      ast_free(program);
      return NULL;
    }
  }

  return program;
}
