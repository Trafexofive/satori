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

// Forward declarations for expression parsing
static AstNode *parse_expression(Parser *p);
static AstNode *parse_equality(Parser *p);
static AstNode *parse_comparison(Parser *p);
static AstNode *parse_term(Parser *p);
static AstNode *parse_factor(Parser *p);
static AstNode *parse_unary(Parser *p);
static AstNode *parse_call(Parser *p);
static AstNode *parse_primary(Parser *p);

// Expression parsing with precedence climbing
// Precedence (lowest to highest):
//   equality:     == !=
//   comparison:   < <= > >=
//   term:         + -
//   factor:       * / %
//   unary:        - !
//   primary:      literals, identifiers, calls

static AstNode *parse_expression(Parser *p) {
  return parse_equality(p);
}

static AstNode *parse_equality(Parser *p) {
  AstNode *expr = parse_comparison(p);
  
  while (match(p, TOKEN_EQUAL_EQUAL) || match(p, TOKEN_BANG_EQUAL)) {
    Token op_token = p->previous;
    BinaryOperator op = (op_token.type == TOKEN_EQUAL_EQUAL) ? BIN_EQ : BIN_NEQ;
    AstNode *right = parse_comparison(p);
    expr = ast_make_binary_op(op, expr, right, op_token.line, op_token.column);
  }
  
  return expr;
}

static AstNode *parse_comparison(Parser *p) {
  AstNode *expr = parse_term(p);
  
  while (match(p, TOKEN_LESS) || match(p, TOKEN_LESS_EQUAL) ||
         match(p, TOKEN_GREATER) || match(p, TOKEN_GREATER_EQUAL)) {
    Token op_token = p->previous;
    BinaryOperator op;
    switch (op_token.type) {
      case TOKEN_LESS: op = BIN_LT; break;
      case TOKEN_LESS_EQUAL: op = BIN_LTE; break;
      case TOKEN_GREATER: op = BIN_GT; break;
      case TOKEN_GREATER_EQUAL: op = BIN_GTE; break;
      default: op = BIN_LT; break;  // Should never happen
    }
    AstNode *right = parse_term(p);
    expr = ast_make_binary_op(op, expr, right, op_token.line, op_token.column);
  }
  
  return expr;
}

static AstNode *parse_term(Parser *p) {
  AstNode *expr = parse_factor(p);
  
  while (match(p, TOKEN_PLUS) || match(p, TOKEN_MINUS)) {
    Token op_token = p->previous;
    BinaryOperator op = (op_token.type == TOKEN_PLUS) ? BIN_ADD : BIN_SUB;
    AstNode *right = parse_factor(p);
    expr = ast_make_binary_op(op, expr, right, op_token.line, op_token.column);
  }
  
  return expr;
}

static AstNode *parse_factor(Parser *p) {
  AstNode *expr = parse_unary(p);
  
  while (match(p, TOKEN_STAR) || match(p, TOKEN_SLASH) || match(p, TOKEN_PERCENT)) {
    Token op_token = p->previous;
    BinaryOperator op;
    switch (op_token.type) {
      case TOKEN_STAR: op = BIN_MUL; break;
      case TOKEN_SLASH: op = BIN_DIV; break;
      case TOKEN_PERCENT: op = BIN_MOD; break;
      default: op = BIN_MUL; break;  // Should never happen
    }
    AstNode *right = parse_unary(p);
    expr = ast_make_binary_op(op, expr, right, op_token.line, op_token.column);
  }
  
  return expr;
}

static AstNode *parse_unary(Parser *p) {
  if (match(p, TOKEN_MINUS) || match(p, TOKEN_BANG)) {
    Token op_token = p->previous;
    UnaryOperator op = (op_token.type == TOKEN_MINUS) ? UNARY_NEG : UNARY_NOT;
    AstNode *operand = parse_unary(p);  // Right-associative
    return ast_make_unary_op(op, operand, op_token.line, op_token.column);
  }
  
  return parse_call(p);
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
               check(p, TOKEN_FLOAT) || check(p, TOKEN_IDENTIFIER) ||
               check(p, TOKEN_MINUS) || check(p, TOKEN_BANG) ||
               check(p, TOKEN_LEFT_PAREN)) {
      // Function call with arguments (comma-separated, no parens)
      int arg_capacity = 4;
      int arg_count = 0;
      AstNode **args = malloc(sizeof(AstNode *) * arg_capacity);
      
      // Parse first argument as expression
      args[arg_count++] = parse_expression(p);
      
      // Parse additional comma-separated arguments
      while (match(p, TOKEN_COMMA)) {
        if (arg_count >= arg_capacity) {
          arg_capacity *= 2;
          args = realloc(args, sizeof(AstNode *) * arg_capacity);
        }
        args[arg_count++] = parse_expression(p);
      }
      
      expr = ast_make_call(expr, args, arg_count, p->previous.line, p->previous.column);
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

  if (match(p, TOKEN_LET)) {
    // let name := value
    consume(p, TOKEN_IDENTIFIER, "expected variable name after 'let'");
    char *name = token_to_string(p->previous);
    int line = p->previous.line;
    int column = p->previous.column;
    
    consume(p, TOKEN_COLON_EQUAL, "expected ':=' after variable name");
    
    AstNode *value = parse_expression(p);  // Parse the value as expression
    AstNode *node = ast_make_let(name, value, line, column);
    free(name);
    return node;
  }
  
  if (match(p, TOKEN_IF)) {
    // if condition then statement [else statement]
    int line = p->previous.line;
    int column = p->previous.column;
    
    AstNode *condition = parse_expression(p);
    consume(p, TOKEN_THEN, "expected 'then' after if condition");
    skip_newlines(p);
    
    AstNode *then_branch = parse_statement(p);
    AstNode *else_branch = NULL;
    
    skip_newlines(p);
    if (match(p, TOKEN_ELSE)) {
      skip_newlines(p);
      else_branch = parse_statement(p);
    }
    
    return ast_make_if(condition, then_branch, else_branch, line, column);
  }
  
  if (match(p, TOKEN_WHILE)) {
    // while condition then statement
    int line = p->previous.line;
    int column = p->previous.column;
    
    AstNode *condition = parse_expression(p);
    consume(p, TOKEN_THEN, "expected 'then' after while condition");
    skip_newlines(p);
    
    AstNode *body = parse_statement(p);
    return ast_make_while(condition, body, line, column);
  }
  
  if (match(p, TOKEN_LOOP)) {
    // loop statement
    int line = p->previous.line;
    int column = p->previous.column;
    skip_newlines(p);
    
    AstNode *body = parse_statement(p);
    return ast_make_loop(body, line, column);
  }
  
  if (match(p, TOKEN_BREAK)) {
    return ast_make_break(p->previous.line, p->previous.column);
  }
  
  if (match(p, TOKEN_CONTINUE)) {
    return ast_make_continue(p->previous.line, p->previous.column);
  }

  // Expression statement
  return parse_expression(p);
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
