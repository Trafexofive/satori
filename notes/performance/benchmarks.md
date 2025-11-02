# Performance Benchmarks & Profiling

## Benchmark Suite Plan

### Micro-benchmarks

**1. Function Call Overhead**
```satori
void noop() {}

void benchmark_call()
    let start := time.clock_ms()
    for i in 0..1_000_000
        noop()
    let elapsed := time.clock_ms() - start
    io.println "1M calls: {}ms", elapsed
```

**Target:** < 10ns per call (100M calls/sec)

---

**2. Variable Access**
```satori
void benchmark_locals()
    let start := time.clock_ms()
    let x := 42
    for i in 0..10_000_000
        let y := x
        let z := y
    let elapsed := time.clock_ms() - start
    io.println "10M local reads: {}ms", elapsed
```

**Target:** < 2ns per read

---

**3. Arithmetic Operations**
```satori
void benchmark_arithmetic()
    let start := time.clock_ms()
    let sum := 0
    for i in 0..10_000_000
        sum += i
    let elapsed := time.clock_ms() - start
    io.println "10M additions: {}ms", elapsed
```

**Target:** < 1ns per operation

---

### Macro-benchmarks

**1. Fibonacci (Recursive)**
```satori
int fib(int n)
    if n <= 1: return n
    return fib(n-1) + fib(n-2)

void benchmark_fib()
    let start := time.clock_ms()
    let result := fib(30)
    let elapsed := time.clock_ms() - start
    io.println "fib(30) = {} in {}ms", result, elapsed
```

**Baseline comparison:**
- Python 3.11: ~1500ms
- Lua 5.4: ~400ms
- LuaJIT: ~30ms
- Go: ~15ms
- C (gcc -O2): ~5ms

**Satori Target (v1.0):** < 500ms (3x faster than Python)

---

**2. String Concatenation**
```satori
void benchmark_string_concat()
    let start := time.clock_ms()
    let s := ""
    for i in 0..100_000
        s = s + "x"
    let elapsed := time.clock_ms() - start
    io.println "100k concats: {}ms (len={})", elapsed, s.len()
```

**Target:** < 100ms with StringBuilder optimization

---

**3. Hash Table Operations**
```satori
void benchmark_map()
    let map := Map[string, int]{}
    
    // Insert
    let start := time.clock_ms()
    for i in 0..100_000
        map.set("key" + i.to_string(), i)
    let insert_time := time.clock_ms() - start
    
    // Lookup
    start = time.clock_ms()
    let sum := 0
    for i in 0..100_000
        sum += map.get("key" + i.to_string()) or 0
    let lookup_time := time.clock_ms() - start
    
    io.println "Map: 100k inserts={}ms, lookups={}ms", 
               insert_time, lookup_time
```

**Target:** 
- Insert: < 50ms
- Lookup: < 30ms

---

**4. JSON Parsing**
```satori
void benchmark_json_parse()
    let json := generate_large_json()  // 1MB
    
    let start := time.clock_ms()
    let data := json.parse(json) or panic("Parse error")
    let elapsed := time.clock_ms() - start
    
    io.println "JSON parse (1MB): {}ms", elapsed
```

**Target:** < 50ms (comparable to C libraries)

---

**5. HTTP Echo Server**
```satori
import net

void handle_client(net.Socket client)
    defer client.close()
    let req := client.read(4096) or return
    client.write(req)  // Echo back

void benchmark_server()
    let listener := net.listen("127.0.0.1", 9000) or return
    
    io.println "Echo server on :9000"
    loop
        let client := listener.accept() or continue
        spawn handle_client(client)
```

**Test with:** `wrk -t4 -c100 -d10s http://127.0.0.1:9000`

**Target:** > 10,000 req/s

---

## Profiling Tools

### Built-in Profiler (Future)

**Time-based profiling:**
```bash
$ satori --profile program.sat

Profile Report:
  Total time: 1.234s
  
  Function          Calls      Total    Avg      %
  -----------------------------------------------
  process_data      1000       850ms    0.85ms   68.9%
  parse_line        10000      250ms    0.02ms   20.3%
  validate          1000       100ms    0.10ms   8.1%
  main              1          34ms     34.00ms  2.7%
```

**Memory profiling:**
```bash
$ satori --profile-memory program.sat

Memory Report:
  Peak usage: 45.2 MB
  Total allocations: 1,234,567
  GC runs: 42
  
  Type              Count      Memory    Avg Size
  -----------------------------------------------
  String            45000      12.5 MB   278 bytes
  List              5000       8.2 MB    1.6 KB
  Map               1200       5.1 MB    4.2 KB
  Object            8900       3.8 MB    436 bytes
```

---

### External Tools

**Valgrind (memory leaks):**
```bash
$ valgrind --leak-check=full ./bin/satori program.sat
```

**Perf (Linux profiling):**
```bash
$ perf record -g ./bin/satori program.sat
$ perf report
```

**gprof (GNU profiler):**
```bash
$ gcc -pg -o satori src/*.c
$ ./satori program.sat
$ gprof satori gmon.out > analysis.txt
```

---

## Optimization Tracking

### Current Performance (v0.1)

| Benchmark           | Result       | Status |
|---------------------|--------------|--------|
| Function call       | Not measured | ❌     |
| Variable access     | Not measured | ❌     |
| fib(30)            | Not impl     | ❌     |
| String concat      | Not impl     | ❌     |
| Map operations     | Not impl     | ❌     |

### Target Performance (v1.0)

| Benchmark           | Target       | Priority |
|---------------------|--------------|----------|
| Function call       | < 10ns       | High     |
| Variable access     | < 2ns        | High     |
| fib(30)            | < 500ms      | Medium   |
| String concat      | < 100ms      | Medium   |
| Map operations     | < 50ms       | Medium   |
| JSON parse (1MB)   | < 50ms       | Low      |
| HTTP echo          | > 10k req/s  | Low      |

---

## Performance Regression Tests

**CI Integration:**

```bash
# Run benchmarks on every commit
make bench

# Compare with baseline
./scripts/compare_benchmarks.sh baseline.txt current.txt

# Alert if regression > 10%
```

**Benchmark format:**
```json
{
  "timestamp": "2024-11-02T10:30:00Z",
  "commit": "abc123",
  "benchmarks": {
    "fib30": {"time_ms": 456, "iterations": 10},
    "string_concat": {"time_ms": 89, "iterations": 5},
    "map_insert": {"time_ms": 45, "iterations": 10}
  }
}
```

---

## Optimization Opportunities

### Phase 1: Low-hanging fruit

**Constant folding:**
```satori
let x := 2 + 3 * 4  // Compute at compile time → 14
```

**Dead code elimination:**
```satori
if false
    expensive_operation()  // Remove entire branch
```

**String interning:**
```c
// Share identical string literals
ObjString* intern_string(const char *chars, int length) {
  // Check intern table
  ObjString *interned = table_find_string(&vm.strings, chars, length);
  if (interned != NULL) return interned;
  
  // Not found, create and intern
  ObjString *str = allocate_string(chars, length);
  table_set(&vm.strings, str, NIL_VAL);
  return str;
}
```

---

### Phase 2: Advanced optimizations

**Inline caching for method calls:**
```c
struct InlineCache {
  ObjStruct *last_type;
  int method_index;
};

// Monomorphic cache
Value invoke_method(ObjInstance *obj, ObjString *name, 
                   InlineCache *cache) {
  if (cache->last_type == obj->type) {
    // Cache hit - fast path
    return obj->type->methods[cache->method_index];
  }
  
  // Cache miss - slow path
  Value method = lookup_method(obj->type, name);
  cache->last_type = obj->type;
  cache->method_index = find_method_index(obj->type, name);
  return method;
}
```

**NaN boxing for value representation:**
```c
// All values fit in 64 bits
// Saves memory, improves cache locality

// Floats: normal IEEE 754
// Ints: encoded in NaN payload
// Pointers: encoded in NaN payload with tag bits
```

---

### Phase 3: JIT compilation

**Trace-based JIT (like LuaJIT):**

1. Interpret bytecode normally
2. Detect hot loops/functions
3. Record execution trace
4. Compile trace to native code
5. Patch bytecode to jump to native

**Expected speedup:** 10-50x on hot paths

---

## Memory Usage Goals

| Category        | v0.1    | v1.0 Target | Notes                    |
|----------------|---------|-------------|--------------------------|
| VM overhead    | ~1 KB   | ~10 KB      | Larger with optimizations|
| Empty program  | ~100 KB | ~500 KB     | Runtime + stdlib         |
| Per function   | ~100 B  | ~200 B      | Code + metadata          |
| Per object     | ~50 B   | ~30 B       | With NaN boxing          |
| Per string     | len+24B | len+16B     | Header optimization      |

---

## Comparison Matrix (Target for v1.0)

```
Metric          | Satori | Python | Lua   | Go    | C      |
----------------|--------|--------|-------|-------|--------|
Startup time    | 10ms   | 50ms   | 5ms   | 100ms | 1ms    |
Memory (idle)   | 500KB  | 10MB   | 200KB | 2MB   | 100KB  |
fib(30)         | 500ms  | 1500ms | 400ms | 15ms  | 5ms    |
String ops      | 100ms  | 150ms  | 80ms  | 20ms  | 10ms   |
HTTP (req/s)    | 10k    | 5k     | 15k   | 50k   | 100k   |
GC pause (p95)  | 10ms   | 20ms   | 5ms   | 1ms   | N/A    |
```

**Goal:** Competitive with Lua, faster than Python

---

**Living document - update with actual measurements**
