// src/core/value.h - Value representation

#ifndef SATORI_VALUE_H
#define SATORI_VALUE_H

#include "common.h"

// Forward declarations
typedef struct Object Object;
typedef struct ObjString ObjString;
typedef struct Value Value;
typedef Value (*NativeFn)(int arg_count, Value *args);

// Value types
typedef enum {
  VALUE_NIL,
  VALUE_BOOL,
  VALUE_INT,
  VALUE_FLOAT,
  VALUE_STRING,      // Simple string (for now, will use ObjString later)
  VALUE_NATIVE_FN,   // Native C function
  VALUE_OBJ,         // Heap-allocated object
} ValueType;

// Tagged union value
struct Value {
  ValueType type;
  union {
    bool as_bool;
    i64 as_int;
    f64 as_float;
    char *as_string;  // For VALUE_STRING
    NativeFn as_native_fn;  // For VALUE_NATIVE_FN
    Object *as_obj;   // For VALUE_OBJ
  } u;
};

// Type checking macros
#define IS_NIL(value)       ((value).type == VALUE_NIL)
#define IS_BOOL(value)      ((value).type == VALUE_BOOL)
#define IS_INT(value)       ((value).type == VALUE_INT)
#define IS_FLOAT(value)     ((value).type == VALUE_FLOAT)
#define IS_NUMBER(value)    (IS_INT(value) || IS_FLOAT(value))
#define IS_STRING(value)    ((value).type == VALUE_STRING)
#define IS_NATIVE_FN(value) ((value).type == VALUE_NATIVE_FN)
#define IS_OBJ(value)       ((value).type == VALUE_OBJ)

// Value extraction macros
#define AS_BOOL(value)      ((value).u.as_bool)
#define AS_INT(value)       ((value).u.as_int)
#define AS_FLOAT(value)     ((value).u.as_float)
#define AS_STRING(value)    ((value).u.as_string)
#define AS_NATIVE_FN(value) ((value).u.as_native_fn)
#define AS_OBJ(value)       ((value).u.as_obj)

// Value constructors
Value value_make_nil(void);
Value value_make_bool(bool val);
Value value_make_int(i64 val);
Value value_make_float(f64 val);
Value value_make_string(const char *str);
Value value_make_native_fn(NativeFn fn);
Value value_make_obj(Object *obj);

// Value operations
bool value_equal(Value a, Value b);
f64 value_to_float(Value v);  // Convert int or float to float
void value_print(Value value);
void value_free(Value value);  // Free if needed

// Constants
#define NIL_VAL         value_make_nil()
#define BOOL_VAL(b)     value_make_bool(b)
#define INT_VAL(i)      value_make_int(i)
#define FLOAT_VAL(f)    value_make_float(f)
#define OBJ_VAL(obj)    value_make_obj((Object*)(obj))

#endif // SATORI_VALUE_H
