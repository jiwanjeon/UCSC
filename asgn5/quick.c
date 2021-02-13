#include "quick.h"

#include "stack.h"

#include <assert.h>

// counters defined in sorting.c
extern uint32_t quick_moves;
extern uint32_t quick_compares;

static void swap(uint32_t *a, uint32_t *b) {
    uint32_t t = *a;
    *a = *b;
    *b = t;
}

uint32_t partition(uint32_t *A, uint32_t lo, uint32_t hi) {
    uint32_t pivot = A[lo + ((hi - lo) / 2)]; // Prevent overflow
    int64_t i = (int64_t) lo - 1;
    int64_t j = (int64_t) hi + 1;

    while (i < j) {
        do {
            i += 1;
            quick_compares += 1;
        } while (A[i] < pivot);

        do {
            j -= 1;
            quick_compares += 1;
        } while (A[j] > pivot);

        if (i < j) {
            swap(&A[i], &A[j]);
            quick_moves += 3;
        }
    }

    return (uint32_t) j;
}

void quick_sort(uint32_t *A, uint32_t n) {
    Stack *stack = stack_create();
    assert(stack_push(stack, 0));
    assert(stack_push(stack, n - 1));

    while (!stack_empty(stack)) {
        int64_t hi, lo;
        assert(stack_pop(stack, &hi));
        assert(stack_pop(stack, &lo));

        if (lo < hi) {
            int64_t p = partition(A, (uint32_t) lo, (uint32_t) hi);

            if (p > lo) {
                assert(stack_push(stack, lo));
                assert(stack_push(stack, p));
            }

            if (p + 1 < hi) {
                assert(stack_push(stack, p + 1));
                assert(stack_push(stack, hi));
            }
        }
    }

    stack_delete(&stack);
}
