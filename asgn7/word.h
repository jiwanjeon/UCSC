#ifndef __WORD_H__
#define __WORD_H__

#include <stdint.h>

typedef struct Word {
    uint8_t *syms;
    uint32_t len;
} Word;

typedef Word *WordTable;

/*
Constructor for a word where sysms is the array of symbols a Word represents.
The length of the array of symbols is given by len. This function returns a
Word * if successful or NULL otherwise.
*/
Word *word_create(uint8_t *syms, uint32_t len);

/*
Constructs a new Word from the speciﬁed Word, w, appended with a symbol, sym.
The Word speciﬁed to append to may be empty. If the above is the case, the
new Word should contain only the symbol. Returns the new Word which represents
the result of appending.
*/
Word *word_append_sym(Word *w, uint8_t sym);

/*
Destructor for a Word, w.
*/
void word_delete(Word *w);

/*
Creates a new WordTable, which is an array of Words. A WordTable has a
pre-deﬁned size of MAX_CODE, which has the value UINT16_MAX - 1. This is because
codes are 16-bit integers. A WordTable is initialized with a single Word at
index EMPTY_CODE. This Word represents the empty word, a string of length of
zero.
*/
WordTable *wt_create(void);

/*
Resets a WordTable, wt, to contain just the empty Word. Make sure all the other
words in the table are NULL.
*/
void wt_reset(WordTable *wt);

/*
Deletes a WordTable, wt.
*/
void wt_delete(WordTable *wt);

#endif
