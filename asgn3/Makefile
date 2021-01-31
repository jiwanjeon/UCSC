CC = clang
CCFLAGS = -Wall -Werror -Wextra -Wpedantic -Wshadow -std=c17

all: life

life: life.o univers.o
	$(CC) $(CCFLAGS) life.o universe.o -o life -lncurses

life.o:
	$(CC) $(CCFLAGS) -c life.c -o life.o

univers.o:
	$(CC) $(CCFLAGS) -c universe.c -o universe.o

clean:
	rm -f life life.o universe.o

format:
	clang-format --style=file -i life.c universe.c universe.h
