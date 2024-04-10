#include <stdio.h>
#include <time.h>

#include "common.h"
#include "matrix.h"
#include "sampling.h"

real mean(matrix A) {
    real m = 0;
    for (int i = 0; i < A->rows; i++)
        for (int j = 0; j < A->columns; j++) m += (real)matrix_element(A, i, j);
    return m / (real)(A->rows * A->columns);
}

real meanbis(signed_matrix A) {
    real m = 0;
    for (int i = 0; i < A->rows; i++)
        for (int j = 0; j < A->columns; j++) m += (real)matrix_element(A, i, j);
    return m / (real)(A->rows * A->columns);
}

real var(signed_matrix A) {
    real m = meanbis(A);
    real p = 0;
    for (int i = 0; i < A->rows; i++)
        for (int j = 0; j < A->columns; j++)
            p += matrix_element(A, i, j) * matrix_element(A, i, j);
    p /= (real)(A->rows * A->columns);
    return p - m * m;
}

int main() {
    sampler s = create_sampler();
    real start, end;

    // Printing parameters
    printf("Testing sampling with parameters\n");
    printf("\tQ = %d\n", PARAM_Q);
    printf("\tN = %d\n", PARAM_N);
    printf("\tK = %d\n", PARAM_K);
    printf("\tL = %d\n", PARAM_L);
    printf("\tA matrixes are size : N * L = %d\n", PARAM_N * PARAM_L);
    printf("\tTf matrixes are size : L * L = %d\n", PARAM_L * PARAM_L);
    printf("\tH matrixes are size : K * L * L = %d\n",
           PARAM_K * PARAM_L * PARAM_L);

    // Checking one of the Ai matrix
    matrix A = new_matrix(PARAM_N, PARAM_L);
    CHRONO("Sampled Ai in %fs ", { sample_Zq_uniform_matrix(A, s); });
    real diff = (mean(A) - PARAM_Q / 2.0) / (PARAM_Q / 2.0);
    printf("diff to expected mean : %f%%\n", 100 * diff);

    // Checking a trap T
    signed_matrix T = new_signed_matrix(PARAM_L, PARAM_L);
    CHRONO("Sampled T in %fs ", { sample_Z_centered_matrix(T, s); });
    printf("mean: %f var: %f\n", meanbis(T), var(T));

    free_matrix(A);
    free_signed_matrix(T);
}