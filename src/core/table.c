// src/core/table.c - Hash table implementation

#define _POSIX_C_SOURCE 200809L

#include "table.h"
#include <stdlib.h>
#include <string.h>

// FNV-1a hash function
static u32 hash_string(const char *key) {
  u32 hash = 2166136261u;
  for (int i = 0; key[i] != '\0'; i++) {
    hash ^= (u8)key[i];
    hash *= 16777619;
  }
  return hash;
}

// Find entry in table (for get, set, delete)
static Entry *find_entry(Entry *entries, int capacity, const char *key) {
  u32 index = hash_string(key) % capacity;
  
  for (;;) {
    Entry *entry = &entries[index];
    
    if (entry->key == NULL) {
      // Empty slot
      return entry;
    } else if (strcmp(entry->key, key) == 0) {
      // Key matches
      return entry;
    }
    
    // Collision, linear probing
    index = (index + 1) % capacity;
  }
}

// Grow table capacity
static void adjust_capacity(Table *table, int capacity) {
  Entry *entries = calloc(capacity, sizeof(Entry));
  
  // Initialize all entries
  for (int i = 0; i < capacity; i++) {
    entries[i].key = NULL;
  }
  
  // Rehash existing entries
  table->count = 0;
  for (int i = 0; i < table->capacity; i++) {
    Entry *entry = &table->entries[i];
    if (entry->key == NULL) continue;
    
    Entry *dest = find_entry(entries, capacity, entry->key);
    dest->key = entry->key;
    dest->value = entry->value;
    table->count++;
  }
  
  // Free old array
  free(table->entries);
  table->entries = entries;
  table->capacity = capacity;
}

void table_init(Table *table) {
  table->count = 0;
  table->capacity = 0;
  table->entries = NULL;
}

void table_free(Table *table) {
  // Free all keys (they are strdup'd)
  for (int i = 0; i < table->capacity; i++) {
    if (table->entries[i].key != NULL) {
      free(table->entries[i].key);
    }
  }
  free(table->entries);
  table_init(table);
}

bool table_get(Table *table, const char *key, Value *value) {
  if (table->count == 0) return false;
  
  Entry *entry = find_entry(table->entries, table->capacity, key);
  if (entry->key == NULL) return false;
  
  *value = entry->value;
  return true;
}

bool table_set(Table *table, const char *key, Value value) {
  // Grow if needed
  if (table->count + 1 > table->capacity * TABLE_MAX_LOAD) {
    int capacity = table->capacity < 8 ? 8 : table->capacity * 2;
    adjust_capacity(table, capacity);
  }
  
  Entry *entry = find_entry(table->entries, table->capacity, key);
  bool is_new_key = (entry->key == NULL);
  
  if (is_new_key) {
    entry->key = strdup(key);
    table->count++;
  }
  
  entry->value = value;
  return is_new_key;
}

bool table_delete(Table *table, const char *key) {
  if (table->count == 0) return false;
  
  Entry *entry = find_entry(table->entries, table->capacity, key);
  if (entry->key == NULL) return false;
  
  // Free the key and mark as deleted
  free(entry->key);
  entry->key = NULL;
  entry->value = value_make_nil();
  table->count--;
  
  return true;
}
