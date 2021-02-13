#include "shell.h"
#include "gaps.h"

// counters defined in sorting.c
extern uint32_t shell_moves;
extern uint32_t shell_compares;

static void swap(uint32_t *a, uint32_t *b) {
  uint32_t t = *a;
  *a = *b;
  *b = t;
}

void shell_sort(uint32_t *A, uint32_t n) {
  for (uint32_t g = 0; g < GAPS; ++g) {
    uint32_t gap = gaps[g];
    for (uint32_t i = gap; i < n; ++i) {
      for (uint32_t j = i; j > gap - 1; j -= gap) {
        shell_compares += 1;
        if (A[j] < A[j - gap]) {
          swap(&A[j], &A[j - gap]);
          shell_moves += 3;
        }
      }
    }
  }
}
