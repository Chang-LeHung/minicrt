
#ifndef STATIC_STL_MINICRT_IO_H
#define STATIC_STL_MINICRT_IO_H

#include "minicrt.h"
#include <stdarg.h>

typedef struct
{
    u32 fd;
    u32 pos;
    u32 size;
    char *buf;
    u8 mode;
} FILE;

#define CAN_WRITE(f) ((f)->mode & 1)
#define CAN_READ(f) ((f)->mode & 2)
#define ADD_READ(f) (f)->mode |= 2
#define ADD_WRITE(f) (f)->mode |= 1
#define REMOVE_READ(f) (f)->mode &= ~2
#define REMOVE_WRITE(f) (f)->mode &= ~1
#define OPEN(f) f->mode |= 4;
#define CLOSE(f) f->mode &= ~ 4;
#define IS_OPEN(f) ((f)->mode & 4)
#define EOF (-1)

FILE *stdin;
FILE *stdout;
FILE *stderr;

int vprintf(FILE *, const char *, va_list);

int fputc(char, FILE *);

int fputs(const char *, FILE *);

int printf(const char *, ...);

int putn(int);

hidden void minicrt_destroy_io();

#endif //STATIC_STL_MINICRT_IO_H
