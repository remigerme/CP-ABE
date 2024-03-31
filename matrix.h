#pragma once

#include "common.h"

typedef struct matrix {
    unsigned int rows;
    unsigned int columns;
    scalar* data;
} matrix;

typedef struct signed_matrix {
    unsigned int rows;
    unsigned int columns;
    signed_scalar* data;
} signed_matrix;

// Accessing (signed ?) scalar Mi,j
#define matrix_element(M, i, j) (M[((i) * (M.columns)) + (j)])

// Returns a heap-allocated matrix filled with 0s
matrix zeros_matrix(int rows, int columns);

// Returns a fresh heap-allocated copy of A
matrix copy_matrix(matrix A);

// R <- A + B
void add_matrix(matrix A, matrix B, matrix R);

// R <- A + E
void add_matrix_error(matrix A, signed_matrix E, matrix R);

// R <- A - B
void sub_matrix(matrix A, matrix B, matrix R);

// R <- A * B
void mul_matrix(matrix A, matrix B, matrix R);

// R <- A * Tf where A in Zq^{d1 * d2} and Tf in Z^{d2 * d2}
void mul_matrix_trap(matrix A, signed_matrix Tf, matrix R);
