#include "matrix.h"

#include <stdlib.h>

matrix* new_matrix(unsigned int rows, unsigned int columns) {
    scalar* data = calloc(rows * columns, sizeof(scalar));
    matrix* R = calloc(1, sizeof(matrix));
    R->rows = rows;
    R->columns = columns;
    R->data = data;
    return R;
}

void free_matrix(matrix* M) {
    free(M->data);
    free(M);
}

matrix* copy_matrix(matrix* M) {
    matrix* R = new_matrix(M->rows, M->columns);
    for (int i = 0; i < M->rows; i++)
        for (int j = 0; j < M->columns; j++)
            matrix_element(R, i, j) = matrix_element(M, i, j);
    return R;
}

// R <- A + B
void add_matrix(matrix A, matrix B, matrix R, int d1, int d2) {
    for (int i = 0; i < d1; i++) {
        for (int j = 0; j < d2; j++) {
            scalar a = matrix_element(A, d2, i, j);
            scalar b = matrix_element(B, d2, i, j);
            matrix_element(R, d2, i, j) = (a + b) % PARAM_Q;
        }
    }
}

// R <- A + E
void add_matrix_error(matrix A, signed_matrix E, matrix R, int d1, int d2) {
    for (int i = 0; i < d1; i++) {
        for (int j = 0; j < d2; j++) {
            signed_scalar a = (signed_scalar)matrix_element(A, d2, i, j);
            signed_scalar b = matrix_element(E, d2, i, j);
            signed_scalar r = (a + b) % PARAM_Q;
            // To be sure r >= 0
            if (r < 0) r += PARAM_Q;
            matrix_element(R, d2, i, j) = (scalar)r;
        }
    }
}

// R <- A - B
void sub_matrix(matrix A, matrix B, matrix R, int d1, int d2) {
    for (int i = 0; i < d1; i++) {
        for (int j = 0; j < d2; j++) {
            scalar a = matrix_element(A, d2, i, j);
            scalar b = matrix_element(B, d2, i, j);
            // Adding Q artificially to avoid underflow
            matrix_element(R, d2, i, j) = (PARAM_Q + a - b) % PARAM_Q;
        }
    }
}

// R <- A * B
void mul_matrix(matrix A, matrix B, matrix R, int d1, int d2, int d3) {
    for (int i = 0; i < d1; i++) {
        for (int j = 0; j < d3; j++) {
            scalar r = 0;
            for (int k = 0; k < d2; k++) {
                scalar a = matrix_element(A, d2, i, k);
                scalar b = matrix_element(B, d3, k, j);
                r = (r + a * b) % PARAM_Q;
            }
            matrix_element(R, d3, i, j) = r;
        }
    }
}

// R <- A * Tf where A in Zq^{d1 * d2} and Tf in Z^{d2 * d2}
void mul_matrix_trap(matrix A, signed_matrix Tf, matrix R, int d1, int d2) {
    for (int i = 0; i < d1; i++) {
        for (int j = 0; j < d2; j++) {
            signed_scalar r = 0;
            for (int k = 0; k < d2; k++) {
                signed_scalar a = (signed_scalar)matrix_element(A, d2, i, k);
                signed_scalar b = matrix_element(Tf, d2, k, j);
                r = (r + a * b) % PARAM_Q;
                // To be sure r >= 0
                if (r < 0) r += PARAM_Q;
            }
            matrix_element(R, d2, i, j) = (scalar)r;
        }
    }
}
