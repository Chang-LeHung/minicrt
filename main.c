
#include "src/minicrt.h"
#include "src/minicrt_io.h"


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
    char *buf = malloc(1<<10);
    itoa(stdout->size, buf, 10);
    buf[4] = '\n';
    buf[5] = '\0';
    puts(buf);
    printf("%s %d\n", "\nhuchang", 18);
    printf("%f \t %lf\t%d\n", 12.2, 12123.12312312312321321, 2123234);
//    float data = 3.14f;
//    printf("%lf\n", data);
    return 0;
}