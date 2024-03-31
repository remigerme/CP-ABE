#pragma once

#include "common.h"

// A matrix is a POINTER TO A STRUCT _matrix
typedef struct _matrix {
    unsigned int rows;
    unsigned int columns;
    scalar* data;
}* matrix;

// A signed_matrix is a POINTER TO A STRUCT _signed_matrix
typedef struct _signed_matrix {
    unsigned int rows;
    unsigned int columns;
    signed_scalar* data;
}* signed_matrix;

// Accessing (signed_)scalar Mi,j where M has type (signed_)matrix
#define matrix_element(M, i, j) ((M)->data[(i) * (M)->columns + (j)])

// Returns a heap-allocated matrix filled with 0s
matrix new_matrix(unsigned int rows, unsigned int columns);

// Returns a heap-allocated signed_matrix filled with 0s
signed_matrix new_signed_matrix(unsigned int rows, unsigned int columns);

// Clean free of a matrix : data then the matrix itself
void free_matrix(matrix M);

// Clean free of a signed_matrix : data then the matrix itself
void free_signed_matrix(signed_matrix M);

// Returns a fresh heap-allocated copy of A
matrix copy_matrix(matrix M);

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
