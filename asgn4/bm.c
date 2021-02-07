#include "bm.h"

#include <stdio.h>
#include <stdlib.h>
/**
 * Create the bitmatrix
 */
BitMatrix *bm_create(uint32_t rows, uint32_t cols) {
    BitMatrix *ptr;
    uint8_t **mat;
    mat = (uint8_t **) calloc(rows, sizeof(uint8_t));
    for (int i = 0; i < rows; i++) {
        mat[i] = (uint8_t *) calloc(cols, sizeof(uint8_t));
    }
    ptr = (BitMatrix *) calloc(rows * cols, sizeof(uint8_t));
    ptr->cols = cols;
    ptr->rows = rows;
    ptr->mat = mat;
    return ptr;
}

/**
 * Delete the bitmatrix
 */
void bm_delete(BitMatrix **m) {
    for (int i = 0; i < (*m)->rows; i++) {
        free((*m)->mat[i]);
    }
    free((*m)->mat);
    free(*m);
    *m = NULL;
}
/**
 *Get the rows
 */
uint32_t bm_rows(BitMatrix *m) {
    return m->rows;
}
}
/**
 * Get the columns
 */
uint32_t bm_cols(BitMatrix *m) {
    return m->cols;
}
/**
 * Set the bit
 */
void bm_set_bit(BitMatrix *m, uint32_t r, uint32_t c) {
    /**
   * Clear the bit
   */
    void bm_clr_bit(BitMatrix * m, uint32_t r, uint32_t c) {
    }

    /**
   * Get the bit
   */
    uint8_t bm_get_bit(BitMatrix * m, uint32_t r, uint32_t c) {
        return m->mat[r][c];
    }
    /**
   * Print the bit matrix
   */
    void bm_print(BitMatrix * m) {
        printf("Number Of rows:%d\n", m->rows);
        printf("Number Of cols:%d\n", m->cols);
        for (uint32_t i = 0; i < m->rows; i++) {
            for (uint32_t j = 0; j < m->cols; j++) {
                printf("%d ", m->mat[i][j]);
            }
            printf("\n");
        }
    }
