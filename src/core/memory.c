// src/core/memory.c - Memory management implementation

#include "memory.h"
#include <stdlib.h>
#include <stdio.h>

void *mem_alloc(size_t size) {
  void *ptr = malloc(size);
  if (ptr == NULL && size > 0) {
    fprintf(stderr, "\033[1;31merror:\033[0m out of memory\n");
    exit(1);
  }
  return ptr;
}

void *mem_realloc(void *ptr, size_t new_size) {
  if (new_size == 0) {
    free(ptr);
    return NULL;
  }
  
  void *result = realloc(ptr, new_size);
  if (result == NULL) {
    fprintf(stderr, "\033[1;31merror:\033[0m out of memory\n");
    exit(1);
  }
  return result;
}

void mem_free(void *ptr) {
  free(ptr);
}
