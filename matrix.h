#pragma once

#include "common.h"

// Returns a fresh heap-allocated copy of A
matrix copy_matrix(matrix A, int d1, int d2);
poly_matrix copy_poly_matrix(poly_matrix A, int d1, int d2);

// R <- A + B
void add_matrix(matrix A, matrix B, matrix R, int d1, int d2);
void add_poly_matrix(poly_matrix A, poly_matrix B, poly_matrix R, int d1,
                     int d2);

// R <- A - B
void sub_matrix(matrix A, matrix B, matrix R, int d1, int d2);
void sub_poly_matrix(poly_matrix A, poly_matrix B, poly_matrix R, int d1,
                     int d2);

// R <- A * B
void mul_matrix(matrix A, matrix B, matrix R, int d1, int d2, int d3);
void mul_poly_matrix(poly_matrix A, poly_matrix B, poly_matrix R, int d1,
                     int d2, int d3);
