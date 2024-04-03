#include <stdio.h>

#include "attribute.h"
#include "circuit.h"
#include "matrix.h"
#include "sampling.h"

int main() {
    sampler s = create_sampler();

    init_G();

    printf("Printing A\n");
    matrix* A = new_matrixes(PARAM_K + 1, PARAM_N, PARAM_L);
    for (int i = 0; i < PARAM_K + 1; i++) {
        sample_Zq_uniform_matrix(A[i], s);
        print_matrix(A[i]);
        printf("\n");
    }

    // Testing G * G^-1(A) = A
    matrix inv = new_matrix(PARAM_L, PARAM_L);
    matrix res = new_matrix(PARAM_N, PARAM_L);
    inv_G(A[0], inv);
    mul_matrix(G, inv, res);
    assert(equals(A[0], res));
    free_matrix(inv);
    free_matrix(res);
    printf("G * G^-1(A) = A : yes\n");

    circuit f;
    circuit g;
    circuit h;
    f.left = &g;
    f.right = &h;
    g.left = g.right = NULL;
    h.left = h.right = NULL;
    g.n = 1;
    h.n = 2;

    matrix Af = compute_Af(A, f);

    attribute x = 7;
    matrix H = compute_H(A, f, x);

    matrix R = copy_matrix(Af);
    if (compute_f(f, x)) add_matrix(R, G, R);

    matrix T = new_matrix(PARAM_N, PARAM_L);
    matrix BIG = new_matrix(PARAM_N, PARAM_L * PARAM_K);
    for (int i = 1; i < PARAM_K + 1; i++) {
        matrix ti = copy_matrix(A[i]);
        if (get_xn(x, i)) add_matrix(ti, G, ti);
        for (int j = 0; j < ti->rows; j++)         // ti->rows = PARAM_N
            for (int k = 0; k < ti->columns; k++)  // ti->columns = PARAM_L
                matrix_element(BIG, j, (i - 1) * PARAM_L + k) =
                    matrix_element(ti, j, k);
        free_matrix(ti);
    }
    mul_matrix(BIG, H, T);

    print_matrix(R);
    printf("\n");

    print_matrix(T);
    printf("\n");

    assert(equals(R, T));

    free_matrix(R);
    free_matrix(BIG);
    free_matrix(T);

    free_matrix(Af);
    free_matrix(H);

    free_matrixes(A, PARAM_K + 1);
    free_matrix(G);
}