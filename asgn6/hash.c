#include "hash.h"

#include "speck.h"

#include <assert.h>
#include <stdlib.h>

// Below is the struct definition for a hash table
struct HashTable {
    uint64_t salt[2];
    uint32_t size;
    bool mtf;
    LinkedList **lists;
};

// The constructor for a hash table. The size parameter denotes the number of
// indices, or linked lists, that the hash table can index up to. The salt for
// the hash table has been supplied in the constructor as well.
HashTable *ht_create(uint32_t size, bool mtf) {
    HashTable *ht = (HashTable *) malloc(sizeof(HashTable));
    if (ht) {
        ht->salt[0] = 0x85ae998311115ae3; // Il nome della rosa
        ht->salt[1] = 0xb6fac2ae33a40089;
        ht->size = size;
        ht->mtf = mtf;
        ht->lists = (LinkedList **) calloc(size, sizeof(LinkedList *));
        if (!ht->lists) {
            free(ht);
            ht = NULL;
        }
    }
    return ht;
}

// The destructor for a hash table. Each of the linked lists in lists, the
// underlying array of linked lists, is freed. The pointer that was passed in
// should be set to NULL.
void ht_delete(HashTable **ht) {
    if (ht && *ht) {
        // delete all linked lists
        for (uint32_t i = 0; i < (*ht)->size; ++i) {
            ll_delete(&(*ht)->lists[i]);
        }
        free((*ht)->lists); // delete array of ll
        free(*ht); // delete hash table structure
        *ht = NULL;
    }
}

// Returns the hash table’s size.
uint32_t ht_size(HashTable *ht) {
    if (ht)
        return ht->size;
    return 0;
}

// Searches for an entry, a node, in the hash table that contains oldspeak. A
// node stores oldspeak and its newspeak translation. The index of the linked
// list to perform a look-up on is calculated by hashing the oldspeak. If the
// node is found, the pointer to the node is returned. Else, a NULL pointer is
// returned.
Node *ht_lookup(HashTable *ht, char *oldspeak) {
    if (ht && oldspeak) {
        uint32_t index = hash(ht->salt, oldspeak) % ht->size;
        return ll_lookup(ht->lists[index], oldspeak);
    }
    return NULL;
}

// Inserts the specified oldspeak and its corresponding newspeak translation
// into the hash table. The index of the linked list to insert into is
// calculated by hashing the oldspeak. If the linked list that should be
// inserted into hasn’t been initialized yet, create it first before inserting
// the oldspeak and newspeak.
void ht_insert(HashTable *ht, char *oldspeak, char *newspeak) {
    if (ht && oldspeak) {
        uint32_t index = hash(ht->salt, oldspeak) % ht->size;

        // If the linked list that should be inserted into hasn’t been initialized
        // yet
        if (ht->lists[index] == NULL) {
            // create it first before inserting the oldspeak and newspeak.
            ht->lists[index] = ll_create(ht->mtf);
        }
        ll_insert(ht->lists[index], oldspeak, newspeak);
    }
}

// A debug function to print out the contents of a hash table.
void ht_print(HashTable *ht) {
    if (ht) {
        for (uint32_t i = 0; i < (ht)->size; ++i) {
            ll_print(ht->lists[i]);
        }
    }
}

// simple test
void ht_test(void) {
    HashTable *ht = ht_create(11, false);

    assert(ht_size(ht) == 11);
    assert(ht_lookup(ht, "abc") == NULL);
    ht_print(ht);

    ht_insert(ht, "abc", NULL);
    assert(ht_lookup(ht, "abc") != NULL);
    ht_print(ht);

    ht_insert(ht, "def", "xyz");
    assert(ht_lookup(ht, "def") != NULL);
    ht_print(ht);

    ht_insert(ht, "ghi", "123");
    assert(ht_lookup(ht, "ghi") != NULL);
    ht_print(ht);

    ht_insert(ht, "rst", "555");
    assert(ht_lookup(ht, "rst") != NULL);
    ht_print(ht);

    ht_delete(&ht);
    assert(ht == NULL);
}
