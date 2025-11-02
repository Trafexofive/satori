// src/lexer.h - First light

#ifndef SATORI_LEXER_H
#define SATORI_LEXER_H

#include "core/common.h"

typedef enum {
  // Single-character tokens
  TOKEN_LEFT_PAREN,    // (
  TOKEN_RIGHT_PAREN,   // )
  TOKEN_LEFT_BRACE,    // {
  TOKEN_RIGHT_BRACE,   // }
  TOKEN_LEFT_BRACKET,  // [
  TOKEN_RIGHT_BRACKET, // ]
  TOKEN_COMMA,         // ,
  TOKEN_DOT,           // .
  TOKEN_COLON,         // :
  TOKEN_SEMICOLON,     // ; (optional)

  // One or two character tokens
  TOKEN_PLUS,          // +
  TOKEN_MINUS,         // -
  TOKEN_STAR,          // *
  TOKEN_SLASH,         // /
  TOKEN_PERCENT,       // %
  TOKEN_AMPERSAND,     // &
  TOKEN_PIPE,          // |
  TOKEN_CARET,         // ^
  TOKEN_BANG,          // !
  TOKEN_EQUAL,         // =
  TOKEN_LESS,          //
  TOKEN_GREATER,       // >
  TOKEN_PLUS_EQUAL,    // +=
  TOKEN_MINUS_EQUAL,   // -=
  TOKEN_STAR_EQUAL,    // *=
  TOKEN_SLASH_EQUAL,   // /=
  TOKEN_BANG_EQUAL,    // !=
  TOKEN_EQUAL_EQUAL,   // ==
  TOKEN_LESS_EQUAL,    // <=
  TOKEN_GREATER_EQUAL, // >=
  TOKEN_COLON_EQUAL,   // :=
  TOKEN_DOT_DOT,       // ..
  TOKEN_ARROW,         // -> (maybe for later, but honestly Ive always hated the arrow operator, I always see it as the struct dereference)

  // Literals
  TOKEN_IDENTIFIER,
  TOKEN_STRING,
  TOKEN_NUMBER,
  TOKEN_INT,
  TOKEN_FLOAT,

  // Keywords
  TOKEN_AND,
  TOKEN_OR,
  TOKEN_NOT,
  TOKEN_IF,
  TOKEN_ELSE,
  TOKEN_THEN,
  TOKEN_FOR,
  TOKEN_IN,
  TOKEN_LOOP,
  TOKEN_WHILE,
  TOKEN_BREAK,
  TOKEN_CONTINUE,
  TOKEN_RETURN,
  TOKEN_STRUCT,
  TOKEN_LET,
  TOKEN_IMPORT,
  TOKEN_DEFER,
  TOKEN_SPAWN,
  TOKEN_PANIC,
  TOKEN_TRUE,
  TOKEN_FALSE,
  TOKEN_NIL,

  // Types
  TOKEN_TYPE_INT,
  TOKEN_TYPE_FLOAT,
  TOKEN_TYPE_BOOL,
  TOKEN_TYPE_STRING,
  TOKEN_TYPE_VOID,
  TOKEN_TYPE_BYTE,

  // Special
  TOKEN_NEWLINE,
  TOKEN_EOF,
  TOKEN_ERROR,
} TokenType;

typedef struct {
  TokenType type;
  const char *start;
  int length;
  int line;
  int column;
} Token;

typedef struct {
  const char *start;
  const char *current;
  int line;
  int column;
} Lexer;

void lexer_init(Lexer *lexer, const char *source);
Token lexer_next_token(Lexer *lexer);
Token lexer_peek_token(Lexer *lexer);
void lexer_print_token(Token token);

#endif // SATORI_LEXER_H
