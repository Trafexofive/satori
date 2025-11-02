// tests/test_parser.c - Fixed with unique test names

#include "../src/parser.h"

TEST(parser_import) {
  const char *source = "import io";
  Lexer lexer;
  lexer_init(&lexer, source);

  Parser parser;
  parser_init(&parser, &lexer, "test");

  AstNode *ast = parser_parse(&parser);
  TEST_ASSERT(ast != NULL);
  TEST_ASSERT_EQ(ast->type, AST_PROGRAM);
  TEST_ASSERT_EQ(ast->as.program.statement_count, 1);
  TEST_ASSERT_EQ(ast->as.program.statements[0]->type, AST_IMPORT);
  TEST_ASSERT_STR_EQ(ast->as.program.statements[0]->as.import.module_name,
                     "io");

  ast_free(ast);
  return true;
}

TEST(parser_simple_call) {
  const char *source = "println \"hello\"";
  Lexer lexer;
  lexer_init(&lexer, source);

  Parser parser;
  parser_init(&parser, &lexer, "test");

  AstNode *ast = parser_parse(&parser);
  TEST_ASSERT(ast != NULL);
  TEST_ASSERT_EQ(ast->type, AST_PROGRAM);
  TEST_ASSERT_EQ(ast->as.program.statement_count, 1);

  AstNode *call = ast->as.program.statements[0];
  TEST_ASSERT_EQ(call->type, AST_CALL);
  TEST_ASSERT_EQ(call->as.call.arg_count, 1);

  ast_free(ast);
  return true;
}

TEST(parser_member_access) {
  const char *source = "io.println \"test\"";
  Lexer lexer;
  lexer_init(&lexer, source);

  Parser parser;
  parser_init(&parser, &lexer, "test");

  AstNode *ast = parser_parse(&parser);
  TEST_ASSERT(ast != NULL);

  AstNode *call = ast->as.program.statements[0];
  TEST_ASSERT_EQ(call->type, AST_CALL);
  TEST_ASSERT_EQ(call->as.call.callee->type, AST_MEMBER_ACCESS);

  ast_free(ast);
  return true;
}
