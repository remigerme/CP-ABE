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

    matrix A = new_matrix(PARAM_N, PARAM_L);
    start = (real)clock() / CLOCKS_PER_SEC;
    sample_Zq_uniform_matrix(A, s);
    end = (real)clock() / CLOCKS_PER_SEC;
    real diff = (mean(A) - PARAM_Q / 2.0) / (PARAM_Q / 2.0);
    printf("Time : %fs diff to expected mean : %f%%\n", end - start,
           100 * diff);

    signed_matrix B = new_signed_matrix(PARAM_L, PARAM_L);
    start = (real)clock() / CLOCKS_PER_SEC;
    sample_Z_centered_matrix(B, s);
    end = (real)clock() / CLOCKS_PER_SEC;
    printf("Time : %fs mean: %f var: %f\n", end - start, meanbis(B), var(B));

    // Example output with N = 1024, K = 30
    // Time : 0.903941s diff to expected mean : 0.005221 %
    // Time : 27.481249s mean : -0.000243 var : 49.001157
}