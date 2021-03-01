CC=clang
CFLAGS=-Wall -Wextra -Werror -Wpedantic -g

all:
	$(CC) $(CFLAGS) bf.c bv.c hash.c ll.c node.c parser.c speck.c banhammer.c -o banhammer

clean:
	rm -f banhammer

format:
	clang-format --style=file -i *.c *.h
