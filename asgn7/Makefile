CC=clang
CFLAGS=-Wall -Wextra -Werror -Wpedantic -g

all: encode decode

encode: trie.c word.c io.c encode.c
	$(CC) $(CFLAGS) trie.c word.c io.c encode.c -o encode -lm

decode: trie.c word.c io.c decode.c
	$(CC) $(CFLAGS) trie.c word.c io.c decode.c -o decode -lm

clean:
	rm -f encode decode

format:
	clang-format --style=file -i *.c *.h

test:
	./encode -v -i corpora/calgary/book1 -o book1.lzv
	./decode -v -i book1.lzv -o book1.txt
	diff corpora/calgary/book1 book1.txt