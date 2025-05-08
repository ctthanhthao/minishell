// mem_debug.c
#include "mem_debug.h"
#ifdef malloc
#  undef malloc
#endif
#ifdef free
#  undef free
#endif
#ifdef strdup
#  undef strdup
#endif
typedef struct MemNode {
	void *ptr;
	size_t size;
	const char *file;
	int line;
	struct MemNode *next;
} MemNode;

static MemNode *alloc_list = NULL;

void *dbg_malloc(size_t size, const char *file, int line) {
    void *ptr = malloc(size);
    if (ptr) {
        MemNode *node = malloc(sizeof(MemNode));
        node->ptr = ptr;
        node->size = size;
        node->file = file;
        node->line = line;
        node->next = alloc_list;
        alloc_list = node;

        // Print the allocation details
        printf("[ALLOC] %p (%zu bytes) at %s:%d\n", ptr, size, file, line);
    } else {
        printf("[ERROR] malloc failed at %s:%d\n", file, line);
    }
    return ptr;
}

char *dbg_strdup(const char *s, const char *file, int line) {
    char *dup = strdup(s);
    if (dup) {
        MemNode *node = malloc(sizeof(MemNode));
        node->ptr = dup;
        node->size = strlen(s) + 1;
        node->file = file;
        node->line = line;
        node->next = alloc_list;
        alloc_list = node;

        // Print the strdup details
        printf("[STRDUP] %p (%zu bytes) at %s:%d\n", dup, strlen(s) + 1, file, line);
    } else {
        printf("[ERROR] strdup failed at %s:%d\n", file, line);
    }
    return dup;
}

void dbg_free(void *ptr, const char *file, int line) {
	if (!ptr) return;

	MemNode **cur = &alloc_list;
	while (*cur) {
		if ((*cur)->ptr == ptr) {
			MemNode *tmp = *cur;
			*cur = (*cur)->next;
			free(tmp);
			printf("[FREE] %p at %s:%d\n", ptr, file, line);
			free(ptr);
			return;
		}
		cur = &((*cur)->next);
	}
	fprintf(stderr, "[WARNING] Attempt to free unknown pointer %p at %s:%d\n", ptr, file, line);
}

void print_allocs(void) {
	printf("Currently allocated blocks:\n");
	for (MemNode *cur = alloc_list; cur; cur = cur->next) {
		printf(" - %p (%zu bytes) from %s:%d\n", cur->ptr, cur->size, cur->file, cur->line);
	}
}