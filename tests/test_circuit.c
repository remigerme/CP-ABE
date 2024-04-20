#include <assert.h>
#include <stdio.h>
#include <time.h>

#include "attribute.h"
#include "circuit.h"
#include "common.h"
#include "gen_circuit.h"
#include "matrix.h"
#include "sampling.h"

int main() {
    init_sampler();
    init_params_default();
    init_G();
    real start, end;

    // Printing parameters
    printf("Testing circuit with parameters\n");
    print_params();

    // Generating A
    matrix* A = new_matrixes(PARAMS.K + 1, PARAMS.N, PARAMS.L);
    CHRONO("Generated A in %fs\n", {
        for (int i = 0; i < PARAMS.K + 1; i++) sample_Zq_uniform_matrix(A[i]);
    });

    // Testing G * G^-1(A) = A
    matrix inv = new_matrix(PARAMS.L, PARAMS.L);
    matrix res = new_matrix(PARAMS.N, PARAMS.L);
    CHRONO("Checked G * G^-1(A) = A in %fs\n", {
        inv_G(A[0], inv);
        mul_matrix(G, inv, res);
        assert(equals(A[0], res));
    });
    free_matrix(inv);
    free_matrix(res);

    // Example hand-crafted circuit
    // f(x) = not(x1 | (x2 & x3)) = not(x1) & (not(x2) | not(x3))
    // 2nd version gives extremely similar results
    circuit* f = circuit_not(circuit_or(
        gen_leaf(1, true), circuit_and(gen_leaf(2, true), gen_leaf(3, true))));

    printf("Circuit : ");
    print_circuit(*f);
    printf("\n");

    matrix Af = compute_Af(A, *f);

    matrix T = new_matrix(PARAMS.N, PARAMS.L);
    matrix BIG = new_matrix(PARAMS.N, PARAMS.L * PARAMS.K);

    int x_max = 1;
    for (int i = 0; i < PARAMS.K; i++) x_max *= 2;
    char output[80];

    for (attribute x = 0; x < x_max; x++) {
        printf("f(%d)=%d\n", x, compute_f(*f, x));
        sprintf(output, "BIG * H = Af + f(x)G for x = %d : done in %%fs\n", x);
        CHRONO(output, {
            matrix H = compute_H(A, *f, x);
            printf("Norm H : %f\n", norm(H));
            matrix R = copy_matrix(Af);
            if (compute_f(*f, x)) add_matrix(R, G, R);

            for (int i = 1; i < PARAMS.K + 1; i++) {
                matrix ti = copy_matrix(A[i]);
                if (get_xn(x, i)) add_matrix(ti, G, ti);
                for (int j = 0; j < PARAMS.N; j++)      // ti.rows = PARAMS.N
                    for (int k = 0; k < PARAMS.L; k++)  // ti.columns = PARAMS.L
                        matrix_element(BIG, j, (i - 1) * PARAMS.L + k) =
                            matrix_element(ti, j, k);
                free_matrix(ti);
            }
            mul_matrix(BIG, H, T);

            assert(equals(R, T));
            free_matrix(H);
            free_matrix(R);
        });
    }

    free_matrix(BIG);
    free_matrix(T);

    free_matrix(Af);

    free_matrixes(A, PARAMS.K + 1);
    free_matrix(G);
}