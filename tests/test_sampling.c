#include <stdio.h>
#include <time.h>

#include "common.h"
#include "matrix.h"
#include "sampling.h"

real mean(matrix A) {
    real m = 0;
    for (int i = 0; i < A.rows; i++)
        for (int j = 0; j < A.columns; j++) m += (real)matrix_element(A, i, j);
    return m / (real)(A.rows * A.columns);
}

real meanbis(signed_matrix A) {
    real m = 0;
    for (int i = 0; i < A.rows; i++)
        for (int j = 0; j < A.columns; j++) m += (real)matrix_element(A, i, j);
    return m / (real)(A.rows * A.columns);
}

real var(signed_matrix A) {
    real m = meanbis(A);
    real p = 0;
    for (int i = 0; i < A.rows; i++)
        for (int j = 0; j < A.columns; j++)
            p += matrix_element(A, i, j) * matrix_element(A, i, j);
    p /= (real)(A.rows * A.columns);
    return p - m * m;
}

int main() {
    init_sampler();
    init_params_default();
    real start, end;

    printf("Testing sampling\n");
    print_params();

    // Checking A matrix vector
    matrix* A = new_matrixes(PARAMS.K, PARAMS.N, PARAMS.L);
    CHRONO("Sampled A vector in %fs ", {
        for (int k = 0; k < PARAMS.K; k++) sample_Zq_uniform_matrix(A[k]);
    });

    real m = 0;
    for (int k = 0; k < PARAMS.K; k++) m += mean(A[k]);
    m /= PARAMS.K;
    real diff = (m - PARAMS.Q / 2.0) / (PARAMS.Q / 2.0);
    printf("diff to expected mean : %f%%\n", 100 * diff);

    real n = 0;
    for (int k = 0; k < PARAMS.K; k++) n += norm(A[k]);
    n /= PARAMS.K;
    printf("Average norm of A matrixes : %f\n", n);

    // Checking a trap T
    signed_matrix T = new_signed_matrix(PARAMS.L, PARAMS.L);
    CHRONO("Sampled T in %fs ", { sample_Z_centered_matrix(T); });
    printf("mean: %f var: %f\n", meanbis(T), var(T));
    printf("Norm of T : %f\n", norm_signed(T));

    free_matrixes(A, PARAMS.K);
    free_signed_matrix(T);

    // Determine how many numbers can be sampled by second
    int N = 1000;
    char output[80];

    // For a uniform matrix
    matrix U = new_matrix(N, N);
    sprintf(output, "Uniformely sampled %d scalars in %%fs\n", N * N);
    CHRONO(output, sample_Zq_uniform_matrix(U););
    free_matrix(U);

    // For a gaussian matrix
    signed_matrix D = new_signed_matrix(N, N);
    sprintf(output, "Gaussianly sampled %d scalars in %%fs\n", N * N);
    CHRONO(output, sample_Z_centered_matrix(D););
    free_signed_matrix(D);
}