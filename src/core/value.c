// src/core/value.c - Value implementation

#define _POSIX_C_SOURCE 200809L

#include "value.h"
#include "object.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

Value value_make_nil(void) {
  Value v;
  v.type = VALUE_NIL;
  return v;
}

Value value_make_bool(bool val) {
  Value v;
  v.type = VALUE_BOOL;
  v.u.as_bool = val;
  return v;
}

Value value_make_int(i64 val) {
  Value v;
  v.type = VALUE_INT;
  v.u.as_int = val;
  return v;
}

Value value_make_float(f64 val) {
  Value v;
  v.type = VALUE_FLOAT;
  v.u.as_float = val;
  return v;
}

Value value_make_string(const char *str) {
  Value v;
  v.type = VALUE_STRING;
  v.u.as_string = strdup(str);
  return v;
}

Value value_make_native_fn(NativeFn fn) {
  Value v;
  v.type = VALUE_NATIVE_FN;
  v.u.as_native_fn = fn;
  return v;
}

Value value_make_obj(Object *obj) {
  Value v;
  v.type = VALUE_OBJ;
  v.u.as_obj = obj;
  return v;
}

bool value_equal(Value a, Value b) {
  if (a.type != b.type) return false;
  
  switch (a.type) {
    case VALUE_NIL: return true;
    case VALUE_BOOL: return AS_BOOL(a) == AS_BOOL(b);
    case VALUE_INT: return AS_INT(a) == AS_INT(b);
    case VALUE_FLOAT: return AS_FLOAT(a) == AS_FLOAT(b);
    case VALUE_OBJ: return AS_OBJ(a) == AS_OBJ(b);
    default: return false;
  }
}

f64 value_to_float(Value v) {
  if (IS_INT(v)) return (f64)AS_INT(v);
  if (IS_FLOAT(v)) return AS_FLOAT(v);
  return 0.0;  // Error case
}

void value_print(Value value) {
  switch (value.type) {
    case VALUE_NIL:
      printf("nil");
      break;
    case VALUE_BOOL:
      printf(AS_BOOL(value) ? "true" : "false");
      break;
    case VALUE_INT:
      printf("%lld", (long long)AS_INT(value));
      break;
    case VALUE_FLOAT:
      printf("%g", AS_FLOAT(value));
      break;
    case VALUE_STRING:
      printf("%s", AS_STRING(value));
      break;
    case VALUE_NATIVE_FN:
      printf("<native fn>");
      break;
    case VALUE_OBJ:
      object_print(AS_OBJ(value));
      break;
  }
}

void value_free(Value value) {
  // Free string memory
  if (value.type == VALUE_STRING) {
    free(value.u.as_string);
  }
  // Objects are freed by GC
}
