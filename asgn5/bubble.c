#include "bubble.h"

#include <stdbool.h>

// counters defined in sorting.c
extern uint32_t bubble_moves;
extern uint32_t bubble_compares;

static void swap(uint32_t *a, uint32_t *b) {
  uint32_t t = *a;
  *a = *b;
  *b = t;
}

void bubble_sort(uint32_t *A, uint32_t n) {
  bool swapped = true;
  while (swapped) {
    swapped = false;
    for (uint32_t i = 1; i < n; ++i) {
      bubble_compares += 1;
      if (A[i] < A[i - 1]) {
        swap(&A[i], &A[i - 1]);
        swapped = true;
        bubble_moves += 3;
      }
    }
    n -= 1;
  }
}
