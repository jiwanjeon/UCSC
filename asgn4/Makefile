CC = clang
CFLAGS = -Wall -Werror -Wextra -Wpedantic


all: hamming
	
hamming: hamming.o generator.o decoder.o bm.o error.o
	   $(CC) $(CFLAGS) hamming.o generator.o decoder.o bm.o error.o -o hamming

hamming.o: hamming.c
	   $(CC) $(CFLAGS) -c hamming.c

generator.o: genertor.c
	   $(CC) $(CFLAGS) -c generator.c 

decoder.o: decoder.c
     	   $(CC) $(CFLAGS) -c decoder.c

bm.o: bm.c
	   $(CC) $(CFLAGS) -c bm.c

error.o: error.c
	   $(CC) $(CFLAGS) -c error.c 

clean:
	   rm -f hamming hamming.o generator.o decoder.o bm.o error.o

format:
	   clang-format --style=file -i hamming.c generator.c decoder.c bm.c bm.h
	
