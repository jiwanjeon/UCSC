#include "trie.h"

#include "code.h"

#include <stdlib.h>

/*
Constructor for a TrieNode. The node's code is set to code.
Make sure each of the children node pointers are NULL.
*/
TrieNode *trie_node_create(uint16_t code) {
    TrieNode *newnode = calloc(1, sizeof(*newnode));
    newnode->code = code;
    return newnode;
}

/*
Destructor for a TrieNode.
*/
void trie_node_delete(TrieNode *n) {
    if (n) {
        for (int i = 0; i < ALPHABET; ++i) {
            trie_node_delete(n->children[i]);
        }
        free(n);
    }
}

/*
Initializes a trie: a root TrieNode with the code EMPTY_CODE.
Returns the root, a TrieNode *, if successful, NULL otherwise.
*/
TrieNode *trie_create(void) {
    TrieNode *root = trie_node_create(EMPTY_CODE);
    return root;
}

/*
Resets a trie to just the root TrieNode. Since we are working with finite codes,
eventually we will arrive at the end of the available codes (MAX_CODE). At that
point, we must reset the trie by deleting its children so that we can continue
compressing/decompressing the file. Make sure that each of the root’s children
nodes are NULL.
*/
void trie_reset(TrieNode *root) {
    trie_delete(root);
}

/*
Deletes a sub-trie starting from the trie rooted at node n. This will require
recursive calls on each of n’s children. Make sure to set the pointer to
the children nodes to NULL after you free them with trie_node_delete().
*/
void trie_delete(TrieNode *n) {
    if (n) {
        for (int i = 0; i < ALPHABET; ++i) {
            trie_node_delete(n->children[i]);
            n->children[i] = NULL;
        }
    }
}

/*
Returns a pointer to the child node reprsenting the symbol sym.
If the symbol doesn’t exist, NULL is returned.
*/
TrieNode *trie_step(TrieNode *n, uint8_t sym) {
    if (n) {
        return n->children[sym];
    }
    return NULL;
}
