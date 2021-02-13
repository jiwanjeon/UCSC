#define _GNU_SOURCE
#include "bubble.h"
#include "heap.h"
#include "quick.h"
#include "set.h"
#include "shell.h"

#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Options
#define OPT_A (1 << 0)
#define OPT_B (1 << 1)
#define OPT_S (1 << 2)
#define OPT_Q (1 << 3)
#define OPT_H (1 << 4)

// Bubble counters
uint32_t bubble_moves = 0;
uint32_t bubble_compares = 0;
// Shell counters
uint32_t shell_moves = 0;
uint32_t shell_compares = 0;
// Quick counters
uint32_t quick_moves = 0;
uint32_t quick_compares = 0;
// Heap counters
uint32_t heap_moves = 0;
uint32_t heap_compares = 0;

// copy A to B
void array_copy(uint32_t A[], uint32_t B[], uint32_t n) {
    for (uint32_t i = 0; i < n; ++i) {
        B[i] = A[i];
    }
}

// print array
void array_print(uint32_t A[], uint32_t n, uint32_t r) {
    uint32_t limit = r < n ? r : n;
    for (uint32_t i = 0; i < limit; ++i) {
        printf("%13" PRIu32, A[i]);
        if (i < limit - 1 && (i + 1) % 5 == 0) {
            printf("\n");
        }
    }
    printf("\n");
}

// fill array with random numbers
void make_array(uint32_t A[], uint32_t n) {
    for (uint32_t i = 0; i < n; ++i) {
        A[i] = random() & 0x3FFFFFFF;
    }
}

void make_ordered_array(uint32_t A[], uint32_t n, bool reverse) {
    for (uint32_t i = 0; i < n; ++i) {
        if (reverse) {
            A[i] = n - i;
        } else {
            A[i] = i + 1;
        }
    }
}

void make_equal_array(uint32_t A[], uint32_t n) {
    uint32_t elem = 0x3FFFFFFF;
    for (uint32_t i = 0; i < n; ++i) {
        A[i] = elem;
    }
}

// ensure array is sorted
int array_sorted(uint32_t A[], uint32_t n) {
    for (uint32_t i = 1; i < n; ++i) {
        if (A[i - 1] > A[i]) {
            return 0;
        }
    }
    return 1;
}

// Bubble Sort
void perform_bubble(uint32_t X[], uint32_t n, uint32_t r) {
    printf("Bubble Sort\n");
    uint32_t *A = malloc(n * sizeof(*A));
    if (!A)
        return;

    array_copy(X, A, n);
    bubble_sort(A, n);
    assert(array_sorted(A, n));
    if (r > 0) {
        array_print(A, n, r);
    }
    free(A);

    printf("%u elements, %u moves, %u compares\n", n, bubble_moves, bubble_compares);
}

// Shell Sort
void perform_shell(uint32_t X[], uint32_t n, uint32_t r) {
    printf("Shell Sort\n");
    uint32_t *A = malloc(n * sizeof(*A));
    if (!A)
        return;

    array_copy(X, A, n);
    shell_sort(A, n);
    assert(array_sorted(A, n));
    if (r > 0) {
        array_print(A, n, r);
    }
    free(A);

    printf("%u elements, %u moves, %u compares\n", n, shell_moves, shell_compares);
}

// Quick Sort
void perform_quick(uint32_t X[], uint32_t n, uint32_t r) {
    printf("Quick Sort\n");
    uint32_t *A = malloc(n * sizeof(*A));
    if (!A)
        return;

    array_copy(X, A, n);
    quick_sort(A, n);
    assert(array_sorted(A, n));
    if (r > 0) {
        array_print(A, n, r);
    }
    free(A);

    printf("%u elements, %u moves, %u compares\n", n, quick_moves, quick_compares);
}

// Heap Sort
void perform_heap(uint32_t X[], uint32_t n, uint32_t r) {
    printf("Heap Sort\n");
    uint32_t *A = malloc(n * sizeof(*A));
    if (!A)
        return;

    array_copy(X, A, n);
    heap_sort(A, n);
    assert(array_sorted(A, n));
    if (r > 0) {
        array_print(A, n, r);
    }
    free(A);

    printf("%u elements, %u moves, %u compares\n", n, heap_moves, heap_compares);
}

int main(int argc, char *argv[]) {
    unsigned seed = 7092016;
    uint32_t size = 100;
    uint32_t elements = 100;
    // for testing ordered arrays and arrays with all elements equal
    bool ordered = false;
    bool reverse;
    bool equal = false;

    Set s = set_empty();
    int opt;

    // parse command line
    while ((opt = getopt(argc, argv, "absqhr:n:p:ORE")) != -1) {
        switch (opt) {
        case 'a': s = set_insert(s, OPT_A); break;
        case 'b': s = set_insert(s, OPT_B); break;
        case 's': s = set_insert(s, OPT_S); break;
        case 'q': s = set_insert(s, OPT_Q); break;
        case 'h': s = set_insert(s, OPT_H); break;
        case 'r':
            seed = atoi(optarg);
            // should be non-negative
            if (seed <= 0 && !(optarg[0] == '0' && optarg[1] == '\0')) {
                fprintf(stderr, "Invalid seed parameter\n");
                exit(EXIT_FAILURE);
            }
            break;
        case 'n':
            // should be positive
            size = atoi(optarg);
            if (size <= 0) {
                fprintf(stderr, "Invalid size parameter\n");
                exit(EXIT_FAILURE);
            }
            break;
        case 'p':
            // should be non-negative
            elements = atoi(optarg);
            if (elements < 0 || (elements == 0 && !(optarg[0] == '0' && optarg[1] == '\0'))) {
                fprintf(stderr, "Invalid elements parameter\n");
                exit(EXIT_FAILURE);
            }
            break;
        case 'O':
            ordered = true;
            reverse = false;
            break;
        case 'R':
            ordered = true;
            reverse = true;
            break;
        case 'E': equal = true; break;
        default: fprintf(stderr, "Unknown option: %c\n", opt); break;
        }
    }

    // set seed
    srandom(seed);

    // Make reference array for all sortings
    uint32_t *X = malloc(size * sizeof(*X));
    if (!ordered && !equal) {
        // make random array
        make_array(X, size);
    } else if (ordered) {
        make_ordered_array(X, size, reverse);
    } else if (equal) {
        make_equal_array(X, size);
    }

    if (set_member(s, OPT_A) || set_member(s, OPT_B)) {
        perform_bubble(X, size, elements);
    }

    if (set_member(s, OPT_A) || set_member(s, OPT_S)) {
        perform_shell(X, size, elements);
    }

    if (set_member(s, OPT_A) || set_member(s, OPT_Q)) {
        perform_quick(X, size, elements);
    }

    if (set_member(s, OPT_A) || set_member(s, OPT_H)) {
        perform_heap(X, size, elements);
    }

    // free memory
    free(X);

    return 0;
}
