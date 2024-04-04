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

    circuit f;
    circuit g;
    circuit h;
    f.left = &g;
    f.right = &h;
    g.left = g.right = NULL;
    h.left = h.right = NULL;
    g.n = 1;
    h.n = 2;

    bgg_keys keys = BGG_KeyGen(f, s);

    matrix Af = compute_Af(keys.A, f);
    matrix res = new_matrix(PARAM_N, PARAM_L);
    mul_matrix_trap(Af, keys.Tf, res);
    assert(equals(res, keys.A[0]));
    free_matrix(Af);
    free_matrix(res);

    matrix* Ctf_0 = BGG_OfflineEnc(keys.A, 0, s);
    matrix* Ctf_1 = BGG_OfflineEnc(keys.A, 1, s);

    free_signed_matrix(keys.Tf);
    free_matrixes(keys.A, PARAM_K + 1);
    free_matrix(G);

    printf("Test BGG : done\n");
}