#ifndef __IO_H__
#define __IO_H__

#include "word.h"

#include <stdbool.h>
#include <stdint.h>

#define BLOCK 4096 // 4KB blocks.
#define MAGIC 0xBAADBAAC // Unique encoder/decoder magic number.

extern uint64_t total_syms; // To count the symbols processed.
extern uint64_t total_bits; // To count the bits processed.

typedef struct FileHeader {
    uint32_t magic;
    uint16_t protection;
} FileHeader;

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
int read_bytes(int infile, uint8_t *buf, int to_read);

/*
This function is very much the same as read_bytes(), except that it is for
looping calls to write(). As you may imagine, write() isn’t guaranteed to write
out all the speciﬁed bytes (to_write), and so we loop until we have either
written out all the bytes speciﬁed, or no bytes were written. The number of
bytes written out is returned. You should use this function whenever you need to
perform a write.
*/
int write_bytes(int outfile, uint8_t *buf, int to_write);

/*
This reads in sizeof(FileHeader) bytes from the input ﬁle. These bytes are read
into the supplied header. Endi- anness is swapped if byte order isn’t little
endian. Along with reading the header, it must verify the magic number.
*/
void read_header(int infile, FileHeader *header);

/*
Writes sizeof(FileHeader) bytes to the output ﬁle. These bytes are from the
supplied header. Endianness is swapped if byte order isn’t little endian.
*/
void write_header(int outfile, FileHeader *header);

/*
An index keeps track of the currently read symbol in the buffer. Once all
symbols are processed , another block is read. If less than a block is read ,
the end of the buffer is updated. Returns true if there are symbols to be read ,
false otherwise.
*/
bool read_sym(int infile, uint8_t *sym);

/*
“Writes” a pair to outfile. In reality, the pair is buffered. A pair is
comprised of a code and a symbol. The bits of the code are buffered ﬁrst,
starting from the LSB. The bits of the symbol are buffered next, also starting
from the LSB. The code buffered has a bit-length of bitlen. The buffer is
written out whenever it is ﬁlled.
*/
void write_pair(int outfile, uint16_t code, uint8_t sym, int bitlen);

/*
Writes out any remaining pairs of symbols and codes to the output ﬁle.
*/
void flush_pairs(int outfile);

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
bool read_pair(int infile, uint16_t *code, uint8_t *sym, int bitlen);

/*
“Writes” a pair to the output ﬁle. Each symbol of the Word is placed into a
buffer. The buffer is written out when it is ﬁlled.
*/
void write_word(int outfile, Word *w);

/*
Writes out any remaining symbols in the buffer to the outﬁle.
Note that the output ﬁle in which you write to must have the same protection
bits as the original ﬁle. Like in assignment 4, you will make use of fstat() and
fchmod().
*/
void flush_words(int outfile);

#endif
