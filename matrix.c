#include "matrix.h"

#include <stdlib.h>

#include "poly.h"

/* -------------------- */
/* Functions for matrix */
/* -------------------- */

matrix copy_matrix(poly_matrix A, int d1, int d2) {
    poly_matrix R = malloc(sizeof(scalar) * d1 * d2);
    for (int i = 0; i < d1; i++) {
        for (int j = 0; j < d2; j++) {
            matrix_element(R, d2, i, j) = matrix_element(A, d2, i, j);
        }
    }
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

/* ------------------------- */
/* Functions for poly_matrix */
/* ------------------------- */

poly_matrix copy_poly_matrix(poly_matrix A, int d1, int d2) {
    poly_matrix R = malloc(sizeof(scalar) * PARAM_N * d1 * d2);
    for (int i = 0; i < d1; i++) {
        for (int j = 0; j < d2; j++) {
            poly a = poly_matrix_element(A, d2, i, j);
            poly r = poly_matrix_element(R, d2, i, j);
            copy_poly(a, r);
        }
    }
    return R;
}

// R <- A + B
void add_poly_matrix(poly_matrix A, poly_matrix B, poly_matrix R, int d1,
                     int d2) {
    for (int i = 0; i < d1; i++) {
        for (int j = 0; j < d2; j++) {
            poly a = poly_matrix_element(A, d2, i, j);
            poly b = poly_matrix_element(B, d2, i, j);
            poly r = poly_matrix_element(R, d2, i, j);
            add_poly(a, b, r);
        }
    }
}

// R <- A - B
void sub_poly_matrix(poly_matrix A, poly_matrix B, poly_matrix R, int d1,
                     int d2) {
    for (int i = 0; i < d1; i++) {
        for (int j = 0; j < d2; j++) {
            poly a = poly_matrix_element(A, d2, i, j);
            poly b = poly_matrix_element(B, d2, i, j);
            poly r = poly_matrix_element(R, d2, i, j);
            sub_poly(a, b, r);
        }
    }
}

// R <- A * B
void mul_poly_matrix(poly_matrix A, poly_matrix B, poly_matrix R, int d1,
                     int d2, int d3) {
    scalar temp[PARAM_N];  // used to store product A[i][k] * B[k][j]
    for (int i = 0; i < d1; i++) {
        for (int j = 0; j < d3; j++) {
            poly r = poly_matrix_element(R, d3, i, j);
            for (int k = 0; k < d2; k++) {
                poly a = poly_matrix_element(A, d2, i, k);
                poly b = poly_matrix_element(B, d3, k, j);
                mul_poly(a, b, temp);
                if (k == 0) {  // Existing values in r are not relevant
                    add_poly(temp, POLY_ZERO, r);
                } else {
                    add_poly(r, temp, r);
                }
            }
        }
    }
}
