# Satori Developer Learning Path

A structured curriculum for building Satori from scratch.

## 🎯 Learning Objectives

By following this path, you will:
- Understand how programming languages are implemented
- Build a complete interpreter from scratch
- Learn compiler design patterns
- Master virtual machine architecture
- Implement garbage collection
- Design and implement a type system
- Optimize for performance

---

## 📚 Week-by-Week Curriculum

### Week 1: Foundations

**Goal:** Understand the basics of language implementation

**Reading:**
- [ ] Crafting Interpreters: Chapters 1-3 (Introduction, Map of Territory)
- [ ] Crafting Interpreters: Chapters 4-5 (Scanning, Representing Code)
- [ ] Bob Nystrom: "What Color is Your Function?" (async/sync)
- [ ] Go language spec: Lexical elements section

**Coding:**
- [ ] Study Satori's current lexer implementation
- [ ] Add missing token types (if any)
- [ ] Improve error messages in lexer
- [ ] Write lexer unit tests

**Exercise:**
Create a simple calculator lexer that tokenizes: `2 + 3 * (4 - 1)`

**Key Concepts:**
- Lexical analysis
- Token streams
- Regular expressions vs hand-written scanners
- Error reporting with source locations

---

### Week 2: Parsing & AST

**Goal:** Build and understand the parser

**Reading:**
- [ ] Crafting Interpreters: Chapters 6-8 (Parsing, Evaluating, Statements)
- [ ] Language Implementation Patterns: Chapters 1-5
- [ ] "Parsing Techniques" by Grune & Jacobs (intro chapter)

**Coding:**
- [ ] Extend parser to handle arithmetic expressions
- [ ] Add binary operators with precedence
- [ ] Implement statement parsing
- [ ] Write parser unit tests

**Exercise:**
Parse: `let x = 2 + 3 * 4` into an AST and pretty-print it

**Key Concepts:**
- Recursive descent parsing
- Operator precedence
- Abstract Syntax Trees
- Grammar design
- Parse error recovery

---

### Week 3-4: Type Checking

**Goal:** Implement static type checking

**Reading:**
- [ ] Types and Programming Languages: Chapters 1-3, 8-9
- [ ] Crafting Interpreters: Chapter 11 (Resolving and Binding)
- [ ] "A Brief Introduction to Type Theory" (online)
- [ ] Go spec: Type system section

**Coding:**
- [ ] Design type representation
- [ ] Implement symbol table
- [ ] Build type checker for expressions
- [ ] Add type inference for `:=`
- [ ] Write type checker tests

**Exercise:**
Type check: `let x := 42; let y: float = x` (should error)

**Key Concepts:**
- Type systems (static vs dynamic)
- Type inference
- Symbol tables and scopes
- Type compatibility
- Error messages for type errors

---

### Week 5-6: Control Flow & Functions

**Goal:** Add control structures and functions

**Reading:**
- [ ] Crafting Interpreters: Chapters 9-10, 12-13
- [ ] SICP: Chapter 1 (procedures)
- [ ] Lua paper: Section on function calls

**Coding:**
- [ ] Implement if/else statements
- [ ] Add loops (for, while, loop)
- [ ] Implement break/continue
- [ ] Add function declarations
- [ ] Implement function calls with arguments
- [ ] Add return statements

**Exercise:**
Implement factorial: 
```satori
int factorial(int n)
    if n <= 1: return 1
    return n * factorial(n - 1)
```

**Key Concepts:**
- Control flow graphs
- Jump instructions
- Call frames
- Stack management
- Recursion

---

### Week 7-8: Bytecode VM

**Goal:** Build a stack-based virtual machine

**Reading:**
- [ ] Crafting Interpreters: Chapters 14-24 (entire VM section)
- [ ] Lua 5.0 Implementation paper (COMPLETE)
- [ ] "A No-Frills Intro to Lua 5.1 VM Instructions"
- [ ] Study CPython bytecode

**Coding:**
- [ ] Design instruction set
- [ ] Implement VM execution loop
- [ ] Add arithmetic/comparison opcodes
- [ ] Implement local variables
- [ ] Add jump instructions
- [ ] Compile functions to bytecode

**Exercise:**
Compile and run: `let sum = 0; for i in 0..10: sum += i`

**Key Concepts:**
- Stack-based vs register-based VMs
- Instruction encoding
- Bytecode design
- VM stack management
- Instruction dispatch

---

### Week 9-10: Garbage Collection

**Goal:** Implement automatic memory management

**Reading:**
- [ ] Crafting Interpreters: Chapters 25-26 (GC)
- [ ] GC Handbook: Chapters 1-4 (mark-sweep)
- [ ] "Baby's First Garbage Collector" (blog)
- [ ] Lua GC documentation

**Coding:**
- [ ] Implement mark phase (trace roots)
- [ ] Implement sweep phase
- [ ] Add GC triggers
- [ ] Implement weak references
- [ ] Stress test GC

**Exercise:**
Detect and fix memory leaks in current implementation

**Key Concepts:**
- Tracing garbage collection
- Mark-and-sweep algorithm
- Roots and reachability
- GC performance tuning
- Generational GC (theory)

---

### Week 11: Structs & Methods

**Goal:** Add user-defined types

**Reading:**
- [ ] Crafting Interpreters: Chapters 27-28 (Classes)
- [ ] Object-oriented programming patterns
- [ ] Method dispatch techniques

**Coding:**
- [ ] Implement struct declarations
- [ ] Add field access
- [ ] Implement methods
- [ ] Add method calls
- [ ] Handle `this` pointer

**Exercise:**
```satori
struct Point {
    int x, y
    int distance() -> math.sqrt(x*x + y*y)
}
```

**Key Concepts:**
- Record types
- Method dispatch
- Member access
- Value vs reference semantics

---

### Week 12: Error Handling

**Goal:** Implement `or` operator and optionals

**Reading:**
- [ ] "The Error Model" by Joe Duffy (FULL ARTICLE)
- [ ] Rust error handling documentation
- [ ] Zig error unions documentation

**Coding:**
- [ ] Design optional type representation
- [ ] Implement `or` operator
- [ ] Add `panic` function
- [ ] Implement `defer` statement
- [ ] Test error propagation

**Exercise:**
```satori
let file = io.open("data.txt") or return
defer file.close()
```

**Key Concepts:**
- Error handling models
- Optional/Result types
- Exception-free error handling
- RAII and defer

---

### Month 4: Standard Library

**Goal:** Build essential stdlib modules

**Reading:**
- [ ] POSIX API documentation
- [ ] Go standard library source
- [ ] Lua standard library implementation

**Coding:**
- [ ] Implement io module (file operations)
- [ ] Add collections (List, Map, Set)
- [ ] Build string module
- [ ] Implement math module
- [ ] Add time module

**Exercise:**
Build a grep clone using stdlib

**Key Concepts:**
- FFI (Foreign Function Interface)
- Native functions
- System programming
- API design

---

### Month 5: Concurrency

**Goal:** Add cooperative multitasking

**Reading:**
- [ ] "Communicating Sequential Processes" (selected chapters)
- [ ] Go concurrency patterns
- [ ] Lua coroutine documentation
- [ ] BEAM scheduler design

**Coding:**
- [ ] Implement coroutine/task primitives
- [ ] Add `spawn` keyword
- [ ] Build cooperative scheduler
- [ ] Add `yield` operation
- [ ] Design channel primitives

**Exercise:**
```satori
spawn worker(data)
spawn worker(data)
// Concurrent processing
```

**Key Concepts:**
- Coroutines vs threads
- Cooperative vs preemptive scheduling
- Message passing
- Synchronization

---

### Month 6: Optimization

**Goal:** Make it fast

**Reading:**
- [ ] Computer Systems: A Programmer's Perspective (Ch. 5-6)
- [ ] LuaJIT performance guide
- [ ] "Optimization of Dynamic Languages" papers
- [ ] Inline caching papers

**Coding:**
- [ ] Profile current implementation
- [ ] Implement constant folding
- [ ] Add peephole optimization
- [ ] Implement string interning
- [ ] Add inline caching for method calls
- [ ] Benchmark suite

**Exercise:**
Optimize fibonacci to be 2x faster

**Key Concepts:**
- Profiling techniques
- Common optimizations
- Inline caching
- JIT compilation (theory)
- Benchmarking methodology

---

## 🎓 Advanced Topics (After Core Implementation)

### AOT Compilation (Month 7+)

**Reading:**
- [ ] LLVM Tutorial (complete)
- [ ] "Engineering a Compiler" (code generation chapters)
- [ ] "Modern Compiler Implementation in C"

**Project:**
Implement ahead-of-time compiler to native code

---

### Advanced Type System (Month 8+)

**Reading:**
- [ ] "Types and Programming Languages" (advanced chapters)
- [ ] Generics in other languages (Rust, C++, Java)
- [ ] Hindley-Milner type inference

**Project:**
Add generic types to Satori

---

### JIT Compilation (Month 9+)

**Reading:**
- [ ] LuaJIT internals documentation
- [ ] "Trace-based JIT compilation" papers
- [ ] V8 optimization techniques

**Project:**
Build a tracing JIT compiler

---

## 🛠️ Practical Exercises

### Mini-Projects to Build Along the Way

1. **Calculator** (Week 1-2)
   - Lexer + parser for math expressions
   - Evaluate with tree-walk interpreter

2. **JSON Parser** (Week 3-4)
   - Complete JSON parser in Satori
   - Tests for all JSON features

3. **Todo CLI App** (Week 5-6)
   - File I/O, data structures
   - Command parsing
   - Persistence

4. **Simple HTTP Server** (Week 11-12)
   - TCP sockets
   - HTTP request parsing
   - Concurrent request handling

5. **Game of Life** (Month 4)
   - 2D arrays
   - Game loop
   - Visualization (text-based)

6. **Package Manager** (Month 5+)
   - Dependency resolution
   - Version management
   - Download and installation

---

## 📊 Progress Tracking

### Checklist

```markdown
## Core Features
- [ ] Lexer (Week 1)
- [ ] Parser (Week 2)
- [ ] Type Checker (Week 3-4)
- [ ] Control Flow (Week 5-6)
- [ ] VM (Week 7-8)
- [ ] GC (Week 9-10)
- [ ] Structs (Week 11)
- [ ] Error Handling (Week 12)
- [ ] Standard Library (Month 4)
- [ ] Concurrency (Month 5)
- [ ] Optimization (Month 6)

## Advanced
- [ ] AOT Compiler
- [ ] Generics
- [ ] JIT Compiler
```

### Knowledge Checkpoints

After each section, you should be able to:

**Week 1:** Explain how tokenization works, implement a lexer from scratch

**Week 2:** Draw a parse tree, explain operator precedence, handle parse errors

**Week 4:** Explain type inference, implement a symbol table, check type compatibility

**Week 6:** Implement any control structure, explain call frames

**Week 8:** Design a bytecode instruction set, explain VM execution

**Week 10:** Explain mark-and-sweep GC, identify GC roots

**Month 6:** Profile code, identify bottlenecks, apply optimizations

---

## 🎯 Learning Resources by Topic

### When Stuck On...

**Parsing:**
- Crafting Interpreters Ch. 6
- "Parsing Techniques" online resources
- r/ProgrammingLanguages

**Type Systems:**
- Types and Programming Languages
- Hindley-Milner tutorial
- TypeScript/Rust documentation

**VM Design:**
- Lua papers
- Crafting Interpreters Part III
- Study Wren source code

**GC:**
- GC Handbook
- "Baby's First GC" blog
- Study Lua GC code

**Performance:**
- Computer Systems book
- LuaJIT wiki
- Profile your code!

---

## 💡 Study Tips

1. **Read → Implement → Reflect**
   - Don't just read, code along
   - Implement concepts immediately
   - Write notes on what you learned

2. **Start Simple**
   - Get something working first
   - Refine and optimize later
   - Don't get stuck in perfectionism

3. **Debug by Understanding**
   - Use GDB/LLDB
   - Add assertions liberally
   - Print intermediate states

4. **Learn from Others**
   - Study Lua source code
   - Read Wren implementation
   - Join programming language communities

5. **Build Something Real**
   - Use your language for actual projects
   - Find pain points
   - Iterate on design

---

**Estimated Timeline:** 6 months to working interpreter, 12 months to production-ready

**Remember:** Building a language is a marathon, not a sprint. Enjoy the journey! 🌸
