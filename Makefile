
SRCS := $(wildcard src/*.c)
OBJS := $(patsubst %.c,%.o,$(SRCS))
CFLAGS := -c -g -fno-stack-protector -nostdlib -fno-builtin -Wno-unused-variable -Wall -Werror
CC := gcc
lib: $(OBJS)
	ld -static -e _start $(OBJS) -o std

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

main: main.c $(OBJS)
	$(CC) $(CFLAGS) main.c -o main.o
	ld -static -e _start $(OBJS) main.o -o main

clean:
	rm -rf $(OBJS) std a.out main
