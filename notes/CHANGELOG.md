# Changelog

All notable changes to Satori will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Planned for v0.1
- [ ] Type checker implementation
- [ ] Variable declarations and assignment
- [ ] Control flow (if/else, for, while, loop)
- [ ] Functions (declaration, calls, returns)
- [ ] Basic arithmetic and comparison operations
- [ ] Format string interpolation in io.println

## [0.0.2] - 2024-11-02

### Added
- Comprehensive documentation in `docs/`
  - Language specification (`spec.md`)
  - Standard library reference (`stdlib.md`)
  - Internals documentation (`internals.md`)
- Notes and research folder with:
  - TODO list
  - Language design research
  - VM design notes
  - Code patterns and examples
  - Performance benchmarks plan
  - Standard library implementation sketches
  - Brainstorming document

### Changed
- Enhanced README with better examples and clearer philosophy

## [0.0.1] - 2024-11-02

### Added
- Initial project structure
- Lexer implementation
  - Token recognition for all operators and keywords
  - String and number literal parsing
  - Comment handling
  - Line and column tracking
- Parser implementation
  - Recursive descent parser
  - AST construction for basic expressions
  - Import statements
  - Function calls
  - Member access (dot notation)
- AST (Abstract Syntax Tree)
  - Node types for literals, identifiers, calls
  - Memory management functions
  - Pretty-printer for debugging
- Virtual Machine (basic)
  - Stack-based bytecode interpreter
  - Value representation (tagged union)
  - Constant pool
  - Basic opcodes: CONSTANT, PRINT, HALT
- Code Generator
  - AST to bytecode compilation
  - Special case for `io.println`
- Memory management
  - Basic allocation functions
  - Dynamic array growth strategy
- Error reporting
  - Source location tracking
  - Formatted error messages
- Build system
  - Makefile with debug/release targets
  - Test target
- Basic example: `examples/hello.sat`

### Development Tools
- Command-line options:
  - `-t, --tokens` - Dump tokens
  - `-a, --ast` - Dump AST
  - `-i, --interpret` - Interpret mode
  - `-v, --version` - Show version
  - `-h, --help` - Show help

### Known Limitations
- No type checking yet
- No variables or assignments
- No control flow
- No functions (only method calls)
- Only `io.println` works from stdlib
- Minimal error recovery in parser

## [0.0.0] - 2024-11-01

### Added
- Initial repository setup
- Project vision and philosophy
- Basic directory structure
- License (GPL v3)
- README with language overview

---

## Version History Summary

- **v0.0.0** - Project inception, vision
- **v0.0.1** - Basic lexer, parser, AST, VM skeleton
- **v0.0.2** - Comprehensive documentation
- **v0.1** (planned) - Type checker, variables, control flow, functions
- **v0.2** (planned) - Structs, arrays, error handling, stdlib expansion
- **v0.3** (planned) - Concurrency, modules, performance optimization
- **v1.0** (planned) - Stable language, complete stdlib, AOT compiler

---

**Built from scratch, with focus. 🌸**
