
#include "src/mincrt.h"

int main(int argc, char** argv, char** envp) {
    puts("Hello World, Awesome\n");
    puts("argv as follows:\n");
    for(int i = 0; i < argc; i++) {
        puts(argv[i]);
        putchar('\n');
    }
//    puts("envp as follows:\n");
//    for(int i = 0; envp[i]; i++) {
//        puts(envp[i]);
//        putchar('\n');
//    }
    char buf[1<<4];
    itoa(1<<10, buf, 10);
    buf[4] = '\n';
    buf[5] = '\0';
    puts(buf);
    return 0;
}