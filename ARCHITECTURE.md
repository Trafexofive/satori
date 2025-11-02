# Satori Architecture

**Modular, decoupled design for maintainability and extensibility.**

## Design Philosophy

Like a well-designed Factorio factory:
- **Clear material flow** - Data flows through distinct processing stages
- **Modular components** - Each subsystem is independent
- **Well-defined interfaces** - Clean APIs between modules
- **Easy to extend** - Add new features without touching everything
- **Testable** - Each module can be tested in isolation

## Directory Structure

```
src/
├── core/              # Core data structures and utilities
│   ├── common.h       # Common types and macros
│   ├── value.c/h      # Value representation
│   ├── object.c/h     # Object system (strings, arrays, etc.)
│   ├── memory.c/h     # Memory allocation
│   ├── table.c/h      # Hash tables
│   └── array.c/h      # Dynamic arrays
│
├── frontend/          # Source → AST pipeline
│   ├── lexer.c/h      # Tokenization
│   ├── token.h        # Token types
│   ├── parser.c/h     # Parsing
│   ├── ast.c/h        # AST representation
│   ├── resolver.c/h   # Name resolution
│   └── typechecker.c/h # Type checking
│
├── backend/           # AST → Bytecode pipeline
│   ├── codegen.c/h    # Code generation
│   ├── optimizer.c/h  # Bytecode optimization
│   ├── bytecode.c/h   # Bytecode representation
│   └── disasm.c/h     # Disassembler (debug)
│
├── runtime/           # Execution engine
│   ├── vm.c/h         # Virtual machine
│   ├── gc.c/h         # Garbage collector
│   ├── fiber.c/h      # Coroutines/tasks
│   ├── call.c/h       # Function calls
│   └── debug.c/h      # Debug support
│
├── stdlib/            # Standard library modules
│   ├── core_lib.c/h   # Core builtins
│   ├── io_lib.c/h     # I/O operations
│   ├── math_lib.c/h   # Math functions
│   ├── string_lib.c/h # String operations
│   └── ... (more modules)
│
├── error/             # Error handling
│   ├── error.c/h      # Error reporting
│   └── diagnostics.c/h # Rich diagnostics
│
├── main.c             # Entry point
└── satori.h           # Public API
```

## Module Layers

### Layer 0: Core (No dependencies)
- **value** - Tagged union value representation
- **memory** - Allocation, GC interface
- **array** - Dynamic arrays (generic)
- **table** - Hash tables (generic)
- **object** - Heap-allocated objects

**Dependencies:** None (only libc)

### Layer 1: Frontend (Depends on: core)
- **lexer** - String → Tokens
- **parser** - Tokens → AST
- **ast** - AST data structures
- **resolver** - Bind names to declarations
- **typechecker** - Type checking and inference

**Input:** Source code (string)
**Output:** Typed AST

### Layer 2: Backend (Depends on: core, frontend)
- **codegen** - AST → Bytecode
- **optimizer** - Bytecode optimizations
- **bytecode** - Bytecode structures
- **disasm** - Bytecode → Human-readable

**Input:** Typed AST
**Output:** Bytecode chunk

### Layer 3: Runtime (Depends on: core, backend)
- **vm** - Bytecode interpreter
- **gc** - Garbage collection
- **fiber** - Lightweight threads
- **call** - Call frame management
- **debug** - Debugging support

**Input:** Bytecode chunk
**Output:** Execution, side effects

### Layer 4: Standard Library (Depends on: runtime)
- **core_lib** - Fundamental operations
- **io_lib** - File I/O
- **math_lib** - Math functions
- **string_lib** - String operations
- ... more modules

**Input:** VM state
**Output:** Native functions, modules

### Layer 5: Error Handling (Cross-cutting)
- **error** - Error messages
- **diagnostics** - Source locations, pretty printing

**Used by:** All layers

## Data Flow

```
┌─────────────┐
│ Source Code │
└──────┬──────┘
       │
       ▼
  ┌─────────┐
  │  Lexer  │ ── Tokens ──┐
  └─────────┘             │
                          ▼
                     ┌─────────┐
                     │ Parser  │ ── AST ──┐
                     └─────────┘          │
                                          ▼
                                   ┌──────────────┐
                                   │  Resolver    │
                                   └──────┬───────┘
                                          │
                                          ▼
                                   ┌──────────────┐
                                   │ Type Checker │ ── Typed AST ──┐
                                   └──────────────┘                │
                                                                   ▼
                                                            ┌──────────┐
                                                            │ Codegen  │
                                                            └─────┬────┘
                                                                  │
                                                                  ▼
                                                            ┌──────────┐
                                                            │Optimizer │ ── Bytecode ──┐
                                                            └──────────┘               │
                                                                                       ▼
                                                                                ┌────────┐
                                                                                │   VM   │
                                                                                └────┬───┘
                                                                                     │
                                                                                     ▼
                                                                              Execution
```

## Module Interfaces

### Core Module Interface

**value.h:**
```c
// Public API
typedef struct Value Value;
Value value_make_int(i64 val);
Value value_make_float(f64 val);
bool value_is_number(Value v);
// ... etc

// Internal (only used by runtime)
typedef struct {
  ValueType type;
  union { /* ... */ } u;
} Value;
```

**memory.h:**
```c
void *mem_alloc(size_t size);
void *mem_realloc(void *ptr, size_t size);
void mem_free(void *ptr);
void mem_register_gc(GC *gc);
```

### Frontend Interface

**lexer.h:**
```c
typedef struct Lexer Lexer;
void lexer_init(Lexer *lexer, const char *source);
Token lexer_next_token(Lexer *lexer);
```

**parser.h:**
```c
typedef struct Parser Parser;
AstNode *parse(const char *source, const char *filename);
void parser_free(Parser *p);
```

**typechecker.h:**
```c
typedef struct TypeChecker TypeChecker;
bool typecheck(TypeChecker *tc, AstNode *ast);
void typechecker_free(TypeChecker *tc);
```

### Backend Interface

**codegen.h:**
```c
typedef struct Compiler Compiler;
Chunk *compile(AstNode *ast);
void compiler_free(Compiler *c);
```

### Runtime Interface

**vm.h:**
```c
typedef struct VM VM;
void vm_init(VM *vm);
bool vm_run(VM *vm, Chunk *chunk);
void vm_free(VM *vm);
```

## Compilation Units

Each module is self-contained:

**Example: value module**
```
src/core/value.c     - Implementation
src/core/value.h     - Public interface
```

**No circular dependencies:**
- Core doesn't depend on anything
- Frontend depends only on core
- Backend depends on core + frontend
- Runtime depends on core + backend
- Stdlib depends on runtime

## Testing Strategy

### Unit Tests (per module)
```
tests/
├── core/
│   ├── test_value.c
│   ├── test_memory.c
│   └── test_table.c
├── frontend/
│   ├── test_lexer.c
│   ├── test_parser.c
│   └── test_typechecker.c
├── backend/
│   ├── test_codegen.c
│   └── test_optimizer.c
└── runtime/
    ├── test_vm.c
    └── test_gc.c
```

### Integration Tests
```
tests/integration/
├── e2e_simple.c       # End-to-end: source → execution
├── e2e_stdlib.c       # Standard library tests
└── e2e_errors.c       # Error handling tests
```

## Build System

**Modular compilation:**

```makefile
# Core objects
CORE_OBJS = $(BUILD_DIR)/core/value.o \
            $(BUILD_DIR)/core/memory.o \
            $(BUILD_DIR)/core/table.o

# Frontend objects
FRONTEND_OBJS = $(BUILD_DIR)/frontend/lexer.o \
                $(BUILD_DIR)/frontend/parser.o \
                $(BUILD_DIR)/frontend/ast.o

# Backend objects
BACKEND_OBJS = $(BUILD_DIR)/backend/codegen.o \
               $(BUILD_DIR)/backend/optimizer.o

# Runtime objects
RUNTIME_OBJS = $(BUILD_DIR)/runtime/vm.o \
               $(BUILD_DIR)/runtime/gc.o

# Stdlib objects
STDLIB_OBJS = $(BUILD_DIR)/stdlib/core_lib.o \
              $(BUILD_DIR)/stdlib/io_lib.o

ALL_OBJS = $(CORE_OBJS) $(FRONTEND_OBJS) $(BACKEND_OBJS) \
           $(RUNTIME_OBJS) $(STDLIB_OBJS)
```

## Extension Points

### Adding a New Standard Library Module

1. Create `src/stdlib/mymodule_lib.c/h`
2. Implement module functions
3. Register in `stdlib_init()`
4. No other files need changes!

### Adding a New AST Node Type

1. Add to `src/frontend/ast.h`
2. Update `src/frontend/parser.c`
3. Update `src/backend/codegen.c`
4. Done! (Resolver and typechecker auto-handle)

### Adding a New Optimization Pass

1. Create `src/backend/opt_mypass.c/h`
2. Implement pass interface
3. Register in optimizer pipeline
4. No changes to codegen or VM!

## Performance Characteristics

**Compilation time:**
- Lexer: O(n) where n = source length
- Parser: O(n) tokens
- Typechecker: O(n) AST nodes
- Codegen: O(n) AST nodes
- Total: **O(n) linear**

**Memory usage:**
- Each stage can free previous data
- Source → Tokens (can discard source)
- Tokens → AST (can discard tokens)
- AST → Bytecode (can discard AST)
- Peak: ~3x source size

**Hot path optimization:**
- VM is the hot path
- Keep VM.c focused and optimized
- Other modules prioritize clarity

## Migration Plan

### Phase 1: Core extraction ✓
- [x] Create directory structure
- [ ] Move value/object code
- [ ] Move memory code
- [ ] Move utilities (table, array)

### Phase 2: Frontend split
- [ ] Move lexer to frontend/
- [ ] Move parser to frontend/
- [ ] Move AST to frontend/
- [ ] Create resolver module
- [ ] Update includes

### Phase 3: Backend split
- [ ] Move codegen to backend/
- [ ] Extract bytecode module
- [ ] Create optimizer skeleton
- [ ] Add disassembler

### Phase 4: Runtime split
- [ ] Move VM to runtime/
- [ ] Extract GC module
- [ ] Create call module
- [ ] Add fiber module

### Phase 5: Stdlib extraction
- [ ] Create stdlib structure
- [ ] Move io functions
- [ ] Create module system
- [ ] Add remaining modules

### Phase 6: Testing
- [ ] Unit tests for each module
- [ ] Integration tests
- [ ] Update build system
- [ ] Documentation

## Benefits

**For developers:**
- ✅ Easier to understand (smaller files)
- ✅ Easier to test (isolated modules)
- ✅ Easier to modify (clear boundaries)
- ✅ Easier to extend (defined interfaces)

**For the project:**
- ✅ Better organization
- ✅ Parallel development possible
- ✅ Reduced compilation times
- ✅ Cleaner git history

**For users:**
- ✅ Can embed just what they need
- ✅ Smaller binaries (with proper linking)
- ✅ Better error messages
- ✅ Faster compilation

---

**Built from scratch, with focus. Engineered for maintainability. 🌸**
