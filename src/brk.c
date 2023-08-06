
#include "minicrt.h"
#include "mini_syscall.h"
#include <syscall.h>

hidden void *sbrk(u64 end_data_segment)
{
    return (void *) __syscall1(SYS_brk, (long) end_data_segment);
}

