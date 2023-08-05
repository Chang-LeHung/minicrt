
#ifndef STATIC_STL_MINICRT_H
#define STATIC_STL_MINICRT_H

typedef unsigned long u64;
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;
typedef long long i64;
typedef int i32;
typedef short i16;
typedef char i8;
typedef float f32;
typedef double f64;
typedef u64 size_t;

#define hidden __attribute__((visibility("hidden")))

void _exit(int);

size_t write(int, const char *, size_t);

#pragma GCC diagnostic ignored "-Wbuiltin-declaration-mismatch"

int putchar(char c);

size_t strlen(const char *s);

int puts(const char *s);

char *itoa(int, char *, int);

char *strcpy(char *, const char *);

int strcmp(const char *, const char *);

void free(void *);

#define kernel_destroy_regs "rcx", "r11", "memory"
#define STDIN 0
#define STDOUT 1
#define STDERR 2
#define weak_alias(old, new) \
    extern __typeof(old) new __attribute__((__weak__, __alias__(#old)))
#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0
#define assert(condition) \
    do { \
        if (!(condition)) { \
            fprintf(stderr, "Assertion failed: %s, file: %s, line: %d\n", \
                #condition, __FILE__, __LINE__); \
            _exit(EXIT_FAILURE); \
        } \
    } while (0)
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define BLUE "\x1b[34m"
#define RESET "\x1b[0m"
#define NULL 0
#define KB *(1<<10)
#define PAGE_SIZE  4 KB
#endif //STATIC_STL_MINICRT_H
