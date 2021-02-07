#include "hamming.h"

// initialize the hamming code matrix
ham_rc ham_init(void) { return HAM_OK; }
// Destory the hamming code matrix
void ham_destroy(void) {}
// Encode the data using hamming code
ham_rc ham_encode(uint8_t data, uint8_t *code) { return HAM_OK; }
// Decode the data using hamming code
ham_rc ham_decode(uint8_t code, uint8_t *data) { return HAM_OK; }
