# Satori TODO List

**Last Updated:** 2024-11-02  
**Version:** 0.1.0-alpha

## 🔥 Critical Path (v0.1)

### Core Language Features
- [ ] **Type System**
  - [ ] Implement type checker skeleton
  - [ ] Add type inference for `:=` operator
  - [ ] Validate binary operations type compatibility
  - [ ] Check function call argument types
  - [ ] Implement struct field type checking
  
- [ ] **Variables & Assignment**
  - [ ] Add variable declaration to AST (`let` statements)
  - [ ] Implement variable scope tracking
  - [ ] Add assignment bytecode ops (`OP_SET_LOCAL`, `OP_GET_LOCAL`)
  - [ ] Support multiple variable declarations
  
- [ ] **Control Flow**
  - [ ] Implement `if/else` statements
  - [ ] Add `for` loops (range-based)
  - [ ] Add `while` loops
  - [ ] Add `loop` (infinite loop)
  - [ ] Implement `break` and `continue`
  - [ ] Add jump bytecode ops (`OP_JUMP`, `OP_JUMP_IF_FALSE`)

- [ ] **Functions**
  - [ ] Parse function declarations
  - [ ] Implement function call mechanism
  - [ ] Add return statements
  - [ ] Local variable scoping in functions
  - [ ] Function parameter passing

### Standard Library (io module)
- [x] Basic `io.println` with single string
- [ ] Format string interpolation (`{}` placeholders)
- [ ] Multiple argument support
- [ ] `io.print` (no newline)
- [ ] `io.readln` for input
- [ ] File operations (`io.open`, `File` struct)

### VM Improvements
- [ ] Add arithmetic bytecode ops (`OP_ADD`, `OP_SUB`, `OP_MUL`, `OP_DIV`)
- [ ] Add comparison ops (`OP_EQUAL`, `OP_LESS`, `OP_GREATER`)
- [ ] Add logical ops (`OP_AND`, `OP_OR`, `OP_NOT`)
- [ ] Stack overflow detection
- [ ] Better error messages with stack traces

### Parser Enhancements
- [ ] Better error recovery (don't stop at first error)
- [ ] Synchronization points for error recovery
- [ ] More informative error messages
- [ ] Support for expression statements
- [ ] Operator precedence table

### Memory Management
- [ ] Implement basic garbage collection
- [ ] Mark-and-sweep GC algorithm
- [ ] GC root tracking
- [ ] Stress testing for memory leaks

---

## 🎯 High Priority (v0.2)

### Structs
- [ ] Parse struct declarations
- [ ] Struct field access
- [ ] Struct methods
- [ ] Struct initialization syntax
- [ ] Method call resolution

### Arrays
- [ ] Fixed-size array support
- [ ] Array literal syntax
- [ ] Array indexing
- [ ] Bounds checking
- [ ] Dynamic arrays (slices)

### Error Handling
- [ ] Implement `or` operator for optionals
- [ ] Optional type representation
- [ ] `panic` function
- [ ] `defer` statement implementation
- [ ] Stack unwinding

### Standard Library Expansion
- [ ] `string` module (split, join, trim)
- [ ] `math` module (basic operations)
- [ ] `collections` module (List, Map basics)

---

## 📋 Medium Priority (v0.3)

### Concurrency
- [ ] Design task/coroutine system
- [ ] Implement `spawn` keyword
- [ ] Cooperative scheduler
- [ ] `yield` operation
- [ ] Channel design (future)

### Modules
- [ ] Module resolution algorithm
- [ ] Module caching
- [ ] Circular dependency detection
- [ ] Module search paths (`SATORI_PATH`)

### Type System Advanced
- [ ] Generic types (initial design)
- [ ] Type aliases
- [ ] Interface/trait system (research)

### Performance
- [ ] Constant folding optimization
- [ ] Dead code elimination
- [ ] Benchmark suite
- [ ] Profiling tools

---

## 🔮 Future (v0.4+)

### AOT Compilation
- [ ] Research LLVM integration
- [ ] Design IR (intermediate representation)
- [ ] Code generation for x86_64
- [ ] Static binary output
- [ ] Cross-compilation support

### Advanced Features
- [ ] Pattern matching
- [ ] Destructuring
- [ ] Closures/lambdas
- [ ] Iterators protocol

### Tooling
- [ ] Language server protocol (LSP)
- [ ] Syntax highlighting (vim, vscode)
- [ ] Debugger integration (gdb)
- [ ] Package manager
- [ ] Documentation generator

### Standard Library Complete
- [ ] `net` module (full TCP/UDP)
- [ ] `http` module
- [ ] `crypto` module
- [ ] `regex` module
- [ ] `compress` module

---

## 🐛 Known Bugs

- [ ] Lexer doesn't handle all escape sequences in strings
- [ ] Parser doesn't recover from errors gracefully
- [ ] No bounds checking on stack operations
- [ ] Memory leaks in AST construction on parse errors
- [ ] Newline handling in certain contexts is inconsistent

---

## 📝 Documentation

- [x] Language specification (spec.md)
- [x] Standard library reference (stdlib.md)
- [x] Internals documentation (internals.md)
- [ ] Tutorial series
  - [ ] Getting started guide
  - [ ] Building a CLI tool
  - [ ] Writing a simple web server
  - [ ] Game development example
- [ ] API reference generator
- [ ] Code examples in docs/
- [ ] Contributing guide (detailed)
- [ ] Architecture diagrams

---

## 🧪 Testing

- [ ] Unit tests for lexer
- [ ] Unit tests for parser
- [ ] Unit tests for type checker
- [ ] Unit tests for VM
- [ ] Integration tests (end-to-end)
- [ ] Fuzzing for parser
- [ ] Performance regression tests
- [ ] Memory leak detection in CI

---

## 🏗️ Infrastructure

- [ ] CI/CD setup (GitHub Actions)
- [ ] Automated testing on commits
- [ ] Code coverage tracking
- [ ] Static analysis (clang-tidy)
- [ ] Release automation
- [ ] Binary distribution

---

## 💡 Ideas / Research Needed

- [ ] Should we support operator overloading?
- [ ] Async/await vs cooperative multitasking
- [ ] Error handling: exceptions vs optionals vs result types
- [ ] Memory model: GC vs manual vs reference counting
- [ ] Foreign function interface (FFI) design
- [ ] Macro system possibilities
- [ ] Build system integration (make vs custom)
- [ ] Standard library organization (monolithic vs modular)

---

## 📊 Metrics / Goals

### v0.1 (Current Sprint)
- **Target Date:** 2024-12-01
- **Lines of Code:** ~3000
- **Features:** Basic interpreter, variables, control flow, io.println
- **Tests:** 50+ unit tests

### v0.2 
- **Target Date:** 2025-01-15
- **Features:** Structs, arrays, error handling, stdlib expansion
- **Tests:** 150+ tests

### v0.3
- **Target Date:** 2025-03-01
- **Features:** Concurrency, modules, performance optimization
- **Tests:** 300+ tests

### v1.0 (Stable)
- **Target Date:** 2025-06-01
- **Features:** Complete language spec, comprehensive stdlib, AOT compiler
- **Tests:** 500+ tests
- **Documentation:** Complete with tutorials

---

## Notes

- Prioritize correctness over performance in early phases
- Build strong, working solutions - no half-baked features
- Keep the philosophy: explicit, minimal, zero magic
- Test on real use cases from recreational engineers
- Get feedback early and often

**Built from scratch, with focus. 🌸**
