
#include "minicrt.h"
#include <stdarg.h>

typedef struct {
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

FILE *stdin;
FILE *stdout;
FILE *stderr;


static inline void _fwrite(FILE *stream) {
    write((int) stream->fd, stream->buf, stream->pos);
    stream->pos = 0;
}

int fwrite(const void *buf, int size, FILE *stream) {
    if (!CAN_WRITE(stream)) return -1;
    if (size <= 0) return 0;
    u32 i = size + stream->pos;
    if (i > stream->size) {
        _fwrite(stream);
        i = size;
    }
    if (i > stream->size) {
        write((int) stream->fd, buf, size);
        return 0;
    } else {
        strcpy_n(stream->buf + stream->pos, buf, size);
        return 0;
    }
}

int vprintf(const char *format, va_list args) {
    int translating = 0;
    int ret = 0;
    const char *p = NULL;
    for (p = format; *p; p++) {
        if (*p == '%') {
            if (!translating) {
                translating = 1;
            } else {
            }

        }
    }
    return 0;
}

int printf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    return 0;
}