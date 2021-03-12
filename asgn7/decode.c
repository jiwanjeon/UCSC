#include "code.h"
#include "io.h"
#include "trie.h"
#include "word.h"

#include <fcntl.h>
#include <getopt.h>
#include <inttypes.h>
#include <math.h>
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
Your decode program must support the following getopt() options:
• -v : Print decompression statistics to stderr.
• -i <input> : Specify input to decompress (stdin by default)
• -o <output> : Specify output of decompressed input (stdout by default)
*/

/*
1. Open infile with open(). If an error occurs, print a helpful message and exit
with a status code indicating that an error occurred. infile should be stdin if
an input ﬁle wasn’t speciﬁed.

2. Read in the ﬁle header with read_header(), which also veriﬁes the magic
number. If the magic number is veriﬁed then decompression is good to go and you
now have a header which contains the original protection bit mask.

3. Open outfile using open(). The permissions for outfile should match the
protection bits as set in your ﬁle header that you just read. Any errors with
opening outfile should be handled like with infile. outfile should be stdout if
an output ﬁle wasn’t speciﬁed.

4. Create a new word table with wt_create() and make sure each of its entries
are set to NULL. Initialize the table to have just the empty word, a word of
length 0, at the index EMPTY_CODE. We will refer to this table as table.

5. You will need two uint16_t to keep track of the current code and next code.
These will be referred to as curr_code and next_code, respectively. next_code
should be initialized as START_CODE and functions exactly the same as the
monotonic counter used during compression, which was also called next_code.

6. Use read_pair() in a loop to read all the pairs from infile. We will refer to
the code and symbol from each read pair as curr_code and curr_sym, respectively.
The bit-length of the code to read is the bit-length of next_code. The loop
breaks when the code read is STOP_CODE. For each read pair, perform the
following:
(a)
As seen in the decompression example, we will need to append the
read symbol with the word de- noted by the read code and add the result to table
at the index next_code. The word denoted by the read code is stored in
table[curr_code]. We will append table[curr_code] and curr_sym using
word_append_sym().
(b)
Write the word that we just constructed and added to the table with
write_word(). This word should have been stored in table[next_code].
(c)
Increment next_code and check if it equals MAX_CODE. If it has, reset the table
using wt_reset() and set next_code to be START_CODE. This mimics the resetting
of the trie during compression.

7. Flush any buffered words using flush_words(). Like with write_pair(),
write_word() buffers words under the hood, so we have to remember to ﬂush the
contents of our buffer.

8. Close infile and outfile with close().

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

    fstat(infile, &buf);

    FileHeader fh;
    read_header(infile, &fh);
    if (fh.magic != MAGIC) {
        fprintf(stderr, "Invalid magic number in input file.\n");
        close(infile);
        return -1;
    }

    if (fh.protection != buf.st_mode) {
        fprintf(stderr, "Invalid protection bits in input file.\n");
        close(infile);
        return -1;
    }

    if (outfilename) {
        outfile = open(outfilename, O_WRONLY | O_CREAT | O_TRUNC);
        if (outfile < 0) {
            fprintf(stderr, "Can't write output file.\n");
            close(infile);
            return -1;
        }
    }

    fchmod(outfile, buf.st_mode);

    WordTable *table = wt_create();
    uint16_t curr_code = 0;
    uint8_t curr_sym = 0;
    uint16_t next_code = START_CODE;

    while (read_pair(infile, &curr_code, &curr_sym, bitlength(next_code))) {
        // fprintf(stderr, "pair {%d (%d), %d}\n", curr_code, bitlength(next_code),
        // curr_sym);
        table[next_code] = word_append_sym(table[curr_code], curr_sym);
        write_word(outfile, table[next_code]);
        ++next_code;
        if (next_code == MAX_CODE) {
            next_code = START_CODE;
            wt_reset(table);
        }
    }

    flush_words(outfile);

    if (verbose) {
        fstat(infile, &buf);
        off_t compressed_size = buf.st_size;
        printf("Compressed file size: %ld bytes\n", compressed_size);

        fstat(outfile, &buf);
        off_t uncompressed_size = buf.st_size;
        printf("Uncompressed file size: %ld bytes\n", uncompressed_size);
        float space_saving = (1.0 - (float) compressed_size / (float) uncompressed_size) * 100;
        printf("Space saving: %2.2f%%\n", space_saving);
    }

    close(infile);
    close(outfile);

    wt_delete(table);

    return 0;
}
