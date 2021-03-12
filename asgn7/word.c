#include "word.h"

#include "code.h"

#include <memory.h>
#include <stdlib.h>

/*
Constructor for a word where sysms is the array of symbols a Word represents.
The length of the array of symbols is given by len. This function returns a
Word * if successful or NULL otherwise.
*/
Word *word_create(uint8_t *syms, uint32_t len) {
    Word *w = calloc(1, sizeof(*w));
    if (w) {
        w->syms = malloc(sizeof(*w->syms) * len);
        if (!w->syms && len > 0) {
            free(w);
            w = NULL;
        } else {
            if (len > 0) {
                memcpy(w->syms, syms, len);
            }
            w->len = len;
        }
    }
    return w;
}

/*
Constructs a new Word from the speciﬁed Word, w, appended with a symbol, sym.
The Word specified to append to may be empty. If the above is the case, the
new Word should contain only the symbol. Returns the new Word which represents
the result of appending.
*/
Word *word_append_sym(Word *w, uint8_t sym) {
    Word *neww = word_create(w->syms, w->len);
    neww->syms = realloc(neww->syms, neww->len + 1);
    if (neww->syms) {
        neww->syms[neww->len++] = sym;
    } else {
        free(neww);
        neww = NULL;
    }
    return neww;
}

/*
Destructor for a Word, w.
*/
void word_delete(Word *w) {
    if (w) {
        free(w->syms);
        free(w);
    }
}

/*
Creates a new WordTable, which is an array of Words. A WordTable has a
pre-deﬁned size of MAX_CODE, which has the value UINT16_MAX - 1. This is because
codes are 16-bit integers. A WordTable is initialized with a single Word at
index EMPTY_CODE. This Word represents the empty word, a string of length of
zero.
*/
WordTable *wt_create(void) {
    WordTable *wt = calloc(MAX_CODE, sizeof(*wt));
    if (wt) {
        wt[EMPTY_CODE] = word_create(NULL, 0);
    }
    return wt;
}

/*
Resets a WordTable, wt, to contain just the empty Word. Make sure all the other
words in the table are NULL.
*/
void wt_reset(WordTable *wt) {
    if (wt) {
        for (int i = 0; i < MAX_CODE; ++i) {
            word_delete(wt[i]);
            wt[i] = NULL;
        }
        wt[EMPTY_CODE] = word_create(NULL, 0);
    }
}

/*
Deletes a WordTable, wt.
*/
void wt_delete(WordTable *wt) {
    if (wt) {
        for (int i = 0; i < MAX_CODE; ++i) {
            word_delete(wt[i]);
        }
        free(wt);
    }
}
