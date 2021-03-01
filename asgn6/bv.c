#include "bv.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

// A bit vector is an ADT that represents an array of bits,
// the bits in which are used to denote if something is true or false (1 or 0).
struct BitVector {
    uint32_t length;
    uint8_t *vector;
};

// function to extract a bit from a byte
static uint8_t get_bit(uint8_t byte, uint8_t i) {
    return byte & (1 << (7 - i)) ? 1 : 0;
}

// function to set a bit in a byte
static void set_bit(uint8_t *byte, uint8_t i) {
    *byte |= (1 << (7 - i));
}

// function to clear a bit in a byte
static void clr_bit(uint8_t *byte, uint8_t i) {
    *byte &= ~(1 << (7 - i));
}

// This constructor will allocate memory for the bit vector.
// The number of bits that vector should hold is length. If
// at any point allocating memory with calloc() fails,
// the function must return NULL, else it must return a BitVector*
// or a pointer to a BitVector.
BitVector *bv_create(uint32_t length) {
    BitVector *bv = malloc(sizeof(*bv));
    if (bv) {
        bv->length = length;
        // determine the size of the array (how many of bytes are needed to store
        // length bits)
        uint32_t size = length / sizeof(uint8_t);
        if (length > size * sizeof(uint8_t)) {
            size += 1;
        }
        // create array of uint8_t
        bv->vector = calloc(size, sizeof(uint8_t));
        if (!bv->vector) {
            free(bv);
            bv = NULL;
        }
    }
    return bv;
}

// The destructor will free the memory allocated for the bit vector.
void bv_delete(BitVector **bv) {
    if (bv && *bv) {
        free((*bv)->vector);
        free(*bv);
        *bv = NULL;
    }
}

// Returns the bit vector’s length.
uint32_t bv_length(BitVector *bv) {
    if (bv)
        return bv->length;
    return 0;
}

// Since we cannot directly access a bit in a uint8_t,
// we instead need to perform bitwise operations on bytes
// in order to set a speciﬁc bit
void bv_set_bit(BitVector *bv, uint32_t i) {
    if (bv && i < bv->length) {
        set_bit(&bv->vector[i / 8], i % 8);
    }
}

// In some cases it may be necessary to clear a bit or element
// in a bit vector. Similarly to setting a bit, it is necessary
// to access the byte where the bit speciﬁed by i is located.
void bv_clr_bit(BitVector *bv, uint32_t i) {
    if (bv && i < bv->length) {
        clr_bit(&bv->vector[i / 8], i % 8);
    }
}

// A function get a bit from a bit vector.
uint8_t bv_get_bit(BitVector *bv, uint32_t i) {
    if (bv && i < bv->length) {
        return get_bit(bv->vector[i / 8], i % 8);
    }
    return 0;
}

// For debugging purposes it is helpful to print out a bit vector.
void bv_print(BitVector *bv) {
    for (uint32_t i = 0; i < bv->length; ++i) {
        printf("%d ", bv_get_bit(bv, i));
    }
    printf("\n");
}

// simple test
void test_bv(void) {
    BitVector *bv = bv_create(15);
    bv_set_bit(bv, 8);
    bv_set_bit(bv, 14);
    bv_print(bv);
    bv_clr_bit(bv, 14);
    bv_print(bv);

    bv_delete(&bv);
    assert(bv == NULL);
}
