
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
#define REMOVE_READ(f) (f)->mode &= ~2
#define REMOVE_WRITE(f) (f)->mode &= ~1
#define OPEN(f) f->mode |= 4;
#define CLOSE(f) f->mode &= ~ 4;
#define IS_OPEN(f) ((f)->mode & 4)
#define EOF (-1)

FILE *stdin;
FILE *stdout;
FILE *stderr;

int fclose(FILE *stream);

hidden void minicrt_init_io() {
    stdin = malloc(sizeof(FILE));
    stdin->fd = 0;
    stdin->size = 128;
    stdin->buf = malloc(stdin->size);

    stdout = malloc(sizeof(FILE));
    stdout->fd = 1;
    stdout->size = 128;
    stdout->buf = malloc(stdout->size);

    stderr = malloc(sizeof(FILE));
    stderr->fd = 2;
    stderr->size = 128;
    stderr->buf = malloc(stderr->size);

    OPEN(stdin);
    OPEN(stdout);
    OPEN(stderr);
    ADD_READ(stdin);
    ADD_WRITE(stdout);
    ADD_WRITE(stderr);
}

hidden void minicrt_destroy_io() {
    fclose(stdout);
    fclose(stderr);
    fclose(stdin);
}


static inline int _fwrite(FILE *stream) {
    int ret = write((int) stream->fd, stream->buf, stream->pos);
    if (ret >= 0) {
        stream->pos = 0;
        return ret;
    }
    return -1;
}

int fwrite(const void *buf, int size, FILE *stream) {
    if (!CAN_WRITE(stream) || !IS_OPEN(stream)) return -1;
    if (size <= 0) return 0;
    u32 i = size + stream->pos;
    if (i > stream->size) {
        _fwrite(stream);
        write((int) stream->fd, buf, size);
    } else {
        if (strcpy_n(stream->buf + stream->pos, buf, size)) {
            write((int) stream->fd, buf, size + stream->pos);
            stream->pos = 0;
        } else
            stream->pos += size;
    }
    return 0;
}

int putn(int n) {
    char buf[16];
    itoa(n, buf, 10);
    puts(buf);
    putchar('\n');
    return (int) strlen(buf) + 1;
}

int fputc(char c, FILE *stream) {
    return fwrite(&c, 1, stream);
}

int fputs(const char *buf, FILE *stream) {
    u32 size = strlen(buf);
    return fwrite(buf, (int) size, stream);
}

int fclose(FILE *stream) {
    if (stream == NULL || !IS_OPEN(stream)) return -1;
    if (stream->pos != 0)
        _fwrite(stream);
    CLOSE(stream);
    free(stream);
    return 0;
}

void reverseString(char str[], int length) {
    int start = 0;
    int end = length - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

static int int_to_string(long num, char str[], int precision) {
    int index = 0;
    int isNegative = 0;

    // Handle negative numbers
    if (num < 0) {
        isNegative = 1;
        num = -num;
    }

    // Convert integer part to string
    while (num != 0) {
        str[index++] = num % 10 + '0';
        num /= 10;
    }

    // Add a negative sign if necessary
    if (isNegative) {
        str[index++] = '-';
    }

    // Reverse the string
    reverseString(str, index);

    return index;
}

static void float_to_string(float num, char str[], int precision) {
    // Handle 0.0 case
    if (num == 0) {
        str[0] = '0';
        str[1] = '\0';
        return;
    }

    // Extract the integer part
    int integerPart = (int) num;

    // Convert the integer part to a string
    int integerLength = int_to_string(integerPart, str, precision);

    // Append decimal point
    str[integerLength++] = '.';

    // Extract the decimal part
    double decimalPart = num - integerPart;

    // Convert the decimal part to a string
    int i = 0;
    if (precision == -1) {
        while (1) {
            decimalPart *= 10;
            int digit = (int) decimalPart;
            str[integerLength + i++] = digit + '0';
            if (!digit) break;
            decimalPart -= digit;
        }
    } else {
        while (i < precision) {
            decimalPart *= 10;
            int digit = (int) decimalPart;
            str[integerLength + i++] = digit + '0';
            decimalPart -= digit;
        }
    }
    // Null-terminate the string
    str[integerLength + i] = '\0';
}

static void double_to_string(double num, char str[], int precision) {
    // Handle 0.0 case
    if (num == 0) {
        str[0] = '0';
        str[1] = '\0';
        return;
    }

    // Extract the integer part
    int integerPart = (int) num;

    // Convert the integer part to a string
    int integerLength = int_to_string(integerPart, str, precision);

    // Append decimal point
    str[integerLength++] = '.';

    // Extract the decimal part
    double decimalPart = num - integerPart;

    // Convert the decimal part to a string
    int i = 0;
    if (precision == -1) {
        while (1) {
            decimalPart *= 10;
            int digit = (int) decimalPart;
            str[integerLength + i++] = digit + '0';
            if (!digit) break;
            decimalPart -= digit;
        }
    } else {
        while (i < precision) {
            decimalPart *= 10;
            int digit = (int) decimalPart;
            str[integerLength + i++] = digit + '0';
            decimalPart -= digit;
        }
    }
    // Null-terminate the string
    str[integerLength + i] = '\0';
}

int vprintf(FILE *file, const char *format, va_list args) {
    int translating = 0;
    char from_l = 0;
    int ret = 0;
    const char *p = NULL;
    for (p = format; *p; p++) {
        switch (*p) {
            case '%':
                if (!translating) {
                    translating = 1;
                } else {
                    if (fputc('%', file) < 0)
                        return EOF;
                    ret++;
                    translating = 0;
                }
                break;
            case 'l':
                if (translating) {
                    from_l = 1;
                    if (*(p + 1) == 'f') {
                        goto f;
                    }
                } else {
                    if (fputc('l', file) < 0) return EOF;
                    ret++;
                    break;
                }
            case 'd':
                if (translating) {
                    char buf[16];
                    translating = 0;
                    if (from_l) {
                        itoa(va_arg(args, long), buf, 10);
                        from_l = 0;
                    } else {
                        itoa(va_arg(args, int), buf, 10);
                    }
                    if (fputs(buf, file) < 0)
                        return EOF;
                    ret += (int) strlen(buf);
                } else {
                    if (fputc('d', file) < 0) return EOF;
                    ret++;
                }
                break;
            case 's':
                if (translating) {
                    const char *str = va_arg(args, const char*);
                    translating = 0; // reset state
                    if (fputs(str, file) < 0) return EOF;
                    ret += (int) strlen(str);
                } else {
                    if (fputc('s', file) < 0) return EOF;
                    ret++;
                }
                break;
            case 'f':
            f:
                if (translating) {
                    char buf[32];
                    if (from_l) {
                        double_to_string(va_arg(args, double), buf, -1);
                    } else {
                        float_to_string((float) va_arg(args, double), buf, -1);
                    }
                    from_l = 0;
                    translating = 0;
                    if (fputs(buf, file) < 0) return EOF;
                    ret += (int) strlen(buf);
                } else {
                    if (fputc('f', file) < 0) return EOF;
                    ret++;
                }
                p++;
                break;
            default:
                translating = 0;
                if (fputc(*p, file) < 0) return EOF;
                ret++;
        }
    }
    return ret;
}

// just for rsp rbp align to 16 for using sse instructions
__attribute__((force_align_arg_pointer)) int printf(const char *restrict format, ...) {
    int ret;
    va_list args;
    va_start(args, format);
    ret = vprintf(stdout, format, args);
    va_end(args);
    return ret;
}
