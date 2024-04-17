#include "sampling.h"

#include "random.h"

void init_sampler() { random_bytes_init(); }

void TrapGen(matrix* B, signed_matrix T) {
    // Pre trap computation
    signed_matrix rho = new_signed_matrix(PARAM_P, 1);
    signed_matrix mu = new_signed_matrix(PARAM_P, 1);
    sample_Z_centered_matrix(rho);
    sample_Z_centered_matrix(mu);

    // Trap computation : T = [rho | -g + mu | Ip] : size P * M
    for (int i = 0; i < PARAM_P; i++)
        matrix_element(T, i, 0) = matrix_element(rho, i, 0);

    signed_scalar g = 1;
    for (int i = 0; i < PARAM_P; i++) {
        matrix_element(T, i, 1) = -g + matrix_element(mu, i, 0);
        g *= 2;
    }

    for (int i = 0; i < PARAM_P; i++) matrix_element(T, i, i + 2) = 1;

    // B matrixes computation
    // For each matrix Bk
    for (int k = 0; k < 2 * PARAM_K + 1; k++) {
        // We treat each column independantly
        // For each column c of matrix Bk
        for (int c = 0; c < PARAM_N; c++) {
            scalar a = uniform_mod_n(PARAM_Q);

            // Two first terms by hand
            matrix_element(B[k], 0, c) = a;
            matrix_element(B[k], 1, c) = 1;

            // Then use the formula for the PARAM_P last terms
            scalar g = 1;
            for (int i = 0; i < PARAM_P; i++) {
                signed_scalar t = g;
                t -= a * matrix_element(rho, i, 0);
                t -= matrix_element(mu, i, 0);
                t %= PARAM_Q;
                if (t < 0) t += PARAM_Q;
                matrix_element(B[k], i + 2, c) = (scalar)t;
                g *= 2;
            }
        }
    }

    free_signed_matrix(rho);
    free_signed_matrix(mu);
}

signed_matrix TrapSamp(matrix* B, signed_matrix T, attribute x) {
    signed_matrix Tx = new_signed_matrix(PARAM_P, PARAM_M);
    // We simply copy T for now...
    // We give full knowledge instead of specific knowledge related to x
    for (int i = 0; i < PARAM_P; i++) {
        for (int j = 0; j < PARAM_M; j++) {
            matrix_element(Tx, i, j) = matrix_element(T, i, j);
        }
    }
    return Tx;
}

/* -------------------- */
/* Functions for matrix */
/* -------------------- */

void sample_Zq_uniform_matrix(matrix A) {
    for (int i = 0; i < A.rows; i++)
        for (int j = 0; j < A.columns; j++)
            matrix_element(A, i, j) = uniform_mod_n(PARAM_Q);
}

signed_scalar sample_Z_centered() { return algorithmF(0, PARAM_SIGMA); }

void sample_Z_centered_matrix(signed_matrix A) {
    for (int i = 0; i < A.rows; i++)
        for (int j = 0; j < A.columns; j++)
            matrix_element(A, i, j) = sample_Z_centered();
}
