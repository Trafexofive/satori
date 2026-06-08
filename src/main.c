// src/main.c - Entry point

#include "backend/codegen.h"
#include "core/common.h"
#include "core/source.h"
#include "error/error.h"
#include "frontend/ast.h"
#include "frontend/lexer.h"
#include "frontend/parser.h"
#include "runtime/vm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// ── Styled help output ────────────────────────────────────────────

#define C_BOLD   "\033[1m"
#define C_DIM    "\033[90m"
#define C_GREEN  "\033[32m"
#define C_CYAN   "\033[36m"
#define C_YELLOW "\033[33m"
#define C_RESET  "\033[0m"

static bool use_color_stdout(void) {
    static int cached = -1;
    if (cached < 0) cached = isatty(STDOUT_FILENO) ? 1 : 0;
    return cached == 1;
}

#define c_help(code) (use_color_stdout() ? (code) : "")
#define with_color(code) (use_color_stdout())

static void print_usage(const char *program) {
    bool col = use_color_stdout();
    printf("\n");
    printf("%s    \U0001f338 satori %s%s\n",
           col ? C_BOLD C_GREEN : "", col ? C_RESET : "",
           col ? C_DIM "-- Clean. Fast. Explicit." C_RESET : "-- Clean. Fast. Explicit.");
    printf("\n");
    printf("%sUSAGE%s\n", c_help(C_BOLD), c_help(C_RESET));
    printf("  %s%s %s[options] %s<file>%s\n",
           c_help(C_DIM), program, c_help(C_RESET),
           c_help(C_YELLOW), c_help(C_RESET));
    printf("\n");
    printf("%sEXECUTION%s\n", c_help(C_BOLD), c_help(C_RESET));
    printf("  %s-i, --interpret%s   Run the program %s(default)%s\n",
           c_help(C_CYAN), c_help(C_RESET),
           c_help(C_DIM), c_help(C_RESET));
    printf("\n");
    printf("%sINSPECTION%s\n", c_help(C_BOLD), c_help(C_RESET));
    printf("  %s-t, --tokens%s     Show token stream from lexer\n",
           c_help(C_CYAN), c_help(C_RESET));
    printf("  %s-a, --ast%s        Dump Abstract Syntax Tree\n",
           c_help(C_CYAN), c_help(C_RESET));
    printf("\n");
    printf("%sINFO%s\n", c_help(C_BOLD), c_help(C_RESET));
    printf("  %s-h, --help%s       Show this help\n",
           c_help(C_CYAN), c_help(C_RESET));
    printf("  %s-v, --version%s    Show version info\n",
           c_help(C_CYAN), c_help(C_RESET));
    printf("\n");
    printf("%sEXAMPLES%s\n", c_help(C_BOLD), c_help(C_RESET));
    printf("  %ssatori hello.sat%s             Run a program\n",
           c_help(C_DIM), c_help(C_RESET));
    printf("  %ssatori -a hello.sat%s          Inspect the AST\n",
           c_help(C_DIM), c_help(C_RESET));
    printf("  %ssatori -t hello.sat | head%s   Debug the lexer\n",
           c_help(C_DIM), c_help(C_RESET));
    printf("  %ssatori -v%s                    Version info\n",
           c_help(C_DIM), c_help(C_RESET));
    printf("\n");
    printf("%sFor more see %sdocs/ %sand %sexamples/%s\n",
           c_help(C_DIM),
           c_help(C_YELLOW), c_help(C_DIM),
           c_help(C_YELLOW), c_help(C_RESET));
    printf("\n");
}

static void print_version(void) {
    printf("satori %d.%d.%d\n", SATORI_VERSION_MAJOR, SATORI_VERSION_MINOR,
           SATORI_VERSION_PATCH);
    printf("Built from scratch with focus 🌸\n");
    printf("Error system: Rust-style diagnostics v1 (diag_emit)\n");
    printf("Standard: C99, GCC\n");
}

static char *read_file(const char *path) {
    FILE *file = fopen(path, "rb");
    if (!file) {
        error_report_simple("Could not open file '%s'", path);
        return NULL;
    }

    fseek(file, 0L, SEEK_END);
    size_t file_size = ftell(file);
    rewind(file);

    char *buffer = (char *)malloc(file_size + 1);
    if (!buffer) {
        fclose(file);
        error_report_simple("Not enough memory to read '%s'", path);
        return NULL;
    }

    size_t bytes_read = fread(buffer, sizeof(char), file_size, file);
    if (bytes_read < file_size) {
        free(buffer);
        fclose(file);
        error_report_simple("Could not read file '%s'", path);
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
        } else if (strcmp(argv[i], "-t") == 0 ||
                   strcmp(argv[i], "--tokens") == 0) {
            dump_tokens_only = true;
        } else if (strcmp(argv[i], "-a") == 0 ||
                   strcmp(argv[i], "--ast") == 0) {
            dump_ast_only = true;
        } else if (strcmp(argv[i], "-i") == 0 ||
                   strcmp(argv[i], "--interpret") == 0) {
            // Default mode
        } else if (argv[i][0] != '-') {
            file_path = argv[i];
        } else {
            diag_emit(LEVEL_ERROR, NULL, 0, 0, 0,
                      "Unknown option", NULL, NULL, 0);
            print_usage(argv[0]);
            return 1;
        }
    }

    if (!file_path) {
        diag_emit(LEVEL_ERROR, NULL, 0, 0, 0,
                  "No input file specified", NULL, NULL, 0);
        print_usage(argv[0]);
        return 1;
    }

    char *source = read_file(file_path);
    if (!source) {
        return 1;
    }

    // Build SourceInfo for error display
    SourceInfo source_info;
    source_init(&source_info, file_path, source);

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
        parser_set_source(&parser, &source_info);

        AstNode *program = parser_parse(&parser);
        if (!program) {
            source_free(&source_info);
            return 1;
        }

        VM vm;
        vm_init(&vm);
        vm_set_source(&vm, &source_info);

        if (!codegen_compile(program, &vm.chunk, &source_info)) {
            ast_free(program);
            vm_free(&vm);
            return 1;
        }

        ast_free(program);

        bool success = vm_run(&vm);
        vm_free(&vm);

        if (!success) {
            return 1;
        }
        // source freed by vm_free -> source_free
        return 0;
    }

    // Dump paths: clean up source_info manually
    source_free(&source_info);
    return 0;
}
