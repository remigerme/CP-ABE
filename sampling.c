#include "sampling.h"

sampler create_sampler() {
    CSPRNG rng = create_csprng();
    dgs_disc_gauss_dp_t *D = create_dgs();
    sampler s = {rng, D};
    return s;
}

void sample_Rq_uniform(poly a, sampler s) {
    for (int i = 0; i < PARAM_N; i++) {
        a[i] = uniform_mod_q(s.rng);
    }
}

void sample_Zq_uniform_matrix(matrix A, int d1, int d2, sampler s) {
    for (int i = 0; i < d1; i++)
        for (int j = 0; j < d2; j++)
            *matrix_element(A, d2, i, j) = uniform_mod_q(s.rng);
}

void sample_Rq_uniform_matrix(poly_matrix A, int d1, int d2, sampler s) {
    for (int i = 0; i < d1; i++) {
        for (int j = 0; j < d2; j++) {
            poly a = poly_matrix_element(A, d2, i, j);
            sample_Rq_uniform(a, s);
        }
    }
}

signed_scalar sample_Z_centered(sampler s) { return signed_scalar_dgs(s.D); }

void sample_Zq_centered_matrix(signed_matrix A, int d1, int d2, sampler s) {
    for (int i = 0; i < d1; i++)
        for (int j = 0; j < d2; j++)
            *matrix_element(A, d2, i, j) = sample_Z_centered(s);
}

void sample_R_centered(signed_poly a, sampler s) {
    for (int i = 0; i < PARAM_N; i++) {
        a[i] = signed_scalar_dgs(s.D);
    }
}

void sample_R_centered_matrix(signed_poly_matrix A, int d1, int d2, sampler s) {
    for (int i = 0; i < d1; i++) {
        for (int j = 0; j < d2; j++) {
            signed_poly a = poly_matrix_element(A, d2, i, j);
            sample_R_centered(a, s);
        }
    }
}
