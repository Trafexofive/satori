// src/core/memory.h - Memory management

#ifndef SATORI_MEMORY_H
#define SATORI_MEMORY_H

#include "common.h"
#include <stddef.h>

// Memory allocation
void *mem_alloc(size_t size);
void *mem_realloc(void *ptr, size_t new_size);
void mem_free(void *ptr);

// Dynamic array growth
#define GROW_CAPACITY(capacity) \
  ((capacity) < 8 ? 8 : (capacity) * 2)

#define GROW_ARRAY(type, pointer, old_count, new_count) \
  (type*)mem_realloc(pointer, sizeof(type) * (new_count))

#define FREE_ARRAY(type, pointer, old_count) \
  mem_realloc(pointer, 0)

#endif // SATORI_MEMORY_H
