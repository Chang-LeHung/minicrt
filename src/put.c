
#include "mincrt.h"

int putchar(char c) {
    write(STDOUT, &c, 1);
    return c;
}

int puts(const char *s) {
    write(STDOUT, s, strlen(s));
    return 0;
}