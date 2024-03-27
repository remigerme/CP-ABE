#include "bgg.h"

#include "matrix.h"

// Dimension à déterminer à transposition près
int DIM_TODO = 1;

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

void BGG_OfflineEnc(matrix *A, bool u, sampler s, matrix CTf) {
    // Generating LWE uniform secret
    scalar secret[PARAM_M * PARAM_N];
    sample_Zq_uniform_matrix(secret, PARAM_M, PARAM_N, s);

    // Short gaussian error vector
    signed_scalar E[PARAM_M * PARAM_L];
    sample_Z_centered_matrix(E, PARAM_M, PARAM_L, s);

    // Instantiating first term of CTf (`u` in p.12)
    if (u) {
        sample_Zq_uniform_matrix(CTf, PARAM_M, PARAM_L, s);
    } else {
        mul_matrix(A[0], secret, CTf, PARAM_M, PARAM_N, PARAM_L);
        add_matrix_error(CTf, E, CTf, PARAM_M, PARAM_L);
    }
}
