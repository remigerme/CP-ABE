#include <assert.h>
#include <stdio.h>

#include "attribute.h"
#include "bgg.h"
#include "circuit.h"
#include "common.h"
#include "cp.h"
#include "gen_circuit.h"
#include "matrix.h"
#include "sampling.h"

int main() {
    real start, end;
    init_cp();

    attribute x = 39837839;
    circuit *f = gen_circuit(x);

    cp_keys keys = Setup();

    cp_ciphertext res_0 = Enc(keys.B, *f, 0);
    cp_ciphertext res_1 = Enc(keys.B, *f, 1);

    signed_matrix Tx = KeyGen(keys.B, keys.T, x);

    bool plain_0 = Dec(x, *f, keys.T, res_0);
    bool plain_1 = Dec(x, *f, keys.T, res_1);

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