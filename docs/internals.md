# Satori Internals

**Version:** 0.1.0-alpha  
**Status:** Development

## Architecture Overview

Satori is implemented as a bytecode interpreter written in C. The architecture follows a traditional compilation pipeline with distinct phases: lexical analysis, parsing, semantic analysis (type checking), code generation, and execution.

```
Source Code (.sat)
    ↓
[Lexer] → Tokens
    ↓
[Parser] → AST (Abstract Syntax Tree)
    ↓
[Type Checker] → Typed AST
    ↓
[Code Generator] → Bytecode
    ↓
[Virtual Machine] → Execution
```

## Directory Structure

Satori uses a modular, layered architecture with clean separation of concerns:

```
satori/
├── src/                    # Core implementation
│   ├── core/              # Layer 0: Foundation
│   │   ├── common.h       # Common type definitions
│   │   ├── value.c/h      # Value representation
│   │   ├── object.c/h     # Heap objects (strings, etc.)
│   │   └── memory.c/h     # Memory management & GC
│   ├── frontend/          # Layer 1: Source processing
│   │   ├── lexer.c/h      # Lexical analyzer
│   │   ├── parser.c/h     # Syntax parser
│   │   ├── ast.c/h        # AST data structures
│   │   └── typechecker.c/h # Type checking (stub)
│   ├── backend/           # Layer 2: Code generation
│   │   └── codegen.c/h    # AST to bytecode compiler
│   ├── runtime/           # Layer 3: Execution
│   │   └── vm.c/h         # Stack-based virtual machine
│   ├── error/             # Cross-cutting: Diagnostics
│   │   └── error.c/h      # Error reporting
│   ├── common.h           # Legacy common header
│   └── main.c             # Entry point
├── stdlib/                # Layer 4: Standard library (stubs)
├── tests/                 # Test suite
├── examples/              # Example programs
├── docs/                  # Documentation
├── notes/                 # Development resources
├── scripts/               # Build and utility scripts
└── build/                 # Build artifacts (generated)
```

### Architectural Layers

The codebase follows a strict layering discipline:

**Layer 0 - Core** (`src/core/`)
- Value representation and type system
- Object model for heap-allocated data
- Memory management and garbage collection
- No dependencies on other layers

**Layer 1 - Frontend** (`src/frontend/`)
- Lexical analysis (tokenization)
- Syntax analysis (parsing to AST)
- Semantic analysis (type checking)
- Depends only on Layer 0

**Layer 2 - Backend** (`src/backend/`)
- Code generation (AST to bytecode)
- Optimization passes (future)
- Depends on Layers 0 and 1

**Layer 3 - Runtime** (`src/runtime/`)
- Virtual machine execution
- Bytecode interpretation
- Stack management
- Depends on Layers 0 and 2

**Cross-cutting - Error** (`src/error/`)
- Error reporting and diagnostics
- Can be used by any layer

**Layer 4 - Standard Library** (`stdlib/`)
- Built-in modules (io, net, fs, etc.)
- Implemented in C, exposed to Satori
- Depends on all lower layers

This design ensures:
- ✅ No circular dependencies
- ✅ Clear module boundaries
- ✅ Independent testing
- ✅ Easy to extend and modify

## Core Components

### 1. Lexer (src/frontend/lexer.c/h)

The lexer performs lexical analysis, converting source text into a stream of tokens.

**Location:** `src/frontend/lexer.c` and `src/frontend/lexer.h`

#### Token Types

Defined in `TokenType` enum in `src/frontend/lexer.h`:
- **Single-character**: `(`, `)`, `{`, `}`, `[`, `]`, `,`, `.`, `:`, `;`
- **Operators**: `+`, `-`, `*`, `/`, `%`, `&`, `|`, `^`, `!`, `=`, `<`, `>`
- **Two-character**: `==`, `!=`, `<=`, `>=`, `:=`, `..`, `->`
- **Compound assignment**: `+=`, `-=`, `*=`, `/=`
- **Literals**: `IDENTIFIER`, `STRING`, `INT`, `FLOAT`
- **Keywords**: `if`, `else`, `for`, `while`, `let`, `struct`, etc.
- **Type keywords**: `int`, `float`, `bool`, `string`, `void`, `byte`

#### Lexer State

```c
typedef struct {
  const char *start;      // Start of current token
  const char *current;    // Current position
  int line;               // Current line number
  int column;             // Current column
} Lexer;
```

#### Key Functions

- `void lexer_init(Lexer *lexer, const char *source)` - Initialize lexer
- `Token lexer_next_token(Lexer *lexer)` - Get next token
- `static Token make_token(Lexer *lexer, TokenType type)` - Create token
- `static bool is_at_end(Lexer *lexer)` - Check EOF
- `static char advance(Lexer *lexer)` - Consume character
- `static bool match(Lexer *lexer, char expected)` - Conditional advance

#### Implementation Details

The lexer uses a single-pass scan with lookahead. It handles:
- **Whitespace**: Skipped, but newlines are significant
- **Comments**: `//` single-line and `/* */` multi-line
- **String literals**: Escape sequences (`\n`, `\t`, `\\`, `\"`)
- **Number literals**: Integers and floats (including scientific notation)
- **Keyword recognition**: Hash table lookup for identifiers

---

### 2. Parser (src/frontend/parser.c/h)

Recursive descent parser that builds an Abstract Syntax Tree (AST) from tokens.

**Location:** `src/frontend/parser.c` and `src/frontend/parser.h`

#### Parser State

```c
typedef struct {
  Lexer *lexer;
  Token current;          // Current token
  Token previous;         // Previous token
  bool had_error;         // Error flag
  const char *file_path;  // Source file path
} Parser;
```

#### Grammar

The parser implements a recursive descent parser based on the grammar defined in `spec.md`. Key parsing functions:

- `AstNode *parse_program(Parser *p)` - Entry point
- `AstNode *parse_statement(Parser *p)` - Statement dispatcher
- `AstNode *parse_expression(Parser *p)` - Expression entry
- `AstNode *parse_primary(Parser *p)` - Literals and identifiers
- `AstNode *parse_call(Parser *p)` - Function/method calls
- `AstNode *parse_member_access(Parser *p)` - Dot notation

#### Operator Precedence

Implemented via recursive descent with precedence climbing:

1. Assignment (`=`, `:=`, `+=`, etc.)
2. Logical OR (`or`)
3. Logical AND (`and`)
4. Equality (`==`, `!=`)
5. Comparison (`<`, `>`, `<=`, `>=`)
6. Term (`+`, `-`)
7. Factor (`*`, `/`, `%`)
8. Unary (`not`, `-`, `&`, `*`)
9. Call (function calls, member access)
10. Primary (literals, identifiers, parentheses)

---

### 3. AST (src/frontend/ast.c/h)

Abstract Syntax Tree representation of the parsed program.

**Location:** `src/frontend/ast.c` and `src/frontend/ast.h`

#### Node Types

```c
typedef enum {
  AST_PROGRAM,
  AST_IMPORT,
  AST_CALL,
  AST_MEMBER_ACCESS,
  AST_IDENTIFIER,
  AST_STRING_LITERAL,
  AST_INT_LITERAL,
  AST_FLOAT_LITERAL,
  // ... more to come
} AstNodeType;
```

#### Node Structure

```c
struct AstNode {
  AstNodeType type;
  int line;
  int column;
  
  union {
    AstProgram program;
    AstImport import;
    AstCall call;
    AstMemberAccess member_access;
    AstIdentifier identifier;
    AstStringLiteral string_literal;
    AstIntLiteral int_literal;
    AstFloatLiteral float_literal;
  } as;
};
```

Each node variant has its own struct:

```c
typedef struct {
  char *module_name;
} AstImport;

typedef struct {
  AstNode *callee;
  AstNode **args;
  int arg_count;
} AstCall;

typedef struct {
  AstNode *object;
  char *member;
} AstMemberAccess;
```

#### Constructor Functions

All AST nodes are created via constructor functions:

```c
AstNode *ast_make_program(void);
AstNode *ast_make_import(char *module_name, int line, int column);
AstNode *ast_make_call(AstNode *callee, AstNode **args, int arg_count, 
                       int line, int column);
// ... etc
```

#### Memory Management

- Nodes are heap-allocated
- `ast_free(AstNode *node)` recursively frees the entire tree
- String data is duplicated and owned by the AST

---

### 4. Type Checker (src/frontend/typechecker.c/h)

**Status:** Planned, not yet implemented

**Location:** `src/frontend/typechecker.c` and `src/frontend/typechecker.h`

The type checker will:
- Verify type correctness
- Resolve type inference (`:=`)
- Check struct member access
- Validate function signatures
- Detect type mismatches

Will annotate AST nodes with resolved types.

---

### 5. Code Generator (src/backend/codegen.c/h)

Emits bytecode from the AST.

**Location:** `src/backend/codegen.c` and `src/backend/codegen.h`

#### Compiler State

```c
typedef struct {
  Chunk *chunk;       // Output bytecode chunk
  bool had_error;     // Error flag
} Compiler;
```

#### Code Generation

The compiler walks the AST and emits bytecode:

```c
static void compile_node(Compiler *c, AstNode *node) {
  switch (node->type) {
    case AST_PROGRAM:
      compile_program(c, node);
      break;
    case AST_IMPORT:
      compile_import(c, node);
      break;
    case AST_CALL:
      compile_call(c, node);
      break;
    // ... etc
  }
}
```

#### Special Cases

**io.println optimization**: Directly compiles to `OP_PRINT` instead of generic function call:

```c
static void compile_call(Compiler *c, AstNode *node) {
  // Check if this is io.println
  if (is_io_println(call->callee)) {
    for (int i = 0; i < call->arg_count; i++) {
      compile_node(c, call->args[i]);
    }
    emit_bytes(c, OP_PRINT, call->arg_count);
    return;
  }
  // Generic call handling...
}
```

---

### 6. Virtual Machine (src/runtime/vm.c/h)

Stack-based bytecode interpreter.

**Location:** `src/runtime/vm.c` and `src/runtime/vm.h`

#### VM State

```c
typedef struct {
  Chunk chunk;                        // Bytecode
  u8 *ip;                             // Instruction pointer
  Value stack[SATORI_STACK_MAX];      // Value stack
  int stack_top;                      // Stack top index
} VM;
```

#### Bytecode Instructions

```c
typedef enum {
  OP_CONSTANT,    // Load constant onto stack
  OP_POP,         // Pop value from stack
  OP_CALL,        // Call function
  OP_IMPORT,      // Import module
  OP_GET_MEMBER,  // Get struct/module member
  OP_PRINT,       // Built-in print
  OP_RETURN,      // Return from function
  OP_HALT,        // Stop execution
} OpCode;
```

#### Value Representation

**Now in:** `src/core/value.c/h`

Values are represented using a tagged union with support for multiple types:

```c
typedef enum {
  VALUE_NIL,
  VALUE_BOOL,
  VALUE_INT,
  VALUE_FLOAT,
  VALUE_STRING,      // Simple string (will use ObjString later)
  VALUE_NATIVE_FN,   // Native C function
  VALUE_OBJ,         // Heap-allocated object
} ValueType;

struct Value {
  ValueType type;
  union {
    bool as_bool;
    i64 as_int;
    f64 as_float;
    char *as_string;      // For VALUE_STRING
    NativeFn as_native_fn; // For VALUE_NATIVE_FN
    Object *as_obj;       // For VALUE_OBJ
  } u;
};
```

Value operations (constructors, printing, freeing) are in `src/core/value.c`.

#### Execution Loop

The VM runs a fetch-decode-execute loop:

```c
bool vm_run(VM *vm) {
  for (;;) {
    u8 instruction = READ_BYTE();
    
    switch (instruction) {
      case OP_CONSTANT: {
        u8 constant = READ_BYTE();
        push(vm->chunk.constants[constant]);
        break;
      }
      
      case OP_PRINT: {
        u8 arg_count = READ_BYTE();
        for (int i = 0; i < arg_count; i++) {
          Value val = pop();
          value_print(val);
        }
        printf("\n");
        break;
      }
      
      case OP_HALT:
        return true;
      
      // ... other opcodes
    }
  }
}
```

#### Stack Operations

```c
static void push(VM *vm, Value value) {
  vm->stack[vm->stack_top++] = value;
}

static Value pop(VM *vm) {
  return vm->stack[--vm->stack_top];
}

static Value peek(VM *vm, int distance) {
  return vm->stack[vm->stack_top - 1 - distance];
}
```

---

### 7. Memory Management (src/core/memory.c/h)

**Location:** `src/core/memory.c` and `src/core/memory.h`

Custom memory allocator for the VM.

**Status:** Stub - basic malloc/free wrappers currently

#### Allocation Functions

```c
void *mem_alloc(size_t size);
void *mem_realloc(void *ptr, size_t new_size);
void mem_free(void *ptr);
```

#### Growth Strategy

Dynamic arrays (chunks, constant pools) use exponential growth:

```c
#define GROW_CAPACITY(capacity) \
  ((capacity) < 8 ? 8 : (capacity) * 2)

#define GROW_ARRAY(type, pointer, old_count, new_count) \
  (type*)mem_realloc(pointer, sizeof(type) * (new_count))
```

#### Garbage Collection

**Status:** Planned

Future GC will use mark-and-sweep algorithm:
1. Mark phase: Trace reachable objects from roots
2. Sweep phase: Free unmarked objects

---

### 8. Object System (src/core/object.c/h)

**Location:** `src/core/object.c` and `src/core/object.h`

Heap-allocated objects with a common header.

#### Object Types

```c
typedef enum {
  OBJ_STRING,    // Heap-allocated string
  OBJ_ARRAY,     // Dynamic array (future)
  OBJ_TABLE,     // Hash table (future)
  OBJ_FUNCTION,  // User function (future)
} ObjectType;
```

#### Object Header

```c
typedef struct Object {
  ObjectType type;
  bool marked;           // For GC
  struct Object *next;   // Intrusive list for GC
} Object;
```

#### String Objects

```c
typedef struct {
  Object obj;
  int length;
  char *chars;
  u32 hash;
} ObjString;
```

Strings are interned for efficient comparison and reduced memory usage.

---

### 9. Error Reporting (src/error/error.c/h)

**Location:** `src/error/error.c` and `src/error/error.h`

Unified error reporting with source location.

#### Error Functions

```c
void error_report(const char *file_path, int line, int column, 
                  const char *format, ...);
void error_report_simple(const char *format, ...);
```

#### Error Format

```
error: <file>:<line>:<column>: <message>
  10 | io.println "Hello, {}, name
     |                          ^
     | expected closing quote
```

---

## Compilation Pipeline

### Phase 1: Lexical Analysis

```c
Lexer lexer;
lexer_init(&lexer, source_code);

Token token;
do {
  token = lexer_next_token(&lexer);
  // Process token
} while (token.type != TOKEN_EOF);
```

### Phase 2: Parsing

```c
Parser parser;
parser_init(&parser, &lexer, file_path);

AstNode *ast = parse_program(&parser);

if (parser.had_error) {
  ast_free(ast);
  return false;
}
```

### Phase 3: Type Checking

**Status:** Not yet implemented

```c
TypeChecker checker;
typechecker_init(&checker);

bool success = typecheck(&checker, ast);
if (!success) {
  return false;
}
```

### Phase 4: Code Generation

```c
VM vm;
vm_init(&vm);

Compiler compiler;
compiler_init(&compiler, &vm.chunk);

compile(&compiler, ast);

if (compiler.had_error) {
  return false;
}
```

### Phase 5: Execution

```c
bool success = vm_run(&vm);

vm_free(&vm);
ast_free(ast);

return success;
```

---

## Data Structures

### Chunk (Bytecode Container)

```c
typedef struct {
  u8 *code;                   // Bytecode array
  int count;                  // Number of bytes
  int capacity;               // Allocated capacity
  Value *constants;           // Constant pool
  int constant_count;         // Number of constants
  int constant_capacity;      // Constant pool capacity
} Chunk;
```

Operations:
- `chunk_write(chunk, byte)` - Append byte
- `chunk_add_constant(chunk, value)` - Add constant, return index

### Value Stack

Fixed-size stack for VM execution:

```c
#define SATORI_STACK_MAX 256

Value stack[SATORI_STACK_MAX];
int stack_top;
```

**Future:** Dynamic stack with overflow checking.

---

## Build System

### Modular Makefile

The Makefile supports modular compilation with layer-based organization.

**Key Features:**
- Separate compilation of each module
- Automatic dependency management
- Support for debug and release builds  
- Install/uninstall targets
- Example runner targets

### Build Process

```makefile
# Layer 0: Core (no dependencies)
build/core/%.o: src/core/%.c
    gcc $(CFLAGS) -Isrc -c $< -o $@

# Layer 1: Frontend (depends on core)
build/frontend/%.o: src/frontend/%.c
    gcc $(CFLAGS) -Isrc -c $< -o $@

# Layer 2: Backend (depends on core, frontend)
build/backend/%.o: src/backend/%.c
    gcc $(CFLAGS) -Isrc -c $< -o $@

# Layer 3: Runtime (depends on core, backend)
build/runtime/%.o: src/runtime/%.c
    gcc $(CFLAGS) -Isrc -c $< -o $@
```

All includes use `-Isrc` prefix, so headers are referenced as:
- `#include "core/value.h"`
- `#include "frontend/lexer.h"`
- `#include "runtime/vm.h"`

### Makefile Targets

- `make` - Build release binary
- `make debug` - Build with debug symbols
- `make clean` - Remove build artifacts
- `make install` - Install to `/usr/local/bin`
- `make uninstall` - Remove from `/usr/local/bin`
- `make run-hello` - Build and run hello.sat example
- `make test-lexer` - Test lexer only (future)

### Compiler Flags

**Release:**
```
-O3 -DNDEBUG -std=c99 -Wall -Wextra -pedantic
```

**Debug:**
```
-g -O0 -DDEBUG -std=c99 -Wall -Wextra -pedantic
```

### Dependencies

- C99 compiler (GCC or Clang)
- Standard C library
- Math library (`-lm`)
- POSIX system (Linux, BSD, macOS)

---

## Testing

### Test Runner

Located in `tests/runner.c`. Links against all source files except `main.c`.

### Test Organization

```c
void test_lexer_tokens(void);
void test_parser_literals(void);
void test_vm_execution(void);
// ... more tests
```

### Running Tests

```bash
make test
./bin/test_runner
```

---

## Debug Features

### Token Dumping

```bash
./satori -t examples/hello.sat
```

Outputs:
```
TOKEN_IMPORT "import" (line 1, col 1)
TOKEN_IDENTIFIER "io" (line 1, col 8)
TOKEN_NEWLINE "\n" (line 1, col 10)
...
```

### AST Dumping

```bash
./satori -a examples/hello.sat
```

Outputs tree structure:
```
PROGRAM
  IMPORT
    module: io
  CALL
    callee: MEMBER_ACCESS
      object: IDENTIFIER(io)
      member: println
    args:
      STRING_LITERAL("Hello, World!")
```

---

## Performance Considerations

### Current State

As an early alpha interpreter, performance is not yet optimized. Focus is on correctness and feature completeness.

### Future Optimizations

1. **Constant folding** - Evaluate constant expressions at compile time
2. **Jump threading** - Optimize control flow
3. **Inline caching** - Cache method lookups
4. **JIT compilation** - Compile hot paths to native code (future v2.0+)
5. **Incremental GC** - Reduce pause times

### Memory Usage

Current implementation uses simple allocation:
- AST: ~100 bytes per node
- VM stack: 2KB (256 values × 8 bytes)
- Bytecode: ~1 byte per operation + constant pool

---

## Contributing

### Code Style

- **Indentation**: 2 spaces (no tabs)
- **Line length**: 80 characters max
- **Naming**: 
  - Functions: `snake_case`
  - Types: `PascalCase` 
  - Macros: `UPPER_CASE`
- **Comments**: `//` for single-line, `/* */` for multi-line

### Adding Features

1. Update grammar in `docs/spec.md`
2. Add token types in `src/frontend/lexer.h`
3. Implement lexing in `src/frontend/lexer.c`
4. Add AST node type in `src/frontend/ast.h`
5. Implement parsing in `src/frontend/parser.c`
6. Add opcode in `src/runtime/vm.h` (if needed)
7. Implement codegen in `src/backend/codegen.c`
8. Implement VM execution in `src/runtime/vm.c`
9. Add tests in `tests/`
10. Update documentation

### Module Boundaries

When adding features, respect the layer boundaries:
- **Layer 0 (core)** - No dependencies on other layers
- **Layer 1 (frontend)** - May import from core only
- **Layer 2 (backend)** - May import from core and frontend
- **Layer 3 (runtime)** - May import from core and backend
- **Error module** - May be used by any layer (cross-cutting)

Do not create circular dependencies between modules.

---

## Implementation Status

### ✅ Completed
- Lexer with full tokenization
- Parser for basic constructs (imports, calls, member access)
- AST representation
- Bytecode generation with optimizations
- Stack-based VM execution
- Value system with multiple types
- Object system with string interning
- Modular architecture (Nov 2024)

### 🚧 In Progress
- Type checker (stub exists)
- Variables and assignment
- Control flow structures
- Function declarations

### 📋 Planned
- Structs and methods
- Arrays and collections
- Error handling (`or`, `defer`)
- Garbage collection
- Standard library expansion
- Concurrency primitives
- AOT compilation

---

## Version History

- **v0.1.0-alpha** (2024-11-02) - Initial release with modular architecture
  - Complete lexer and parser
  - Basic VM with io.println
  - Modular code structure (core, frontend, backend, runtime, error)
  - Comprehensive documentation
  
- **v0.0.1** (2024-11-01) - Initial project structure

---

**Last Updated:** 2024-11-02  
**Version:** 0.1.0-alpha  
**Status:** Active Development

**Built from scratch, with focus. 🌸**
