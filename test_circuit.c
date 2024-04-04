#include <stdio.h>

#include "attribute.h"
#include "circuit.h"
#include "matrix.h"
#include "sampling.h"

int main() {
    sampler s = create_sampler();

    init_G();

    matrix* A = new_matrixes(PARAM_K + 1, PARAM_N, PARAM_L);
    for (int i = 0; i < PARAM_K + 1; i++) sample_Zq_uniform_matrix(A[i], s);

    // Testing G * G^-1(A) = A
    printf("G * G^-1(A) = A : ");
    matrix inv = new_matrix(PARAM_L, PARAM_L);
    matrix res = new_matrix(PARAM_N, PARAM_L);
    inv_G(A[0], inv);
    mul_matrix(G, inv, res);
    assert(equals(A[0], res));
    free_matrix(inv);
    free_matrix(res);
    printf("done\n");

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

    matrix T = new_matrix(PARAM_N, PARAM_L);
    matrix BIG = new_matrix(PARAM_N, PARAM_L * PARAM_K);

    int x_max = 1;
    for (int i = 0; i < PARAM_K; i++) x_max *= 2;

    for (attribute x = 0; x < x_max; x++) {
        printf("BIG * H = Af + f(x)G for x = %d : ", x);
        matrix H = compute_H(A, f, x);
        matrix R = copy_matrix(Af);
        if (compute_f(f, x)) add_matrix(R, G, R);

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

        assert(equals(R, T));
        printf("done\n");

        free_matrix(H);
        free_matrix(R);
    }

    free_matrix(BIG);
    free_matrix(T);

    free_matrix(Af);

    free_matrixes(A, PARAM_K + 1);
    free_matrix(G);
}