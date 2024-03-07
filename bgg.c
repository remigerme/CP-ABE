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

void BGG_OfflineEnc(poly_matrix A, sampler s, poly_matrix CTf) {}
