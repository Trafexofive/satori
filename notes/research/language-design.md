# Language Design Research

## Type System Considerations

### Static Typing Approaches

**Option 1: Hindley-Milner (ML/Haskell style)**
- Pros: Complete type inference, elegant
- Cons: Error messages can be cryptic, learning curve
- Example: OCaml, Haskell

**Option 2: Local Type Inference (Go/Rust style)**
- Pros: Clear, explicit where needed, good errors
- Cons: More type annotations required
- Example: Go, Rust, C#
- **⭐ Current choice for Satori**

**Option 3: Gradual Typing (TypeScript style)**
- Pros: Flexible, can start untyped
- Cons: Runtime overhead, complexity
- Example: TypeScript, Python with mypy

### Decision: Local Type Inference
```satori
let x := 42              // Inferred as int
let y: float = 3.14      // Explicit when needed
```

**Rationale:**
- Aligns with "explicit over implicit" philosophy
- Good error messages
- No runtime type checks needed
- Similar to Go/Zig, familiar to systems programmers

---

## Error Handling Models

### Survey of Approaches

| Language   | Model              | Pros                          | Cons                    |
|------------|-------------------|-------------------------------|-------------------------|
| Go         | Multiple returns  | Explicit, clear               | Verbose                 |
| Rust       | Result<T, E>      | Type-safe, composable         | Complex for beginners   |
| Zig        | Error unions      | Explicit, lightweight         | New concept             |
| Java/C#    | Exceptions        | Familiar, clean happy path    | Hidden control flow     |
| Lua/Nim    | nil + or pattern  | Simple, scripting-friendly    | Less type safety        |

### Satori Choice: `or` operator with optionals

```satori
let file := io.open("data.txt") or panic("Cannot open")
let data := file.read() or return
let num := parse_int(str) or 0
```

**Rationale:**
- Explicit at call sites (visible error handling)
- Scripting-friendly (concise)
- No hidden exceptions
- Composes well with `defer`

**Implementation:**
- Functions return `T?` (optional type)
- `or` operator: if left is nil, evaluate right
- Right side can be: expression, return, panic, continue, break

---

## Memory Management

### Options Evaluated

**1. Garbage Collection**
- Pros: Simple for users, no manual memory management
- Cons: Pauses, memory overhead, non-deterministic
- Examples: Java, Go, Python

**2. Manual Memory Management**
- Pros: Full control, predictable
- Cons: Error-prone, cognitive load
- Examples: C, C++

**3. Reference Counting**
- Pros: Deterministic cleanup, simple
- Cons: Cycle issues, overhead
- Examples: Swift, Python (CPython)

**4. Ownership + Borrow Checker**
- Pros: Safe, no GC, no runtime cost
- Cons: Complex, steep learning curve
- Examples: Rust

### Decision: Tracing GC (Mark-Sweep)

**Phase 1 (Current):** Simple mark-and-sweep
```c
// Mark phase: trace from roots
mark_roots();
mark_reachable();

// Sweep phase: free unmarked
sweep_memory();
```

**Phase 2 (Future):** Incremental GC
- Smaller pause times
- Interleave marking with execution

**Rationale:**
- Scripting language expectations
- Simplifies usage for recreational engineers
- Can add manual memory management later if needed
- Most overhead acceptable for v0.1

---

## Concurrency Model

### Research Notes

**1. OS Threads (Java, C++)**
- Heavy, expensive context switches
- Good for CPU-bound parallel work
- ❌ Too heavyweight for scripting

**2. Green Threads / Goroutines (Go)**
- Lightweight, thousands possible
- Preemptive scheduling
- Requires runtime scheduler
- ✅ Good model, but complex

**3. Async/Await (Rust, JS, Python)**
- Explicit async points
- Colored functions problem
- Good for I/O bound
- ⚠️ Too complex for v0.1

**4. Cooperative Coroutines (Lua)**
- Simple, explicit yielding
- Full control over scheduling
- No parallelism (single-threaded)
- ✅ **Best for Satori v0.1**

### Decision: Cooperative Tasks

```satori
spawn worker_task(data)
spawn {
    loop
        do_work()
        yield  // Explicit yield point
}
```

**Implementation Plan:**
1. v0.1: Cooperative coroutines (Lua-style)
2. v0.2: Add preemptive scheduler (Go-style)
3. v0.3: Multi-threaded runtime with work-stealing

**Rationale:**
- Start simple, explicit
- Can evolve to preemptive later
- Fits "no magic" philosophy
- Good for web servers, games

---

## Module System

### Approaches Compared

**Python:** Import anything, dynamic
**JavaScript:** ES6 modules, static
**Go:** Package based, explicit
**Rust:** Crate system, explicit dependencies

### Satori Design

```satori
import io
import net
import myproject.utils

io.println "Hello"
net.listen("0.0.0.0", 8080)
utils.helper()
```

**Rules:**
1. No wildcard imports (no `import *`)
2. Always qualified access (`io.println`, not `println`)
3. Module = file or directory with init
4. Resolution: stdlib → local → SATORI_PATH

**Rationale:**
- Explicit, clear where things come from
- No namespace pollution
- Easy to understand
- Simple implementation

---

## Syntax Decisions

### Indentation vs Braces

**Option A: Braces (C-style)**
```c
if (condition) {
    do_something();
}
```

**Option B: Indentation (Python)**
```python
if condition:
    do_something()
```

**Option C: Hybrid (Satori)**
```satori
if condition
    do_something()

struct Point {
    int x, y
}
```

**Decision:** Significant whitespace for flow, braces for data
- Control flow: indentation
- Structs/data: braces
- Best of both worlds

### Semicolons

**Decision:** Optional, automatically inserted
- Newline ends statement by default
- Semicolon for multiple statements per line
- Like Go, Swift, JavaScript (ASI)

---

## Performance Goals

### Benchmark Targets (v1.0)

| Operation              | Target       | Notes                    |
|-----------------------|--------------|--------------------------|
| Function call         | < 10ns       | Comparable to Lua        |
| Variable access       | < 2ns        | Local variable lookup    |
| String concatenation  | < 100ns      | Small strings            |
| JSON parse (1MB)      | < 50ms       | Competitive with C libs  |
| HTTP request handling | > 10k req/s  | Simple echo server       |
| GC pause              | < 10ms       | 95th percentile          |

### Optimization Strategy

**Phase 1 (v0.1-0.3):** Correctness first
- Naive implementation
- Focus on features
- Measure, don't optimize

**Phase 2 (v0.4-0.9):** Targeted optimization
- Profile real workloads
- Optimize hot paths
- Benchmark regression suite

**Phase 3 (v1.0+):** Advanced optimization
- JIT compilation for hot functions
- Inline caching
- Escape analysis for stack allocation

---

## Standard Library Philosophy

### Principles

1. **Batteries included, but not too many**
   - Cover common use cases
   - Don't try to do everything
   - Let packages fill niches

2. **Predictable, boring APIs**
   - No surprises
   - Consistent naming
   - Explicit over clever

3. **Production-ready quality**
   - Well-tested
   - Good error messages
   - Performance matters

### Core vs Extended

**Core (always available):**
- io, fs, string, math
- collections (List, Map, Set)
- os basics

**Extended (opt-in):**
- net, http
- json, xml
- crypto
- compress

---

## FFI (Foreign Function Interface)

### Research: Calling C from Satori

```satori
// Idea for v0.4+
@extern("libc.so.6")
void printf(string format, ...args)

printf("Hello from C! %d\n", 42)
```

**Considerations:**
- Safety: type checking at boundary
- ABI compatibility
- Error handling across boundaries
- Memory ownership

**Inspiration:**
- Zig: Clean C interop
- Rust: `extern "C"` blocks
- Go: cgo (but simpler)

---

## Inspiration & References

### Languages Studied

**Primary Influences:**
- **C**: Low-level control, explicit, no magic
- **Lua**: Lightweight, scripting ergonomics
- **Go**: Simple concurrency, practical design
- **Zig**: Explicit over implicit, C interop

**Secondary Influences:**
- Python: Readability, batteries included
- Rust: Error handling, type safety
- Nim: Pragmatic choices
- Wren: Simplicity

### Papers & Resources

- "The Design and Implementation of Lua" - R. Ierusalimschy
- "Crafting Interpreters" - Robert Nystrom
- "Engineering a Compiler" - Cooper & Torczon
- Go blog: Error handling, concurrency patterns
- Zig documentation: Philosophy

---

## Open Questions

1. **Should we support operator overloading?**
   - Pros: Natural math operations for custom types
   - Cons: Can be abused, complex
   - Leaning: NO for v1.0, maybe later

2. **Nullable by default or explicit optionals?**
   - Current: Explicit `T?` for optional
   - Alternative: All types nullable (like Java)
   - Decision: Explicit optionals (Rust/Swift style)

3. **Should methods be syntactic sugar or special?**
   - Current: Methods are struct-scoped functions
   - No dynamic dispatch (for now)
   - No inheritance

4. **Generic programming: when and how?**
   - v0.1-0.3: No generics
   - v0.4+: Monomorphization (Rust/C++ style)
   - Avoid type erasure complexity

---

**Living document - update as we learn**
