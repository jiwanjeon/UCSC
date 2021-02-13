#include "set.h"

Set set_empty(void) {
    Set s = 0;
    return s;
}

bool set_member(Set s, uint8_t x) {
    return s & (1 << x);
}

Set set_insert(Set s, uint8_t x) {
    s |= (1 << x);
    return s;
}

Set set_remove(Set s, uint8_t x) {
    s &= ~(1 << x);
    return s;
}

Set set_intersect(Set s, Set t) {
    return s & t;
}

Set set_union(Set s, Set t) {
    return s | t;
}

Set set_complement(Set s) {
    return ~s;
}

Set set_difference(Set s, Set t) {
    return s & set_complement(t);
}
