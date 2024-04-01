#include "circuit.h"

#include <stdbool.h>
#include <stdlib.h>

#include "common.h"

#define G 2

// R <- G^-1(A)
void inv_G(matrix A, matrix R) {
    for (int n = 0; n < PARAM_N; n++) {
        for (int l = 0; l < PARAM_L; l++) {
            scalar divisor = 1;
            for (int k = 0; k < PARAM_K; k++) {
                scalar Anl = matrix_element(A, n, l);
                matrix_element(R, n * PARAM_K + k, l) = (Anl / divisor) % 2;
                divisor *= 2;
            }
        }
    }
}

/*
Returns A * G^-1(B) - G
where G is the gadget matrix and G^-1
the inverse function defined as follows in 2020-191
We define the inverse function G−1 : Zq^{n×m} → {0, 1}^{N×m}
which expands each entry a ∈ Zq of the input matrix into
a column of size K = ceil(log q) consisting of the bits of the
binary representation of a. We have the property that
for any matrix A ∈ Zq^{n×l}, it holds that G*G−1(A) = A.
*/
matrix nand(matrix A, matrix B) {
    // We need to heap-allocate for the matrix to survive
    matrix R = new_matrix(PARAM_N, PARAM_L);
    matrix temp = new_matrix(PARAM_L, PARAM_L);
    inv_G(B, temp);          // temp <- G^-1(B)
    matrix_mul(A, temp, R);  // R <- A * temp = A * G^-1(B)
    matrix_sub(R, G, R);     // R <- R - G = A * G^-1(B) - G
    free_matrix(temp);
    return R;
}

// Returns Af = f(A) = f(A1, ..., Ak)
matrix apply_f(matrix* A, circuit f) {
    if (f.left && f.right) {
        matrix R_left = apply_f(A, *f.left);
        matrix R_right = apply_f(A, *f.right);
        matrix R = nand(R_left, R_right);
        free_matrix(R_left);
        free_matrix(R_right);
        return R;
    } else if ((!f.left) && (!f.right)) {
        matrix An = A[f.n];
        return copy_matrix(An);  // A fresh copy of An
    } else {
        // Help ! This is not supposed to happen.
        return (void*)0;  // Good luck trying to recover from that.
    }
}

void compute_Af(matrix* A, circuit f, matrix Af) { Af = apply_f(A, f); }

/***************/
/* Computing H */
/***************/

typedef struct H_triplet {
    matrix A;
    bool x;
    matrix H;
} H_triplet;

H_triplet* new_H_triplet() {
    matrix A = new_matrix(PARAM_N, PARAM_L);
    matrix H = new_matrix(PARAM_K * PARAM_L, PARAM_L);
    H_triplet* t = calloc(1, sizeof(H_triplet));
    t->A = A;
    t->x = 0;
    t->H = H;
    return t;
}

void free_H_triplet(H_triplet* t) {
    free_matrix(t->A);
    free_matrix(t->H);
    free(t);
}

H_triplet* leaf(matrix* A, attribute x, int n) {
    H_triplet* t = new_H_triplet();
    t->A = copy_matrix(A[n]);
    t->x = get_xk(x, n);
    // H seen as a column is empty except
    // in n-th position which is the identity
    for (int i = 0; i < PARAM_L; i++)
        matrix_element(t->H, n * PARAM_L + i, i) = 1;
    return t;
}

H_triplet* compute_H_triplet(matrix* A, circuit f, attribute x) {}

void compute_H(matrix* A, circuit f, attribute x, matrix H) {
    H_triplet* t = compute_H_triplet(A, f, x);
    H = copy_matrix(t->H);
    free_H_triplet(t);
}
