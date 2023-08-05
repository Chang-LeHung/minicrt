
#include "mincrt.h"

static int dummy(int argc, char **argv, char **envp) {
    puts(RED "default main function, please preprae your own main function\n" RESET);
    return 0;
}

weak_alias(dummy, main);

static void minicrt_init(int, char **, char **);

// this is the entry point of the program
void _start() {
    // push %rbp        rsp will be decreased by 8
    // movq %rsp, %rbp
    // so argc position is %rbp
    // you could use assembly as well
    u64 rbp;
    __asm__ __volatile__ (
        "movq %%rbp, %0"
        : "=r" (rbp)
        : /* no input */
        : "memory"
        );

    int argc = *(int *) (rbp + 8);
    char **argv = (char **) (rbp + 16);
    char **envp = argv + argc + 1;
    minicrt_init(argc, argv, envp);
}

static void minicrt_init(int argc, char **argv, char **envp) {
//    minicrt_init_stage2(argc, argv, envp);
    _exit(main(argc, argv, envp));
}

// rsp 指向 initial stack frame 的低地址的原因是因为后面进行函数调用的时候可以读写内存了
// 否则如果 rsp 在高地址的话，那么如果进行函数调用后面的函数写内存的话将会覆盖这些内存
// _exit(write(1, "Hello world!\n", 13)); 变成
// int ret = write(1, "Hello world!\n", 13);
// _exit(ret);
// 将会产生 segmentation fault
//  40013b:       48 8d 35 1a 00 00 00    lea    0x1a(%rip),%rsi        # 40015c <_start+0x5a>
//  400142:       bf 01 00 00 00          mov    $0x1,%edi
//  400147:       e8 82 ff ff ff          callq  4000ce <write>
//  40014c:       89 45 dc                mov    %eax,-0x24(%rbp)
//  40014f:       8b 45 dc                mov    -0x24(%rbp),%eax # 在这里产生 segmentation fault
//  400152:       89 c7                   mov    %eax,%edi
//  400154:       e8 57 ff ff ff          callq  4000b0 <_exit>
//  400159:       90                      nop
//  40015a:       c9                      leaveq
//  40015b:       c3                      retq
