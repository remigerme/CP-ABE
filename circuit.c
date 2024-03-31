#include "circuit.h"

#include <stdlib.h>

#define G 2

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
    compute_inv_G(B, temp);  // temp <- G^-1(B)
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
