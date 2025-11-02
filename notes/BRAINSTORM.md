# Brainstorm & Random Ideas

## Language Features to Consider

### Pattern Matching
Could make code cleaner:
```satori
match response
    Ok(value): process(value)
    Err(msg): log_error(msg)
    
match event.type
    EventType.Click: handle_click(event)
    EventType.Key: handle_key(event)
    _: ignore()
```

**Pros:** Cleaner than if/else chains, exhaustiveness checking
**Cons:** Complexity, need to implement well
**Decision:** Maybe v0.5+, research more

---

### Destructuring
Convenient for unpacking:
```satori
let Point{x, y} := get_position()
let [first, second, ...rest] := array

for {name, age} in users
    io.println "{} is {} years old", name, age
```

**Pros:** Ergonomic, common in modern languages
**Cons:** Parser complexity
**Decision:** Nice-to-have for v0.4+

---

### Generics
Type-safe collections and functions:
```satori
struct Stack[T] {
    []T items
    
    void push(T item)
        items.push(item)
    
    T? pop()
        return items.pop()
}

let stack := Stack[int]{}
stack.push(42)
```

**Pros:** Type safety without code duplication
**Cons:** Significant complexity, compile-time overhead
**Decision:** 
- v0.1-0.3: No generics, use unsafe casts if needed
- v0.4+: Implement monomorphization (Rust/C++ style)

---

### Operator Overloading
Natural math operations:
```satori
struct Vec3 {
    float x, y, z
    
    Vec3 operator+(Vec3 other)
        return Vec3{x + other.x, y + other.y, z + other.z}
    
    Vec3 operator*(float scalar)
        return Vec3{x * scalar, y * scalar, z * scalar}
}

let v1 := Vec3{1, 2, 3}
let v2 := Vec3{4, 5, 6}
let v3 := v1 + v2 * 2.0
```

**Pros:** Intuitive for math, DSLs
**Cons:** Can be abused, hidden complexity
**Decision:** NO for v1.0, reconsider later

---

### Traits/Interfaces
Polymorphism without inheritance:
```satori
trait Printable {
    string to_string()
}

struct Point {
    int x, y
}

impl Printable for Point {
    string to_string()
        return "Point({}, {})" format x, y
}

void print_value(Printable p)
    io.println p.to_string()
```

**Pros:** Flexible polymorphism, composition
**Cons:** Complex to implement
**Decision:** Research for v0.5+

---

## Syntax Ideas

### Ternary Operator
Short if/else:
```satori
let max := if a > b then a else b
// vs
let max := a > b ? a : b
```

**Decision:** Expression-based if is sufficient, no ternary needed

---

### Pipeline Operator
Function chaining:
```satori
// Current
let result := process(transform(parse(input)))

// With pipeline
let result := input |> parse |> transform |> process
```

**Pros:** Readable for data pipelines
**Cons:** New syntax to learn
**Decision:** Maybe later, not critical

---

### String Interpolation Styles

**Option 1: {} placeholders (current)**
```satori
io.println "Hello, {}", name
```

**Option 2: Inline expressions**
```satori
io.println "Hello, {name}"
io.println "2 + 2 = {2 + 2}"
```

**Option 3: Template strings**
```satori
io.println `Hello, ${name}`
```

**Decision:** Stick with {} placeholders for v0.1, consider inline for v0.2

---

## Standard Library Ideas

### HTTP Module
```satori
import http

let response := http.get("https://api.example.com") or return
io.println "Status: {}", response.status
io.println "Body: {}", response.body

let server := http.Server{port: 8080}
server.handle("/", handle_root)
server.handle("/api/users", handle_users)
server.run()
```

---

### SQL/Database Module
```satori
import db

let conn := db.connect("postgres://localhost/mydb") or panic()
defer conn.close()

let users := conn.query("SELECT * FROM users WHERE age > ?", [18])
for user in users
    io.println user["name"]
```

---

### Async Module (Far Future)
```satori
import async

async int fetch_data(string url)
    let response := await http.get(url)
    return parse_int(response.body)

async void main()
    let results := await async.all([
        fetch_data("http://api1.com"),
        fetch_data("http://api2.com"),
        fetch_data("http://api3.com")
    ])
    
    io.println "Sum: {}", sum(results)
```

**Decision:** Maybe v2.0+, after concurrency stabilizes

---

## Tooling Ideas

### REPL (Read-Eval-Print Loop)
```bash
$ satori
>>> let x := 42
42
>>> x * 2
84
>>> import math
>>> math.sqrt(16.0)
4.0
```

**Priority:** High (v0.3), very useful for learning/debugging

---

### Package Manager
```bash
$ sat install http-client
$ sat install json-schema
$ sat list
$ sat update
```

**Structure:**
```
~/.satori/packages/
  http-client/
    1.0.0/
    1.1.0/
  json-schema/
    0.5.2/
```

**Priority:** Medium (v0.5+), after language stabilizes

---

### Test Framework
```satori
import test

test.describe "Math operations" {
    test.it "should add numbers" {
        test.assert_eq(add(2, 3), 5)
    }
    
    test.it "should multiply numbers" {
        test.assert_eq(multiply(4, 5), 20)
    }
}
```

**Priority:** High (v0.3), critical for stdlib development

---

### Build System
```toml
# satori.toml
[package]
name = "myapp"
version = "1.0.0"
author = "Alice <alice@example.com>"

[dependencies]
http = "1.2.3"
json = "0.5.0"

[build]
output = "bin/myapp"
optimize = true
```

**Priority:** Medium (v0.6+)

---

## Crazy Ideas (Maybe Never)

### Compile-time Execution
Run code at compile time:
```satori
comptime {
    let routes := parse_routes_from_file("routes.txt")
    generate_router(routes)
}
```

Like Zig's comptime - powerful but complex

---

### Embedded DSLs
SQL as a sublanguage:
```satori
let users := sql! {
    SELECT name, age 
    FROM users 
    WHERE age > 18
}
```

HTML templating:
```satori
let page := html! {
    <div>
        <h1>Hello, {name}</h1>
        <p>Welcome back!</p>
    </div>
}
```

**Pros:** Type-safe, validated at compile time
**Cons:** Massive complexity, scope creep
**Decision:** Probably never, but fun to think about

---

### Effect System
Track side effects in types:
```satori
void pure_function(int x) -> int
    return x * 2

void impure_function(int x) -> int !io, !net
    io.println "Computing"
    return fetch_from_api(x)
```

**Pros:** Guarantees about code behavior
**Cons:** Very complex, Haskell-level abstraction
**Decision:** Not for Satori (too academic)

---

## Real-World Use Cases to Target

### 1. CLI Tools
Like grep, cat, find:
```satori
import fs
import string

void grep(string pattern, string file)
    let content := fs.read_file(file) or return
    let lines := string.split(content, "\n")
    
    for i, line in lines
        if string.contains(line, pattern)
            io.println "{}:{}: {}", file, i+1, line
```

---

### 2. Web Services
REST API, microservices:
```satori
import http
import json

struct User {
    string name
    int age
}

void handle_users(http.Request req, http.Response res)
    let users := load_users()
    res.json(users)

http.serve(":8080", {
    "/api/users": handle_users,
    "/health": handle_health,
})
```

---

### 3. System Administration Scripts
Deployment, monitoring:
```satori
import os
import fs

void deploy()
    // Build
    os.exec("make", ["build"]) or panic("Build failed")
    
    // Copy to server
    os.exec("scp", ["bin/app", "server:/opt/app/"]) or return
    
    // Restart service
    os.exec("ssh", ["server", "systemctl restart app"]) or return
    
    io.println "Deployed successfully"
```

---

### 4. Game Development (Simple 2D)
Game of Life, Snake, etc:
```satori
import graphics
import input

struct Game {
    [][]int grid
    
    void update()
        // Game logic
        
    void render()
        graphics.clear()
        for x in 0..grid.width()
            for y in 0..grid.height()
                if grid[x][y]
                    graphics.draw_rect(x*10, y*10, 10, 10)
}

let game := Game{init_grid()}
loop
    game.update()
    game.render()
    graphics.present()
    
    if input.key_pressed(Key.Escape): break
```

---

## Questions to Ponder

1. **How much stdlib is too much?**
   - Keep core minimal?
   - Or batteries-included like Python?
   - Current thinking: Medium - essential tools, not everything

2. **Should we support Windows natively?**
   - POSIX-only is simpler
   - But limits audience
   - Leaning: POSIX first, Windows via WSL or later port

3. **Compiled vs interpreted - which to prioritize?**
   - Interpreter for development
   - AOT compiler for production
   - Both have different optimization needs

4. **How to handle breaking changes before 1.0?**
   - Be pragmatic, fix mistakes early
   - Deprecation warnings
   - Clear migration guides

5. **Should we optimize for small binaries or performance?**
   - Embedded use cases want small
   - Servers want fast
   - Compromise: compile flags for both

---

## Inspiration Wall

**Languages I admire:**
- C - Simplicity, explicitness, control
- Lua - Beautiful implementation, small, fast
- Go - Pragmatic choices, great concurrency
- Zig - Explicit over implicit, no hidden control flow
- Odin - Pragmatic game dev focus
- Nim - Practical metaprogramming

**What makes a good scripting language?**
- Fast startup
- Interactive (REPL)
- Good error messages
- Easy to embed
- Practical stdlib
- Readable code

**What makes a good systems language?**
- Predictable performance
- Low memory overhead
- No GC pauses (or minimal)
- C interop
- Manual control when needed

**Satori aims to be:** Good scripting, decent systems

---

**Random thoughts - revisit periodically**
