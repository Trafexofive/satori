# Satori Standard Library

**Version:** 0.1.0-alpha  
**Status:** In Development

## Overview

The Satori standard library provides essential functionality for I/O, networking, data structures, and system interaction. The library follows Satori's philosophy of being explicit, minimal, and production-ready.

## Module Organization

```
stdlib/
├── io/          # Input/Output operations
├── net/         # Network operations
├── fs/          # File system operations
├── collections/ # Data structures
├── math/        # Mathematical functions
├── time/        # Time and date handling
├── os/          # Operating system interface
├── string/      # String manipulation
└── json/        # JSON parsing and encoding
```

## Core Modules

### io - Input/Output

Basic input and output operations.

#### Functions

**`void print(string format, ...args)`**

Print formatted output to stdout without newline.

```satori
import io

io.print "Hello, "
io.print "World!"
// Output: Hello, World!
```

**`void println(string format, ...args)`**

Print formatted output to stdout with newline.

```satori
io.println "Hello, {}", name
io.println "x={}, y={}", x, y
```

**`string? readln()`**

Read a line from stdin. Returns `nil` on EOF or error.

```satori
let line := io.readln() or return
io.println "You entered: {}", line
```

**`File? open(string path, string mode)`**

Open a file. Mode can be "r" (read), "w" (write), "a" (append).

```satori
let file := io.open("data.txt", "r") or panic("Cannot open file")
defer file.close()
```

#### Types

**`struct File`**

Represents an open file handle.

Methods:
- `[]byte? read(int n)` - Read up to n bytes
- `[]byte? read_all()` - Read entire file
- `int? write([]byte data)` - Write bytes
- `void close()` - Close file
- `void flush()` - Flush write buffer

---

### net - Networking

Network operations for TCP/UDP sockets.

#### Functions

**`Socket? connect(string host, int port)`**

Connect to a TCP server.

```satori
import net

let sock := net.connect("localhost", 8080) or panic("Connection failed")
defer sock.close()

sock.write("GET / HTTP/1.0\r\n\r\n".bytes())
let response := sock.read(4096)
```

**`Listener? listen(string host, int port)`**

Create a TCP listener.

```satori
let listener := net.listen("0.0.0.0", 8080) or panic("Cannot bind")
defer listener.close()

loop
    let client := listener.accept() or continue
    spawn handle_client(client)
```

#### Types

**`struct Socket`**

TCP socket connection.

Methods:
- `[]byte? read(int n)` - Read up to n bytes
- `int? write([]byte data)` - Write data
- `void close()` - Close socket
- `void set_timeout(int ms)` - Set read/write timeout

**`struct Listener`**

TCP listener for accepting connections.

Methods:
- `Socket? accept()` - Accept incoming connection (blocking)
- `void close()` - Close listener

---

### fs - File System

File system operations beyond basic I/O.

#### Functions

**`bool exists(string path)`**

Check if file or directory exists.

```satori
import fs

if fs.exists("config.json")
    load_config()
```

**`void? mkdir(string path)`**

Create directory. Returns `nil` on error.

```satori
fs.mkdir("output") or panic("Cannot create directory")
```

**`void? remove(string path)`**

Remove file or empty directory.

```satori
fs.remove("temp.txt")
```

**`[]string? list_dir(string path)`**

List directory contents.

```satori
let files := fs.list_dir(".") or return
for file in files
    io.println file
```

**`FileInfo? stat(string path)`**

Get file information.

```satori
let info := fs.stat("data.txt") or return
io.println "Size: {} bytes", info.size
io.println "Modified: {}", info.modified
```

#### Types

**`struct FileInfo`**

File metadata.

Fields:
- `string name` - File name
- `int size` - Size in bytes
- `bool is_dir` - Is directory
- `int modified` - Last modified timestamp (Unix time)

---

### collections - Data Structures

Common data structures.

#### Types

**`struct List[T]`**

Dynamic array (growable).

Methods:
- `void push(T item)` - Append item
- `T? pop()` - Remove and return last item
- `T get(int index)` - Get item at index
- `void set(int index, T value)` - Set item at index
- `int len()` - Get length
- `void clear()` - Remove all items

```satori
import collections

let list := List[int]{}
list.push(1)
list.push(2)
list.push(3)

io.println "Length: {}", list.len()
let val := list.pop()
```

**`struct Map[K, V]`**

Hash map (dictionary).

Methods:
- `void set(K key, V value)` - Insert or update
- `V? get(K key)` - Get value for key
- `bool has(K key)` - Check if key exists
- `void remove(K key)` - Remove key
- `int len()` - Get size
- `[]K keys()` - Get all keys
- `[]V values()` - Get all values

```satori
let map := Map[string, int]{}
map.set("one", 1)
map.set("two", 2)

let val := map.get("one") or 0
if map.has("three")
    io.println "Found three"
```

**`struct Set[T]`**

Unordered set of unique values.

Methods:
- `void add(T item)` - Add item
- `bool contains(T item)` - Check membership
- `void remove(T item)` - Remove item
- `int len()` - Get size

---

### math - Mathematics

Mathematical functions and constants.

#### Constants

```satori
import math

math.PI        // 3.14159265358979323846
math.E         // 2.71828182845904523536
```

#### Functions

**Trigonometry:**
- `float sin(float x)` - Sine
- `float cos(float x)` - Cosine
- `float tan(float x)` - Tangent
- `float asin(float x)` - Arc sine
- `float acos(float x)` - Arc cosine
- `float atan(float x)` - Arc tangent
- `float atan2(float y, float x)` - Two-argument arc tangent

**Exponential and Logarithmic:**
- `float exp(float x)` - e^x
- `float log(float x)` - Natural logarithm
- `float log10(float x)` - Base-10 logarithm
- `float pow(float base, float exp)` - Power
- `float sqrt(float x)` - Square root

**Rounding:**
- `float floor(float x)` - Round down
- `float ceil(float x)` - Round up
- `float round(float x)` - Round to nearest
- `float abs(float x)` - Absolute value

**Other:**
- `float min(float a, float b)` - Minimum
- `float max(float a, float b)` - Maximum
- `int min_int(int a, int b)` - Integer minimum
- `int max_int(int a, int b)` - Integer maximum

---

### time - Time and Date

Time operations and measurements.

#### Functions

**`int now()`**

Get current Unix timestamp (seconds since epoch).

```satori
import time

let ts := time.now()
io.println "Current time: {}", ts
```

**`void sleep(int ms)`**

Sleep for milliseconds.

```satori
time.sleep(1000)  // Sleep for 1 second
```

**`int clock_ms()`**

Get monotonic clock in milliseconds (for measuring elapsed time).

```satori
let start := time.clock_ms()
do_work()
let elapsed := time.clock_ms() - start
io.println "Took {} ms", elapsed
```

**`DateTime parse(string format, string str)`**

Parse time string.

```satori
let dt := time.parse("2006-01-02", "2024-03-15") or panic("Bad format")
```

**`string format(DateTime dt, string format)`**

Format time as string.

```satori
let str := time.format(dt, "2006-01-02 15:04:05")
```

#### Types

**`struct DateTime`**

Date and time representation.

Fields:
- `int year`
- `int month` (1-12)
- `int day` (1-31)
- `int hour` (0-23)
- `int minute` (0-59)
- `int second` (0-59)

Methods:
- `int unix()` - Convert to Unix timestamp
- `string iso()` - Format as ISO 8601

---

### os - Operating System

System-level operations.

#### Functions

**`string? getenv(string name)`**

Get environment variable.

```satori
import os

let home := os.getenv("HOME") or "/tmp"
```

**`void setenv(string name, string value)`**

Set environment variable.

```satori
os.setenv("DEBUG", "1")
```

**`[]string args()`**

Get command-line arguments.

```satori
let args := os.args()
for arg in args
    io.println arg
```

**`void exit(int code)`**

Exit program with status code.

```satori
os.exit(1)
```

**`int? exec(string command, []string args)`**

Execute command and wait. Returns exit code.

```satori
let code := os.exec("ls", ["-l", "/tmp"]) or panic("exec failed")
if code != 0
    io.println "Command failed with code {}", code
```

---

### string - String Manipulation

String utility functions.

#### Functions

**`[]string split(string s, string sep)`**

Split string by separator.

```satori
import string

let parts := string.split("a,b,c", ",")
// parts = ["a", "b", "c"]
```

**`string join([]string parts, string sep)`**

Join strings with separator.

```satori
let s := string.join(["a", "b", "c"], ",")
// s = "a,b,c"
```

**`string trim(string s)`**

Remove leading/trailing whitespace.

```satori
let s := string.trim("  hello  ")
// s = "hello"
```

**`bool starts_with(string s, string prefix)`**

Check if string starts with prefix.

```satori
if string.starts_with(path, "/home/")
    io.println "Home directory"
```

**`bool ends_with(string s, string suffix)`**

Check if string ends with suffix.

```satori
if string.ends_with(filename, ".sat")
    compile(filename)
```

**`bool contains(string s, string substr)`**

Check if string contains substring.

```satori
if string.contains(text, "error")
    handle_error()
```

**`string replace(string s, string old, string new)`**

Replace all occurrences.

```satori
let s := string.replace("hello world", "world", "Satori")
// s = "hello Satori"
```

**`int len(string s)`**

Get string length in bytes.

```satori
let n := string.len("hello")
// n = 5
```

**`string to_upper(string s)`**

Convert to uppercase.

```satori
let s := string.to_upper("hello")
// s = "HELLO"
```

**`string to_lower(string s)`**

Convert to lowercase.

```satori
let s := string.to_lower("HELLO")
// s = "hello"
```

---

### json - JSON

JSON encoding and decoding.

#### Functions

**`Value? parse(string json_str)`**

Parse JSON string.

```satori
import json

let data := json.parse('{"name": "Alice", "age": 30}') or panic("Bad JSON")
let name := data.get("name").as_string()
let age := data.get("age").as_int()
```

**`string encode(Value value)`**

Encode value to JSON string.

```satori
let obj := json.object()
obj.set("name", json.string("Alice"))
obj.set("age", json.number(30))

let json_str := json.encode(obj)
io.println json_str
```

#### Types

**`struct Value`**

JSON value (object, array, string, number, bool, null).

Methods:
- `bool is_object()` - Check type
- `bool is_array()`
- `bool is_string()`
- `bool is_number()`
- `bool is_bool()`
- `bool is_null()`
- `Value? get(string key)` - Get object field
- `Value? at(int index)` - Get array element
- `string as_string()` - Convert to string
- `int as_int()` - Convert to int
- `float as_float()` - Convert to float
- `bool as_bool()` - Convert to bool

#### Constructors

```satori
let obj := json.object()
let arr := json.array()
let str := json.string("hello")
let num := json.number(42)
let b := json.bool(true)
let n := json.null()
```

---

## Error Handling Convention

All fallible operations return optional types (denoted with `?`). Use the `or` operator to handle failures:

```satori
// Provide default value
let config := load_config() or default_config()

// Return on error
let data := file.read() or return

// Panic on critical error
let db := connect_db() or panic("Database unavailable")

// Continue loop iteration
let client := listener.accept() or continue
```

## Naming Conventions

- **Module names**: lowercase, single word (e.g., `io`, `net`)
- **Function names**: lowercase with underscores (e.g., `read_line`, `parse_json`)
- **Type names**: PascalCase (e.g., `Socket`, `FileInfo`)
- **Constants**: UPPERCASE with underscores (e.g., `MAX_SIZE`, `PI`)

## Implementation Status

| Module       | Status      | Notes                          |
|--------------|-------------|--------------------------------|
| io           | ✅ Partial  | Basic print/println working    |
| net          | 🚧 Planned  | Sockets design in progress     |
| fs           | 🚧 Planned  | File operations planned        |
| collections  | 🚧 Planned  | List/Map/Set planned           |
| math         | 🚧 Planned  | Math functions planned         |
| time         | 🚧 Planned  | Time operations planned        |
| os           | 🚧 Planned  | System interface planned       |
| string       | 🚧 Planned  | String utilities planned       |
| json         | 🚧 Planned  | JSON support planned           |

---

**Note:** This is a working specification. APIs may change as the language evolves. Feedback welcome from recreational engineers building strong, working solutions.
