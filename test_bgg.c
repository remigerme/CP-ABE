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

    matrix* A = new_matrixes(PARAM_K + 1, PARAM_N, PARAM_L);
    signed_matrix Tf = new_signed_matrix(PARAM_L, PARAM_L);

    circuit f;
    circuit g;
    circuit h;
    f.left = &g;
    f.right = &h;
    g.left = g.right = NULL;
    h.left = h.right = NULL;
    g.n = 1;
    h.n = 2;

    BGG_KeyGen(f, s, A, Tf);

    matrix Af = compute_Af(A, f);
    matrix res = new_matrix(PARAM_N, PARAM_L);
    mul_matrix_trap(Af, Tf, res);
    assert(equals(res, A[0]));
    free_matrix(Af);
    free_matrix(res);

    matrix* Ctf_0 = BGG_OfflineEnc(A, 0, s);
    matrix* Ctf_1 = BGG_OfflineEnc(A, 1, s);

    free_signed_matrix(Tf);
    free_matrixes(A, PARAM_K + 1);
    free_matrix(G);

    printf("Test BGG : done\n");
}