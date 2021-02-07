CC = clang
CFLAGS = -Wall -Werror -Wextra -Wpedantic


all: hamming
	
hamming: hamming.o generator.o decoder.o bm.o error.o
	   $(CC) $(CFLAGS) hamming.o generator.o decoder.o bm.o error.o -o hamming

hamming.o:
	   $(CC) $(CFLAGS) -c hamming.c -o hamming.o

generator.o:
	   $(CC) $(CFLAGS) -c generator.c -o generator.o

decoder.o:
     	   $(CC) $(CFLAGS) -c decoer.c -o decoder.o

bm.o:
	   $(CC) $(CFLAGS) -c bm.c -o bm.o

error.o:
	   $(CC) $(CFLAGS) -c error.c -o error.o

clean:
	   rm -f hamming hamming.o generator.o decoder.o bm.o error.o

format:
	   clang-format --style=file -i hamming.c generator.c decoder.c
	