
#include "minicrt.h"

typedef struct heap_header
{
    union
    {
        u32 size;
        u8 used: 1;
    };
    struct heap_header *next;
    struct heap_header *prev;
} heap_header_t;

static heap_header_t *heap_start = NULL;
static heap_header_t *base = NULL;
static heap_header_t *end = NULL;
static const int head_size = sizeof(heap_header_t);

#define REAL_START_FROM_USER_PTR(ptr) (heap_header_t* )(((char*) ptr - head_size ))
#define USER_START_FROM_REAL_PTR(ptr) (void*)((char*) ptr + head_size )
#define IS_USED(ptr) (ptr->used == 1)
#define IS_FREE(ptr) (ptr->used == 0)
#define SET_USED(ptr) (ptr->used = 1)
#define SET_FREE(ptr) (ptr->used = 0)
#define ALIGN_TO (16)
#define HEAP_SIZE(ptr) ((ptr->size) & ~(ALIGN_TO - 1))
#define ROUND_UP(x) (((x) + (ALIGN_TO - 1)) & ~(ALIGN_TO - 1))

void free(void *ptr)
{
    heap_header_t *header = REAL_START_FROM_USER_PTR(ptr);
    if (IS_FREE(header))
        return;
    SET_FREE(header);
    if (header->prev != NULL && IS_FREE(header->prev))
    {
        header->prev->next = header->next;
        header->prev->size += HEAP_SIZE(header);
        if (header->next != NULL)
        {
            header->next->prev = header->prev;
        }
        header = header->prev;
    }
    if (header->next != NULL && IS_FREE(header->next))
    {
        header->size += HEAP_SIZE(header->next);
        header->next = header->next->next;
        if (header->next != NULL)
        {
            header->next->prev = header;
        }
    }
}

void *malloc(size_t size)
{
    if (size <= 0)
        return NULL;
    size = ROUND_UP(size);
    heap_header_t *header = heap_start;
    while (header != NULL)
    {

        if (IS_USED(header))
        {
            header = header->next;
            continue;
        }
        if (HEAP_SIZE(header) >= size + head_size &&
            HEAP_SIZE(header) <= size + head_size * 2)
        {
            SET_USED(header);
        }
        if (HEAP_SIZE(header) >= size + head_size * 2)
        {
            heap_header_t *next = (heap_header_t *) ((char *) header + size + head_size);
            next->prev = header;
            next->next = header->next;
            SET_FREE(next);
            next->size = HEAP_SIZE(header) - size + head_size;
            header->next = next;
            header->size = size + head_size;
            SET_USED(header);
            return USER_START_FROM_REAL_PTR(header);
        }
        if (header->next == NULL)
            break;
        header = header->next;
    }
    header->next = end;
    end = sbrk((u64) ((char *) end + 4 MB));
    return malloc(size);
}

hidden void mini_crt_init_heap()
{
    heap_start = (heap_header_t *) sbrk(0);
    heap_start = (heap_header_t *) ROUND_UP((u64) heap_start);
    base = heap_start;
    end = sbrk((u64) ((char *) base + 4 MB));
#ifdef TEST
    char buf[16];
    if((u64)(((char*)end - (char*)base)) == (u64)(4 MB))
        puts( GREEN "finish memory allocation\n" RESET);
#endif
    heap_start->size = 4 MB;
    SET_FREE(heap_start);
    heap_start->next = NULL;
    heap_start->prev = NULL;

#ifdef TEST
    itoa((u64)base, buf, 16);
    puts(buf);
    putchar('\n');
    itoa((u64)end, buf, 16);
    puts(buf);
    putchar('\n');
#endif
}