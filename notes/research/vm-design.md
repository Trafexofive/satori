# Virtual Machine Design Research

## Bytecode VM Architectures

### Stack-Based vs Register-Based

| Aspect          | Stack-Based              | Register-Based           |
|----------------|--------------------------|--------------------------|
| **Examples**   | JVM, Python, Lua 5.0     | Lua 5.1+, Dalvik         |
| **Code Size**  | Smaller (implicit args)  | Larger (explicit regs)   |
| **Speed**      | Slower (more ops)        | Faster (fewer ops)       |
| **Complexity** | Simpler implementation   | More complex             |

### Decision: Stack-Based (Current)

**Satori v0.1-0.3:** Stack-based VM

```c
// Stack operations
OP_CONSTANT 0    // push constants[0]
OP_CONSTANT 1    // push constants[1]
OP_ADD          // pop 2, push result
OP_RETURN       // return top of stack
```

**Rationale:**
- Simpler to implement initially
- Easier to understand for contributors
- Smaller bytecode
- Can switch to register-based later if needed

**Future (v0.4+):** Consider register-based
- Performance-critical after features stabilize
- Lua 5.1+ showed ~50% speedup with registers

---

## Instruction Set Design

### Current Opcodes (v0.1)

```c
OP_CONSTANT,    // Load constant onto stack
OP_POP,         // Pop and discard
OP_PRINT,       // Built-in print (temporary)
OP_RETURN,      // Return from function
OP_HALT,        // Stop execution
```

### Planned Opcodes

#### Arithmetic
```c
OP_ADD,         // a + b
OP_SUBTRACT,    // a - b
OP_MULTIPLY,    // a * b
OP_DIVIDE,      // a / b
OP_MODULO,      // a % b
OP_NEGATE,      // -a
```

#### Comparison
```c
OP_EQUAL,       // a == b
OP_NOT_EQUAL,   // a != b
OP_LESS,        // a < b
OP_LESS_EQUAL,  // a <= b
OP_GREATER,     // a > b
OP_GREATER_EQUAL, // a >= b
```

#### Logical
```c
OP_NOT,         // !a
OP_AND,         // a && b (short-circuit)
OP_OR,          // a || b (short-circuit)
```

#### Variables
```c
OP_DEFINE_GLOBAL,   // Define global variable
OP_GET_GLOBAL,      // Read global variable
OP_SET_GLOBAL,      // Write global variable
OP_GET_LOCAL,       // Read local variable (by stack offset)
OP_SET_LOCAL,       // Write local variable
```

#### Control Flow
```c
OP_JUMP,            // Unconditional jump
OP_JUMP_IF_FALSE,   // Jump if top of stack is false
OP_LOOP,            // Jump backward (for loops)
```

#### Functions
```c
OP_CALL,            // Call function
OP_RETURN,          // Return from function
OP_CLOSURE,         // Create closure
```

#### Structs
```c
OP_GET_FIELD,       // obj.field
OP_SET_FIELD,       // obj.field = value
OP_METHOD,          // Define method
OP_INVOKE,          // Optimized method call
```

---

## Value Representation

### Tagged Union (Current)

```c
typedef struct {
  ValueType type;
  union {
    i64 as_int;
    f64 as_float;
    bool as_bool;
    char *as_string;
    void *as_object;
  } u;
} Value;
```

**Size:** 16 bytes (8 type + 8 union)

### Alternatives Considered

#### 1. NaN Boxing (Lua, LuaJIT)
Pack type info in unused NaN bits of float64

```c
// All values are 64 bits
// Floats: normal IEEE 754
// Others: encoded in NaN payload
```

**Pros:** Compact (8 bytes), fast
**Cons:** Complex, platform-specific

#### 2. Pointer Tagging
Use low bits of pointers for type

```c
// Last 3 bits:
// 000 = pointer to object
// 001 = small integer
// 010 = boolean
// etc.
```

**Pros:** Compact, common technique
**Cons:** Pointer alignment required

#### Decision: Tagged Union → NaN Boxing Later

**v0.1-0.3:** Tagged union (current)
- Clear, debuggable
- Straightforward implementation

**v0.4+:** NaN boxing optimization
- After feature-complete
- Benchmark to verify benefit

---

## Garbage Collection Design

### Mark-Sweep (Planned v0.2)

```
1. MARK PHASE
   ├─ Mark roots (stack, globals)
   ├─ Follow references
   └─ Mark reachable objects

2. SWEEP PHASE
   ├─ Iterate all objects
   ├─ Free unmarked
   └─ Reset marks
```

**Implementation:**

```c
typedef struct Object {
  ObjectType type;
  bool is_marked;
  struct Object *next;  // Intrusive list
} Object;

void gc_mark_roots() {
  // Mark stack values
  for (int i = 0; i < vm.stack_top; i++) {
    gc_mark_value(vm.stack[i]);
  }
  // Mark globals
  gc_mark_table(&vm.globals);
}

void gc_collect() {
  gc_mark_roots();
  gc_sweep();
}
```

### GC Triggers

**Option 1:** Allocation threshold
```c
if (bytes_allocated > next_gc) {
  gc_collect();
  next_gc = bytes_allocated * GC_HEAP_GROW_FACTOR;
}
```

**Option 2:** Object count
```c
if (num_objects > MAX_OBJECTS) {
  gc_collect();
}
```

**Decision:** Hybrid approach
- Trigger on allocation bytes primarily
- Cap at object count for small allocations

---

## Optimizations Roadmap

### Phase 1: Correctness (v0.1-0.3)
Focus on working implementation, measure don't optimize

### Phase 2: Low-Hanging Fruit (v0.4)

**Constant Folding**
```satori
let x := 2 + 3 * 4  // Compute at compile time
// Bytecode: OP_CONSTANT 14
```

**Peephole Optimization**
```
Before:
  OP_CONSTANT 0
  OP_POP
After:
  (remove both)
```

**String Interning**
```c
// Share identical strings
"hello" == "hello"  // Same pointer
```

### Phase 3: Advanced (v0.5+)

**Inline Caching (Method Calls)**
```c
// Cache method lookup result
// Invalidate on type change
struct InlineCache {
  StructType *type;
  int method_index;
};
```

**Jump Threading**
```
Before:
  JUMP addr1 → JUMP addr2 → actual_code
After:
  JUMP actual_code
```

**Dead Code Elimination**
```satori
if false        // Eliminate entire branch
    do_thing()
```

### Phase 4: JIT (v1.0+)

**Approach:** Tier-based compilation
1. Start with interpreter
2. Profile hot functions
3. JIT compile hot paths to native

**Technology Options:**
- LLVM (heavy but powerful)
- Custom x86-64 backend (lightweight)
- libjit or similar

---

## Instruction Encoding

### Current: Simple 1-byte opcodes

```
+--------+
| opcode |
+--------+
   1 byte

OR

+--------+--------+
| opcode |  arg   |
+--------+--------+
   1 byte  1 byte
```

**Limitations:**
- Max 256 constants per chunk
- Single-byte operands

### Future: Variable-length encoding

```
+--------+----------------+
| opcode | operand (1-3B) |
+--------+----------------+

OP_CONSTANT → OP_CONSTANT (1B), OP_CONSTANT_LONG (3B)
```

**Benefits:**
- More constants
- Larger jumps
- Backward compatible

---

## Call Frame Management

### Current: Implicit (stack-based)

Function calls use the value stack directly.

### Planned: Explicit Call Frames

```c
typedef struct {
  Closure *closure;      // Function being executed
  u8 *ip;               // Return address
  Value *slots;         // Stack slot for locals
} CallFrame;

CallFrame frames[MAX_FRAMES];
int frame_count;
```

**Benefits:**
- Proper recursion support
- Stack traces for errors
- Local variable scoping

---

## Bytecode File Format (Future)

For AOT compilation, save bytecode to file:

```
SATORI BYTECODE FILE FORMAT (v1)

Header (16 bytes):
  Magic: "SAT\0" (4 bytes)
  Version: u32
  Flags: u32
  Entry point: u32

Constants Section:
  Count: u32
  Constants: [Value...]

Code Section:
  Count: u32
  Instructions: [u8...]

Debug Info Section (optional):
  Line numbers
  Variable names
  Source maps
```

---

## Performance Benchmarks

### Target Performance (v1.0)

| Benchmark              | Target       | Baseline      |
|------------------------|--------------|---------------|
| Empty function call    | < 10 ns      | CPython: 100ns|
| Fib(30) recursive      | < 500 ms     | Lua: 400ms    |
| String concat (100k)   | < 100 ms     | Python: 150ms |
| Simple HTTP echo       | > 10k req/s  | Go: 50k       |

### Comparison Matrix

```
            | Satori Goal | Python | Lua   | Go      | C       |
------------|-------------|--------|-------|---------|---------|
Speed       | 5x          | 1x     | 10x   | 100x    | 1000x   |
Memory      | 2x          | 1x     | 0.5x  | 2x      | 0.1x    |
Startup     | Fast        | Slow   | Fast  | Medium  | Instant |
```

---

## Threading Model (Future)

### Phase 1 (v0.3): Single-threaded cooperative

```satori
spawn task1()
spawn task2()
// Both run on same thread, yield explicitly
```

### Phase 2 (v0.5): Multi-threaded runtime

```
┌─────────────┐
│   Scheduler │
└──────┬──────┘
       │
   ┌───┴───┬───────┬───────┐
   │       │       │       │
  T1      T2      T3      T4  (OS Threads)
   │       │       │       │
  Work-Stealing Queues
```

**Design:**
- M:N threading (M tasks on N OS threads)
- Work-stealing scheduler
- Per-thread VM state
- Shared heap with locking

**Challenges:**
- GC with multiple threads (stop-the-world)
- Lock-free data structures
- Race condition debugging

---

## Debug Support

### Bytecode Disassembler

```bash
$ satori --disasm program.sat

0000  OP_CONSTANT    0  // "Hello"
0002  OP_PRINT       1
0004  OP_RETURN
0005  OP_HALT
```

### Stack Trace on Error

```
Runtime Error: Division by zero
  at calculate (math.sat:42)
  at process_data (main.sat:15)
  at main (main.sat:5)
```

### Breakpoint Support (Future)

Integration with debuggers:
- gdb/lldb protocol
- Custom debugger protocol
- Breakpoints in source
- Variable inspection

---

## References

- "Crafting Interpreters" - Robert Nystrom (primary guide)
- Lua 5.x source code (excellent VM design)
- CPython bytecode implementation
- BEAM (Erlang VM) for concurrency ideas
- "Virtual Machines: Versatile Platforms for Systems and Processes"

---

**Notes:**
- Start simple, optimize later
- Measure before optimizing
- Learn from Lua (best-in-class small VM)
- Prioritize debuggability in early versions
