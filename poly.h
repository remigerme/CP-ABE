#pragma once

#include "common.h"

const scalar POLY_ZERO[PARAM_N];

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
