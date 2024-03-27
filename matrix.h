#pragma once

#include "common.h"

/* -------------------- */
/* Functions for matrix */
/* -------------------- */

// Returns a heap-allocated matrix filled with 0s
matrix zeros(int d1, int d2);

// Returns a fresh heap-allocated copy of A
matrix copy_matrix(matrix A, int d1, int d2);

// R <- A + B
void add_matrix(matrix A, matrix B, matrix R, int d1, int d2);

// R <- A + E
void add_matrix_error(matrix A, signed_matrix E, matrix R, int d1, int d2);

// R <- A - B
void sub_matrix(matrix A, matrix B, matrix R, int d1, int d2);

// R <- A * B
void mul_matrix(matrix A, matrix B, matrix R, int d1, int d2, int d3);

// R <- A * Tf where A in Zq^{d1 * d2} and Tf in Z^{d2 * d2}
void mul_matrix_trap(matrix A, signed_matrix Tf, matrix R, int d1, int d2);

/* ------------------------- */
/* Functions for poly_matrix */
/* ------------------------- */

// Returns a fresh heap-allocated copy of A
poly_matrix copy_poly_matrix(poly_matrix A, int d1, int d2);

// R <- A + B
void add_poly_matrix(poly_matrix A, poly_matrix B, poly_matrix R, int d1,
                     int d2);

// R <- A - B
void sub_poly_matrix(poly_matrix A, poly_matrix B, poly_matrix R, int d1,
                     int d2);

// R <- A * B
void mul_poly_matrix(poly_matrix A, poly_matrix B, poly_matrix R, int d1,
                     int d2, int d3);
