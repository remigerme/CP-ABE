#include <assert.h>
#include <stdio.h>

#include "attribute.h"
#include "bgg.h"
#include "circuit.h"
#include "matrix.h"
#include "sampling.h"

int main() {
    sampler s = create_sampler();
    init_G();
    real start, end;

    // Printing parameters
    printf("Testing BGG with parameters\n");
    printf("\tN = %d\n", PARAM_N);
    printf("\tK = %d\n", PARAM_K);
    printf("\tL = %d\n", PARAM_L);
    printf("\tA matrixes are size : N * L = %d\n", PARAM_N * PARAM_L);
    printf("\tT matrixes are size : L * L = %d\n", PARAM_L * PARAM_L);
    printf("\tH matrixes are size : K * L * L = %d\n",
           PARAM_K * PARAM_L * PARAM_L);

    circuit f;
    circuit g;
    circuit h;
    f.left = &g;
    f.right = &h;
    g.left = g.right = NULL;
    h.left = h.right = NULL;
    g.n = 1;
    h.n = 2;

    printf("Circuit : ");
    print_circuit(f);
    printf("\n");

    bgg_keys keys;
    CHRONO("BGG.KeyGen in %fs\n", { keys = BGG_KeyGen(f, s); });

    matrix Af = compute_Af(keys.A, f);
    matrix res = new_matrix(PARAM_N, PARAM_L);
    mul_matrix_trap(Af, keys.Tf, res);
    assert(equals(res, keys.A[0]));
    free_matrix(Af);
    free_matrix(res);

    matrix* CTf_0;
    matrix* CTf_1;
    CHRONO("Computing CTf for u = 0 in %fs\n",
           CTf_0 = BGG_OfflineEnc(keys.A, 0, s););
    CHRONO("Computing CTf for u = 1 in %fs\n",
           CTf_1 = BGG_OfflineEnc(keys.A, 1, s););

    free_signed_matrix(keys.Tf);
    free_matrixes(keys.A, PARAM_K + 1);
    free_matrixes(CTf_0, 2 * PARAM_K + 1);
    free_matrixes(CTf_1, 2 * PARAM_K + 1);
    free_matrix(G);
}