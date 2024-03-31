#include "bgg.h"

#include "matrix.h"

#define G 2  // temp placeholder

void BGG_KeyGen(circuit f, sampler s, matrix *A, signed_matrix Tf) {
    // Generate A1, ..., Ak uniformely over Zq^{n * l}
    for (int i = 0; i < PARAM_K; i++) sample_Zq_uniform_matrix(A[i + 1], s);

    // Compute Af
    matrix Af = new_matrix(PARAM_N, PARAM_L);
    compute_Af(A, f, Af);

    // Generate Tf
    sample_Z_centered_matrix(Tf, s);

    // Compute A0
    mul_matrix_trap(Af, Tf, A[0]);
}

void BGG_OfflineEnc(matrix *A, bool u, sampler s, matrix *CTf) {
    // Generating LWE uniform secret
    matrix S = new_matrix(PARAM_M, PARAM_N);
    sample_Zq_uniform_matrix(S, s);

    // Short gaussian error vector
    signed_matrix E = new_signed_matrix(PARAM_M, PARAM_L);
    sample_Z_centered_matrix(E, s);

    // Instantiating first term of CTf (`u` in p.12 or C0 in p.13)
    matrix CTf_0 = new_matrix(PARAM_M, PARAM_L);
    if (u) {
        sample_Zq_uniform_matrix(CTf_0, s);
    } else {
        mul_matrix(A[0], S, CTf_0);
        add_matrix_error(CTf_0, E, CTf_0);
    }
    CTf[0] = CTf_0;

    // Computing S * G only once
    matrix SG = new_matrix(PARAM_M, PARAM_L);
    mul_matrix(S, G, SG);

    for (int i = 0; i < PARAM_K; i++) {
        for (int b = 0; b < 2; b++) {
            matrix R = new_matrix(PARAM_N, PARAM_L);
            sample_Z_centered_matrix(E, s);
            mul_matrix(S, A[1 + i], R);
            if (b) add_matrix(R, SG, R);
            add_matrix_error(R, E, R);
            CTf[1 + 2 * i + b] = R;
        }
    }

    free_matrix(SG);
}
