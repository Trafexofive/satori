# Getting Started with Satori Development

**Welcome, recreational engineer!** 🌸

This guide will get you started building and contributing to Satori.

## 🚀 First Steps

### 1. Build Satori

```bash
cd /home/mlamkadm/repos/satori
make clean
make
```

This builds the interpreter at `bin/satori`.

### 2. Run Hello World

```bash
./bin/satori examples/hello.sat
```

Output: `Hello, World!`

### 3. Explore the Codebase

```bash
# View source structure
tree src/

# Check what's implemented
grep -r "TODO" src/

# Read the docs
ls docs/
```

## 📖 Understanding the Architecture

Satori follows a traditional compilation pipeline:

```
Source Code → Lexer → Parser → AST → Type Checker → Codegen → VM → Execution
```

### Key Files

- **`src/lexer.c`** - Tokenizes source code
- **`src/parser.c`** - Builds Abstract Syntax Tree
- **`src/ast.c`** - AST data structures
- **`src/typechecker.c`** - Type checking (TODO)
- **`src/codegen.c`** - Emits bytecode
- **`src/vm.c`** - Executes bytecode
- **`src/main.c`** - Entry point

Read `docs/internals.md` for detailed architecture.

## 🎯 What to Work On

Check `notes/TODO.md` for prioritized tasks. Current focus:

**High Priority:**
- Type checker implementation
- Variable declarations
- Control flow (if/else, loops)
- Functions

**Good First Issues:**
- Improve error messages
- Add more unit tests
- Implement missing operators
- Documentation improvements

## 📚 Learning Path

### Option A: Start from Scratch

Follow `notes/research/learning-path.md`:
1. Week 1-2: Understand lexers and parsers
2. Week 3-4: Learn type systems
3. Week 5-6: Implement control flow
4. Continue building...

### Option B: Jump In

1. Read `docs/spec.md` - Understand the language
2. Read `docs/internals.md` - Understand the implementation
3. Pick a TODO item
4. Implement it!
5. Test it
6. Submit PR

## 🛠️ Development Workflow

### Making Changes

```bash
# 1. Create a branch
git checkout -b feature/my-feature

# 2. Make changes
vim src/parser.c

# 3. Build
make clean && make

# 4. Test manually
./bin/satori examples/hello.sat

# 5. Run tests (when available)
make test

# 6. Commit
git add src/parser.c
git commit -m "Add support for X"
```

### Testing Your Changes

```bash
# Dump tokens
./bin/satori -t examples/hello.sat

# Dump AST
./bin/satori -a examples/hello.sat

# Run normally
./bin/satori examples/hello.sat
```

### Debug Build

```bash
make debug
gdb ./bin/satori
```

Inside GDB:
```
(gdb) break parse_expression
(gdb) run examples/hello.sat
(gdb) print *node
```

## 📖 Essential Reading

Before diving deep, read these (in order):

1. **[Crafting Interpreters](https://craftinginterpreters.com/)** (FREE online)
   - Part I: Tree-walk interpreter
   - Part II: Bytecode virtual machine
   - This is THE guide

2. **[docs/spec.md](../docs/spec.md)** (in repo)
   - Satori language specification
   - What we're building

3. **[docs/internals.md](../docs/internals.md)** (in repo)
   - How Satori is implemented
   - Architecture details

4. **[Lua 5.0 Paper](https://www.lua.org/doc/jucs05.pdf)** (PDF)
   - Learn from a masterclass VM
   - Very relevant to Satori

See `notes/research/READING_LIST.md` for complete list.

## 💡 Code Examples

### Adding a New Token Type

**1. Define in lexer.h:**
```c
typedef enum {
  // ... existing tokens
  TOKEN_MOD,      // %
  // ...
} TokenType;
```

**2. Implement in lexer.c:**
```c
case '%': return make_token(&lexer, TOKEN_MOD);
```

**3. Add to parser if needed**

### Adding a New AST Node

**1. Define in ast.h:**
```c
typedef enum {
  // ... existing types
  AST_BINARY_OP,
} AstNodeType;

typedef struct {
  AstNode *left;
  TokenType op;
  AstNode *right;
} AstBinaryOp;
```

**2. Add to AstNode union:**
```c
union {
  // ... existing
  AstBinaryOp binary_op;
}
```

**3. Implement constructor:**
```c
AstNode *ast_make_binary_op(AstNode *left, TokenType op, AstNode *right);
```

### Adding a New Opcode

**1. Define in vm.h:**
```c
typedef enum {
  // ... existing
  OP_ADD,
} OpCode;
```

**2. Implement in vm.c:**
```c
case OP_ADD: {
  Value b = pop(vm);
  Value a = pop(vm);
  push(vm, value_add(a, b));
  break;
}
```

## 🐛 Common Issues

### Build Fails

```bash
# Clean and rebuild
make clean
make

# Check compiler version
gcc --version

# Enable verbose
make VERBOSE=1
```

### Segmentation Fault

```bash
# Use debug build
make debug

# Run with GDB
gdb ./bin/satori
run examples/hello.sat

# Get backtrace
bt
```

### Memory Leaks

```bash
# Use valgrind
valgrind --leak-check=full ./bin/satori examples/hello.sat
```

## 🎨 Code Style

Follow these conventions:

**Naming:**
- Functions: `snake_case`
- Types: `PascalCase`
- Macros: `UPPER_CASE`
- Variables: `snake_case`

**Indentation:**
- 2 spaces (no tabs)
- K&R brace style

**Comments:**
```c
// Single line for brief comments

/* Multi-line for
   longer explanations */
```

**Example:**
```c
// Parse a primary expression
static AstNode *parse_primary(Parser *p) {
  if (match(p, TOKEN_NUMBER)) {
    return ast_make_int_literal(/* ... */);
  }
  
  error_report(p->file_path, p->current.line, 
               p->current.column, "Expected expression");
  return NULL;
}
```

## 🤝 Getting Help

**Stuck? Questions? Ideas?**

1. Check `notes/` directory for answers
2. Read relevant docs in `docs/`
3. Study similar code in Lua/Wren
4. Ask in r/ProgrammingLanguages
5. Open an issue with questions

## 🎯 Quick Wins

Easy tasks to get started:

- [ ] Add more examples in `examples/`
- [ ] Improve error messages
- [ ] Write tests for lexer
- [ ] Document edge cases
- [ ] Fix TODOs in source code
- [ ] Add missing operators

## 📊 Progress Tracking

Track your journey:

```bash
# Count lines of code
wc -l src/*.c src/*.h

# Check git stats
git log --oneline

# See what's changed
git diff main
```

## 🌟 Philosophy

Remember the Satori way:

- **Explicit over implicit** - Make things clear
- **Minimal ceremony** - Don't over-engineer
- **Zero magic** - No hidden behaviors
- **Production ready** - Build strong solutions
- **Build for yourself** - Learn by doing

## 🚦 Next Steps

Now that you're set up:

1. ✅ Read this guide
2. ✅ Build Satori
3. ✅ Run examples
4. 📖 Read docs/spec.md
5. 📖 Read docs/internals.md
6. 💻 Pick a TODO item
7. 🔨 Start coding!

---

**Welcome to the journey. Build from scratch, with focus. 🌸**

*Questions? Check the notes. Still stuck? That's part of learning!*
