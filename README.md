# 🌸 Satori

**A statically typed scripting language for recreational engineers who build from scratch.**

Satori combines the safety of static typing with the ergonomics of scripting languages. Built in C, designed for clarity, made for your own infrastructure.

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg)]()
[![Version](https://img.shields.io/badge/version-0.1.0--alpha-orange.svg)]()

## Philosophy

- **Explicit over implicit** - Know what the machine does
- **Minimal ceremony** - Write less, express more  
- **Production ready** - Strong working solutions for real systems
- **Zero magic** - No hidden runtime costs
- **Build for yourself** - Your infra, your rules, your wheel to reinvent

## Features

- 🎯 **Static typing** with type inference
- 🔧 **Simple OOP** - structs with methods, no inheritance
- ⚡ **Lightweight concurrency** with `spawn`
- 🛡️ **Explicit error handling** with `or` operator
- 📦 **Module system** - no header file dance
- 🔗 **C FFI** - seamless interop
- 🎨 **Script or compile** - interpreted for dev, AOT for prod

## Quick Example

```satori
import io

struct Point {
    int x, y
    
    int distance()
        return x*x + y*y  // Simplified distance squared
}

let p := Point{3, 4}
io.println "Distance: {}", p.distance()
```

## More Examples

### Simple Server
```satori
import io
import net

void handle_client(net.Socket client)
    defer client.close()
    let request := client.read(4096) or return
    client.write("HTTP/1.0 200 OK\r\n\r\nHello!\n".bytes())

let listener := net.listen("0.0.0.0", 8080) or panic("Cannot bind")
io.println "Server running on :8080"

loop
    let client := listener.accept() or continue
    spawn handle_client(client)
```

### Error Handling
```satori
import io
import fs

void process_config()
    let file := fs.open("config.json", "r") or return
    defer file.close()
    
    let content := file.read_all() or return
    let config := json.parse(content) or panic("Invalid JSON")
    
    io.println "Config loaded: {}", config
```

### Functions & Control Flow
```satori
import io

int factorial(int n)
    if n <= 1: return 1
    return n * factorial(n - 1)

for i in 0..10
    io.println "{}! = {}", i, factorial(i)
```

More examples in [`examples/`](examples/)

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

io.println "Hello, World!"
```

Run it:
```bash
./bin/satori hello.sat
```

## Language Tour

### Variables

```satori
let x := 42              // Type inference
let y: float = 3.14      // Explicit type
let name := "Satori"     // Strings
let active := true       // Booleans
```

### Functions

```satori
int add(int a, int b)
    return a + b

void greet(string name)
    io.println "Hello, {}", name
```

### Structs & Methods

```satori
struct Rectangle {
    int width, height
    
    int area()
        return width * height
    
    void scale(int factor)
        width *= factor
        height *= factor
}

let rect := Rectangle{10, 20}
io.println "Area: {}", rect.area()
```

### Control Flow

```satori
// If/else
if x > 10
    io.println "Large"
else
    io.println "Small"

// For loops
for i in 0..10
    io.println i

// While loops
while condition
    do_work()

// Infinite loop
loop
    if should_break: break
```

### Error Handling

```satori
// Provide default
let port := parse_int(env("PORT")) or 8080

// Early return
let file := fs.open("data.txt") or return

// Panic on critical error
let db := connect_db() or panic("Database required")

// Always cleanup with defer
defer file.close()
```

### Concurrency

```satori
// Spawn lightweight tasks
spawn worker(data)
spawn another_worker()

// Cooperative scheduling
loop
    do_work()
    yield  // Let other tasks run
```

## Project Status

🚧 **Early Development** - Core interpreter in progress

### Implemented ✅
- [x] Lexer - Full tokenization
- [x] Parser - AST construction
- [x] Basic VM - Bytecode execution
- [x] Simple codegen - AST to bytecode
- [x] `io.println` - Basic output

### In Progress 🚧
- [ ] Type checker - Type inference and validation
- [ ] Variables - Declaration and assignment
- [ ] Control flow - if/else, loops
- [ ] Functions - Declarations and calls
- [ ] Error reporting - Better diagnostics

### Planned 📋
- [ ] Structs - User-defined types
- [ ] Standard library - io, fs, net, collections
- [ ] Concurrency - spawn, channels
- [ ] Error handling - `or` operator, defer
- [ ] AOT compiler - Native code generation

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

### v0.1 - Core Interpreter (Current)
- Lexer, parser, basic VM
- Variables and functions
- Control flow structures
- Basic type checking

### v0.2 - Language Features
- Structs with methods
- Arrays and slices
- Error handling (`or`, `defer`)
- Standard library basics

### v0.3 - Concurrency & Modules
- Lightweight tasks (`spawn`)
- Module system
- Channels for communication
- Performance optimization

### v0.4 - AOT Compilation
- Compile to native code
- LLVM backend
- Optimization passes
- Static linking

### v1.0 - Stable Release
- Complete language spec
- Comprehensive stdlib
- Full documentation
- Production ready

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
