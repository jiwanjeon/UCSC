#include "code.h"
#include "io.h"
#include "trie.h"
#include "word.h"

#include <fcntl.h>
#include <getopt.h>
#include <inttypes.h>
#include <math.h>
#include <memory.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

static uint8_t bitlength(uint16_t word) {
    return word ? (uint8_t) log2(word) + 1 : 1;
}

/*
Your encode program must support the following getopt() options:
• -v : Print compression statistics to stderr.
• -i <input> : Specify input to compress (stdin by default)
• -o <output> : Specify output of compressed input (stdout by default)
*/

/*
1. Open infile with open(). If an error occurs, print a helpful message and exit
with a status code indicating that an error occurred. infile should be stdin if
an input ﬁle wasn’t speciﬁed.

2. The ﬁrst thing in outfile must be the ﬁle header, as deﬁned in the ﬁle io.h.
The magic number in the header must be 0xBAADBAAC. The ﬁle size and the
protection bit mask you will obtain using fstat(). See the man page on it for
details.

3. Open outfile using open(). The permissions for outfile should match the
protection bits as set in your ﬁle header. Any errors with opening outfile
should be handled like with infile. outfile should be stdout if an output ﬁle
wasn’t speciﬁed.

4. Write the ﬁlled out ﬁle header to outfile using write_header(). This means
writing out the struct itself to the ﬁle, as described in the comment block of
the function.

5. Create a trie. The trie initially has no children and consists solely of the
root. The code stored by this root trie node should be EMPTY_CODE to denote the
empty word. You will need to make a copy of the root node and use the copy to
step through the trie to check for existing preﬁxes. This root node copy will be
referred to as curr_node. The reason a copy is needed is that you will
eventually need to reset whatever trie node you’ve stepped to back to the top of
the trie, so using a copy lets you use the root node as a base to return to.

6. You will need a monotonic counter to keep track of the next available code.
This counter should start at START_CODE, as deﬁned in the supplied code.h ﬁle.
The counter should be a uint16_t since the codes used are unsigned 16-bit
integers. This will be referred to as next_code.

7. You will also need two variables to keep track of the previous trie node and
previously read symbol. We will refer to these as prev_node and prev_sym,
respectively.

8. Use read_sym() in a loop to read in all the symbols from infile. Your loop
should break when read_sym() returns false. For each symbol read in, call it
curr_sym, perform the following:
(a)
Set next_node to be trie_step(curr_node,
curr_sym), stepping down from the current node to the currently read symbol.
(b)
If next_node is not NULL, that means we have seen the current preﬁx. Set
prev_node to be curr_node and then curr_node to be next_node.
(c)
Else, since
next_node is NULL, we know we have not encountered the current preﬁx. We write
the pair (curr_node->code, curr_sym), where the bit-length of the written code
is the bit-length of next_code. We now add the current preﬁx to the trie. Let
curr_node->children[curr_sym] be a new trie node whose code is next_code. Reset
curr_node to point at the root of the trie and increment the value of next_code.
(d)
Check if next_code is equal to MAX_CODE. If it is, use trie_reset() to
reset the trie to just having the root node. This reset is necessary since we
have a ﬁnite number of codes.
(e)
Update prev_sym to be curr_sym.

9. After processing all the characters in infile, check if curr_node points to
the root trie node. If it does not, it means we were still matching a preﬁx.
Write the pair (prev_node->code, prev_sym). The bit-length of the code written
should be the bit-length of next_code. Make sure to increment next_code and that
it stays within the limit of MAX_CODE. Hint: use the modulo operator.

10. Write the pair (STOP_CODE, 0) to signal the end of compressed output. Again,
the bit-length of code written should be the bit-length of next_code.

11. Make sure to use flush_pairs() to ﬂush any unwritten, buffered pairs.
Remember, calls to write_pair() end up buffering them under the hood. So, we
have to remember to ﬂush the contents of our buffer.

12. Use close() to close infile and outfile.

*/

int main(int argc, char *argv[]) {
    bool verbose = false;
    int infile = STDIN_FILENO;
    int outfile = STDOUT_FILENO;
    char *infilename = NULL;
    char *outfilename = NULL;
    struct stat buf;
    int opt;

    // parse command line
    while ((opt = getopt(argc, argv, "vi:o:")) != -1) {
        switch (opt) {
        case 'v': verbose = true; break;
        case 'i': infilename = optarg; break;
        case 'o': outfilename = optarg; break;
        default: fprintf(stderr, "Unknown option: %c\n", opt); break;
        }
    }

    if (infilename) {
        infile = open(infilename, O_RDONLY);
        if (infile < 0) {
            fprintf(stderr, "Can't read input file.\n");
            return -1;
        }
    }

    memset(&buf, 0, sizeof(buf));
    fstat(infile, &buf);

    if (outfilename) {
        outfile = open(outfilename, O_WRONLY | O_CREAT | O_TRUNC);
        if (outfile < 0) {
            fprintf(stderr, "Can't write output file.\n");
            close(infile);
            return -1;
        }
    }

    fchmod(outfile, buf.st_mode);

    FileHeader fh;
    fh.magic = MAGIC;
    fh.protection = buf.st_mode;

    write_header(outfile, &fh);

    TrieNode *root = trie_create();
    TrieNode *curr_node = root;
    TrieNode *prev_node = NULL;
    uint8_t prev_sym = 0;
    uint8_t curr_sym = 0;
    uint16_t next_code = START_CODE;

    while (read_sym(infile, &curr_sym)) {
        TrieNode *next_node = trie_step(curr_node, curr_sym);
        if (next_node != NULL) {
            prev_node = curr_node;
            curr_node = next_node;
        } else {
            // fprintf(stderr, "pair {%d (%d), %c}\n", curr_node->code,
            // bitlength(next_code), curr_sym);
            write_pair(outfile, curr_node->code, curr_sym, bitlength(next_code));
            curr_node->children[curr_sym] = trie_node_create(next_code);
            curr_node = root;
            next_code++;
        }
        if (next_code == MAX_CODE) {
            trie_reset(root);
            curr_node = root;
            next_code = START_CODE;
        }

        prev_sym = curr_sym;
    }

    if (curr_node != root) {
        // fprintf(stderr, "pair {%d (%d), %c}\n", prev_node->code,
        // bitlength(next_code), prev_sym);
        write_pair(outfile, prev_node->code, prev_sym, bitlength(next_code));
        next_code = (next_code + 1) % MAX_CODE;
    }

    write_pair(outfile, STOP_CODE, 0, bitlength(next_code));
    flush_pairs(outfile);

    if (verbose) {
        fstat(outfile, &buf);
        off_t compressed_size = buf.st_size;
        printf("Compressed file size: %ld bytes\n", compressed_size);

        fstat(infile, &buf);
        off_t uncompressed_size = buf.st_size;
        printf("Uncompressed file size: %ld bytes\n", uncompressed_size);
        float space_saving = (1.0 - (float) compressed_size / (float) uncompressed_size) * 100;
        printf("Space saving: %2.2f%%\n", space_saving);
    }

    close(infile);
    close(outfile);

    trie_delete(root);
    trie_node_delete(root);

    return 0;
}
