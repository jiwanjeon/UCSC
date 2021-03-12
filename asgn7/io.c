#include "io.h"

#include "code.h"
#include "endian.h"

#include <assert.h>
#include <errno.h>
#include <memory.h>
#include <stdlib.h>
#include <unistd.h>

static uint8_t pairs_buffer[BLOCK] = { 0 };
static uint32_t pairs_buffer_size = 0;
static uint32_t pairs_buffer_index = 0;

static uint8_t char_buffer[BLOCK] = { 0 };
static uint32_t char_buffer_size = 0;
static uint32_t char_buffer_index = 0;

// function to extract a bit from a byte
static uint8_t get_bit_from_byte(uint8_t byte, uint8_t i) {
    return byte & (1 << (i)) ? 1 : 0;
}

// function to extract a bit from a word
static uint8_t get_bit_from_word(uint16_t word, uint8_t i) {
    return word & (1 << (i)) ? 1 : 0;
}

// function to set a bit in a byte
static void set_bit_to_byte(uint8_t *byte, uint8_t i) {
    *byte |= (1 << (i));
}

// function to set a bit in a word
static void set_bit_to_word(uint16_t *word, uint8_t i) {
    *word |= (1 << (i));
}

// function to extract a bit from a byte
static uint8_t get_bit(uint8_t byte, uint8_t i) {
    return byte & (1 << (i)) ? 1 : 0;
}

// function to set a bit in a byte
static void set_bit(uint8_t *byte, uint8_t i) {
    *byte |= (1 << (i));
}

/*
All reads and writes in this program must be done using the system calls read()
and write(), which means that you must use the system calls open() and close()
to get your ﬁle descriptors. As stated earlier, all reads and writes must be
performed in efﬁcient blocks of 4KB. You will want to use two static 4KB uint8_t
arrays to serve as buffers: one to store binary pairs and the other to store
characters. Each of these buffers should have an index, or a variable, to keep
track of the current byte or bit that has been processed.
*/

/*
This will be a useful helper function to perform reads. As you may know,
the read() syscall does not always guarantee that it will read all the
bytes speciﬁed. For example, a call could be issued to read a block of bytes,
but it might only read half a block. So, we write a wrapper function to loop
calls to read() until we have either read all the bytes that were speciﬁed
(to_read), or there are no more bytes to read. The number of bytes that were
read are returned. You should use this function whenever you need to perform
a read.
*/
int read_bytes(int infile, uint8_t *buf, int to_read) {
    int curr_read, total_read = 0;
    while (total_read < to_read) {
        curr_read = read(infile, buf + total_read, to_read - total_read);

        if (curr_read == -1 && errno == EINTR) {
            continue;
        }

        if (curr_read <= 0) {
            break;
        }

        total_read += curr_read;
    }
    return total_read;
}

/*
This function is very much the same as read_bytes(), except that it is for
looping calls to write(). As you may imagine, write() isn’t guaranteed to write
out all the speciﬁed bytes (to_write), and so we loop until we have either
written out all the bytes speciﬁed, or no bytes were written. The number of
bytes written out is returned. You should use this function whenever you need to
perform a write.
*/
int write_bytes(int outfile, uint8_t *buf, int to_write) {
    int curr_write, total_write = 0;
    while (total_write < to_write) {
        curr_write = write(outfile, buf + total_write, to_write - total_write);

        if (curr_write == 0 || (curr_write == -1 && errno == EINTR)) {
            continue;
        }

        if (curr_write < 0) {
            break;
        }

        total_write += curr_write;
    }
    return total_write;
}

/*
This reads in sizeof(FileHeader) bytes from the input file. These bytes are read
into the supplied header. Endi- anness is swapped if byte order isn’t little
endian. Along with reading the header, it must verify the magic number.
*/
void read_header(int infile, FileHeader *header) {
    read_bytes(infile, (uint8_t *) &header->magic, sizeof(header->magic));
    read_bytes(infile, (uint8_t *) &header->protection, sizeof(header->protection));
    if (big_endian()) {
        header->magic = swap32(header->magic);
        header->protection = swap32(header->protection);
    }
}

/*
Writes sizeof(FileHeader) bytes to the output file. These bytes are from the
supplied header. Endianness is swapped if byte order isn’t little endian.
*/
void write_header(int outfile, FileHeader *header) {
    // FileHeader h = *header;
    if (big_endian()) {
        header->magic = swap32(header->magic);
        header->protection = swap32(header->protection);
    }
    write_bytes(outfile, (uint8_t *) &header->magic, sizeof(header->magic));
    write_bytes(outfile, (uint8_t *) &header->protection, sizeof(header->protection));
}

/*
An index keeps track of the currently read symbol in the buffer. Once all
symbols are processed , another block is read. If less than a block is read ,
the end of the buffer is updated. Returns true if there are symbols to be read ,
false otherwise.
*/
bool read_sym(int infile, uint8_t *sym) {
    if (char_buffer_index == char_buffer_size) {
        char_buffer_size = read_bytes(infile, char_buffer, BLOCK);
        char_buffer_index = 0;
    }
    if (char_buffer_index == char_buffer_size) {
        return false;
    }
    *sym = char_buffer[char_buffer_index++];
    return true;
}

/*
“Writes” a pair to outfile. In reality, the pair is buffered. A pair is
comprised of a code and a symbol. The bits of the code are buffered first,
starting from the LSB. The bits of the symbol are buffered next, also starting
from the LSB. The code buffered has a bit-length of bitlen. The buffer is
written out whenever it is filled.
*/
void write_pair(int outfile, uint16_t code, uint8_t sym, int bitlen) {

    // copy code bits
    for (int i = 0; i < bitlen; ++i) {
        uint8_t bit = get_bit_from_word(code, i);
        if (bit) {
            set_bit(&pairs_buffer[pairs_buffer_index / 8], pairs_buffer_index % 8);
        }
        pairs_buffer_index++;
        if (pairs_buffer_index == BLOCK * 8) {
            flush_pairs(outfile);
        }
    }
    // copy symbol bits
    for (int i = 0; i < 8; ++i) {
        uint8_t bit = get_bit_from_byte(sym, i);
        if (bit) {
            set_bit(&pairs_buffer[pairs_buffer_index / 8], pairs_buffer_index % 8);
        }
        pairs_buffer_index++;
        if (pairs_buffer_index == BLOCK * 8) {
            flush_pairs(outfile);
        }
    }
}

/*
Writes out any remaining pairs of symbols and codes to the output ﬁle.
*/
void flush_pairs(int outfile) {
    uint32_t to_write = pairs_buffer_index / 8;
    if (to_write * 8 < pairs_buffer_index) {
        ++to_write;
    }
    write_bytes(outfile, pairs_buffer, to_write);
    pairs_buffer_index = 0;
    memset(pairs_buffer, 0, BLOCK);
}

/*
“Reads” a pair (code and symbol) from the input ﬁle. The “read” code is placed
in the pointer to code (e.g. *code = val) The “read” symbol is placed in the
pointer to sym (e.g. *sym = val). In reality, a block of pairs is read into a
buffer. An index keeps track of the current bit in the buffer. Once all bits
have been processed, another block is read. The ﬁrst bitlen bits are the code,
starting from the LSB. The last 8 bits of the pair are the symbol, starting from
the LSB. Returns true if there are pairs left to read in the buffer, else false.
There are pairs left to read if the read code is not STOP_CODE.
*/
bool read_pair(int infile, uint16_t *code, uint8_t *sym, int bitlen) {
    // read code
    *code = 0;
    for (int i = 0; i < bitlen; ++i) {
        if (pairs_buffer_index == pairs_buffer_size * 8) {
            pairs_buffer_size = read_bytes(infile, pairs_buffer, BLOCK);
            pairs_buffer_index = 0;
        }

        uint8_t bit = get_bit(pairs_buffer[pairs_buffer_index / 8], pairs_buffer_index % 8);
        if (bit) {
            set_bit_to_word(code, i);
        }
        pairs_buffer_index++;
    }
    if (*code == STOP_CODE) {
        return false;
    }

    // read sym
    *sym = 0;
    for (int i = 0; i < 8; ++i) {
        if (pairs_buffer_index == pairs_buffer_size * 8) {
            pairs_buffer_size = read_bytes(infile, pairs_buffer, BLOCK);
            pairs_buffer_index = 0;
        }

        uint8_t bit = get_bit(pairs_buffer[pairs_buffer_index / 8], pairs_buffer_index % 8);
        if (bit) {
            set_bit_to_byte(sym, i);
        }
        pairs_buffer_index++;
    }

    return true;
}

/*
“Writes” a pair to the output ﬁle. Each symbol of the Word is placed into a
buffer. The buffer is written out when it is ﬁlled.
*/
void write_word(int outfile, Word *w) {
    for (uint32_t i = 0; i < w->len; ++i) {
        char_buffer[char_buffer_index++] = w->syms[i];
        if (char_buffer_index == BLOCK) {
            flush_words(outfile);
        }
    }
}

/*
Writes out any remaining symbols in the buffer to the outﬁle.
Note that the output ﬁle in which you write to must have the same protection
bits as the original ﬁle. Like in assignment 4, you will make use of fstat() and
fchmod().
*/
void flush_words(int outfile) {
    write_bytes(outfile, char_buffer, char_buffer_index);
    char_buffer_index = 0;
}
