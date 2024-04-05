#include "sampling.h"

sampler create_sampler() {
    CSPRNG rng = create_csprng();
    dgs_disc_gauss_dp_t *D = create_dgs();
    sampler s = {rng, D};
    return s;
}

/* -------------------- */
/* Functions for matrix */
/* -------------------- */

void sample_Zq_uniform_matrix(matrix A, sampler s) {
    for (int i = 0; i < A->rows; i++)
        for (int j = 0; j < A->columns; j++)
            matrix_element(A, i, j) = uniform_mod_q(s.rng);
}

signed_scalar sample_Z_centered(sampler s) { return signed_scalar_dgs(s.D); }

void sample_Z_centered_matrix(signed_matrix A, sampler s) {
    for (int i = 0; i < A->rows; i++)
        for (int j = 0; j < A->columns; j++)
            matrix_element(A, i, j) = sample_Z_centered(s);
}
