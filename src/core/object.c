// src/core/object.c - Object implementation

#include "object.h"
#include "memory.h"
#include <stdio.h>
#include <string.h>

void object_print(Object *obj) {
  switch (obj->type) {
    case OBJ_STRING:
      printf("%s", ((ObjString*)obj)->chars);
      break;
    case OBJ_FUNCTION:
      printf("<function>");
      break;
    case OBJ_NATIVE:
      printf("<native fn>");
      break;
    default:
      printf("<object>");
      break;
  }
}

void object_free(Object *obj) {
  switch (obj->type) {
    case OBJ_STRING: {
      ObjString *str = (ObjString*)obj;
      mem_free(str->chars);
      mem_free(str);
      break;
    }
    default:
      mem_free(obj);
      break;
  }
}

// FNV-1a hash function
u32 string_hash(const char *key, int length) {
  u32 hash = 2166136261u;
  for (int i = 0; i < length; i++) {
    hash ^= (u8)key[i];
    hash *= 16777619;
  }
  return hash;
}

static ObjString *string_allocate(char *chars, int length, u32 hash) {
  ObjString *str = (ObjString*)mem_alloc(sizeof(ObjString));
  str->obj.type = OBJ_STRING;
  str->obj.is_marked = false;
  str->obj.next = NULL;
  str->chars = chars;
  str->length = length;
  str->hash = hash;
  return str;
}

ObjString *string_make(const char *chars, int length) {
  u32 hash = string_hash(chars, length);
  // TODO: Intern strings here
  return string_copy(chars, length);
}

ObjString *string_copy(const char *chars, int length) {
  u32 hash = string_hash(chars, length);
  char *heap_chars = (char*)mem_alloc(length + 1);
  memcpy(heap_chars, chars, length);
  heap_chars[length] = '\0';
  return string_allocate(heap_chars, length, hash);
}

ObjString *string_take(char *chars, int length) {
  u32 hash = string_hash(chars, length);
  return string_allocate(chars, length, hash);
}

ObjString *string_concat(ObjString *a, ObjString *b) {
  int length = a->length + b->length;
  char *chars = (char*)mem_alloc(length + 1);
  memcpy(chars, a->chars, a->length);
  memcpy(chars + a->length, b->chars, b->length);
  chars[length] = '\0';
  return string_take(chars, length);
}
