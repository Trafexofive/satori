// src/main.c - Entry point

#include "frontend/ast.h"
#include "backend/codegen.h"
#include "core/common.h"
#include "frontend/lexer.h"
#include "frontend/parser.h"
#include "runtime/vm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void print_usage(const char *program) {
  printf("Usage: %s [options] <file>\n", program);
  printf("Options:\n");
  printf("  -h, --help       Show this help message\n");
  printf("  -v, --version    Show version\n");
  printf("  -t, --tokens     Dump tokens only\n");
  printf("  -a, --ast        Dump AST only\n");
  printf("  -i, --interpret  Interpret mode (default)\n");
  printf("\n");
}

static void print_version(void) {
  printf("satori %d.%d.%d\n", SATORI_VERSION_MAJOR, SATORI_VERSION_MINOR,
         SATORI_VERSION_PATCH);
  printf("Built from scratch with focus 🌸\n");
}

static char *read_file(const char *path) {
  FILE *file = fopen(path, "rb");
  if (!file) {
    fprintf(stderr, "Error: Could not open file '%s'\n", path);
    return NULL;
  }

  fseek(file, 0L, SEEK_END);
  size_t file_size = ftell(file);
  rewind(file);

  char *buffer = (char *)malloc(file_size + 1);
  if (!buffer) {
    fprintf(stderr, "Error: Not enough memory to read '%s'\n", path);
    fclose(file);
    return NULL;
  }

  size_t bytes_read = fread(buffer, sizeof(char), file_size, file);
  if (bytes_read < file_size) {
    fprintf(stderr, "Error: Could not read file '%s'\n", path);
    free(buffer);
    fclose(file);
    return NULL;
  }

  buffer[bytes_read] = '\0';
  fclose(file);
  return buffer;
}

static void dump_tokens(const char *source) {
  Lexer lexer;
  lexer_init(&lexer, source);

  Token token;
  do {
    token = lexer_next_token(&lexer);
    lexer_print_token(token);
  } while (token.type != TOKEN_EOF);
}

static void dump_ast(const char *source, const char *file_path) {
  Lexer lexer;
  lexer_init(&lexer, source);

  Parser parser;
  parser_init(&parser, &lexer, file_path);

  AstNode *program = parser_parse(&parser);
  if (program) {
    ast_print(program, 0);
    ast_free(program);
  } else {
    fprintf(stderr, "Parse failed\n");
  }
}

int main(int argc, char **argv) {
  if (argc < 2) {
    print_usage(argv[0]);
    return 1;
  }

  bool dump_tokens_only = false;
  bool dump_ast_only = false;
  const char *file_path = NULL;

  // Parse arguments
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
      print_usage(argv[0]);
      return 0;
    } else if (strcmp(argv[i], "-v") == 0 ||
               strcmp(argv[i], "--version") == 0) {
      print_version();
      return 0;
    } else if (strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "--tokens") == 0) {
      dump_tokens_only = true;
    } else if (strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "--ast") == 0) {
      dump_ast_only = true;
    } else if (strcmp(argv[i], "-i") == 0 ||
               strcmp(argv[i], "--interpret") == 0) {
      // Default mode
    } else if (argv[i][0] != '-') {
      file_path = argv[i];
    } else {
      fprintf(stderr, "Unknown option: %s\n", argv[i]);
      print_usage(argv[0]);
      return 1;
    }
  }

  if (!file_path) {
    fprintf(stderr, "Error: No input file specified\n");
    print_usage(argv[0]);
    return 1;
  }

  char *source = read_file(file_path);
  if (!source) {
    return 1;
  }

  if (dump_tokens_only) {
    dump_tokens(source);
  } else if (dump_ast_only) {
    dump_ast(source, file_path);
  } else {
    // Full interpretation
    Lexer lexer;
    lexer_init(&lexer, source);

    Parser parser;
    parser_init(&parser, &lexer, file_path);

    AstNode *program = parser_parse(&parser);
    if (!program) {
      free(source);
      return 1;
    }

    VM vm;
    vm_init(&vm);

    if (!codegen_compile(program, &vm.chunk)) {
      ast_free(program);
      vm_free(&vm);
      free(source);
      return 1;
    }

    ast_free(program);

    bool success = vm_run(&vm);
    vm_free(&vm);

    if (!success) {
      free(source);
      return 1;
    }
  }

  free(source);
  return 0;
}
