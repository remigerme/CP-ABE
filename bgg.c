#include "bgg.h"

#include "matrix.h"

void BGG_KeyGen(circuit f, sampler s, poly_matrix A, signed_poly_matrix Tf) {
    // Generate A1, ..., Ak uniformely over Rq^m
    poly_matrix A_start = poly_matrix_element(A, PARAM_M, 1, 0);
    sample_Rq_uniform_matrix(A_start, PARAM_K, PARAM_M, s);

    // Compute Af
    scalar Af[PARAM_N * PARAM_M];
    compute_Af(A_start, f, Af);

    // Generate Tf
    sample_R_centered_matrix(Tf, PARAM_M, PARAM_M, s);

    // Compute A0
    // TODO : check types (signed)
    mul_poly_matrix(Af, Tf, A, 1, PARAM_M, PARAM_M);
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
