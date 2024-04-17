#include "bgg.h"

#include "common.h"

bgg_keys BGG_KeyGen(circuit f) {
    // Allocating new matrixes
    matrix* A = new_matrixes(PARAM_K + 1, PARAM_N, PARAM_L);
    signed_matrix Tf = new_signed_matrix(PARAM_L, PARAM_L);

    // Generate A1, ..., Ak uniformely over Zq^{n * l}
    for (int i = 0; i < PARAM_K; i++) sample_Zq_uniform_matrix(A[i + 1]);

    // Compute Af
    matrix Af = compute_Af(A, f);

    // Generate Tf
    sample_Z_centered_matrix(Tf);

    // Compute A0
    mul_matrix_trap(Af, Tf, A[0]);
    free_matrix(Af);

    bgg_keys key = {A, Tf};
    return key;
}

matrix* BGG_OfflineEnc(matrix* A, bool u) {
    matrix* CTf = new_matrixes(2 * PARAM_K + 1, PARAM_M, PARAM_L);

    // Generating LWE uniform secret
    matrix S = new_matrix(PARAM_M, PARAM_N);
    sample_Zq_uniform_matrix(S);

    // Short gaussian error vector
    signed_matrix E = new_signed_matrix(PARAM_M, PARAM_L);
    sample_Z_centered_matrix(E);

    // Instantiating first term of CTf (`u` in p.12 or C0 in p.13)
    if (u)
        sample_Zq_uniform_matrix(CTf[0]);
    else {
        mul_matrix(S, A[0], CTf[0]);
        add_matrix_error(CTf[0], E, CTf[0]);
    }

    // Computing S * G only once
    matrix SG = new_matrix(PARAM_M, PARAM_L);
    mul_matrix(S, G, SG);

    for (int i = 0; i < PARAM_K; i++) {
        for (int b = 0; b < 2; b++) {
            sample_Z_centered_matrix(E);
            mul_matrix(S, A[1 + i], CTf[1 + 2 * i + b]);
            if (b) add_matrix(CTf[1 + 2 * i + b], SG, CTf[1 + 2 * i + b]);
            add_matrix_error(CTf[1 + 2 * i + b], E, CTf[1 + 2 * i + b]);
        }
    }

    free_matrix(S);
    free_signed_matrix(E);
    free_matrix(SG);

    return CTf;
}
