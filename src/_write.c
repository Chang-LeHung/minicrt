
#include "mincrt.h"

size_t write(int fd, const char *buffer, size_t len) {
    // be careful with return val type, which is size_t
    // otherwise, return val will update partial last stack rbp
    // pushed on current stack
    size_t ret = 0;
    __asm__ __volatile__(
        "mov $1, %%rax\n\t"       // syscall number for 'write'
        "mov %1, %%edi\n\t"       // file descriptor to write to
        "mov %2, %%rsi\n\t"       // buffer containing data to write
        "mov %3, %%rdx\n\t"       // length of data to write
        "syscall\n\t"             // invoke the syscall
        "mov %%rax, %0\n\t"       // store the return value in 'ret'
        : "=m" (ret)
        : "m" (fd), "r" (buffer), "m" (len)
        : "rax", "edi", "rsi", "rdx", kernel_destroy_regs
        );
    return ret;
}