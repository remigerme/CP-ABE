#pragma once

#include "common.h"

// Polynomial in standard (coefficients) representation
typedef scalar *poly;
typedef signed_scalar *signed_poly;

// Matrix of polynomials (single pointer on scalars, everything is accessed
// through macros)
typedef scalar *poly_matrix;
typedef signed_scalar *signed_poly_matrix;

// Accessing polynomial Mi,j
#define poly_matrix_element(M, nb_col, i, j) \
    (&M[(PARAM_N) * (((i) * (nb_col)) + (j))])

extern poly POLY_ZERO;

/*
Functions for `poly` which live in Rq
*/

// r <- a
void copy_poly(poly a, poly r);

// r <- a + b
void add_poly(poly a, poly b, poly r);

// r <- a - b
void sub_poly(poly a, poly b, poly r);

// r <- a * b
void mul_poly(poly a, poly b, poly r);
