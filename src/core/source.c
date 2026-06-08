// src/core/source.c - Source text management implementation

#define _POSIX_C_SOURCE 200809L

#include "core/source.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

void source_init(SourceInfo *si, const char *file_path, const char *source) {
  si->file_path = file_path;
  si->source = source;

  // Count lines to size the offset table
  int count = 1;  // At least one line even if empty
  for (const char *p = source; *p; p++) {
    if (*p == '\n') count++;
  }
  si->line_count = count;

  // Allocate and fill line-start offsets
  si->line_starts = (int *)malloc((count + 1) * sizeof(int));
  si->line_starts[0] = 0;
  int idx = 1;
  for (const char *p = source; *p; p++) {
    if (*p == '\n') {
      // Next line starts after the newline
      si->line_starts[idx++] = (int)(p - source + 1);
    }
  }
  // Sentinel: end of source (for length calculation)
  si->line_starts[idx] = (int)(source + strlen(source) - source);
}

void source_free(SourceInfo *si) {
  free((char *)si->source);
  si->source = NULL;
  free(si->line_starts);
  si->line_starts = NULL;
  si->line_count = 0;
  si->file_path = NULL;
}

const char *source_get_line(SourceInfo *si, int line, int *length) {
  // Lines are 1-indexed in error messages
  if (line < 1 || line > si->line_count) return NULL;
  int idx = line - 1;
  int start = si->line_starts[idx];
  int end = si->line_starts[idx + 1];
  // Strip trailing newline/carriage return for display
  while (end > start && (si->source[end - 1] == '\n' ||
                         si->source[end - 1] == '\r')) {
    end--;
  }
  if (length) *length = end - start;
  return si->source + start;
}

int source_offset_to_column(SourceInfo *si, int offset) {
  // Binary search to find which line contains this offset
  int lo = 0, hi = si->line_count - 1;
  while (lo < hi) {
    int mid = (lo + hi + 1) / 2;
    if (si->line_starts[mid] <= offset)
      lo = mid;
    else
      hi = mid - 1;
  }
  return offset - si->line_starts[lo] + 1;  // 1-based column
}
