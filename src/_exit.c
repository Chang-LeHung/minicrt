#include <sys/syscall.h>
#include "minicrt.h"
#include "minicrt_io.h"
#include "mini_syscall.h"

void _exit(int status)
{
//    __asm__ __volatile__(
//        "movq %0, %%rdi\n"   // Move 'status' to rdi
//        "movq %1, %%rax\n"   // Move SYS_exit (60) to rax
//        "syscall\n"         // Invoke the syscall
//        "hlt\n"             // Halt the processor (optional)
//        ::"r"((long) status), "r"((long) SYS_exit)
//        : "rdi", "rax", kernel_destroy_regs
//        );
    __syscall1(SYS_exit, 0);
}

void exit(int status)
{
    minicrt_destroy_io();
    _exit(status);
}