
#ifndef STATIC_STL_MINICRT_IO_H
#define STATIC_STL_MINICRT_IO_H
#include "minicrt.h"
typedef struct {
    u32 fd;
    u32 pos;
    u32 size;
    char *buf;
    u8 mode;
} FILE;

int fwrite(const void *, int, FILE *);
extern FILE *stderr;
extern FILE *stdin;
extern FILE *stdout;

#define CAN_WRITE(f) ((f)->mode & 1)
#define CAN_READ(f) ((f)->mode & 2)
#define ADD_READ(f) (f)->mode |= 2
#define ADD_WRITE(f) (f)->mode |= 1
#endif //STATIC_STL_MINICRT_IO_H
