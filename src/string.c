
#include "minicrt.h"


size_t strlen(const char *s) {
    size_t i = 0;
    while (s[i]) i++;
    return i;
}

char *itoa(int n, char *str, int radix) {
    static char digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";
    char *p = str;
    char *head = str;
    if (!p || radix < 2 || radix > 36) return NULL;

    if (radix != 10 && n < 0) {
        return p;
    }
    if (n == 0) {
        *p++ = '0';
        *p = '\0';
        return p;
    }
    while (n) {
        int rem = n % radix;
        *p++ = digits[rem];
        n /= radix;
    }
    *p = '\0';
    for (--p; head < p; ++head, --p) {
        char tmp = *head;
        *head = *p;
        *p = tmp;
    }
    return str;
}

int strcmp(const char *s1, const char *s2) {
    while (*s1 && *s1 == *s2) {
        s1++;
        s2++;
    }
    return *(const unsigned char *) s1 - *(const unsigned char *) s2;
}

char *strcpy(char *dest, const char *src) {
    char *ret = dest;
    while (*src) {
        *dest++ = *src++;
    }
    *dest = 0;
    return ret;
}