#include "bgg.h"

#include "matrix.h"

#define G 2  // temp placeholder

void BGG_KeyGen(circuit f, sampler s, matrix *A, signed_matrix Tf) {
    // Generate A1, ..., Ak uniformely over Zq^{n * l}
    for (int i = 0; i < PARAM_K; i++)
        sample_Zq_uniform_matrix(A[i + 1], PARAM_N, PARAM_L, s);

    // Compute Af
    scalar Af[PARAM_N * PARAM_L];
    compute_Af(A + 1, f, Af);

    // Generate Tf
    sample_Z_centered_matrix(Tf, PARAM_L, PARAM_L, s);

    // Compute A0
    mul_matrix_trap(Af, Tf, A[0], PARAM_N, PARAM_L);
}

void BGG_OfflineEnc(matrix *A, bool u, sampler s, matrix *CTf) {
    // Generating LWE uniform secret
    scalar S[PARAM_M * PARAM_N];
    sample_Zq_uniform_matrix(S, PARAM_M, PARAM_N, s);

    // Short gaussian error vector
    signed_scalar E[PARAM_M * PARAM_L];
    sample_Z_centered_matrix(E, PARAM_M, PARAM_L, s);

    // Instantiating first term of CTf (`u` in p.12 or C0 in p.13)
    if (u) {
        sample_Zq_uniform_matrix(CTf[0], PARAM_M, PARAM_L, s);
    } else {
        mul_matrix(A[0], S, CTf[0], PARAM_M, PARAM_N, PARAM_L);
        add_matrix_error(CTf[0], E, CTf[0], PARAM_M, PARAM_L);
    }

    // Computing S * G only once
    scalar SG[PARAM_M * PARAM_L];
    mul_matrix(S, G, SG, PARAM_M, PARAM_N, PARAM_L);

    for (int i = 0; i < PARAM_K; i++) {
        for (int b = 0; b < 2; b++) {
            matrix R = zero(PARAM_N, PARAM_L);
            sample_Z_centered_matrix(E, PARAM_M, PARAM_L, s);
            mul_matrix(S, A[1 + i], R, PARAM_M, PARAM_N, PARAM_L);
            if (b) add_matrix(R, SG, R, PARAM_M, PARAM_L);
            add_matrix_error(R, E, R, PARAM_M, PARAM_L);
            CTf[1 + 2 * i + b] = R;
        }
    }
}
