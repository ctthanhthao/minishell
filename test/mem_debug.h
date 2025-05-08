// mem_debug.h
#ifndef MEM_DEBUG_H
#define MEM_DEBUG_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void *dbg_malloc(size_t size, const char *file, int line);
void dbg_free(void *ptr, const char *file, int line);
char *dbg_strdup(const char *s, const char *file, int line);

#define malloc(size) dbg_malloc(size, __FILE__, __LINE__)
#define free(ptr) dbg_free(ptr, __FILE__, __LINE__)
#define strdup(s) dbg_strdup(s, __FILE__, __LINE__)

void print_allocs(void);

#endif