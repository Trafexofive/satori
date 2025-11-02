# Standard Library Implementation Sketches

## io Module - Implementation Notes

### print/println Design

**Current (v0.1):**
- Hardcoded in VM as `OP_PRINT`
- Minimal formatting

**Future (v0.2):**
```c
// Native function approach
Value native_println(int arg_count, Value *args) {
  for (int i = 0; i < arg_count; i++) {
    value_print(args[i]);
    if (i < arg_count - 1) printf(" ");
  }
  printf("\n");
  return value_make_nil();
}

// Register in global table
vm_register_native("io.println", native_println);
```

**Format String (v0.3):**
```c
// Parse format string with {} placeholders
Value native_println_fmt(int arg_count, Value *args) {
  if (arg_count == 0) {
    printf("\n");
    return value_make_nil();
  }
  
  char *fmt = AS_STRING(args[0]);
  int arg_idx = 1;
  
  for (char *p = fmt; *p; p++) {
    if (*p == '{' && *(p+1) == '}') {
      if (arg_idx < arg_count) {
        value_print(args[arg_idx++]);
      }
      p++;  // Skip '}'
    } else {
      putchar(*p);
    }
  }
  printf("\n");
  return value_make_nil();
}
```

### File Operations

**File Handle Structure:**
```c
typedef struct {
  Object obj;      // GC header
  FILE *fp;
  char *path;
  bool is_open;
} ObjFile;

Value native_io_open(int arg_count, Value *args) {
  EXPECT_ARG_COUNT(2);
  EXPECT_STRING(args[0]);  // path
  EXPECT_STRING(args[1]);  // mode
  
  char *path = AS_STRING(args[0]);
  char *mode = AS_STRING(args[1]);
  
  FILE *fp = fopen(path, mode);
  if (!fp) {
    return value_make_nil();  // Error
  }
  
  ObjFile *file = alloc_file(fp, path);
  return OBJ_VAL(file);
}

Value native_file_read(int arg_count, Value *args) {
  EXPECT_ARG_COUNT(2);
  EXPECT_FILE(args[0]);
  EXPECT_INT(args[1]);
  
  ObjFile *file = AS_FILE(args[0]);
  int n = AS_INT(args[1]);
  
  if (!file->is_open) return value_make_nil();
  
  char *buffer = malloc(n);
  size_t read = fread(buffer, 1, n, file->fp);
  
  ObjString *str = make_string(buffer, read);
  free(buffer);
  
  return OBJ_VAL(str);
}
```

---

## net Module - TCP Sockets

**Socket Structure:**
```c
typedef struct {
  Object obj;
  int fd;
  bool is_open;
  struct sockaddr_in addr;
} ObjSocket;

Value native_net_connect(int arg_count, Value *args) {
  EXPECT_ARG_COUNT(2);
  EXPECT_STRING(args[0]);  // host
  EXPECT_INT(args[1]);     // port
  
  char *host = AS_STRING(args[0]);
  int port = AS_INT(args[1]);
  
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) return value_make_nil();
  
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  
  // Resolve host
  struct hostent *server = gethostbyname(host);
  if (!server) {
    close(sockfd);
    return value_make_nil();
  }
  
  memcpy(&addr.sin_addr.s_addr, server->h_addr, server->h_length);
  
  if (connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
    close(sockfd);
    return value_make_nil();
  }
  
  ObjSocket *sock = alloc_socket(sockfd, addr);
  return OBJ_VAL(sock);
}

Value native_net_listen(int arg_count, Value *args) {
  EXPECT_ARG_COUNT(2);
  EXPECT_STRING(args[0]);  // host
  EXPECT_INT(args[1]);     // port
  
  // Similar setup...
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  
  // Set SO_REUSEADDR
  int opt = 1;
  setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
  
  // Bind and listen
  bind(sockfd, (struct sockaddr*)&addr, sizeof(addr));
  listen(sockfd, SOMAXCONN);
  
  ObjListener *listener = alloc_listener(sockfd, addr);
  return OBJ_VAL(listener);
}
```

---

## collections Module - Dynamic Arrays

**List Implementation:**
```c
typedef struct {
  Object obj;
  Value *items;
  int count;
  int capacity;
} ObjList;

Value native_list_push(int arg_count, Value *args) {
  EXPECT_ARG_COUNT(2);
  EXPECT_LIST(args[0]);
  
  ObjList *list = AS_LIST(args[0]);
  Value item = args[1];
  
  if (list->count + 1 > list->capacity) {
    int new_cap = GROW_CAPACITY(list->capacity);
    list->items = GROW_ARRAY(Value, list->items, 
                             list->capacity, new_cap);
    list->capacity = new_cap;
  }
  
  list->items[list->count++] = item;
  return value_make_nil();
}

Value native_list_pop(int arg_count, Value *args) {
  EXPECT_ARG_COUNT(1);
  EXPECT_LIST(args[0]);
  
  ObjList *list = AS_LIST(args[0]);
  
  if (list->count == 0) {
    return value_make_nil();  // Error: empty list
  }
  
  return list->items[--list->count];
}
```

**Map Implementation (Hash Table):**
```c
typedef struct {
  Value key;
  Value value;
} Entry;

typedef struct {
  Object obj;
  Entry *entries;
  int count;
  int capacity;
} ObjMap;

// FNV-1a hash
static uint32_t hash_value(Value value) {
  switch (value.type) {
    case VALUE_INT:
      return hash_int(AS_INT(value));
    case VALUE_STRING:
      return hash_string(AS_STRING(value));
    // ... etc
  }
}

static Entry* find_entry(Entry *entries, int capacity, Value key) {
  uint32_t index = hash_value(key) % capacity;
  Entry *tombstone = NULL;
  
  for (;;) {
    Entry *entry = &entries[index];
    
    if (IS_NIL(entry->key)) {
      if (IS_NIL(entry->value)) {
        return tombstone != NULL ? tombstone : entry;
      } else {
        if (tombstone == NULL) tombstone = entry;
      }
    } else if (values_equal(entry->key, key)) {
      return entry;
    }
    
    index = (index + 1) % capacity;
  }
}

Value native_map_set(int arg_count, Value *args) {
  EXPECT_ARG_COUNT(3);
  EXPECT_MAP(args[0]);
  
  ObjMap *map = AS_MAP(args[0]);
  Value key = args[1];
  Value value = args[2];
  
  // Grow if needed
  if (map->count + 1 > map->capacity * MAP_LOAD_FACTOR) {
    int new_cap = GROW_CAPACITY(map->capacity);
    // Rehash...
  }
  
  Entry *entry = find_entry(map->entries, map->capacity, key);
  bool is_new = IS_NIL(entry->key);
  
  entry->key = key;
  entry->value = value;
  
  if (is_new) map->count++;
  
  return value_make_nil();
}
```

---

## string Module - String Operations

**Split:**
```c
Value native_string_split(int arg_count, Value *args) {
  EXPECT_ARG_COUNT(2);
  EXPECT_STRING(args[0]);  // string
  EXPECT_STRING(args[1]);  // separator
  
  char *str = AS_STRING(args[0]);
  char *sep = AS_STRING(args[1]);
  int sep_len = strlen(sep);
  
  ObjList *result = alloc_list();
  
  char *start = str;
  char *p = str;
  
  while (*p) {
    if (strncmp(p, sep, sep_len) == 0) {
      // Found separator
      int len = p - start;
      ObjString *part = make_string(start, len);
      list_push(result, OBJ_VAL(part));
      
      p += sep_len;
      start = p;
    } else {
      p++;
    }
  }
  
  // Last part
  ObjString *part = make_string(start, strlen(start));
  list_push(result, OBJ_VAL(part));
  
  return OBJ_VAL(result);
}
```

**Join:**
```c
Value native_string_join(int arg_count, Value *args) {
  EXPECT_ARG_COUNT(2);
  EXPECT_LIST(args[0]);    // array of strings
  EXPECT_STRING(args[1]);  // separator
  
  ObjList *list = AS_LIST(args[0]);
  char *sep = AS_STRING(args[1]);
  int sep_len = strlen(sep);
  
  // Calculate total length
  int total_len = 0;
  for (int i = 0; i < list->count; i++) {
    EXPECT_STRING(list->items[i]);
    total_len += strlen(AS_STRING(list->items[i]));
  }
  total_len += (list->count - 1) * sep_len;
  
  // Build result
  char *result = malloc(total_len + 1);
  char *p = result;
  
  for (int i = 0; i < list->count; i++) {
    char *str = AS_STRING(list->items[i]);
    int len = strlen(str);
    memcpy(p, str, len);
    p += len;
    
    if (i < list->count - 1) {
      memcpy(p, sep, sep_len);
      p += sep_len;
    }
  }
  *p = '\0';
  
  ObjString *str = make_string_take(result, total_len);
  return OBJ_VAL(str);
}
```

---

## json Module - Parsing

**Approach: Recursive descent parser**

```c
typedef struct {
  const char *source;
  const char *current;
} JsonParser;

Value parse_json_value(JsonParser *p) {
  skip_whitespace(p);
  
  switch (*p->current) {
    case '{': return parse_json_object(p);
    case '[': return parse_json_array(p);
    case '"': return parse_json_string(p);
    case 't': case 'f': return parse_json_bool(p);
    case 'n': return parse_json_null(p);
    default:
      if (isdigit(*p->current) || *p->current == '-') {
        return parse_json_number(p);
      }
      // Error
      return value_make_nil();
  }
}

Value parse_json_object(JsonParser *p) {
  EXPECT(p, '{');
  
  ObjMap *obj = alloc_map();
  
  skip_whitespace(p);
  if (*p->current == '}') {
    advance(p);
    return OBJ_VAL(obj);
  }
  
  for (;;) {
    // Parse key
    skip_whitespace(p);
    Value key = parse_json_string(p);
    
    // Expect colon
    skip_whitespace(p);
    EXPECT(p, ':');
    
    // Parse value
    Value value = parse_json_value(p);
    
    // Insert
    map_set(obj, key, value);
    
    // Check for comma
    skip_whitespace(p);
    if (*p->current == ',') {
      advance(p);
    } else if (*p->current == '}') {
      advance(p);
      break;
    } else {
      // Error
      return value_make_nil();
    }
  }
  
  return OBJ_VAL(obj);
}
```

---

## math Module - Mathematical Functions

**Wrapper around C math library:**

```c
Value native_math_sqrt(int arg_count, Value *args) {
  EXPECT_ARG_COUNT(1);
  EXPECT_NUMBER(args[0]);
  
  double x = value_to_float(args[0]);
  double result = sqrt(x);
  
  return value_make_float(result);
}

Value native_math_sin(int arg_count, Value *args) {
  EXPECT_ARG_COUNT(1);
  EXPECT_NUMBER(args[0]);
  
  double x = value_to_float(args[0]);
  return value_make_float(sin(x));
}

// Register all math functions
void register_math_module(VM *vm) {
  vm_register_native("math.sqrt", native_math_sqrt);
  vm_register_native("math.sin", native_math_sin);
  vm_register_native("math.cos", native_math_cos);
  // ... etc
  
  // Constants
  vm_set_global("math.PI", value_make_float(3.14159265358979323846));
  vm_set_global("math.E", value_make_float(2.71828182845904523536));
}
```

---

## Module Registration System

**Module loader:**

```c
typedef struct {
  const char *name;
  void (*init_fn)(VM *vm);
} Module;

static Module builtin_modules[] = {
  {"io", register_io_module},
  {"net", register_net_module},
  {"fs", register_fs_module},
  {"collections", register_collections_module},
  {"math", register_math_module},
  {"string", register_string_module},
  {"time", register_time_module},
  {"os", register_os_module},
  {"json", register_json_module},
  {NULL, NULL}
};

void vm_load_module(VM *vm, const char *name) {
  for (int i = 0; builtin_modules[i].name != NULL; i++) {
    if (strcmp(builtin_modules[i].name, name) == 0) {
      builtin_modules[i].init_fn(vm);
      return;
    }
  }
  
  // Not found in builtins, try loading from file
  load_module_from_file(vm, name);
}
```

**Lazy loading:**
```c
// Only load modules when first imported
// Track loaded modules in a hash table

typedef struct {
  Table loaded_modules;  // name -> bool
} VM;

void compile_import(Compiler *c, AstNode *node) {
  char *module_name = node->as.import.module_name;
  
  // Emit OP_IMPORT with module name constant
  int name_const = make_constant(c, OBJ_VAL(
    make_string(module_name, strlen(module_name))
  ));
  
  emit_bytes(c, OP_IMPORT, name_const);
}

// In VM
case OP_IMPORT: {
  u8 name_idx = READ_BYTE();
  ObjString *name = AS_STRING(vm->chunk.constants[name_idx]);
  
  if (!table_get(&vm->loaded_modules, name)) {
    vm_load_module(vm, name->chars);
    table_set(&vm->loaded_modules, name, value_make_bool(true));
  }
  break;
}
```

---

## Performance Considerations

**Native Function Overhead:**
- Each call requires argument validation
- Type checking at runtime
- Can be optimized with inline caching

**String Operations:**
- Immutable strings (share memory)
- String interning for literals
- StringBuilder for concatenation

**Collection Growth:**
- Exponential growth (2x) for dynamic arrays
- Load factor 0.75 for hash tables
- Lazy shrinking to avoid thrashing

---

**Next Steps:**
1. Implement io module first (most critical)
2. Add collections (needed for everything)
3. string module (common operations)
4. Other modules as needed
