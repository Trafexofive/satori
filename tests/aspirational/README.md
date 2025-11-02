# Aspirational Tests

This directory contains `.sat` files that demonstrate **features we want to implement** but haven't coded yet. These files serve as:

1. **Target specifications** - What the language should support
2. **Design validation** - Does the syntax feel right?
3. **Implementation roadmap** - What to build next
4. **Regression tests** - Once implemented, these become real tests

## Philosophy

> "Code the future you want to see."

These are not just TODO lists. They're working examples of what Satori will become. By writing code we *can't run yet*, we validate the design before committing to implementation.

## Test Files

### 01_variables_and_control.sat
**Status:** 🚧 Not yet working  
**Features tested:**
- Variable declarations (`let x := 42`, `let name: string = "hello"`)
- Type inference with `:=`
- Arithmetic operations (`+`, `-`, `*`, `/`, `%`)
- Comparison operators (`>`, `<`, `==`, `!=`, `>=`, `<=`)
- Boolean logic (`and`, `or`, `not`)
- If-else statements
- While loops
- For loops with initialization, condition, update
- Compound assignments (`+=`, `-=`, `*=`, `/=`)

**Implementation order:**
1. ✅ String literals
2. ✅ Integer literals
3. ✅ Float literals
4. ⬜ Variable declarations (`let`)
5. ⬜ Type inference (`:=`)
6. ⬜ Assignment statements
7. ⬜ Binary operators (arithmetic)
8. ⬜ Comparison operators
9. ⬜ Boolean operators
10. ⬜ If-else statements
11. ⬜ While loops
12. ⬜ For loops
13. ⬜ Compound assignments

---

### 02_functions_and_structs.sat
**Status:** 🚧 Not yet working  
**Features tested:**
- Function declarations with parameters
- Function return types
- Function calls with arguments
- Multiple return values
- Recursive functions
- Struct definitions
- Struct instantiation
- Struct field access
- Struct methods (`self` parameter)
- Nested structs

**Implementation order:**
1. ⬜ Function declarations (`fn name() {}`)
2. ⬜ Function parameters with types
3. ⬜ Return statements
4. ⬜ Function calls with arguments
5. ⬜ Struct definitions (`struct Name { ... }`)
6. ⬜ Struct instantiation
7. ⬜ Field access (already partially working for modules)
8. ⬜ Field assignment
9. ⬜ Method definitions
10. ⬜ Method calls
11. ⬜ Multiple return values (tuple unpacking)

---

### 03_arrays_and_advanced.sat
**Status:** 🚧 Not yet working  
**Features tested:**
- Array literals (`[1, 2, 3]`)
- Array indexing (`arr[0]`)
- Array slicing (`arr[1:4]`)
- Array methods (`.len()`, `.append()`, `.sum()`, `.max()`)
- For-in loops (iteration)
- Higher-order functions (`.map()`, `.filter()`)
- Hash maps / dictionaries
- Error handling (`or` operator)
- Defer statements
- Match expressions (pattern matching)
- Enum types
- Concurrency (spawn, channels)
- String methods

**Implementation order:**
1. ⬜ Array literals
2. ⬜ Array indexing
3. ⬜ Array type annotations
4. ⬜ For-in loops
5. ⬜ Array methods (built-in)
6. ⬜ Hash maps (dictionaries)
7. ⬜ Error type
8. ⬜ `or` operator for error handling
9. ⬜ Defer statements
10. ⬜ Match expressions
11. ⬜ Enum types
12. ⬜ First-class functions (lambdas)
13. ⬜ Higher-order functions
14. ⬜ Channels (`chan` type)
15. ⬜ Spawn for concurrency
16. ⬜ String methods

---

## How to Use

### As a Developer

1. **Pick a feature** from the files above
2. **Read the aspirational code** to understand the syntax
3. **Implement the feature** in the compiler/VM
4. **Run the test** to see if it works
5. **Move checklist items** from ⬜ to ✅

### As a Designer

1. **Read the code** - Does it feel natural?
2. **Suggest changes** - Better syntax? Different operators?
3. **Add examples** - Create new aspirational tests
4. **Validate decisions** - Does this fit Satori's philosophy?

### Running Aspirational Tests

These won't work yet! They'll fail with parse errors or "not implemented" messages.

```bash
# This will fail (expected)
./bin/satori tests/aspirational/01_variables_and_control.sat

# As features get implemented, they'll start working
```

When a feature is fully implemented, we can add it to the examples or create proper unit tests.

---

## Design Principles Validated

These tests demonstrate Satori's core philosophy:

✅ **Explicit over implicit**
- Type annotations are clear: `let x: int = 42`
- Type inference is opt-in with `:=`

✅ **Minimal ceremony**
- No semicolons required
- Clean struct syntax
- Simple function definitions

✅ **Zero magic**
- No hidden type conversions
- Explicit error handling with `or`
- Clear ownership with no hidden copies

✅ **Production ready**
- Error handling is first-class
- Concurrency is built-in
- Pattern matching for exhaustive checks

---

## Adding New Aspirational Tests

1. Create a new file: `NN_feature_name.sat`
2. Write code using features you want
3. Document what's being tested
4. Add implementation checklist
5. Update this README

**Keep tests focused**: Each file should test related features, not everything at once.

**Write realistic code**: These should be programs you'd actually write, not just feature demos.

**Document assumptions**: If you're inventing syntax, explain why.

---

## Current Status

| Test File | Features | Status | Next Step |
|-----------|----------|--------|-----------|
| 01_variables_and_control.sat | Variables, control flow | 🚧 0% | Implement variable declarations |
| 02_functions_and_structs.sat | Functions, structs | 🚧 0% | Implement function declarations |
| 03_arrays_and_advanced.sat | Collections, advanced | 🚧 0% | Implement arrays |

---

**Last Updated:** 2024-11-02  
**Total Features:** ~50  
**Implemented:** ~3 (literals, imports, member access)  
**Progress:** 6%

---

Built from scratch, with focus. 🌸
