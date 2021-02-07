CC = clang
CFLAGS = -Wall -Werror -Wextra -Wpedantic


all: hamming
	
hamming: hamming.o generator.o decoder.o bm.o error.o
	   $(CC) $(CCFLAGS) hamming.o generator.o decoder.o bm.o error.o -o hamming

hamming.o:
	   $(CC) $(CCFLAGS) -c hamming.c -o hamming.o

generator.o:
	   $(CC) $(CCFLAGS) -c generator.c -o generator.o

decoder.o:
     	   $(CC) $(CCFLAGS) -c decoder.c -o decoder.o

bm.o:
	   $(CC) $(CCFLAGS) -c bm.c -o bm.o

error.o:
	   $(CC) $(CCFLAGS) -c error.c -o error.o

clean:
	   rm -f hamming hamming.o generator.o decoder.o bm.o error.o

format:
	   clang-format --style=file -i hamming.c generator.c decoder.c bm.c bm.h
	
