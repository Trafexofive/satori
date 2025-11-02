# 🌸 Satori

**A statically typed scripting language for recreational engineers who build from scratch.**

Satori combines the safety of static typing with the ergonomics of scripting languages. Built in C, designed for clarity, made for your own infrastructure.

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg)]()
[![Version](https://img.shields.io/badge/version-0.1.0--alpha-blue.svg)]()

> **Status:** Core interpreter working! Modules, variables, arithmetic, and control flow implemented.

## Philosophy

- **Explicit over implicit** - Know what the machine does
- **Minimal ceremony** - Write less, express more  
- **Production ready** - Strong working solutions for real systems
- **Zero magic** - No hidden runtime costs
- **Build for yourself** - Your infra, your rules, your wheel to reinvent

## Quick Example

**What works right now:**

```satori
import io

// Variables with type inference
let score := 85
let name := "Alice"

// Arithmetic with correct precedence
let result := 2 + 3 * 4  // 14

// Comparisons
io.println "{} > 80 = {}", score, score > 80

// Control flow
if score >= 90 then
    io.println "{}: A - Excellent!", name
else
    if score >= 80 then
        io.println "{}: B - Good!", name
    else
        io.println "{}: C - Average", name

// Format strings with multiple arguments
io.println "Student: {}, Score: {}, Result: {}", name, score, result
```

**Output:**
```
85 > 80 = true
Alice: B - Good!
Student: Alice, Score: 85, Result: 14
```

## Running Examples

Try the included examples:

```bash
# Feature showcase
./bin/satori examples/showcase.sat

# Grade calculator
./bin/satori examples/grading.sat

# Number classification
./bin/satori examples/number_classifier.sat

# FizzBuzz
./bin/satori examples/fizzbuzz.sat
```

See [`examples/`](examples/) for more.

## Quick Start

### Installation

```bash
# Clone repository
git clone https://github.com/yourusername/satori.git
cd satori

# Build
make

# Run examples
./bin/satori examples/hello.sat
```

### Requirements

- GCC or Clang
- Make
- POSIX system (Linux, BSD, macOS)

### Your First Program

Create `hello.sat`:
```satori
import io

let name := "World"
io.println "Hello, {}!", name
```

Run it:
```bash
./bin/satori hello.sat
```

Output:
```
Hello, World!
```

## Language Tour

### Variables

```satori
let x := 42              // Type inference - int
let pi := 3.14159        // float
let name := "Satori"     // string
```

### Arithmetic

```satori
let result := 2 + 3 * 4  // 14 (correct precedence)
let remainder := 17 % 5  // 2
let quotient := 10 / 3   // 3.333...

// All operators: + - * / %
// Comparisons: < <= > >= == !=
// Unary: - (negation), ! (not)
```

### Control Flow

```satori
// If/else with then keyword
if x > 10 then
    io.println "Large"
else
    io.println "Small"

// Nested conditions
if score >= 90 then
    io.println "A"
else
    if score >= 80 then
        io.println "B"
    else
        io.println "C"

// While loops (structure ready, needs assignment)
while count < 10 then
    io.println count
    count = count + 1  // Assignment coming soon
```

### Format Strings

```satori
// Multiple arguments with {} placeholders
io.println "Name: {}, Score: {}", name, score
io.println "x={}, y={}, sum={}", x, y, x + y

// Expressions in arguments
io.println "Result: {}", 2 + 3 * 4
```

## Project Status

✅ **Working Now** - Core interpreter functional with real programs

### Implemented ✅

**Phase 1: Module System**
- [x] Module imports (`import io`)
- [x] Native C function binding
- [x] Lazy loading and caching
- [x] Namespaced calls (`io.println`)

**Phase 2: Variables**
- [x] Variable declarations (`let x := 42`)
- [x] Type inference (int, float, string)
- [x] Local variable storage
- [x] Variable usage in expressions

**Phase 3: Multiple Arguments & Format Strings**
- [x] Comma-separated arguments
- [x] `{}` placeholder interpolation
- [x] Variadic native functions

**Phase 4: Arithmetic & Operators**
- [x] Arithmetic operators: `+ - * / %`
- [x] Comparison operators: `< <= > >= == !=`
- [x] Unary operators: `-` (negate), `!` (not)
- [x] Correct operator precedence
- [x] Mixed int/float arithmetic

**Phase 5: Control Flow**
- [x] `if/then/else` statements
- [x] Nested conditionals
- [x] `while` loop structure
- [x] `loop` (infinite loop) structure
- [x] Jump opcodes and patching

**Core Infrastructure**
- [x] Lexer with full tokenization
- [x] Parser with expression precedence
- [x] AST construction and traversal
- [x] Bytecode VM with stack
- [x] Code generation (AST → bytecode)
- [x] Hash table for fast lookups
- [x] Value system (int, float, string, bool)

### In Progress 🚧

- [ ] Assignment operator (AST ready, parser needs work)
- [ ] Parentheses in expressions
- [ ] `break` and `continue` (needs loop context)
- [ ] Boolean literals (`true`, `false`)
- [ ] Logical operators (`and`, `or`)

### Planned 📋

**Near Term**
- [ ] Functions - declarations and calls
- [ ] Return statements
- [ ] Arrays and indexing
- [ ] String manipulation stdlib
- [ ] Better error messages with line numbers

**Medium Term**
- [ ] Structs - user-defined types
- [ ] Methods on structs
- [ ] Type checker - static validation
- [ ] Standard library expansion (fs, math, string)
- [ ] Garbage collection

**Long Term**
- [ ] Concurrency (`spawn`, channels)
- [ ] Error handling (`or` operator, `defer`)
- [ ] AOT compilation to native code
- [ ] LLVM backend
- [ ] Full stdlib (net, json, etc.)

See [`notes/TODO.md`](notes/TODO.md) for detailed roadmap.

## Architecture

Satori uses a modular, layered architecture:

```
src/
├── core/       # Layer 0: Value, object, memory
├── frontend/   # Layer 1: Lexer, parser, AST
├── backend/    # Layer 2: Code generation
├── runtime/    # Layer 3: VM, GC
└── stdlib/     # Layer 4: Standard library
```

See [`ARCHITECTURE.md`](ARCHITECTURE.md) for details.

## Documentation

- **[Language Specification](docs/spec.md)** - Complete language reference
- **[Standard Library](docs/stdlib.md)** - Built-in modules and functions
- **[Internals](docs/internals.md)** - Implementation details
- **[Architecture](ARCHITECTURE.md)** - System design
- **[Examples](examples/)** - Code examples

## Building From Source

### Build Options

```bash
make           # Release build (optimized)
make debug     # Debug build (with symbols)
make clean     # Clean build artifacts
make install   # Install to /usr/local/bin
```

### Development

```bash
# Build and run
make && ./bin/satori examples/hello.sat

# Debug with GDB
make debug
gdb ./bin/satori

# Run tests (when available)
make test
```

## Roadmap

### v0.1 - Core Interpreter ✅ (Current - Working!)
- ✅ Lexer, parser, VM
- ✅ Variables and expressions
- ✅ Control flow (if/else)
- ✅ Arithmetic and comparisons
- ✅ Module system with native functions
- 🚧 Assignment operator
- 🚧 Boolean literals and logical operators

### v0.2 - Functions & Data Structures
- [ ] Function declarations and calls
- [ ] Return statements
- [ ] Arrays and slices
- [ ] Structs (basic)
- [ ] Type checking foundation

### v0.3 - Standard Library & Features
- [ ] Structs with methods
- [ ] String manipulation
- [ ] File I/O (fs module)
- [ ] Math module
- [ ] Error handling basics
- [ ] Garbage collection

### v0.4 - Advanced Features
- [ ] Concurrency primitives
- [ ] Error handling (`or`, `defer`)
- [ ] Module imports from files
- [ ] JSON parsing
- [ ] Network I/O

### v0.5 - Performance & Polish
- [ ] AOT compilation option
- [ ] Optimization passes
- [ ] Complete standard library
- [ ] Comprehensive documentation

### v1.0 - Stable Release
- [ ] Language spec finalized
- [ ] Full stdlib
- [ ] Production ready
- [ ] Performance benchmarks

## Philosophy & Design

Satori is built for recreational engineers who:
- Reinvent the wheel because that's how you learn
- Build for themselves and their friends
- Host on their own infrastructure
- Value clarity over cleverness
- Want static guarantees without ceremony
- Need strong, working, prod-ready solutions
- Appreciate knowing what happens at runtime
- Have a strong suit in low-level and sysadmin work

Not interested in following the beaten path. Interested in forging your own.

**Inspired by:** C, Lua, Go, Zig

## Contributing

Contributions welcome! Built by recreational engineers, for recreational engineers.

### Getting Started

1. Read [`ARCHITECTURE.md`](ARCHITECTURE.md)
2. Check [`notes/TODO.md`](notes/TODO.md) for tasks
3. Read [`notes/GETTING_STARTED.md`](notes/GETTING_STARTED.md)
4. Pick an issue or feature
5. Submit a PR

### Guidelines

- Strong, working solutions - no half-baked PRs
- Follow the philosophy: explicit, minimal, zero magic
- Add tests for new features
- Update documentation
- Keep commits focused and well-described

### Development Setup

```bash
# Clone and build
git clone https://github.com/yourusername/satori.git
cd satori
make debug

# Run examples
./bin/satori examples/hello.sat

# Check code
make check-deps  # Verify no circular dependencies
```

## Community

- **IRC:** `#satori` on libera.chat
- **Mailing list:** satori-lang@lists.sr.ht
- **Issues:** GitHub issues for bugs and features
- **Discussions:** GitHub discussions for questions

## License

MIT - See [LICENSE](LICENSE) file

## Acknowledgments

- **Crafting Interpreters** by Robert Nystrom - Excellent guide
- **Lua** - Inspiration for clean VM design
- **Go** - Inspiration for pragmatic simplicity
- **Zig** - Inspiration for explicitness

---

<p align="center">
  <strong>Built from scratch, with focus. 🌸</strong><br>
  <em>For recreational engineers who build their own infrastructure.</em>
</p>

<p align="center">
  <a href="docs/spec.md">Language Spec</a> •
  <a href="docs/stdlib.md">Standard Library</a> •
  <a href="examples/">Examples</a> •
  <a href="ARCHITECTURE.md">Architecture</a> •
  <a href="notes/TODO.md">Roadmap</a>
</p>
