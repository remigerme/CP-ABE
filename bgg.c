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
    // DIM_TODO probably = PARAM_L
    sample_Zq_centered_matrix(Tf, DIM_TODO, DIM_TODO, s);

    // Compute A0
    // TODO : check types (signed)
    mul_matrix(Af, Tf, A, 1, DIM_TODO, DIM_TODO);
}

void BGG_OfflineEnc(poly_matrix A, bool u, sampler s, poly_matrix CTf) {
    int DIM_TODO = 2;

    // Generating LWE uniform secret
    scalar secret[PARAM_N];
    sample_Rq_uniform_matrix(secret, 1, PARAM_N, s);

    // Short gaussian error vector
    signed E[DIM_TODO];
    sample_R_centered_matrix(E, DIM_TODO, DIM_TODO, s);

    // Instantiating first term of CTf (`u` in p.12)
    if (u) {
        sample_Rq_uniform_matrix(CTf, DIM_TODO, DIM_TODO, s);
    } else {
        poly_matrix A0 = A;
        mul_poly_matrix(A0, secret, CTf, DIM_TODO, DIM_TODO, DIM_TODO);
        // TODO : check types (signed)
        add_poly_matrix(CTf, E, CTf, DIM_TODO, DIM_TODO);
    }
}
