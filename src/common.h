// src/common.h - The foundation

#ifndef SATORI_COMMON_H
#define SATORI_COMMON_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Version
#define SATORI_VERSION_MAJOR 0
#define SATORI_VERSION_MINOR 1
#define SATORI_VERSION_PATCH 0

// Memory
#define SATORI_STACK_MAX 256
#define SATORI_HEAP_INIT_SIZE (1024 * 1024) // 1MB

// Limits
#define SATORI_MAX_LOCALS 256
#define SATORI_MAX_PARAMS 32
#define SATORI_MAX_UPVALUES 256

// Debug flags
#ifdef DEBUG
#define SATORI_DEBUG_PRINT_CODE
#define SATORI_DEBUG_TRACE_EXECUTION
#endif

// Core types
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef float f32;
typedef double f64;

// Utility macros
#define ARRAY_COUNT(arr) (sizeof(arr) / sizeof((arr)[0]))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define UNREACHABLE()                                                          \
  do {                                                                         \
    fprintf(stderr, "Unreachable code at %s:%d\n", __FILE__, __LINE__);        \
    abort();                                                                   \
  } while (0)

#define ASSERT(cond, msg)                                                      \
  do {                                                                         \
    if (!(cond)) {                                                             \
      fprintf(stderr, "Assertion failed: %s at %s:%d\n", msg, __FILE__,        \
              __LINE__);                                                       \
      abort();                                                                 \
    }                                                                          \
  } while (0)

#endif // SATORI_COMMON_H
