#include "matrix.h"

#include <assert.h>
#include <stdlib.h>

matrix new_matrix(unsigned int rows, unsigned int columns) {
    scalar* data = calloc(rows * columns, sizeof(scalar));
    matrix R = calloc(1, sizeof(matrix));
    R->rows = rows;
    R->columns = columns;
    R->data = data;
    return R;
}

signed_matrix new_signed_matrix(unsigned int rows, unsigned int columns) {
    signed_scalar* data = calloc(rows * columns, sizeof(signed_scalar));
    signed_matrix R = calloc(1, sizeof(signed_matrix));
    R->rows = rows;
    R->columns = columns;
    R->data = data;
    return R;
}

void free_matrix(matrix M) {
    free(M->data);
    free(M);
}

void free_signed_matrix(signed_matrix M) {
    free(M->data);
    free(M);
}

matrix copy_matrix(matrix M) {
    matrix R = new_matrix(M->rows, M->columns);
    for (int i = 0; i < M->rows; i++)
        for (int j = 0; j < M->columns; j++)
            matrix_element(R, i, j) = matrix_element(M, i, j);
    return R;
}

// R <- A + B
void add_matrix(matrix A, matrix B, matrix R) {
    // Check dimensions
    assert(A->rows == B->rows && A->rows == R->rows);
    assert(A->columns == B->columns && A->columns == R->columns);
    // Computing the result
    for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < B->rows; j++) {
            scalar a = matrix_element(A, i, j);
            scalar b = matrix_element(B, i, j);
            matrix_element(R, i, j) = (a + b) % PARAM_Q;
        }
    }
}

// R <- A + E
void add_matrix_error(matrix A, signed_matrix E, matrix R) {
    // Check dimensions
    assert(A->rows == E->rows && A->rows == R->rows);
    assert(A->columns == E->columns && A->columns == R->columns);
    // Computing the result
    for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < A->columns; j++) {
            signed_scalar a = (signed_scalar)matrix_element(A, i, j);
            signed_scalar b = matrix_element(E, i, j);
            signed_scalar r = (a + b) % PARAM_Q;
            // To be sure r >= 0
            if (r < 0) r += PARAM_Q;
            matrix_element(R, i, j) = (scalar)r;
        }
    }
}

// R <- A - B
void sub_matrix(matrix A, matrix B, matrix R) {
    // Check dimensions
    assert(A->rows == B->rows && A->rows == R->rows);
    assert(A->columns == B->columns && A->columns == R->columns);
    // Computing the result
    for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < A->columns; j++) {
            scalar a = matrix_element(A, i, j);
            scalar b = matrix_element(B, i, j);
            // Adding Q artificially to avoid underflow
            matrix_element(R, i, j) = (PARAM_Q + a - b) % PARAM_Q;
        }
    }
}

// R <- A * B
void mul_matrix(matrix A, matrix B, matrix R) {
    // Check dimensions
    assert(A->rows == R->rows);
    assert(A->columns == B->rows);
    assert(B->columns == R->columns);
    // Computing the result
    for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < B->columns; j++) {
            scalar r = 0;
            for (int k = 0; k < A->columns; k++) {
                scalar a = matrix_element(A, i, k);
                scalar b = matrix_element(B, k, j);
                r = (r + a * b) % PARAM_Q;
            }
            matrix_element(R, i, j) = r;
        }
    }
}

// R <- A * Tf where A in Zq^{d1 * d2} and Tf in Z^{d2 * d2}
void mul_matrix_trap(matrix A, signed_matrix Tf, matrix R) {
    // Check dimensions
    assert(A->rows == R->rows);
    assert(A->columns == Tf->rows);
    assert(Tf->columns == R->columns);
    // Computing the result
    for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < Tf->columns; j++) {
            signed_scalar r = 0;
            for (int k = 0; k < A->columns; k++) {
                signed_scalar a = (signed_scalar)matrix_element(A, i, k);
                signed_scalar b = matrix_element(Tf, k, j);
                r = (r + a * b) % PARAM_Q;
                // To be sure r >= 0
                if (r < 0) r += PARAM_Q;
            }
            matrix_element(R, i, j) = (scalar)r;
        }
    }
}
