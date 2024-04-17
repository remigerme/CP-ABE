#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "attribute.h"
#include "bgg.h"
#include "circuit.h"
#include "cp.h"
#include "gen_circuit.h"
#include "matrix.h"
#include "sampling.h"

int main(int argc, char* argv[]) {
    if (argc != 4) {
        printf(
            "Illegal number of arguments. Usage : ./test x x_min x_max "
            "to check all attributes within [x_min, x_max] "
            "(warning : must have x_max < 2^K)\n");
        return -1;
    }
    attribute x = strtoul(argv[1], NULL, 10);
    attribute x_min = strtoul(argv[2], NULL, 10);
    attribute x_max = strtoul(argv[3], NULL, 10);
    if (x_min > x_max) {
        printf("x_min should be <= x_max\n");
        return -1;
    }

    init_cp();
    real start, end;

    printf("Testing gen_circuit with parameters\n");
    printf("\tQ = %d\n", PARAM_Q);
    printf("\tK = %d\n", PARAM_K);
    printf("\tx = %u\n", x);
    printf("\tx_min = %u\n", x_min);
    printf("\tx_max = %u\n", x_max);

    circuit* f;
    CHRONO("Circuit generated in %fs\n", f = gen_circuit(x););

    for (attribute xt = x_min; xt < x_max + 1; xt++) {
        if (xt == x)
            assert(compute_f(*f, xt) == 0);
        else
            assert(compute_f(*f, xt) == 1);
    }

    printf("Checked f(x) for x in [%u, %u].\n", x_min, x_max);

    matrix* A = new_matrixes(PARAM_K + 1, PARAM_N, PARAM_L);
    for (int k = 1; k < PARAM_K + 1; k++) sample_Zq_uniform_matrix(A[k]);
    CHRONO("Computed Af in %fs\n", compute_Af(A, *f););

    CHRONO("Computed Hf,x,A in %fs\n", compute_H(A, *f, x););
    free_circuit(f);
}