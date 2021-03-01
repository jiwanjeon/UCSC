#include "ll.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// The struct definition of a linked list is given below
struct LinkedList {
    uint32_t length;
    Node *head; // Head sentinel node.
    Node *tail; // Tail sentinel node.
    bool mtf;
};

// The constructor for a linked list.
LinkedList *ll_create(bool mtf) {
    LinkedList *ll = malloc(sizeof(*ll));
    if (ll) {
        // create dummy nodes
        ll->head = node_create(NULL, NULL);
        ll->tail = node_create(NULL, NULL);

        if (!ll->head || !ll->tail) {
            // on out of memory: free all allocated memory and return NULL
            free(ll->head);
            free(ll->tail);
            free(ll);
            ll = NULL;
        } else {
            // initialize
            // make the head point to the tail and the tail point to the head
            ll->head->next = ll->tail;
            ll->tail->prev = ll->head;
            ll->length = 0;
            ll->mtf = mtf;
        }
    }
    return ll;
}

// The destructor for a linked list.
// Each node in the linked list should be freed using node_delete().
void ll_delete(LinkedList **ll) {
    if (ll && *ll) {
        // iterate list (excluding head and tail nodes)
        Node *n = (*ll)->head->next; // current node
        while (n != (*ll)->tail) {
            Node *deleted = n; // rember current node to delete it
            n = n->next; // make current pointer to point to next node
            node_delete(&deleted); // delete current node
        }
        node_delete(&(*ll)->head);
        node_delete(&(*ll)->tail);
    }
    free(*ll);
    *ll = NULL;
}

// Returns the length of the linked list, which is equivalent to the number of
// nodes in the linked list, not including the head and tail sentinel nodes.
uint32_t ll_length(LinkedList *ll) {
    if (ll) {
        return ll->length;
    }
    return 0;
}

// Searches for a node containing oldspeak.
Node *ll_lookup(LinkedList *ll, char *oldspeak) {
    if (ll) {
        // iterate list (excluding head and tail nodes)
        Node *n = ll->head->next; // current node
        while (n != ll->tail) {
            // compare a node oldspeak and the oldspeak parameter
            if (strcmp(n->oldspeak, oldspeak) == 0) {
                // if move-to-front flag is true
                if (ll->mtf) {
                    // unlink the node
                    n->prev->next = n->next;
                    n->next->prev = n->prev;
                    // re-insert the node at the head
                    ll->head->next->prev = n;
                    n->next = ll->head->next;
                    n->prev = ll->head;
                    ll->head->next = n;
                }

                return n; // return found node
            }
            n = n->next; // make current pointer to point to next node
        }
    }
    return NULL; // not found
}

// Inserts a new node containing the speciﬁed oldspeak and newspeak into the
// linked list.
void ll_insert(LinkedList *ll, char *oldspeak, char *newspeak) {
    if (ll) {
        // add if does not exist yet
        if (!ll_lookup(ll, oldspeak)) {
            Node *newnode = node_create(oldspeak, newspeak);
            if (newnode) {
                // insert at the head
                ll->head->next->prev = newnode;
                newnode->next = ll->head->next;
                newnode->prev = ll->head;
                ll->head->next = newnode;
                ll->length += 1;
            }
        }
    }
}

// Prints out each node in the linked list except for the head and tail sentinel
// nodes.
void ll_print(LinkedList *ll) {
    if (ll) {
        // iterate list (excluding head and tail nodes)
        Node *n = ll->head->next; // current node
        while (n != ll->tail) {
            node_print(n);
            n = n->next; // make current pointer to point to next node
        }
    }
}

// simple test
void test_ll(void) {
    LinkedList *ll = ll_create(true);
    ll_print(ll);
    assert(ll_length(ll) == 0);
    assert(ll_lookup(ll, "abc") == NULL);

    ll_insert(ll, "abc", NULL);
    assert(ll_length(ll) == 1);
    assert(ll_lookup(ll, "abc") != NULL);
    ll_print(ll);

    ll_insert(ll, "def", "xyz");
    assert(ll_length(ll) == 2);
    assert(ll_lookup(ll, "def") != NULL);
    ll_print(ll);

    assert(strcmp(ll_lookup(ll, "abc")->next->oldspeak, "def") == 0);
    ll_print(ll);

    ll_delete(&ll);
    assert(ll == NULL);
}
