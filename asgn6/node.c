#include "node.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// create a copy of the src string
char *my_strdup(const char *src) {
    if (src == NULL)
        return NULL;
    size_t n = strlen(src) + 1;
    char *dst = malloc(n);
    if (dst == NULL)
        return NULL;
    strcpy(dst, src);
    return dst;
}

// The constructor for a node. You will want to make a copy of the oldspeak and
// its newspeak translation thatare passed in. What this means is allocating
// memory and copying over thecharacters for both oldspeak and newspeak.
Node *node_create(char *oldspeak, char *newspeak) {
    Node *n = malloc(sizeof(*n));
    if (n) {
        n->prev = NULL;
        n->next = NULL;

        n->oldspeak = my_strdup(oldspeak);
        n->newspeak = my_strdup(newspeak);

        if ((oldspeak && !n->oldspeak) || (newspeak && !n->newspeak)) {
            node_delete(&n);
        }
    }
    return n;
}

// The destructor for a node. Only the node n is freed. The previous and next
// nodes that n points to are not deleted.
void node_delete(Node **n) {
    if (n && *n) {
        free((*n)->oldspeak);
        free((*n)->newspeak);
        free(*n);
        *n = NULL;
    }
}

// debug
void node_print(Node *n) {
    if (n) {
        if (n->oldspeak && n->newspeak) {
            printf("%s -> %s\n", n->oldspeak, n->newspeak);
        } else {
            printf("%s\n", n->oldspeak);
        }
    }
}
