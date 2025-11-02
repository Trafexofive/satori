// tests/test_lexer.c - Fixed to use TEST_ASSERT macros

#include "../src/lexer.h"

TEST(lexer_keywords) {
  const char *source = "import let if else for";
  Lexer lexer;
  lexer_init(&lexer, source);

  Token t1 = lexer_next_token(&lexer);
  TEST_ASSERT_EQ(t1.type, TOKEN_IMPORT);

  Token t2 = lexer_next_token(&lexer);
  TEST_ASSERT_EQ(t2.type, TOKEN_LET);

  Token t3 = lexer_next_token(&lexer);
  TEST_ASSERT_EQ(t3.type, TOKEN_IF);

  Token t4 = lexer_next_token(&lexer);
  TEST_ASSERT_EQ(t4.type, TOKEN_ELSE);

  Token t5 = lexer_next_token(&lexer);
  TEST_ASSERT_EQ(t5.type, TOKEN_FOR);

  return true;
}

TEST(lexer_identifiers) {
  const char *source = "foo bar_123 _test";
  Lexer lexer;
  lexer_init(&lexer, source);

  Token t1 = lexer_next_token(&lexer);
  TEST_ASSERT_EQ(t1.type, TOKEN_IDENTIFIER);
  TEST_ASSERT_EQ(t1.length, 3);

  Token t2 = lexer_next_token(&lexer);
  TEST_ASSERT_EQ(t2.type, TOKEN_IDENTIFIER);
  TEST_ASSERT_EQ(t2.length, 7);

  Token t3 = lexer_next_token(&lexer);
  TEST_ASSERT_EQ(t3.type, TOKEN_IDENTIFIER);
  TEST_ASSERT_EQ(t3.length, 5);

  return true;
}

TEST(lexer_numbers) {
  const char *source = "42 3.14 0";
  Lexer lexer;
  lexer_init(&lexer, source);

  Token t1 = lexer_next_token(&lexer);
  TEST_ASSERT_EQ(t1.type, TOKEN_INT);

  Token t2 = lexer_next_token(&lexer);
  TEST_ASSERT_EQ(t2.type, TOKEN_FLOAT);

  Token t3 = lexer_next_token(&lexer);
  TEST_ASSERT_EQ(t3.type, TOKEN_INT);

  return true;
}

TEST(lexer_strings) {
  const char *source = "\"hello\" \"world\"";
  Lexer lexer;
  lexer_init(&lexer, source);

  Token t1 = lexer_next_token(&lexer);
  TEST_ASSERT_EQ(t1.type, TOKEN_STRING);
  TEST_ASSERT_EQ(t1.length, 7);

  Token t2 = lexer_next_token(&lexer);
  TEST_ASSERT_EQ(t2.type, TOKEN_STRING);
  TEST_ASSERT_EQ(t2.length, 7);

  return true;
}

TEST(lexer_operators) {
  const char *source = "+ - * / := == !=";
  Lexer lexer;
  lexer_init(&lexer, source);

  Token t1 = lexer_next_token(&lexer);
  TEST_ASSERT_EQ(t1.type, TOKEN_PLUS);

  Token t2 = lexer_next_token(&lexer);
  TEST_ASSERT_EQ(t2.type, TOKEN_MINUS);

  Token t3 = lexer_next_token(&lexer);
  TEST_ASSERT_EQ(t3.type, TOKEN_STAR);

  Token t4 = lexer_next_token(&lexer);
  TEST_ASSERT_EQ(t4.type, TOKEN_SLASH);

  Token t5 = lexer_next_token(&lexer);
  TEST_ASSERT_EQ(t5.type, TOKEN_COLON_EQUAL);

  Token t6 = lexer_next_token(&lexer);
  TEST_ASSERT_EQ(t6.type, TOKEN_EQUAL_EQUAL);

  Token t7 = lexer_next_token(&lexer);
  TEST_ASSERT_EQ(t7.type, TOKEN_BANG_EQUAL);

  return true;
}
