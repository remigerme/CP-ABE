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
    scalar secret[PARAM_N];
    sample_Zq_uniform_matrix(secret, 1, PARAM_N, s);

    // Short gaussian error vector
    signed E[DIM_TODO];
    sample_Z_centered_matrix(E, DIM_TODO, DIM_TODO, s);

    // Instantiating first term of CTf (`u` in p.12)
    if (u) {
        sample_Zq_uniform_matrix(CTf, DIM_TODO, DIM_TODO, s);
    } else {
        matrix A0 = A;
        mul_matrix(A0, secret, CTf, DIM_TODO, DIM_TODO, DIM_TODO);
        // TODO : check types (signed)
        add_matrix(CTf, E, CTf, DIM_TODO, DIM_TODO);
    }
}
