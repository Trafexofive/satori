# Satori Language Specification

**Version:** 0.1.0-alpha  
**Status:** Draft

## Table of Contents

1. [Introduction](#introduction)
2. [Lexical Structure](#lexical-structure)
3. [Types](#types)
4. [Variables and Constants](#variables-and-constants)
5. [Expressions](#expressions)
6. [Statements](#statements)
7. [Functions](#functions)
8. [Structs](#structs)
9. [Control Flow](#control-flow)
10. [Error Handling](#error-handling)
11. [Modules](#modules)
12. [Concurrency](#concurrency)

## Introduction

Satori is a statically typed scripting language designed for recreational engineers who build from scratch. This document defines the syntax and semantics of the Satori programming language.

### Design Principles

- **Explicit over implicit** - No hidden behaviors
- **Minimal ceremony** - Concise without being cryptic
- **Zero magic** - Predictable runtime behavior
- **Production ready** - Strong guarantees for real systems

### Notation

In this specification:
- **MUST**, **REQUIRED** indicate absolute requirements
- **SHOULD**, **RECOMMENDED** indicate best practices
- **MAY**, **OPTIONAL** indicate permitted features

## Lexical Structure

### Source Code Encoding

Source files MUST be UTF-8 encoded. The file extension is `.sat`.

### Comments

```satori
// Single-line comment

/* Multi-line comment
   can span multiple lines */
```

### Identifiers

Identifiers start with a letter or underscore, followed by letters, digits, or underscores:

```
identifier := [a-zA-Z_][a-zA-Z0-9_]*
```

### Keywords

Reserved keywords that cannot be used as identifiers:

```
and       break     continue  defer     else      false
for       if        import    in        let       loop
nil       not       or        panic     return    spawn
struct    then      true      while
```

### Type Keywords

```
int       float     bool      string    void      byte
```

### Literals

#### Integer Literals

```satori
42          // decimal
0xFF        // hexadecimal
0o77        // octal
0b1010      // binary
1_000_000   // underscores allowed for readability
```

#### Floating-Point Literals

```satori
3.14
2.5e10
1.0e-5
```

#### String Literals

```satori
"Hello, World!"
"Line 1\nLine 2"    // Escape sequences: \n \t \r \\ \"
```

#### Boolean Literals

```satori
true
false
```

#### Nil Literal

```satori
nil
```

### Operators and Punctuation

```
+  -  *  /  %        // Arithmetic
&  |  ^              // Bitwise
== != < > <= >=      // Comparison
and or not           // Logical
=  :=                // Assignment
+= -= *= /=          // Compound assignment
.  ..                // Member access, range
:  ,  ;              // Punctuation
(  )  {  }  [  ]     // Brackets
```

## Types

### Primitive Types

| Type     | Description           | Size    | Example         |
|----------|----------------------|---------|-----------------|
| `int`    | Signed integer       | 64-bit  | `42`            |
| `float`  | Floating point       | 64-bit  | `3.14`          |
| `bool`   | Boolean              | 1-bit   | `true`, `false` |
| `string` | UTF-8 string         | Dynamic | `"hello"`       |
| `byte`   | Unsigned 8-bit int   | 8-bit   | `0xFF`          |
| `void`   | No value (functions) | 0       | -               |

### Composite Types

#### Arrays

Fixed-size arrays:

```satori
let arr: [int; 10]              // Array of 10 integers
let arr := [1, 2, 3, 4, 5]      // Inferred type and size
```

Dynamic arrays (slices):

```satori
let slice: []int                // Dynamic array of integers
```

#### Pointers

```satori
let x := 42
let ptr: *int = &x              // Pointer to int
let val := *ptr                 // Dereference
```

#### Structs

User-defined composite types (see [Structs](#structs)).

### Type Inference

The `:=` operator infers type from the right-hand side:

```satori
let x := 42          // Inferred as int
let y := 3.14        // Inferred as float
let s := "hello"     // Inferred as string
```

Explicit type annotation uses `:`:

```satori
let x: int = 42
let y: float = 3.14
```

## Variables and Constants

### Variable Declaration

```satori
let x: int = 42          // Explicit type
let y := 100             // Type inference
let z: int               // Uninitialized (zero-valued)
```

### Multiple Declarations

```satori
let a, b, c := 1, 2, 3
let x, y: int = 10, 20
```

### Scope

Variables are block-scoped:

```satori
let x := 1
{
    let x := 2       // Shadows outer x
    print x          // Prints 2
}
print x              // Prints 1
```

## Expressions

### Arithmetic Expressions

```satori
a + b                // Addition
a - b                // Subtraction
a * b                // Multiplication
a / b                // Division
a % b                // Modulo
```

### Comparison Expressions

```satori
a == b               // Equal
a != b               // Not equal
a < b                // Less than
a > b                // Greater than
a <= b               // Less than or equal
a >= b               // Greater than or equal
```

### Logical Expressions

```satori
a and b              // Logical AND
a or b               // Logical OR
not a                // Logical NOT
```

### Member Access

```satori
obj.field            // Field access
obj.method()         // Method call
```

### Range Expressions

```satori
0..10                // Range from 0 to 9 (exclusive end)
0..=10               // Range from 0 to 10 (inclusive end)
```

### Expression-Based If

```satori
let status := if health > 50 then "good" else "bad"
```

## Statements

### Expression Statement

Any expression can be a statement:

```satori
io.println "Hello"
x + y
```

### Assignment

```satori
x = 42
x += 10
x -= 5
```

### Return Statement

```satori
return
return value
```

### Defer Statement

Execute code when the current scope exits:

```satori
defer file.close()
defer cleanup()
```

Multiple defers execute in LIFO order.

## Functions

### Function Declaration

```satori
void greet(string name)
    io.println "Hello, {}", name
```

With return type:

```satori
int add(int a, int b)
    return a + b
```

### Function Calls

```satori
greet("Alice")
let sum := add(10, 20)
```

### Method Syntax

Functions associated with structs:

```satori
struct Vec3 {
    float x, y, z
    
    float magnitude()
        return sqrt(x*x + y*y + z*z)
}
```

## Structs

### Struct Declaration

```satori
struct Point {
    int x
    int y
}

struct Person {
    string name
    int age
    string email
}
```

### Struct Fields

Multiple fields on one line:

```satori
struct Vec3 {
    float x, y, z
}
```

### Struct Initialization

```satori
let p := Point{10, 20}
let person := Person{"Alice", 30, "alice@example.com"}
```

Named fields:

```satori
let p := Point{x: 10, y: 20}
```

### Methods

```satori
struct Rectangle {
    int width
    int height
    
    int area()
        return width * height
    
    void scale(int factor)
        width *= factor
        height *= factor
}

let rect := Rectangle{10, 20}
let a := rect.area()
rect.scale(2)
```

## Control Flow

### If Statement

```satori
if condition
    // body

if condition then action

if condition
    // true branch
else
    // false branch

if condition1
    // branch 1
else if condition2
    // branch 2
else
    // default
```

### For Loop

#### Range-based:

```satori
for i in 0..10
    print i
```

#### Iteration over collections:

```satori
for item in array
    print item

for key, value in map
    print "{}: {}", key, value
```

### While Loop

```satori
while condition
    // body
```

### Infinite Loop

```satori
loop
    // body
    if condition: break
```

### Break and Continue

```satori
for i in 0..100
    if i % 2 == 0: continue
    if i > 50: break
    print i
```

## Error Handling

### The `or` Operator

Functions that can fail return an optional type, handled with `or`:

```satori
let file := io.open("config.json") or panic("File not found")
let data := file.read() or return
let num := parse_int(str) or 0
```

The `or` operator provides a default value or alternate action when the left side fails.

### Panic

Unrecoverable errors:

```satori
panic("Fatal error")
panic "Something went wrong: {}", reason
```

### Defer for Cleanup

Ensure cleanup happens even on error paths:

```satori
let file := io.open("data.txt") or panic("Cannot open file")
defer file.close()

let content := file.read_all() or return
// file.close() will be called even if read_all fails
```

## Modules

### Import Statement

```satori
import io
import net
import math
```

### Module Usage

```satori
io.println "Hello"
let socket := net.connect("localhost", 8080)
let result := math.sqrt(16.0)
```

### Module Path

Modules are resolved from:
1. Standard library (`stdlib/`)
2. Current directory
3. `SATORI_PATH` environment variable

## Concurrency

### Spawn

Launch lightweight concurrent tasks:

```satori
spawn worker_task(data)
spawn {
    // inline task
    process_data()
}
```

### Cooperative Scheduling

Tasks are scheduled cooperatively, not preemptively. Use `yield` to allow other tasks to run:

```satori
loop
    do_work()
    yield
```

### Communication

(Future feature: channels for task communication)

```satori
let ch := make_channel[int]()
spawn sender(ch)
let value := ch.recv()
```

## Grammar Summary

```ebnf
program        → statement* EOF

statement      → import_stmt
               | let_stmt
               | expr_stmt
               | return_stmt
               | defer_stmt
               | if_stmt
               | for_stmt
               | while_stmt
               | loop_stmt
               | break_stmt
               | continue_stmt
               | struct_decl
               | func_decl

import_stmt    → "import" IDENTIFIER

let_stmt       → "let" IDENTIFIER (":" type)? ("=" expression)?

expr_stmt      → expression

return_stmt    → "return" expression?

defer_stmt     → "defer" expression

if_stmt        → "if" expression ("then" expression | block)
                 ("else" (if_stmt | block))?

for_stmt       → "for" IDENTIFIER "in" expression block

while_stmt     → "while" expression block

loop_stmt      → "loop" block

break_stmt     → "break"

continue_stmt  → "continue"

struct_decl    → "struct" IDENTIFIER "{" field_list "}"

func_decl      → type IDENTIFIER "(" param_list? ")" block

expression     → assignment

assignment     → logic_or (("=" | "+=" | "-=" | "/=") assignment)?

logic_or       → logic_and ("or" logic_and)*

logic_and      → equality ("and" equality)*

equality       → comparison (("==" | "!=") comparison)*

comparison     → term ((">" | ">=" | "<" | "<=") term)*

term           → factor (("+" | "-") factor)*

factor         → unary (("*" | "/" | "%") unary)*

unary          → ("not" | "-" | "&" | "*") unary | call

call           → primary ("(" arguments? ")" | "." IDENTIFIER)*

primary        → NUMBER | STRING | "true" | "false" | "nil"
               | IDENTIFIER | "(" expression ")"

block          → NEWLINE INDENT statement+ DEDENT
               | statement
```

## Implementation Notes

### Significant Whitespace

Satori uses indentation to denote blocks, similar to Python. Tabs and spaces MUST NOT be mixed. The recommended indentation is 4 spaces.

### Semicolons

Semicolons are optional and typically omitted. The lexer inserts implicit semicolons at line endings when appropriate.

### String Interpolation

Format strings use `{}` for placeholders:

```satori
io.println "Hello, {}", name
io.println "x={}, y={}", x, y
```

---

**End of Specification**
