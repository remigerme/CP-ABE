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
for any matrix A ∈ Zq^{n×m}, it holds that G*G−1(A) = A.
TODO : do it in place ?
Might be smart to copy the K inputs and allow ourselves
to modify them, and create temp matrices on the stack
for performance. TODO : check if there is a risk of
segfault (stack is set to 8Mo on this specific arch,
but let's say it's really risky if we reach 1Mo).
*/
poly_matrix nand(poly_matrix A, poly_matrix B) {
    // Warning : this is stack allocated, beware not to segfault
    scalar temp[PARAM_N * PARAM_K];
    // We need to heap-allocate for the matrix to survive
    poly_matrix R = malloc(sizeof(scalar) * PARAM_N * PARAM_K);
    compute_inv_G(B, R);          // R <- G^-1(B)
    poly_matrix_mul(A, R, temp);  // temp <- A * R = A * G^-1(B)
    poly_matrix_sub(temp, G, R);  // R <- temp - G = A * G^-1(B) - G
    return R;
}

// Returns Af = f(A) = f(A1, ..., Ak)
poly_matrix apply_f(circuit f, poly_matrix A) {
    if (f.left && f.right) {
        return nand(apply_f(*f.left, A), apply_f(*f.right, A));
    } else if ((!f.left) && (!f.right)) {
        return copy_poly_matrix(poly_matrix_element(A, 1, f.n, 0), d1,
                                d2);  // A fresh copy of An
    } else {
        // Help ! This is not supposed to happen.
        return (void *)0;  // Good luck trying to recover from that.
    }
}

void compute_Af(poly_matrix A, circuit f, poly_matrix Af) {
    Af = apply_f(f, A);
}
