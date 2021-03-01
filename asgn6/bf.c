#include "bf.h"

#include "speck.h"

#include <assert.h>
#include <stdlib.h>

// The three salts will be stored in the primary, secondary,and tertiary fields.
// Each salt is 128 bits in size. To hold these 128 bits, we use an array of two
// uint64_ts.
struct BloomFilter {
    uint64_t primary[2]; // Primary hash function salt.
    uint64_t secondary[2]; // Secondary hash function salt.
    uint64_t tertiary[2]; // Tertiary hash function salt.
    BitVector *filter;
};

// The constructor for a Bloom filter.
BloomFilter *bf_create(uint32_t size) {
    BloomFilter *bf = (BloomFilter *) malloc(sizeof(BloomFilter));
    if (bf) {
        // Fear & Loathing in Las Vegas
        bf->primary[0] = 0x02d232593fbe42ff;
        bf->primary[1] = 0x3775cfbf0794f152;
        // A Moveable Feast
        bf->secondary[0] = 0xc1706bc17ececc04;
        bf->secondary[1] = 0xe9820aa4d2b8261a;
        // The Cremation of Sam McGee
        bf->tertiary[0] = 0xd37b01df0ae8f8d0;
        bf->tertiary[1] = 0x911d454886ca7cf7;
        bf->filter = bv_create(size);
        if (!bf->filter) {
            free(bf);
            bf = NULL;
        }
    }
    return bf;
}

// The destructor for a Bloom filter.
void bf_delete(BloomFilter **bf) {
    if (bf && *bf) {
        bv_delete(&(*bf)->filter);
        free(*bf);
        *bf = NULL;
    }
}

// Returns the size of the Bloom filter.
// In other words, the number of bits that the Bloom filter can access.
uint32_t bf_length(BloomFilter *bf) {
    if (bf)
        return bv_length(bf->filter);
    return 0;
}

// Takes oldspeak and inserts it into the Bloom filter.
// This entails hashing oldspeak with each of the three salts for
// three indices, and setting the bits at those indices in the underlying bit
// vector.
void bf_insert(BloomFilter *bf, char *oldspeak) {
    if (bf && oldspeak) {
        uint32_t index1 = hash(bf->primary, oldspeak) % bf_length(bf);
        uint32_t index2 = hash(bf->secondary, oldspeak) % bf_length(bf);
        uint32_t index3 = hash(bf->tertiary, oldspeak) % bf_length(bf);

        bv_set_bit(bf->filter, index1);
        bv_set_bit(bf->filter, index2);
        bv_set_bit(bf->filter, index3);
    }
}

// Probes the Bloom filter for oldspeak. Like with bf_insert(),
// oldspeak is hashed with each of the three salts for
// three indices. If all the bits at those indices are set,
// return true to signify that oldspeak was most likely added to
// he Bloom filter. Else, return false.
bool bf_probe(BloomFilter *bf, char *oldspeak) {
    if (bf && oldspeak) {
        uint32_t index1 = hash(bf->primary, oldspeak) % bf_length(bf);
        uint32_t index2 = hash(bf->secondary, oldspeak) % bf_length(bf);
        uint32_t index3 = hash(bf->tertiary, oldspeak) % bf_length(bf);

        return bv_get_bit(bf->filter, index1) && bv_get_bit(bf->filter, index2)
               && bv_get_bit(bf->filter, index3);
    }
    return false;
}

// A debug function to print out a Bloom filter.
void bf_print(BloomFilter *bf) {
    if (bf) {
        bv_print(bf->filter);
    }
}

// simple test
void bf_test(void) {
    BloomFilter *bf = bf_create(32);

    assert(bf_length(bf) == 32);

    assert(bf_probe(bf, "abc") == false);

    bf_insert(bf, "abc");
    assert(bf_probe(bf, "abc") == true);
    assert(bf_probe(bf, "def") == false);

    bf_insert(bf, "def");
    assert(bf_probe(bf, "abc") == true);
    assert(bf_probe(bf, "def") == true);
    assert(bf_probe(bf, "xyz") == false);

    bf_insert(bf, "xyz");
    assert(bf_probe(bf, "abc") == true);
    assert(bf_probe(bf, "def") == true);
    assert(bf_probe(bf, "xyz") == true);

    bf_print(bf);

    bf_delete(&bf);
    assert(bf == NULL);
}
