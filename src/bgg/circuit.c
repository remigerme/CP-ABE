#include "circuit.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include "common.h"
#include "matrix.h"

/***************/
/* Computing G */
/***************/

matrix G;

void init_G() {
    // We need that L = N * K
    G = new_matrix(PARAMS.N, PARAMS.L);
    for (int i = 0; i < PARAMS.N; i++)
        for (int k = 0; k < PARAMS.K; k++)
            matrix_element(G, i, i * PARAMS.K + k) = (1 << k) % PARAMS.Q;
}

// R <- G^-1(A)
void inv_G(matrix A, matrix R) {
    for (int n = 0; n < PARAMS.N; n++) {
        for (int l = 0; l < PARAMS.L; l++) {
            for (int k = 0; k < PARAMS.K; k++) {
                scalar Anl = matrix_element(A, n, l);
                matrix_element(R, n * PARAMS.K + k, l) = (Anl >> k) & 1;
            }
        }
    }
}

/*****************/
/* Circuit utils */
/*****************/

circuit* new_circuit() {
    circuit* f = calloc(1, sizeof(circuit));
    f->left = NULL;
    f->right = NULL;
    f->n = 0;
    return f;
}

void free_circuit(circuit* f) {
    assert((f->left && f->right) || (!f->left || !f->right));

    if (!f->left && !f->right) {
        free(f);
        return;
    }

    // Beware do not double free
    if (f->left == f->right)
        free_circuit(f->left);
    else {
        free_circuit(f->left);
        free_circuit(f->right);
    }
    free(f);
}

void print_circuit(circuit f) {
    assert((f.left && f.right) || (!f.left || !f.right));

    if (!f.left && !f.right) {
        printf("%d", f.n);
        return;
    }

    printf("(");
    print_circuit(*f.left);
    printf(" ^ ");
    print_circuit(*f.right);
    printf(")");
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
    matrix R = new_matrix(PARAMS.N, PARAMS.L);
    matrix temp = new_matrix(PARAMS.L, PARAMS.L);
    inv_G(B, temp);          // temp <- G^-1(B)
    mul_matrix(A, temp, R);  // R <- A * temp = A * G^-1(B)
    sub_matrix(R, G, R);     // R <- R - G = A * G^-1(B) - G
    free_matrix(temp);
    return R;
}

// Returns Af = f(A) = f(A1, ..., Ak)
matrix compute_Af(matrix* A, circuit f) {
    assert((f.left && f.right) || (!f.left || !f.right));

    if (!f.left && !f.right) return copy_matrix(A[f.n]);  // A fresh copy of An

    // f.left may be equal to f.right (exact same circuit cause exact same
    // pointer) then we only need to compute recursively on *f.left but if
    // f.left != f.right we need to recursively compute on *f.right too
    matrix R_left, R_right;
    R_left = R_right = compute_Af(A, *f.left);
    if (f.left != f.right) R_right = compute_Af(A, *f.right);
    matrix R = nand(R_left, R_right);

    free_matrix(R_left);
    if (f.left != f.right) free_matrix(R_right);

    return R;
}

/***************/
/* Computing H */
/***************/

typedef struct H_triplet {
    matrix A;
    bool x;
    matrix H;
} H_triplet;

H_triplet new_H_triplet() {
    matrix A = new_matrix(PARAMS.N, PARAMS.L);
    matrix H = new_matrix(PARAMS.K * PARAMS.L, PARAMS.L);
    H_triplet t;
    t.A = A;
    t.x = 0;
    t.H = H;
    return t;
}

void free_H_triplet(H_triplet* t) {
    free_matrix(t->A);
    free_matrix(t->H);
}

H_triplet leaf(matrix* A, attribute x, int n) {
    H_triplet t = new_H_triplet();
    t.A = copy_matrix(A[n]);
    t.x = get_xn(x, n);
    // H seen as a column is empty except
    // in n-th position which is the identity
    for (int i = 0; i < PARAMS.L; i++)
        matrix_element(t.H, (n - 1) * PARAMS.L + i, i) = 1;
    return t;
}

H_triplet compute_H_triplet(matrix* A, circuit f, attribute x) {
    assert((f.left && f.right) || (!f.left || !f.right));

    if (!f.left && !f.right) return leaf(A, x, f.n);

    // f.left may be equal to f.right (exact same circuit cause exact same
    // pointer) then we only need to compute recursively on *f.left but if
    // f.left != f.right we need to recursively compute on *f.right too
    H_triplet tl, tr;
    tl = tr = compute_H_triplet(A, *f.left, x);
    if (f.left != f.right) tr = compute_H_triplet(A, *f.right, x);

    H_triplet t = new_H_triplet();

    matrix inv = new_matrix(PARAMS.L, PARAMS.L);
    matrix tempA = new_matrix(PARAMS.N, PARAMS.L);
    matrix tempH = new_matrix(PARAMS.K * PARAMS.L, PARAMS.L);

    // Computing new A = Al * G^-1(Ar) - G
    inv_G(tr.A, inv);
    mul_matrix(tl.A, inv, tempA);
    sub_matrix(tempA, G, tempA);
    t.A = copy_matrix(tempA);

    // Computing new x = 1 - xl * xr
    t.x = 1 - tl.x * tr.x;

    // Computing new H = Hl * G^-1(Ar) - xl * Hr
    mul_matrix(tl.H, inv, tempH);
    if (tl.x) sub_matrix(tempH, tr.H, tempH);
    t.H = copy_matrix(tempH);

    // Free time !
    free_matrix(inv);
    free_matrix(tempA);
    free_matrix(tempH);
    free_H_triplet(&tl);
    if (f.left != f.right) free_H_triplet(&tr);

    return t;
}

matrix compute_H(matrix* A, circuit f, attribute x) {
    H_triplet t = compute_H_triplet(A, f, x);
    matrix H = copy_matrix(t.H);
    free_H_triplet(&t);
    return H;
}

bool compute_f(circuit f, attribute x) {
    assert((f.left && f.right) || (!f.left || !f.right));

    if (!f.left && !f.right) return get_xn(x, f.n);

    // f.left may be equal to f.right (exact same circuit cause exact same
    // pointer) then we only need to compute recursively on *f.left but if
    // f.left != f.right we need to recursively compute on *f.right too
    bool xl, xr;
    xl = xr = compute_f(*f.left, x);
    if (f.left != f.right) xr = compute_f(*f.right, x);
    return 1 - xl * xr;
}
