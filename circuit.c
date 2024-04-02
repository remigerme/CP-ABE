#include "circuit.h"

#include <stdbool.h>
#include <stdlib.h>

#include "common.h"
#include "matrix.h"

matrix G;

void init_G() {
    // We need that L = N * K
    G = new_matrix(PARAM_N, PARAM_L);
    for (int i = 0; i < PARAM_N; i++) {
        scalar v = 1;
        for (int k = 0; k < PARAM_K; k++) {
            matrix_element(G, i, i * PARAM_K + k) = v;
            v *= 2;
        }
    }
}

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
    mul_matrix(A, temp, R);  // R <- A * temp = A * G^-1(B)
    sub_matrix(R, G, R);     // R <- R - G = A * G^-1(B) - G
    free_matrix(temp);
    return R;
}

// Returns Af = f(A) = f(A1, ..., Ak)
matrix compute_Af(matrix* A, circuit f) {
    if (f.left && f.right) {
        matrix R_left = compute_Af(A, *f.left);
        matrix R_right = compute_Af(A, *f.right);
        matrix R = nand(R_left, R_right);
        free_matrix(R_left);
        free_matrix(R_right);
        return R;
    } else if (!f.left && !f.right) {
        matrix An = A[f.n];
        return copy_matrix(An);  // A fresh copy of An
    } else {
        // Help ! This is not supposed to happen.
        return (void*)0;  // Good luck trying to recover from that.
    }
}

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
    t->x = get_xn(x, n);
    // H seen as a column is empty except
    // in n-th position which is the identity
    for (int i = 0; i < PARAM_L; i++)
        matrix_element(t->H, (n - 1) * PARAM_L + i, i) = 1;
    return t;
}

H_triplet* compute_H_triplet(matrix* A, circuit f, attribute x) {
    if (!f.left && !f.right)
        return leaf(A, x, f.n);

    else if (!f.left || !f.right)
        // Help ! This is not supposed to happen.
        return (void*)0;  // Good luck trying to recover from that.

    H_triplet* tl = compute_H_triplet(A, *f.left, x);
    H_triplet* tr = compute_H_triplet(A, *f.right, x);

    H_triplet* t = new_H_triplet();

    matrix inv = new_matrix(PARAM_L, PARAM_L);
    matrix tempA = new_matrix(PARAM_N, PARAM_L);
    matrix tempH = new_matrix(PARAM_K * PARAM_L, PARAM_L);
    matrix tempHbis = new_matrix(PARAM_K * PARAM_L, PARAM_L);

    // Computing new A = Al * G^-1(Ar)
    inv_G(tr->A, inv);
    mul_matrix(tl->A, inv, tempA);
    t->A = copy_matrix(tempA);

    // Computing new x = 1 - xl * xr
    t->x = 1 - tl->x * tr->x;

    // Computing new H = Hl * G^-1(Ar) + xl * Hr
    mul_matrix(tl->H, inv, tempH);
    mul_matrix_scalar(tl->x, tr->H, tempHbis);
    add_matrix(tempH, tempHbis, tempH);
    t->H = copy_matrix(tempH);

    // Free time !
    free_matrix(inv);
    free_matrix(tempA);
    free_matrix(tempH);
    free_H_triplet(tl);
    free_H_triplet(tr);

    return t;
}

matrix compute_H(matrix* A, circuit f, attribute x) {
    H_triplet* t = compute_H_triplet(A, f, x);
    matrix H = copy_matrix(t->H);
    free_H_triplet(t);
    return H;
}
