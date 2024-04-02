#include <stdio.h>

#include "attribute.h"
#include "circuit.h"
#include "matrix.h"
#include "sampling.h"

int main() {
    sampler s = create_sampler();

    init_G();
    print_matrix(G);
    printf("\n");

    matrix Af = new_matrix(PARAM_N, PARAM_L);

    matrix* A = new_matrixes(PARAM_K + 1, PARAM_N, PARAM_L);
    for (int i = 0; i < PARAM_K; i++) {
        sample_Zq_uniform_matrix(A[i], s);
        print_matrix(A[i]);
        printf("\n");
    }

    circuit f;
    f.left = NULL;
    f.right = NULL;
    f.n = 1;

    compute_Af(A, f, Af);

    print_matrix(Af);
    printf("\n");

    free_matrixes(A, PARAM_K + 1);
    free_matrix(Af);
    free_matrix(G);
}