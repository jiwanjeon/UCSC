CC=clang
CFLAGS=-Wall -Wextra -Werror -Wpedantic

all: gen dec err

gen: generator.c bm.o hamming.o
	$(CC) $(CFLAGS) generator.c bm.o hamming.o -o gen

dec: decoder.c bm.o hamming.o
	$(CC) $(CFLAGS) decoder.c bm.o hamming.o -o dec

err: error.c bm.o hamming.o
	$(CC) $(CFLAGS) error.c bm.o hamming.o -o err

hamming.o: hamming.c hamming.h
	$(CC) $(CFLAGS) -c hamming.c -o hamming.o

bm.o: bm.c bm.h
	$(CC) $(CFLAGS) -c bm.c -o bm.o

clean:
	rm -f gen dec err *.o

format:
	clang-format --style=file -i *.c *.h
