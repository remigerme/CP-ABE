#include <stdio.h>

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
            "(warning : must havex_max < UINT32_MAX)\n");
        return -1;
    }
    attribute x = strtoul(argv[1], NULL, 10);
    attribute x_min = strtoul(argv[2], NULL, 10);
    attribute x_max = strtoul(argv[3], NULL, 10);
    if (x_min > x_max) {
        printf("x_min should be <= x_max\n");
        return -1;
    }

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

    free_circuit(f);
    printf("Checked f(x) for x in [%u, %u].\n", x_min, x_max);
}