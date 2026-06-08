// src/core/source.h - Source text management
//
// Professional interpreter pattern: keep source text accessible
// through the entire compilation and execution pipeline so errors
// can display the offending line with precise span underlines.

#ifndef SATORI_SOURCE_H
#define SATORI_SOURCE_H

// SourceInfo holds the original source text and a pre-computed
// line-start offset table for O(1) line-by-number lookup.
typedef struct SourceInfo {
  const char *file_path;  // Original file path (not owned)
  const char *source;     // Full source text (owned)
  int *line_starts;       // Offset of each line's first byte (owned)
  int line_count;         // Number of lines
} SourceInfo;

// Build SourceInfo from source text. Scans for newlines to build
// the line offset table. Takes ownership of source (will free it).
void source_init(SourceInfo *si, const char *file_path, const char *source);

// Free all owned memory.
void source_free(SourceInfo *si);

// Get a pointer to the start of a line (1-indexed). Returns NULL
// if line is out of range. Length is the number of bytes in the
// line, excluding the trailing newline.
const char *source_get_line(SourceInfo *si, int line, int *length);

// Get the 1-based column of a source offset within its line.
int source_offset_to_column(SourceInfo *si, int offset);

#endif // SATORI_SOURCE_H
