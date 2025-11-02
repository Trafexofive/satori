# Satori Code Patterns & Examples

## Common Patterns

### 1. Error Handling with `or`

**Pattern: Provide default value**
```satori
let config := load_config() or default_config()
let port := parse_int(env_port) or 8080
```

**Pattern: Early return on error**
```satori
void process_file(string path)
    let file := io.open(path) or return
    defer file.close()
    
    let content := file.read_all() or return
    parse_and_process(content)
```

**Pattern: Panic on critical errors**
```satori
void main()
    let db := connect_database() or panic("Database required")
    defer db.close()
    
    run_server(db)
```

**Pattern: Continue loop on errors**
```satori
for file in files
    let content := io.open(file) or continue
    process(content)
```

---

### 2. Resource Cleanup with `defer`

**Pattern: File handling**
```satori
let file := io.open("data.txt", "w") or panic("Cannot write")
defer file.close()

file.write("Line 1\n")
file.write("Line 2\n")
// file.close() called automatically
```

**Pattern: Multiple defers (LIFO order)**
```satori
void backup_and_process()
    let src := io.open("data.db", "r") or return
    defer src.close()
    
    let dst := io.open("backup.db", "w") or return
    defer dst.close()
    
    copy_data(src, dst)
    // Closes: dst first, then src
```

**Pattern: Lock/unlock**
```satori
let lock := acquire_lock() or return
defer lock.release()

// Critical section
modify_shared_data()
```

---

### 3. Struct Patterns

**Pattern: Constructor function**
```satori
struct Point {
    int x, y
    
    Point new(int x, int y)
        return Point{x, y}
    
    float distance()
        return math.sqrt(x*x + y*y)
}

let p := Point.new(3, 4)
io.println "Distance: {}", p.distance()
```

**Pattern: Builder pattern**
```satori
struct RequestBuilder {
    string url
    string method
    Map[string, string] headers
    
    RequestBuilder new(string url)
        return RequestBuilder{
            url: url,
            method: "GET",
            headers: Map[string, string]{}
        }
    
    RequestBuilder header(string key, string value)
        headers.set(key, value)
        return this
    
    Request build()
        return Request{url, method, headers}
}

let req := RequestBuilder.new("https://api.example.com")
    .header("User-Agent", "Satori/1.0")
    .header("Accept", "application/json")
    .build()
```

---

### 4. Collection Patterns

**Pattern: Map-Filter-Reduce (manual)**
```satori
import collections

let numbers := [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]

// Filter evens
let evens := List[int]{}
for n in numbers
    if n % 2 == 0
        evens.push(n)

// Map to squares
let squares := List[int]{}
for n in evens
    squares.push(n * n)

// Reduce to sum
let sum := 0
for n in squares
    sum += n

io.println "Sum of squares of evens: {}", sum
```

**Pattern: Dictionary as cache**
```satori
let cache := Map[string, Result]{}

Result get_or_compute(string key)
    if cache.has(key)
        return cache.get(key)
    
    let result := expensive_computation(key)
    cache.set(key, result)
    return result
```

---

### 5. Concurrency Patterns

**Pattern: Spawn multiple workers**
```satori
void main()
    let data := load_data()
    let chunk_size := data.len() / 4
    
    spawn worker(data[0..chunk_size])
    spawn worker(data[chunk_size..chunk_size*2])
    spawn worker(data[chunk_size*2..chunk_size*3])
    spawn worker(data[chunk_size*3..])
    
    wait_all()  // Future feature
```

**Pattern: Event loop**
```satori
void event_loop()
    loop
        let event := poll_event() or continue
        
        match event.type
            EventType.Click: handle_click(event)
            EventType.Key: handle_key(event)
            EventType.Quit: break
        
        yield  // Allow other tasks
```

**Pattern: Producer-consumer (future)**
```satori
void producer(Channel[int] ch)
    for i in 0..100
        ch.send(i)
    ch.close()

void consumer(Channel[int] ch)
    loop
        let val := ch.recv() or break
        process(val)

let ch := make_channel[int]()
spawn producer(ch)
spawn consumer(ch)
```

---

### 6. Server Patterns

**Pattern: Basic TCP server**
```satori
import net

void handle_client(net.Socket client)
    defer client.close()
    
    let request := client.read(4096) or return
    io.println "Received: {}", string.from_bytes(request)
    
    let response := "HTTP/1.0 200 OK\r\n\r\nHello, World!"
    client.write(response.bytes())

void main()
    let listener := net.listen("0.0.0.0", 8080) or 
        panic("Cannot bind to port 8080")
    defer listener.close()
    
    io.println "Server listening on :8080"
    
    loop
        let client := listener.accept() or continue
        spawn handle_client(client)
```

**Pattern: Request router**
```satori
struct Router {
    Map[string, Handler] routes
    
    void handle(Request req, Response res)
        let handler := routes.get(req.path) or
            routes.get("/404")
        
        handler(req, res)
}

let router := Router{}
router.routes.set("/", handle_home)
router.routes.set("/api/users", handle_users)
router.routes.set("/404", handle_not_found)
```

---

### 7. File I/O Patterns

**Pattern: Read entire file**
```satori
string? read_file(string path)
    let file := io.open(path, "r") or return nil
    defer file.close()
    
    return file.read_all()
```

**Pattern: Write with error handling**
```satori
bool write_file(string path, string content)
    let file := io.open(path, "w") or return false
    defer file.close()
    
    file.write(content.bytes()) or return false
    return true
```

**Pattern: Line-by-line processing**
```satori
void process_csv(string path)
    let file := io.open(path, "r") or return
    defer file.close()
    
    loop
        let line := file.read_line() or break
        let fields := string.split(line, ",")
        process_row(fields)
```

---

### 8. String Processing

**Pattern: Parse key-value config**
```satori
Map[string, string] parse_config(string content)
    let config := Map[string, string]{}
    
    for line in string.split(content, "\n")
        let trimmed := string.trim(line)
        
        // Skip comments and empty lines
        if string.starts_with(trimmed, "#"): continue
        if string.len(trimmed) == 0: continue
        
        let parts := string.split(trimmed, "=")
        if parts.len() != 2: continue
        
        let key := string.trim(parts[0])
        let value := string.trim(parts[1])
        config.set(key, value)
    
    return config
```

**Pattern: Template rendering**
```satori
string render_template(string tmpl, Map[string, string] vars)
    let result := tmpl
    
    for key, value in vars
        let placeholder := "{" + key + "}"
        result = string.replace(result, placeholder, value)
    
    return result

let tmpl := "Hello, {name}! You have {count} messages."
let vars := Map[string, string]{}
vars.set("name", "Alice")
vars.set("count", "5")

io.println render_template(tmpl, vars)
```

---

### 9. Testing Patterns

**Pattern: Simple assertions**
```satori
void assert_eq(int a, int b, string msg)
    if a != b
        io.println "FAIL: {} (expected {}, got {})", msg, b, a
        os.exit(1)
    io.println "PASS: {}", msg

void test_math()
    assert_eq(add(2, 3), 5, "add(2, 3)")
    assert_eq(multiply(4, 5), 20, "multiply(4, 5)")

test_math()
```

**Pattern: Test harness**
```satori
struct Test {
    string name
    void Function() test_fn
}

void run_tests([]Test tests)
    let passed := 0
    let failed := 0
    
    for test in tests
        io.print "Running {}... ", test.name
        
        // Catch panics (future feature)
        test.test_fn()
        
        io.println "PASS"
        passed += 1
    
    io.println "\n{} passed, {} failed", passed, failed
```

---

### 10. JSON API Pattern

**Pattern: REST client**
```satori
import net
import json

struct ApiClient {
    string base_url
    
    Value? get(string endpoint)
        let url := base_url + endpoint
        let sock := net.connect_http(url) or return nil
        defer sock.close()
        
        sock.write("GET {} HTTP/1.1\r\n".format(endpoint))
        sock.write("Host: api.example.com\r\n")
        sock.write("\r\n")
        
        let response := sock.read(65536) or return nil
        let body := extract_body(response)
        
        return json.parse(body)
}

let client := ApiClient{"https://api.example.com"}
let users := client.get("/users") or panic("API error")

for user in users.as_array()
    io.println "User: {}", user.get("name").as_string()
```

---

### 11. Game Loop Pattern

**Pattern: Fixed timestep**
```satori
void game_loop()
    let dt := 16  // 60 FPS
    let last_time := time.clock_ms()
    
    loop
        let current_time := time.clock_ms()
        let elapsed := current_time - last_time
        
        if elapsed >= dt
            update(dt)
            render()
            last_time = current_time
        else
            time.sleep(dt - elapsed)
        
        if should_quit(): break
```

---

## Anti-Patterns (Avoid These)

**❌ Ignoring errors silently**
```satori
// BAD
let file := io.open("data.txt")  // No error handling!

// GOOD
let file := io.open("data.txt") or return
```

**❌ Not using defer for cleanup**
```satori
// BAD
let file := io.open("data.txt") or return
process(file)
file.close()  // Might not run if process() panics

// GOOD
let file := io.open("data.txt") or return
defer file.close()
process(file)
```

**❌ Mutable global state**
```satori
// BAD - hard to reason about
let global_counter := 0

void increment()
    global_counter += 1

// GOOD - explicit parameter passing
struct Counter {
    int value
    
    void increment()
        value += 1
}
```

---

**Living document - add patterns as they emerge**
