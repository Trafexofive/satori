# Reading List & Resources

Essential books, papers, articles, and resources for building Satori.

## 📚 Essential Books

### Compiler & Interpreter Design

**1. "Crafting Interpreters" by Robert Nystrom**
- **URL:** https://craftinginterpreters.com/
- **Status:** FREE online, also in print
- **Why:** THE definitive guide for building interpreters. Covers both tree-walk and bytecode VMs.
- **Topics:** Scanning, parsing, variables, control flow, functions, closures, classes, GC
- **Priority:** ⭐⭐⭐⭐⭐ MUST READ (reading this is like taking a masterclass)
- **Code:** Java (tree-walk), C (bytecode VM) - very similar to Satori's architecture

**2. "Engineering a Compiler" by Cooper & Torczon**
- **ISBN:** 978-0120884780
- **Why:** Comprehensive compiler theory and practice
- **Topics:** Lexical analysis, parsing, optimization, code generation
- **Priority:** ⭐⭐⭐⭐ Reference for advanced topics
- **Note:** Academic but practical

**3. "Modern Compiler Implementation in C" by Andrew Appel**
- **ISBN:** 978-0521607650
- **Why:** Complete compiler implementation in C
- **Topics:** All phases of compilation, including optimization
- **Priority:** ⭐⭐⭐ Good reference
- **Alternatives:** Also available in ML and Java versions

**4. "Language Implementation Patterns" by Terence Parr**
- **ISBN:** 978-1934356456
- **Why:** Practical patterns for parsers, interpreters, bytecode VMs
- **Topics:** Tree construction, symbol tables, type checking
- **Priority:** ⭐⭐⭐⭐ Very practical
- **Author:** Creator of ANTLR parser generator

---

### Programming Language Design

**5. "Programming Language Pragmatics" by Michael Scott**
- **ISBN:** 978-0124104099
- **Why:** Comprehensive overview of language design choices
- **Topics:** Syntax, semantics, type systems, control flow, concurrency
- **Priority:** ⭐⭐⭐⭐ Excellent survey

**6. "Types and Programming Languages" by Benjamin Pierce**
- **ISBN:** 978-0262162098
- **Why:** Formal treatment of type systems
- **Topics:** Type theory, type checking, type inference
- **Priority:** ⭐⭐⭐ Academic but thorough
- **Note:** Heavy on theory, light on implementation

**7. "Essentials of Programming Languages" by Friedman & Wand**
- **ISBN:** 978-0262062794
- **Why:** Understanding interpreters through different paradigms
- **Topics:** Interpretation techniques, environments, continuations
- **Priority:** ⭐⭐⭐ Academic approach

---

### Virtual Machines & Runtime Systems

**8. "Virtual Machines: Versatile Platforms for Systems and Processes" by Smith & Nair**
- **ISBN:** 978-1558609105
- **Why:** Comprehensive VM design
- **Topics:** Process VMs, system VMs, emulation, JIT compilation
- **Priority:** ⭐⭐⭐ Reference for VM internals

---

### Garbage Collection

**9. "The Garbage Collection Handbook" by Jones, Hosking & Moss**
- **ISBN:** 978-1420082791
- **Why:** Everything about GC algorithms
- **Topics:** Mark-sweep, copying, generational, incremental, concurrent GC
- **Priority:** ⭐⭐⭐⭐ Essential for GC implementation
- **Note:** Dense but comprehensive

---

### General Computer Science

**10. "Structure and Interpretation of Computer Programs" (SICP)**
- **URL:** https://mitp-content-server.mit.edu/books/content/sectbyfn/books_pres_0/6515/sicp.zip/index.html
- **Status:** FREE online
- **Why:** Classic on abstraction, evaluation, and interpreters
- **Topics:** Metacircular evaluator, lazy evaluation, nondeterministic computing
- **Priority:** ⭐⭐⭐⭐ Mind-expanding
- **Language:** Scheme

**11. "The Art of Computer Programming" Vol 1 by Donald Knuth**
- **ISBN:** 978-0201896831
- **Why:** Fundamental algorithms and data structures
- **Topics:** Basic algorithms, data structures, analysis
- **Priority:** ⭐⭐⭐ Reference
- **Note:** Very detailed, mathematical

---

## 📄 Papers & Articles

### Must-Read Papers

**1. "The Implementation of Lua 5.0" by Roberto Ierusalimschy et al.**
- **URL:** https://www.lua.org/doc/jucs05.pdf
- **Why:** Learn from one of the best-designed VMs
- **Topics:** Register-based VM, closures, coroutines
- **Priority:** ⭐⭐⭐⭐⭐ MUST READ

**2. "A No-Frills Introduction to Lua 5.1 VM Instructions" by Kein-Hong Man**
- **URL:** http://luaforge.net/docman/83/98/ANoFrillsIntroToLua51VMInstructions.pdf
- **Why:** Detailed bytecode instruction set
- **Priority:** ⭐⭐⭐⭐

**3. "The Design and Implementation of the Anyfish Scripting Language"**
- **Why:** Modern scripting language design decisions
- **Topics:** Type system, error handling, performance

**4. "LuaJIT 2.0 Intellectual Property Disclosure and Research Opportunities" by Mike Pall**
- **URL:** http://wiki.luajit.org/SSA-IR-2.0
- **Why:** Advanced JIT compilation techniques
- **Topics:** Trace compilation, SSA IR
- **Priority:** ⭐⭐⭐ For future JIT work

**5. "Simple and Effective Type Check Removal through Lazy Basic Block Versioning"**
- **Why:** Optimization technique for dynamic languages
- **Priority:** ⭐⭐ Advanced optimization

---

### Error Handling

**6. "The Error Model" by Joe Duffy**
- **URL:** http://joeduffyblog.com/2016/02/07/the-error-model/
- **Why:** Comprehensive analysis of error handling approaches
- **Topics:** Exceptions, result types, panic, contracts
- **Priority:** ⭐⭐⭐⭐ Excellent analysis

---

### Concurrency

**7. "Communicating Sequential Processes" by Tony Hoare**
- **URL:** http://www.usingcsp.com/cspbook.pdf
- **Status:** FREE
- **Why:** Foundation for Go's concurrency model
- **Topics:** Channels, processes, synchronization
- **Priority:** ⭐⭐⭐ For concurrency design

**8. "Concurrent Programming in Erlang" (Chapters on BEAM VM)**
- **Why:** Excellent actor-based concurrency model
- **Topics:** Lightweight processes, message passing
- **Priority:** ⭐⭐⭐

---

## 🌐 Online Resources

### Interactive Tutorials

**1. Crafting Interpreters Website**
- **URL:** https://craftinginterpreters.com/
- **Content:** Complete book online with exercises
- **Priority:** ⭐⭐⭐⭐⭐

**2. Build Your Own Lisp**
- **URL:** http://www.buildyourownlisp.com/
- **Why:** Hands-on interpreter building in C
- **Priority:** ⭐⭐⭐

**3. Let's Build a Simple Interpreter (Python)**
- **URL:** https://ruslanspivak.com/lsbasi-part1/
- **Why:** Step-by-step interpreter tutorial
- **Priority:** ⭐⭐⭐

---

### Language Implementation Blogs

**4. Bob Nystrom's Blog**
- **URL:** https://journal.stuffwithstuff.com/
- **Topics:** Language design, parsing, performance
- **Priority:** ⭐⭐⭐⭐⭐ Incredible insights

**5. Eli Bendersky's Website**
- **URL:** https://eli.thegreenplace.net/
- **Topics:** Compilers, interpreters, LLVM
- **Priority:** ⭐⭐⭐⭐ Great tutorials

**6. The Wren Programming Language Blog**
- **URL:** https://wren.io/
- **Why:** Modern small scripting language, excellent design
- **Priority:** ⭐⭐⭐⭐

**7. Andreas Kling's YouTube (SerenityOS)**
- **URL:** https://www.youtube.com/c/AndreasKling
- **Why:** Live coding systems programming
- **Topics:** OS development, language implementation
- **Priority:** ⭐⭐⭐ Entertaining and educational

---

### Source Code to Study

**8. Lua Source Code**
- **URL:** https://www.lua.org/source/
- **Why:** Clean, well-commented C code
- **Size:** ~20k lines - readable!
- **Priority:** ⭐⭐⭐⭐⭐ STUDY THIS

**9. Wren Source Code**
- **URL:** https://github.com/wren-lang/wren
- **Why:** Modern, clean VM implementation
- **Language:** C
- **Priority:** ⭐⭐⭐⭐

**10. CPython Source**
- **URL:** https://github.com/python/cpython
- **Why:** Production-quality interpreter
- **Size:** Large, but instructive
- **Priority:** ⭐⭐⭐ Reference

**11. Zig Source Code**
- **URL:** https://github.com/ziglang/zig
- **Why:** Modern systems language with great philosophy
- **Priority:** ⭐⭐⭐

**12. Janet Language**
- **URL:** https://github.com/janet-lang/janet
- **Why:** Small, embeddable, well-designed
- **Priority:** ⭐⭐⭐

---

## 🎓 Courses & Lectures

### Online Courses

**1. Stanford CS143: Compilers**
- **URL:** https://web.stanford.edu/class/cs143/
- **Content:** Lectures, assignments, projects
- **Status:** FREE materials online
- **Priority:** ⭐⭐⭐⭐

**2. MIT 6.035: Computer Language Engineering**
- **URL:** https://ocw.mit.edu/courses/6-035-computer-language-engineering-fall-2005/
- **Status:** FREE on MIT OCW
- **Priority:** ⭐⭐⭐

**3. Programming Languages Course by Dan Grossman (Coursera)**
- **URL:** https://www.coursera.org/learn/programming-languages
- **Why:** Functional programming perspective on language design
- **Priority:** ⭐⭐⭐

---

### YouTube Channels

**4. Tsoding Daily**
- **URL:** https://www.youtube.com/c/TsodingDaily
- **Content:** Live programming, compilers, interpreters
- **Priority:** ⭐⭐⭐⭐ Very practical

**5. Andreas Kling (as mentioned above)**

**6. Low Level Learning**
- **URL:** https://www.youtube.com/@LowLevelLearning
- **Content:** Systems programming, C, assembly
- **Priority:** ⭐⭐⭐

---

## 🛠️ Tools & Documentation

### Parser Generators (for reference)

**1. ANTLR Documentation**
- **URL:** https://www.antlr.org/
- **Why:** Understanding parser generation
- **Note:** We're hand-writing parsers, but good to know

**2. Bison/Flex Documentation**
- **URL:** https://www.gnu.org/software/bison/
- **Why:** Classic parser/lexer tools

---

### LLVM (for future AOT compilation)

**3. LLVM Tutorial**
- **URL:** https://llvm.org/docs/tutorial/
- **Why:** Building compilers with LLVM
- **Priority:** ⭐⭐⭐ For v0.4+ AOT work

**4. "LLVM Essentials" by Suyog Sarda**
- **ISBN:** 978-1785280801
- **Why:** Practical LLVM usage
- **Priority:** ⭐⭐ Future reference

---

## 📊 Benchmarking & Performance

**1. "Computer Systems: A Programmer's Perspective" by Bryant & O'Hallaron**
- **ISBN:** 978-0134092669
- **Why:** Understanding performance at the hardware level
- **Topics:** Memory hierarchy, cache, pipelines
- **Priority:** ⭐⭐⭐⭐

**2. LuaJIT Performance Guide**
- **URL:** http://wiki.luajit.org/Numerical-Computing-Performance-Guide
- **Why:** Optimization techniques
- **Priority:** ⭐⭐⭐

---

## 🔬 Research & Inspiration

### Language Communities & Discussions

**1. r/ProgrammingLanguages (Reddit)**
- **URL:** https://www.reddit.com/r/ProgrammingLanguages/
- **Why:** Active community of language implementers
- **Priority:** ⭐⭐⭐⭐

**2. Hacker News**
- **URL:** https://news.ycombinator.com/
- **Search:** "programming language" "compiler" "interpreter"
- **Priority:** ⭐⭐⭐

**3. Lambda the Ultimate**
- **URL:** http://lambda-the-ultimate.org/
- **Why:** Programming language theory discussions
- **Priority:** ⭐⭐ Academic

---

### Specific Language Documentation

**4. Go Language Specification**
- **URL:** https://go.dev/ref/spec
- **Why:** Well-written, clear spec
- **Priority:** ⭐⭐⭐⭐ Model for our spec

**5. Rust Reference**
- **URL:** https://doc.rust-lang.org/reference/
- **Why:** Comprehensive language reference
- **Priority:** ⭐⭐⭐

**6. Zig Language Reference**
- **URL:** https://ziglang.org/documentation/master/
- **Why:** Clear explanations of design choices
- **Priority:** ⭐⭐⭐⭐

---

## 📖 Reading Order Recommendation

### Phase 1: Getting Started (Week 1-2)
1. ⭐ Crafting Interpreters (Part I & II) - READ FIRST
2. ⭐ Lua 5.0 implementation paper
3. ⭐ Bob Nystrom's blog (selected articles)
4. ⭐ Study Lua source code (lexer & parser)

### Phase 2: Core Implementation (Week 3-6)
1. ⭐ Crafting Interpreters (Part III - VM)
2. ⭐ Language Implementation Patterns
3. ⭐ Study Wren source code
4. ⭐ The Error Model article

### Phase 3: Advanced Topics (Month 2-3)
1. ⭐ The Garbage Collection Handbook (relevant chapters)
2. ⭐ Engineering a Compiler (selected chapters)
3. ⭐ CSP paper (for concurrency)
4. ⭐ Stanford CS143 lectures

### Phase 4: Optimization (Month 3+)
1. ⭐ Computer Systems: A Programmer's Perspective
2. ⭐ LuaJIT papers
3. ⭐ LLVM tutorial
4. ⭐ Virtual Machines book

---

## 🎯 Quick Reference Cheat Sheet

**When you need to...**

- **Understand lexing/parsing:** Crafting Interpreters Ch. 4-6
- **Design a type system:** Types and Programming Languages
- **Implement a VM:** Crafting Interpreters Ch. 14-30, Lua paper
- **Build a GC:** GC Handbook Ch. 1-4, Crafting Interpreters Ch. 26
- **Design error handling:** The Error Model article
- **Optimize code:** Computer Systems book, LuaJIT guide
- **Add concurrency:** CSP paper, BEAM docs
- **Debug problems:** Eli Bendersky's blog, r/ProgrammingLanguages

---

## 💾 Downloadable Resources

**Create a reading folder:**
```bash
mkdir -p ~/satori-reading
cd ~/satori-reading

# Download free resources
wget https://www.lua.org/doc/jucs05.pdf
wget http://www.usingcsp.com/cspbook.pdf
# ... etc
```

---

## 📝 Notes & Annotations

**Recommended workflow:**
1. Read with a notebook or markdown file
2. Write summaries of key concepts
3. Implement immediately (don't just read)
4. Revisit after implementing
5. Share learnings in notes/research/

---

**This is a living document - add resources as you discover them!**

**Built from scratch, with focus. 🌸**
