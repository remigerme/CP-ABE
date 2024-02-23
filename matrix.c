#include <stdlib.h>

#include "matrix.h"
#include "poly.h"

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
