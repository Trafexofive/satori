// src/core/object.h - Heap-allocated objects

#ifndef SATORI_OBJECT_H
#define SATORI_OBJECT_H

#include "common.h"
#include "value.h"

// Object types
typedef enum {
  OBJ_STRING,
  OBJ_FUNCTION,
  OBJ_NATIVE,
  OBJ_ARRAY,
  OBJ_MAP,
} ObjectType;

// Base object (all heap objects start with this)
struct Object {
  ObjectType type;
  bool is_marked;       // For GC
  struct Object *next;  // Intrusive linked list for GC
};

// String object
struct ObjString {
  Object obj;
  int length;
  char *chars;
  u32 hash;  // Cached hash
};

// Type checking
#define OBJ_TYPE(value)     (AS_OBJ(value)->type)
#define IS_OBJ_STRING(value)    (IS_OBJ(value) && OBJ_TYPE(value) == OBJ_STRING)

// Extraction
#define AS_OBJ_STRING(value)    ((ObjString*)AS_OBJ(value))
#define AS_CSTRING(value)   (((ObjString*)AS_OBJ(value))->chars)

// Object operations
void object_print(Object *obj);
void object_free(Object *obj);

// String operations
ObjString *string_make(const char *chars, int length);
ObjString *string_copy(const char *chars, int length);
ObjString *string_take(char *chars, int length);
ObjString *string_concat(ObjString *a, ObjString *b);
u32 string_hash(const char *key, int length);

#endif // SATORI_OBJECT_H
