CC=clang
CFLAGS=-Wall -Wextra -Werror -Wpedantic -g

all:
	$(CC) $(CFLAGS) bubble.c shell.c quick.c heap.c stack.c set.c sorting.c -o sorting

clean:
	rm -f sorting

format:
	clang-format --style=file -i *.c *.h
