
#include "mincrt.h"

typedef struct heap_header {
    enum {
        FREE = 0xabababab,
        USED = 0xcdcdcdcd
    } type;
    u32 size;
    struct heap_header *next;
    struct heap_header *prev;
} heap_header_t;

static heap_header_t *heap_start = NULL;
static heap_header_t *heap_end = NULL;
static const int head_size = sizeof (heap_header_t);

#define HEAP_SIZE(ptr) (ptr->size)
#define REAL_START_FROM_USER_PTR(ptr) (heap_header_t* )(((char*) ptr - head_size ))
#define USER_START_FROM_REAL_PTR(ptr) (void*)((char*) ptr + head_size )
#define IS_USED(ptr) (ptr->type == USED)
#define IS_FREE(ptr) (ptr->type == FREE)

void free(void* ptr) {
    heap_header_t *header = REAL_START_FROM_USER_PTR(ptr);
    header->type = FREE;
    if (IS_FREE(header))
        return;
    header->type = FREE;
    if (header->prev != NULL && IS_FREE(header->prev)) {
        header->prev->next = header->next;
        header->prev->size += HEAP_SIZE(header);
        if(header->next != NULL) {
            header->next->prev = header->prev;
        }
        header = header->prev;
    }
    if(header->next != NULL && IS_FREE(header->next)) {
        header->size += HEAP_SIZE(header->next);
        header->next = header->next->next;
    }
}

void* malloc(size_t size) {
    if(size <= 0)
        return NULL;

    return NULL;
}