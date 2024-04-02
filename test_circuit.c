#include <stdio.h>

#include "attribute.h"
#include "circuit.h"
#include "matrix.h"

int main() {
    init_G();

    print_matrix(G);
    printf("\n");

    matrix* A = new_matrixes(PARAM_K + 1, PARAM_N, PARAM_L);
    for (int i = 0; i < PARAM_K; i++) {
        print_matrix(A[i]);
        printf("\n");
    }
    free_matrixes(A, PARAM_K + 1);

    // Don't forget to free G
    free_matrix(G);
}