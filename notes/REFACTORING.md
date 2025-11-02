# Modularization Refactoring Guide

## Overview

Transforming Satori from a monolithic structure to a clean, modular architecture with clear separation of concerns.

**Factorio principle:** Each module is a production line with clear inputs and outputs.

## Current State (v0.0.2)

```
src/
├── common.h          # 64 lines
├── lexer.c/h         # 370 + 111 lines
├── parser.c/h        # 164 + 22 lines
├── ast.c/h           # 178 + 96 lines
├── codegen.c/h       # 117 + 11 lines
├── vm.c/h            # 220 + 79 lines
├── error.c/h         # 53 + 20 lines
├── memory.c/h        # 0 + 0 lines (stubs)
├── typechecker.c/h   # 0 + 0 lines (stubs)
└── main.c            # 175 lines
```

**Total:** ~1,680 lines in 18 files

**Issues:**
- ❌ Flat structure - hard to navigate
- ❌ Mixed concerns - VM has value code, etc.
- ❌ No clear dependency graph
- ❌ Can't test modules in isolation
- ❌ Hard to parallelize development

## Target State (v0.1+)

```
src/
├── core/              # Layer 0: Foundational (no deps)
│   ├── common.h
│   ├── value.c/h      # Value representation
│   ├── object.c/h     # Heap objects
│   ├── memory.c/h     # Allocation
│   ├── table.c/h      # Hash tables
│   └── array.c/h      # Dynamic arrays
│
├── frontend/          # Layer 1: Source → AST
│   ├── token.h        # Token types
│   ├── lexer.c/h      # Tokenization
│   ├── parser.c/h     # Parsing
│   ├── ast.c/h        # AST structures
│   ├── resolver.c/h   # Name resolution
│   └── typechecker.c/h # Type checking
│
├── backend/           # Layer 2: AST → Bytecode
│   ├── bytecode.c/h   # Bytecode structures
│   ├── codegen.c/h    # Code generation
│   ├── optimizer.c/h  # Optimizations
│   └── disasm.c/h     # Disassembler
│
├── runtime/           # Layer 3: Execution
│   ├── vm.c/h         # Virtual machine
│   ├── gc.c/h         # Garbage collector
│   ├── fiber.c/h      # Coroutines
│   ├── call.c/h       # Call frames
│   └── debug.c/h      # Debug support
│
├── stdlib/            # Layer 4: Standard library
│   ├── core_lib.c/h   # Core functions
│   ├── io_lib.c/h     # I/O operations
│   ├── math_lib.c/h   # Math functions
│   └── ...
│
├── error/             # Cross-cutting
│   ├── error.c/h      # Error reporting
│   └── diagnostics.c/h # Pretty errors
│
├── satori.h           # Public API
└── main.c             # Entry point
```

## Migration Steps

### ✅ Phase 0: Setup (DONE)

- [x] Create directory structure
- [x] Create ARCHITECTURE.md
- [x] Create modular Makefile
- [x] Document refactoring plan

### Phase 1: Core Module (Current)

**Goal:** Extract all value and memory management code

**Tasks:**
- [x] Create `src/core/value.h` - Value type definitions
- [x] Create `src/core/value.c` - Value operations
- [x] Create `src/core/object.h` - Heap object types
- [x] Create `src/core/object.c` - Object operations
- [x] Create `src/core/memory.h` - Memory interface
- [x] Create `src/core/memory.c` - Memory implementation
- [ ] Create `src/core/table.h` - Hash table
- [ ] Create `src/core/table.c` - Hash table impl
- [ ] Create `src/core/array.h` - Dynamic array
- [ ] Create `src/core/array.c` - Dynamic array impl

**Changes needed:**
- [ ] Update VM to use `core/value.h`
- [ ] Update codegen to use `core/value.h`
- [ ] Update all includes
- [ ] Test compilation

**Verification:**
```bash
make core      # Should compile without errors
```

### Phase 2: Frontend Module

**Goal:** Organize all source → AST code

**Tasks:**
- [ ] Move `lexer.c/h` to `frontend/`
- [ ] Extract `token.h` from `lexer.h`
- [ ] Move `parser.c/h` to `frontend/`
- [ ] Move `ast.c/h` to `frontend/`
- [ ] Create `frontend/resolver.c/h` (name resolution)
- [ ] Implement basic `frontend/typechecker.c/h`

**Changes needed:**
- [ ] Update all `#include` paths
- [ ] Update Makefile sources
- [ ] Test each move individually

**Verification:**
```bash
make frontend  # Should compile
./bin/satori -t examples/hello.sat  # Should still work
```

### Phase 3: Backend Module

**Goal:** Separate code generation concerns

**Tasks:**
- [ ] Extract `bytecode.h` from `vm.h`
- [ ] Create `backend/bytecode.c` - Chunk management
- [ ] Move `codegen.c/h` to `backend/`
- [ ] Create `backend/optimizer.c/h` (stub for now)
- [ ] Create `backend/disasm.c/h` - Bytecode disassembler

**Changes needed:**
- [ ] VM imports `backend/bytecode.h`
- [ ] Codegen uses bytecode module
- [ ] Update includes

**Verification:**
```bash
make backend   # Should compile
./bin/satori -a examples/hello.sat  # AST dump works
```

### Phase 4: Runtime Module

**Goal:** Clean VM and execution code

**Tasks:**
- [ ] Move `vm.c/h` to `runtime/`
- [ ] Extract `gc.c/h` from VM
- [ ] Create `runtime/call.c/h` - Call frame management
- [ ] Create `runtime/fiber.c/h` - Coroutine support (stub)
- [ ] Create `runtime/debug.c/h` - Debug helpers

**Changes needed:**
- [ ] Split VM into focused modules
- [ ] Clean interfaces between runtime components
- [ ] Update includes

**Verification:**
```bash
make runtime   # Should compile
./bin/satori examples/hello.sat  # Should execute
```

### Phase 5: Standard Library

**Goal:** Modular stdlib with clean registration

**Tasks:**
- [ ] Create `stdlib/core_lib.c/h` - Core functions
- [ ] Create `stdlib/io_lib.c/h` - I/O operations
- [ ] Create `stdlib/module.c/h` - Module loader
- [ ] Extract current io.println to stdlib
- [ ] Design module registration system

**Changes needed:**
- [ ] VM calls stdlib init
- [ ] Remove hardcoded stdlib from VM
- [ ] Clean module interface

**Verification:**
```bash
./bin/satori examples/hello.sat  # io.println still works
```

### Phase 6: Error Handling

**Goal:** Better error messages with rich context

**Tasks:**
- [ ] Move `error.c/h` to `error/`
- [ ] Create `error/diagnostics.c/h`
- [ ] Add source location tracking
- [ ] Pretty-print errors with context
- [ ] Color support (optional)

**Verification:**
```bash
./bin/satori bad_syntax.sat  # Should show nice error
```

### Phase 7: Testing Infrastructure

**Goal:** Module-level unit tests

**Tasks:**
- [ ] Create `tests/core/` - Test value, memory, table
- [ ] Create `tests/frontend/` - Test lexer, parser
- [ ] Create `tests/backend/` - Test codegen
- [ ] Create `tests/runtime/` - Test VM
- [ ] Create test runner
- [ ] Add to Makefile

**Verification:**
```bash
make test      # All tests pass
```

### Phase 8: Documentation Update

**Goal:** Document new structure

**Tasks:**
- [ ] Update docs/internals.md
- [ ] Update README.md build instructions
- [ ] Add module-level README in each dir
- [ ] Update contribution guide

## Module Interface Contracts

### Core Module

**Provides:**
- Value representation and operations
- Object allocation and management
- Memory allocation interface
- Hash tables
- Dynamic arrays

**Requires:**
- Only libc

**Test:** Can compile and link without any other Satori code

### Frontend Module

**Provides:**
- Lexical analysis (source → tokens)
- Syntax analysis (tokens → AST)
- Semantic analysis (AST validation)
- Name resolution
- Type checking

**Requires:**
- core (for values, memory)

**Test:** Can parse source into AST without VM

### Backend Module

**Provides:**
- Bytecode emission
- Code optimization
- Bytecode disassembly

**Requires:**
- core (for values)
- frontend (for AST)

**Test:** Can compile AST to bytecode without executing

### Runtime Module

**Provides:**
- Bytecode execution
- Garbage collection
- Call frame management
- Fiber/coroutine support

**Requires:**
- core (for values, memory)
- backend (for bytecode)

**Test:** Can execute pre-compiled bytecode

## Build System Changes

### Old Makefile (flat)
```makefile
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))
```

### New Makefile (modular)
```makefile
CORE_OBJS = $(CORE_SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
FRONTEND_OBJS = $(FRONTEND_SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
# ... etc

# Can build modules independently
core: $(CORE_OBJS)
frontend: core $(FRONTEND_OBJS)
backend: core frontend $(BACKEND_OBJS)
```

**Benefits:**
- Incremental builds (only rebuild changed modules)
- Parallel compilation possible
- Can test modules in isolation
- Clear dependency visualization

## Include Path Strategy

### Option A: Relative includes
```c
// In src/frontend/parser.c
#include "../core/value.h"
#include "ast.h"
```

**Pros:** Clear, explicit
**Cons:** Refactoring harder

### Option B: Root-relative includes (CHOSEN)
```c
// In src/frontend/parser.c
#include "core/value.h"
#include "frontend/ast.h"
```

**Pros:** Clean, refactor-friendly
**Cons:** Need `-I$(SRC_DIR)` flag

**Decision:** Use Option B with `CFLAGS += -I$(SRC_DIR)`

## Code Movement Checklist

When moving a file:

1. **Create new file in target directory**
   ```bash
   mkdir -p src/frontend
   cp src/parser.c src/frontend/parser.c
   ```

2. **Update includes in moved file**
   ```c
   // Old
   #include "ast.h"
   
   // New
   #include "frontend/ast.h"
   ```

3. **Update includes in other files**
   ```bash
   grep -r "parser.h" src/ --include="*.c"
   # Update each reference
   ```

4. **Update Makefile**
   ```makefile
   FRONTEND_SRCS += src/frontend/parser.c
   ```

5. **Test compilation**
   ```bash
   make clean && make
   ```

6. **Test functionality**
   ```bash
   ./bin/satori examples/hello.sat
   ```

7. **Remove old file**
   ```bash
   rm src/parser.c
   ```

8. **Commit**
   ```bash
   git add src/frontend/parser.c
   git rm src/parser.c
   git commit -m "refactor: Move parser to frontend module"
   ```

## Testing Strategy

### Unit Tests (per module)

**Core:**
```c
// tests/core/test_value.c
void test_value_creation();
void test_value_equality();
void test_value_to_float();
```

**Frontend:**
```c
// tests/frontend/test_lexer.c
void test_tokenize_keywords();
void test_tokenize_operators();
void test_tokenize_strings();
```

**Integration:**
```c
// tests/integration/test_pipeline.c
void test_full_compilation();
void test_execution();
```

### Continuous Integration

```bash
# Run on every commit
make clean
make
make test
make check-deps  # Verify no circular deps
```

## Performance Impact

**Compilation time:**
- Flat: O(n) files, rebuild all
- Modular: O(changed modules), incremental

**Runtime performance:**
- No change (same final binary)
- Slightly better due to cleaner cache usage

**Binary size:**
- With proper linking: Same or smaller
- Can strip unused stdlib modules

## Common Pitfalls

**❌ Circular dependencies**
```c
// BAD: core includes frontend
// core/value.h
#include "frontend/ast.h"  // NEVER!
```

**Solution:** Keep dependency graph acyclic

**❌ God objects**
```c
// BAD: everything in common.h
#include "common.h"  // 10,000 lines
```

**Solution:** Each module has minimal interface

**❌ Tight coupling**
```c
// BAD: direct access to internals
vm->stack[vm->stack_top - 1]  // From outside VM
```

**Solution:** Provide clean APIs

## Success Criteria

### After Phase 1-4:
- [x] `make core` compiles independently
- [ ] `make frontend` compiles with only core
- [ ] `make backend` compiles with core + frontend
- [ ] `make runtime` compiles with all previous
- [ ] `make check-deps` passes
- [ ] All examples still work
- [ ] No performance regression

### After Phase 5-8:
- [ ] Each module has tests
- [ ] `make test` runs all tests
- [ ] Code coverage > 70%
- [ ] Documentation updated
- [ ] Clean `git log` with incremental commits

## Timeline

- **Week 1:** Phase 1-2 (Core + Frontend)
- **Week 2:** Phase 3-4 (Backend + Runtime)
- **Week 3:** Phase 5-6 (Stdlib + Error handling)
- **Week 4:** Phase 7-8 (Testing + Docs)

**Total:** ~1 month for complete modularization

## Benefits Summary

**For Development:**
- ✅ Easier to navigate codebase
- ✅ Can work on modules in parallel
- ✅ Faster incremental builds
- ✅ Better test coverage
- ✅ Clear ownership of modules

**For Architecture:**
- ✅ Clean dependency graph
- ✅ Easy to add features
- ✅ Easy to remove features
- ✅ Can embed subset of Satori
- ✅ Better code reuse

**For Users:**
- ✅ Smaller binaries (with selective linking)
- ✅ Better error messages
- ✅ More stable (isolated changes)
- ✅ Easier to contribute

---

**Like building a Factorio megabase: each module is a self-contained production line! 🏭**
