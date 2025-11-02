// src/core/table.h - Hash table for strings -> values
//
// Simple hash table for module registry and global variables.
// Uses string keys and Value payloads.

#ifndef SATORI_TABLE_H
#define SATORI_TABLE_H

#include "common.h"
#include "value.h"

// Hash table entry
typedef struct {
  char *key;      // NULL means empty slot
  Value value;
} Entry;

// Hash table structure
typedef struct {
  int count;      // Number of entries
  int capacity;   // Total slots
  Entry *entries; // Array of entries
} Table;

// Table operations
void table_init(Table *table);
void table_free(Table *table);
bool table_get(Table *table, const char *key, Value *value);
bool table_set(Table *table, const char *key, Value value);
bool table_delete(Table *table, const char *key);

// Internal constants
#define TABLE_MAX_LOAD 0.75

#endif // SATORI_TABLE_H
