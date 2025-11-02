# Satori Examples

Example programs demonstrating Satori language features.

## Basic Examples

### hello.sat
The classic "Hello, World!" program.

```bash
./bin/satori examples/hello.sat
```

### variables.sat
Variable declarations, type inference, and basic types.

**Demonstrates:**
- Type inference with `:=`
- Explicit type annotations
- Multiple variable declarations
- Basic data types (int, float, string, bool)

### functions.sat
Function definitions and calls.

**Demonstrates:**
- Function declarations
- Return types
- Parameters
- Recursion (factorial example)

### control_flow.sat
Control flow structures.

**Demonstrates:**
- if/else statements
- Expression-based if
- for loops with ranges
- while loops
- Infinite loop with break

### structs.sat
Struct definitions and methods.

**Demonstrates:**
- Struct declarations
- Field access
- Methods
- Struct initialization

### error_handling.sat
Error handling with the `or` operator.

**Demonstrates:**
- Providing default values
- Early returns
- Panic on critical errors
- Defer for cleanup
- Error chaining

### concurrency.sat
Lightweight concurrent tasks.

**Demonstrates:**
- `spawn` keyword
- Multiple concurrent workers
- Channels for communication
- Producer-consumer pattern

### file_io.sat
File system operations.

**Demonstrates:**
- Reading files
- Writing files
- Line-by-line processing
- Directory operations

### simple_server.sat
Basic HTTP echo server.

**Demonstrates:**
- Network sockets
- Concurrent request handling
- defer for cleanup
- Real-world application structure

## Running Examples

```bash
# Build Satori first
make

# Run any example
./bin/satori examples/<name>.sat
```

## Note

⚠️ **Many examples use features not yet implemented!**

The examples show the intended design of Satori. Current implementation status:

- ✅ Basic `io.println` with strings
- ✅ Import statements (parsing only)
- ⏳ Variables, functions, control flow - in progress
- ❌ Structs, error handling, concurrency - planned

Check `notes/TODO.md` for implementation status.

## Contributing Examples

Have a cool example? Add it!

1. Create a new `.sat` file
2. Add comments explaining what it demonstrates
3. Keep it focused on one or two concepts
4. Add it to this README
5. Submit a PR

Examples should be:
- **Clear** - Easy to understand
- **Focused** - One concept at a time
- **Practical** - Real-world use cases
- **Commented** - Explain what's happening

---

**Built from scratch, with focus. 🌸**
