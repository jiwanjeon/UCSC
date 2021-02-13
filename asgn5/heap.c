#include "heap.h"
#include <assert.h>
#include <stdbool.h>

// counters defined in sorting.c
extern uint32_t heap_moves;
extern uint32_t heap_compares;

static void swap(uint32_t *a, uint32_t *b) {
  uint32_t t = *a;
  *a = *b;
  *b = t;
  heap_moves += 3;
}

// since there are many places in this sort where comparison takes place,
// it is more convenient to have a special function for counting.
static int compare(uint32_t a, uint32_t b) {
  heap_compares += 1;
  if (a < b)
    return -1;
  if (a > b)
    return 1;
  return 0;
}

uint32_t max_child(uint32_t *A, uint32_t first, uint32_t last) {
  uint32_t left = 2 * first;
  uint32_t right = left + 1;
  if (right <= last && compare(A[right - 1], A[left - 1]) > 0)
    return right;
  return left;
}

void fix_heap(uint32_t *A, uint32_t first, uint32_t last) {
  bool found = false;
  uint32_t parent = first;
  uint32_t great = max_child(A, parent, last);

  while (parent <= last / 2 && !found) {
    if (compare(A[parent - 1], A[great - 1]) < 0) {
      swap(&A[parent - 1], &A[great - 1]);
      parent = great;
      if (parent <= last / 2) {
        great = max_child(A, parent, last);
      }
    } else {
      found = true;
    }
  }
}

void build_heap(uint32_t *A, uint32_t first, uint32_t last) {
  for (uint32_t parent = last / 2; parent > first - 1; parent -= 1) {
    fix_heap(A, parent, last);
  }
}

void heap_sort(uint32_t *A, uint32_t n) {
  uint32_t first = 1;
  uint32_t last = n;
  build_heap(A, first, last);
  for (uint32_t leaf = last; leaf > first; leaf -= 1) {
    swap(&A[first - 1], &A[leaf - 1]);
    fix_heap(A, first, leaf - 1);
  }
}
