#include <assert.h>
#include <stdio.h>

#include "attribute.h"
#include "bgg.h"
#include "circuit.h"
#include "common.h"
#include "matrix.h"
#include "sampling.h"

int main() {
    init_sampler();
    init_params_default();
    init_G();
    real start, end;

    // Printing parameters
    printf("Testing BGG with parameters\n");
    print_params();

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
    CHRONO("BGG.KeyGen in %fs\n", { keys = BGG_KeyGen(f); });

    matrix Af = compute_Af(keys.A, f);
    matrix res = new_matrix(PARAMS.N, PARAMS.L);
    mul_matrix_trap(Af, keys.Tf, res);
    assert(equals(res, keys.A[0]));
    free_matrix(Af);
    free_matrix(res);

    matrix* CTf_0;
    matrix* CTf_1;
    CHRONO("Computing CTf for u = 0 in %fs\n",
           CTf_0 = BGG_OfflineEnc(keys.A, 0););
    CHRONO("Computing CTf for u = 1 in %fs\n",
           CTf_1 = BGG_OfflineEnc(keys.A, 1););

    free_signed_matrix(keys.Tf);
    free_matrixes(keys.A, PARAMS.K + 1);
    free_matrixes(CTf_0, 2 * PARAMS.K + 1);
    free_matrixes(CTf_1, 2 * PARAMS.K + 1);
    free_matrix(G);
}