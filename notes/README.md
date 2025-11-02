# Satori Development Notes

This directory contains research, design documents, examples, and development resources for building the Satori programming language.

## 📁 Directory Structure

```
notes/
├── README.md                      # This file
├── TODO.md                        # Task tracking and roadmap
├── CHANGELOG.md                   # Version history
├── BRAINSTORM.md                  # Ideas and explorations
├── research/                      # Research and learning materials
│   ├── READING_LIST.md           # Books, papers, articles to read
│   ├── learning-path.md          # Structured curriculum
│   ├── language-design.md        # Language design decisions
│   └── vm-design.md              # Virtual machine architecture
├── examples/                      # Code patterns and examples
│   └── PATTERNS.md               # Common Satori patterns
├── design/                        # Implementation designs
│   └── stdlib-sketch.md          # Standard library implementation
└── performance/                   # Performance tracking
    └── benchmarks.md             # Benchmark plans and results
```

## 🎯 Quick Navigation

### For New Contributors

Start here:
1. **[READING_LIST.md](research/READING_LIST.md)** - Essential books and resources
2. **[learning-path.md](research/learning-path.md)** - Week-by-week curriculum
3. **[TODO.md](TODO.md)** - What needs to be done
4. **[PATTERNS.md](examples/PATTERNS.md)** - How to write Satori code

### For Core Development

Reference these:
1. **[language-design.md](research/language-design.md)** - Design decisions and rationale
2. **[vm-design.md](research/vm-design.md)** - VM implementation details
3. **[stdlib-sketch.md](design/stdlib-sketch.md)** - stdlib implementation notes
4. **[benchmarks.md](performance/benchmarks.md)** - Performance goals

### For Ideas & Planning

Check out:
1. **[BRAINSTORM.md](BRAINSTORM.md)** - Future features, wild ideas
2. **[TODO.md](TODO.md)** - Prioritized task list
3. **[CHANGELOG.md](CHANGELOG.md)** - What's been done

## 📚 Essential Reading

If you only read three things:

1. **[Crafting Interpreters](https://craftinginterpreters.com/)** by Robert Nystrom
   - The bible of interpreter implementation
   - FREE online, beautifully written
   - Covers everything we're doing

2. **[The Implementation of Lua 5.0](https://www.lua.org/doc/jucs05.pdf)** (PDF)
   - Learn from one of the best VMs
   - Clear explanations of design choices
   - Highly relevant to Satori

3. **[The Error Model](http://joeduffyblog.com/2016/02/07/the-error-model/)** by Joe Duffy
   - Comprehensive error handling analysis
   - Influences our `or` operator design
   - Production systems experience

## 🎓 Learning Resources

### For Beginners
- Start with **learning-path.md** for a structured approach
- Follow along with Crafting Interpreters
- Build simple exercises before tackling Satori

### For Experienced Developers
- Review **language-design.md** for design philosophy
- Study **vm-design.md** for implementation details
- Check **TODO.md** for areas needing work

## 🔍 How to Use These Notes

### When Starting a Feature

1. Check **TODO.md** to see what's prioritized
2. Read relevant sections in **language-design.md** or **vm-design.md**
3. Look at **stdlib-sketch.md** for implementation patterns
4. Review **PATTERNS.md** for usage examples
5. Write code, update docs!

### When Stuck

1. Check **READING_LIST.md** for relevant resources
2. Ask in r/ProgrammingLanguages or related communities
3. Study similar implementations (Lua, Wren)
4. Add your findings to the notes for others!

### When Planning

1. Brain dump in **BRAINSTORM.md**
2. Research feasibility
3. Move viable items to **TODO.md**
4. Update design docs with decisions

## 📝 Contributing to Notes

### Adding Research

Found a great paper or article? Add it to **READING_LIST.md** with:
- Title and author
- URL or ISBN
- Brief description
- Why it's relevant
- Priority rating (⭐)

### Documenting Decisions

Made a design choice? Update the relevant file:
- **language-design.md** for language features
- **vm-design.md** for implementation details
- **stdlib-sketch.md** for library design

Include:
- What you decided
- Why you decided it
- What alternatives you considered
- References or inspiration

### Sharing Examples

Found a useful pattern? Add to **PATTERNS.md**:
- Clear example code
- Explanation of when to use
- Anti-patterns to avoid

## 🎯 Current Focus (v0.1)

Priority areas for immediate work:

1. **Type Checker** - Core feature, blocking other work
2. **Variables & Assignment** - Basic language feature
3. **Control Flow** - if/else, loops
4. **Functions** - Declarations and calls
5. **Testing** - Unit tests for all components

See **TODO.md** for complete list.

## 🌟 Philosophy

These notes embody Satori's philosophy:

- **Explicit over implicit** - Document decisions clearly
- **Minimal ceremony** - No over-engineering in design
- **Zero magic** - Explain how things work
- **Build for yourself** - Personal learning and growth

## 🔗 External Links

- **Main Repo:** [../](../)
- **Documentation:** [../docs/](../docs/)
- **Source Code:** [../src/](../src/)
- **Examples:** [../examples/](../examples/)

## 📊 Version Info

- **Current Version:** 0.0.2
- **Target Version:** 0.1.0
- **Last Updated:** 2024-11-02

---

**Built from scratch, with focus. 🌸**

*Keep learning, keep building, keep notes!*
