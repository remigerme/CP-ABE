#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "attribute.h"
#include "bgg.h"
#include "circuit.h"
#include "common.h"
#include "cp.h"
#include "gen_circuit.h"
#include "matrix.h"
#include "sampling.h"

int main(int argc, char* argv[]) {
    if (argc != 8) {
        printf(
            "Illegal number of arguments. Usage : ./test N Q K P SIGMA "
            "SHORT_THRESHOLD x\n");
        return -1;
    }

    int32_t N = atoi(argv[1]);
    uint32_t Q = strtoul(argv[2], NULL, 10);
    int32_t K = atoi(argv[3]);
    int32_t P = atoi(argv[4]);
    real SIGMA = strtod(argv[5], NULL);
    real SHORT_THRESHOLD = strtod(argv[6], NULL);
    attribute x = strtoul(argv[7], NULL, 10);

    init_cp(N, Q, K, P, SIGMA, SHORT_THRESHOLD);

    /*
    Example hand-crafted small circuit f(x) = not (x1 & (x2 | x3))
    Could be like checking if user is an admin
    or is a dev and has the right to access feature
    must have K >= 3
    */
    assert(K >= 3);
    circuit* f = circuit_not(circuit_or(
        gen_leaf(1, true), circuit_and(gen_leaf(2, true), gen_leaf(3, true))));

    if (compute_f(*f, x)) {
        printf("Unauthorized attribute for circuit f.\n");
        return -1;
    }

    cp_keys keys = Setup();

    cp_ciphertext res_0 = Enc(keys.B, *f, 0);
    cp_ciphertext res_1 = Enc(keys.B, *f, 1);

    signed_matrix Tx = KeyGen(keys.B, keys.T, x);

    Dec(x, *f, keys.T, res_0);
    Dec(x, *f, keys.T, res_1);

    // Free time !
    free_matrixes(res_0.CTf, 2 * PARAMS.K + 1);
    free_matrixes(res_0.A, PARAMS.K);
    free_signed_matrix(res_0.Tf);

    free_matrixes(res_1.CTf, 2 * PARAMS.K + 1);
    free_matrixes(res_1.A, PARAMS.K);
    free_signed_matrix(res_1.Tf);

    free_matrixes(keys.B, 2 * PARAMS.K + 1);
    free_signed_matrix(keys.T);
    free_signed_matrix(Tx);

    free_matrix(G);
}