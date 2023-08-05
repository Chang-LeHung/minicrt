#include <sys/syscall.h>
#include "mincrt.h"

void _exit(int status) {
    __asm__ __volatile__(
        "movq %0, %%rdi\n"   // Move 'status' to rdi
        "movq %1, %%rax\n"   // Move SYS_exit (60) to rax
        "syscall\n"         // Invoke the syscall
        "hlt\n"             // Halt the processor (optional)
        :: "r"((long)status), "r"((long)SYS_exit)
        : "rdi", "rax", kernel_destroy_regs
        );
}
