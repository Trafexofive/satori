// tests/runner.c - Fixed test harness

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  const char *name;
  bool (*test_fn)(void);
} Test;

static int tests_run = 0;
static int tests_passed = 0;
static int tests_failed = 0;

#define TEST(name) bool test_##name(void)
#define RUN_TEST(name) run_test(#name, test_##name)

static void run_test(const char *name, bool (*test_fn)(void)) {
  tests_run++;
  printf("Running %s... ", name);
  fflush(stdout);

  if (test_fn()) {
    printf("\033[1;32mPASS\033[0m\n");
    tests_passed++;
  } else {
    printf("\033[1;31mFAIL\033[0m\n");
    tests_failed++;
  }
}

// Test utilities - different names to avoid conflicts
#define TEST_ASSERT(cond)                                                      \
  do {                                                                         \
    if (!(cond)) {                                                             \
      fprintf(stderr, "  Assertion failed: %s at %s:%d\n", #cond, __FILE__,    \
              __LINE__);                                                       \
      return false;                                                            \
    }                                                                          \
  } while (0)

#define TEST_ASSERT_EQ(a, b)                                                   \
  do {                                                                         \
    if ((a) != (b)) {                                                          \
      fprintf(stderr, "  Expected %s == %s at %s:%d\n", #a, #b, __FILE__,      \
              __LINE__);                                                       \
      return false;                                                            \
    }                                                                          \
  } while (0)

#define TEST_ASSERT_STR_EQ(a, b)                                               \
  do {                                                                         \
    if (strcmp((a), (b)) != 0) {                                               \
      fprintf(stderr, "  Expected '%s' == '%s' at %s:%d\n", (a), (b),          \
              __FILE__, __LINE__);                                             \
      return false;                                                            \
    }                                                                          \
  } while (0)

// Include test files
#include "test_lexer.c"
#include "test_parser.c"

int main(void) {
  printf("=== Satori Test Suite ===\n\n");

  // Lexer tests
  printf("--- Lexer Tests ---\n");
  RUN_TEST(lexer_keywords);
  RUN_TEST(lexer_identifiers);
  RUN_TEST(lexer_numbers);
  RUN_TEST(lexer_strings);
  RUN_TEST(lexer_operators);

  // Parser tests
  printf("\n--- Parser Tests ---\n");
  RUN_TEST(parser_import);
  RUN_TEST(parser_simple_call);
  RUN_TEST(parser_member_access);

  // Summary
  printf("\n=== Summary ===\n");
  printf("Tests run: %d\n", tests_run);
  printf("\033[1;32mPassed: %d\033[0m\n", tests_passed);
  if (tests_failed > 0) {
    printf("\033[1;31mFailed: %d\033[0m\n", tests_failed);
  } else {
    printf("Failed: %d\n", tests_failed);
  }

  return tests_failed == 0 ? 0 : 1;
}
