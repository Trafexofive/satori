// src/lexer.c - Token factory

#include "frontend/lexer.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

void lexer_init(Lexer *lexer, const char *source) {
  lexer->start = source;
  lexer->current = source;
  lexer->line = 1;
  lexer->column = 1;
}

static bool is_at_end(Lexer *lexer) { return *lexer->current == '\0'; }

static char advance(Lexer *lexer) {
  lexer->current++;
  lexer->column++;
  return lexer->current[-1];
}

static char peek(Lexer *lexer) { return *lexer->current; }

static char peek_next(Lexer *lexer) {
  if (is_at_end(lexer))
    return '\0';
  return lexer->current[1];
}

static bool match(Lexer *lexer, char expected) {
  if (is_at_end(lexer))
    return false;
  if (*lexer->current != expected)
    return false;
  lexer->current++;
  lexer->column++;
  return true;
}

static Token make_token(Lexer *lexer, TokenType type) {
  Token token;
  token.type = type;
  token.start = lexer->start;
  token.length = (int)(lexer->current - lexer->start);
  token.line = lexer->line;
  token.column = lexer->column - token.length;
  return token;
}

static Token error_token(Lexer *lexer, const char *message) {
  Token token;
  token.type = TOKEN_ERROR;
  token.start = message;
  token.length = (int)strlen(message);
  token.line = lexer->line;
  token.column = lexer->column;
  return token;
}

static void skip_whitespace(Lexer *lexer) {
  for (;;) {
    char c = peek(lexer);
    switch (c) {
    case ' ':
    case '\r':
    case '\t':
      advance(lexer);
      break;
    case '/':
      if (peek_next(lexer) == '/') {
        // Comment until end of line
        while (peek(lexer) != '\n' && !is_at_end(lexer)) {
          advance(lexer);
        }
      } else {
        return;
      }
      break;
    default:
      return;
    }
  }
}

static Token string(Lexer *lexer) {
  while (peek(lexer) != '"' && !is_at_end(lexer)) {
    if (peek(lexer) == '\n') {
      lexer->line++;
      lexer->column = 0;
    }
    advance(lexer);
  }

  if (is_at_end(lexer)) {
    return error_token(lexer, "Unterminated string");
  }

  advance(lexer); // Closing "
  return make_token(lexer, TOKEN_STRING);
}

static bool is_digit(char c) { return c >= '0' && c <= '9'; }

static bool is_alpha(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

static Token number(Lexer *lexer) {
  while (is_digit(peek(lexer)))
    advance(lexer);

  TokenType type = TOKEN_INT;

  // Look for decimal point
  if (peek(lexer) == '.' && is_digit(peek_next(lexer))) {
    type = TOKEN_FLOAT;
    advance(lexer); // Consume .
    while (is_digit(peek(lexer)))
      advance(lexer);
  }

  return make_token(lexer, type);
}

static TokenType check_keyword(const char *start, int length, const char *rest,
                               TokenType type) {
  if (memcmp(start, rest, length) == 0) {
    return type;
  }
  return TOKEN_IDENTIFIER;
}

static TokenType identifier_type(Lexer *lexer) {
  int length = (int)(lexer->current - lexer->start);
  const char *start = lexer->start;

  switch (start[0]) {
  case 'a':
    return check_keyword(start + 1, length - 1, "nd", TOKEN_AND);
  case 'b':
    if (length > 1) {
      switch (start[1]) {
      case 'o':
        return check_keyword(start + 2, length - 2, "ol", TOKEN_TYPE_BOOL);
      case 'r':
        return check_keyword(start + 2, length - 2, "eak", TOKEN_BREAK);
      case 'y':
        return check_keyword(start + 2, length - 2, "te", TOKEN_TYPE_BYTE);
      }
    }
    break;
  case 'c':
    return check_keyword(start + 1, length - 1, "ontinue", TOKEN_CONTINUE);
  case 'd':
    return check_keyword(start + 1, length - 1, "efer", TOKEN_DEFER);
  case 'e':
    return check_keyword(start + 1, length - 1, "lse", TOKEN_ELSE);
  case 'f':
    if (length > 1) {
      switch (start[1]) {
      case 'a':
        return check_keyword(start + 2, length - 2, "lse", TOKEN_FALSE);
      case 'l':
        return check_keyword(start + 2, length - 2, "oat", TOKEN_TYPE_FLOAT);
      case 'o':
        return check_keyword(start + 2, length - 2, "r", TOKEN_FOR);
      }
    }
    break;
  case 'i':
    if (length > 1) {
      switch (start[1]) {
      case 'f':
        return length == 2 ? TOKEN_IF : TOKEN_IDENTIFIER;
      case 'm':
        return check_keyword(start + 2, length - 2, "port", TOKEN_IMPORT);
      case 'n':
        if (length == 2)
          return TOKEN_IN;
        return check_keyword(start + 2, length - 2, "t", TOKEN_TYPE_INT);
      }
    }
    break;
  case 'l':
    if (length > 1) {
      switch (start[1]) {
      case 'e':
        return check_keyword(start + 2, length - 2, "t", TOKEN_LET);
      case 'o':
        return check_keyword(start + 2, length - 2, "op", TOKEN_LOOP);
      }
    }
    break;
  case 'n':
    if (length > 1) {
      switch (start[1]) {
      case 'i':
        return check_keyword(start + 2, length - 2, "l", TOKEN_NIL);
      case 'o':
        return check_keyword(start + 2, length - 2, "t", TOKEN_NOT);
      }
    }
    break;
  case 'o':
    return check_keyword(start + 1, length - 1, "r", TOKEN_OR);
  case 'p':
    return check_keyword(start + 1, length - 1, "anic", TOKEN_PANIC);
  case 'r':
    return check_keyword(start + 1, length - 1, "eturn", TOKEN_RETURN);
  case 's':
    if (length > 1) {
      switch (start[1]) {
      case 'p':
        if (length > 2 && start[2] == 'a') {
          return check_keyword(start + 3, length - 3, "wn", TOKEN_SPAWN);
        }
        break;
      case 't':
        if (length > 2 && start[2] == 'r') {
          if (length > 3 && start[3] == 'i') {
            return check_keyword(start + 4, length - 4, "ng",
                                 TOKEN_TYPE_STRING);
          }
          return check_keyword(start + 3, length - 3, "uct", TOKEN_STRUCT);
        }
        break;
      }
    }
    break;
  case 't':
    if (length > 1) {
      switch (start[1]) {
      case 'h':
        return check_keyword(start + 2, length - 2, "en", TOKEN_THEN);
      case 'r':
        return check_keyword(start + 2, length - 2, "ue", TOKEN_TRUE);
      }
    }
    break;
  case 'v':
    return check_keyword(start + 1, length - 1, "oid", TOKEN_TYPE_VOID);
  case 'w':
    return check_keyword(start + 1, length - 1, "hile", TOKEN_WHILE);
  }

  return TOKEN_IDENTIFIER;
}

static Token identifier(Lexer *lexer) {
  while (is_alpha(peek(lexer)) || is_digit(peek(lexer))) {
    advance(lexer);
  }
  return make_token(lexer, identifier_type(lexer));
}

Token lexer_next_token(Lexer *lexer) {
  skip_whitespace(lexer);

  lexer->start = lexer->current;

  if (is_at_end(lexer)) {
    return make_token(lexer, TOKEN_EOF);
  }

  char c = advance(lexer);

  if (is_alpha(c))
    return identifier(lexer);
  if (is_digit(c))
    return number(lexer);

  switch (c) {
  case '(':
    return make_token(lexer, TOKEN_LEFT_PAREN);
  case ')':
    return make_token(lexer, TOKEN_RIGHT_PAREN);
  case '{':
    return make_token(lexer, TOKEN_LEFT_BRACE);
  case '}':
    return make_token(lexer, TOKEN_RIGHT_BRACE);
  case '[':
    return make_token(lexer, TOKEN_LEFT_BRACKET);
  case ']':
    return make_token(lexer, TOKEN_RIGHT_BRACKET);
  case ',':
    return make_token(lexer, TOKEN_COMMA);
  case '.':
    return make_token(lexer, match(lexer, '.') ? TOKEN_DOT_DOT : TOKEN_DOT);
  case ':':
    return make_token(lexer,
                      match(lexer, '=') ? TOKEN_COLON_EQUAL : TOKEN_COLON);
  case ';':
    return make_token(lexer, TOKEN_SEMICOLON);
  case '+':
    return make_token(lexer, match(lexer, '=') ? TOKEN_PLUS_EQUAL : TOKEN_PLUS);
  case '-':
    return make_token(lexer,
                      match(lexer, '=') ? TOKEN_MINUS_EQUAL : TOKEN_MINUS);
  case '*':
    return make_token(lexer, match(lexer, '=') ? TOKEN_STAR_EQUAL : TOKEN_STAR);
  case '/':
    return make_token(lexer,
                      match(lexer, '=') ? TOKEN_SLASH_EQUAL : TOKEN_SLASH);
  case '%':
    return make_token(lexer, TOKEN_PERCENT);
  case '&':
    return make_token(lexer, TOKEN_AMPERSAND);
  case '|':
    return make_token(lexer, TOKEN_PIPE);
  case '^':
    return make_token(lexer, TOKEN_CARET);
  case '!':
    return make_token(lexer, match(lexer, '=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
  case '=':
    return make_token(lexer,
                      match(lexer, '=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
  case '<':
    return make_token(lexer, match(lexer, '=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
  case '>':
    return make_token(lexer,
                      match(lexer, '=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
  case '"':
    return string(lexer);
  case '\n':
    lexer->line++;
    lexer->column = 1;
    return make_token(lexer, TOKEN_NEWLINE);
  }

  return error_token(lexer, "Unexpected character");
}

Token lexer_peek_token(Lexer *lexer) {
  Lexer saved = *lexer;
  Token token = lexer_next_token(lexer);
  *lexer = saved;
  return token;
}

void lexer_print_token(Token token) {
  printf("[%d:%d] ", token.line, token.column);

  switch (token.type) {
  case TOKEN_ERROR:
    printf("ERROR: %.*s\n", token.length, token.start);
    break;
  case TOKEN_EOF:
    printf("EOF\n");
    break;
  case TOKEN_NEWLINE:
    printf("NEWLINE\n");
    break;
  case TOKEN_IDENTIFIER:
    printf("IDENTIFIER: %.*s\n", token.length, token.start);
    break;
  case TOKEN_STRING:
    printf("STRING: %.*s\n", token.length, token.start);
    break;
  case TOKEN_INT:
    printf("INT: %.*s\n", token.length, token.start);
    break;
  case TOKEN_FLOAT:
    printf("FLOAT: %.*s\n", token.length, token.start);
    break;
  default:
    printf("TOKEN(%d): %.*s\n", token.type, token.length, token.start);
    break;
  }
}
