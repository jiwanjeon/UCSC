#include "stack.h"

#include <stdio.h>
#include <stdlib.h>

struct Stack {
  uint32_t top;      // Points to the next empty slot.
  uint32_t capacity; // Number of items that can be pushed .
  int64_t *items;    // Holds the items.
};

Stack *stack_create(void) {
  Stack *stack = calloc(1, sizeof(*stack));
  if (stack == NULL)
    return NULL;
  stack->capacity = MIN_CAPACITY;
  stack->top = 0;
  stack->items = calloc(stack->capacity, sizeof(*stack->items));
  if (stack->items == NULL)
    return NULL;
  return stack;
}

void stack_delete(Stack **s) {
  if (s && *s) {
    free((*s)->items);
    free(*s);
    *s = NULL;
  }
}

uint32_t stack_size(Stack *s) { return s->top; }

bool stack_empty(Stack *s) { return s->top == 0; }

bool stack_push(Stack *s, int64_t x) {
  if (s->top == s->capacity) {
    s->capacity *= 2;
    s->items = realloc(s->items, s->capacity * sizeof(*s->items));
    if (s->items == NULL)
      return false;
  }
  s->items[s->top++] = x;
  return true;
}

bool stack_pop(Stack *s, int64_t *x) {
  if (stack_empty(s))
    return false;
  *x = s->items[--s->top];
  return true;
}

void stack_print(Stack *s) {
  for (uint32_t i = 0; i < s->top; ++i) {
    printf("%li ", s->items[i]);
  }
  printf("\n");
}
