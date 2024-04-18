#include <assert.h>
#include <stdio.h>

#include "attribute.h"
#include "bgg.h"
#include "circuit.h"
#include "common.h"
#include "cp.h"
#include "matrix.h"
#include "sampling.h"

int main() {
    init_cp();
    real start, end;

    // Printing parameters
    printf("Testing CP with parameters\n");
    print_params();

    circuit f;
    circuit g;
    circuit h;
    circuit hg;
    circuit hd;
    f.left = &g;
    f.right = &h;
    g.left = g.right = NULL;
    g.n = 1;
    h.left = &hg;
    h.right = &hd;
    hg.left = hg.right = NULL;
    hd.left = hd.right = NULL;
    hg.n = 2;
    hd.n = 3;

    printf("Circuit : ");
    print_circuit(f);
    printf("\n");

    matrix ZERO = new_matrix(PARAMS.P, PARAMS.N);
    matrix res = new_matrix(PARAMS.P, PARAMS.N);
    cp_keys keys;
    CHRONO("TrapGen generated in %fs\n", keys = Setup(););
    for (int k = 0; k < 2 * PARAMS.K + 1; k++) {
        mul_matrix_trap_left(keys.T, keys.B[k], res);
        assert(equals(res, ZERO));
    }
    printf("Checked TrapGen for all B matrixes.\n");

    cp_ciphertext res_0, res_1;
    CHRONO("Computed ciphertext for u = 0 in %fs\n",
           res_0 = Enc(keys.B, f, 0););
    CHRONO("Computed ciphertext for u = 1 in %fs\n",
           res_1 = Enc(keys.B, f, 1););

    attribute x = 3;
    assert(compute_f(f, x) == 0);
    printf("Attribute x is authorized to decrypt.\n");

    signed_matrix Tx;
    CHRONO("Generating Tx trap in %fs\n", Tx = KeyGen(keys.B, keys.T, x););
    mul_matrix_trap_left(Tx, keys.B[0], res);
    assert(equals(res, ZERO));
    for (int i = 0; i < PARAMS.K; i++) {
        bool xi = get_xn(x, i + 1);  // 1 indexed
        // Checking TxBi,xi = 0
        mul_matrix_trap_left(Tx, keys.B[1 + 2 * i + xi], res);
        assert(equals(res, ZERO));
        // Checking TxBi,1-xi != 0, should check if uniformely distributed
        // mul_matrix_trap_left(Tx, keys.B[1 + 2 * i + (1 - xi)], res);
        // assert(!equals(res, ZERO));
    }
    printf("Done checking TxBi,b relations\n");

    bool plain_0, plain_1;
    CHRONO("Deciphering (for plain = 0) in %fs\n", {
        plain_0 = Dec(x, f, keys.T, res_0);
        assert(plain_0 == 0);
    });
    CHRONO("Deciphering (for plain = 1) in %fs\n", {
        plain_1 = Dec(x, f, keys.T, res_1);
        assert(plain_1 == 1);
    });

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

    free_matrix(ZERO);
    free_matrix(res);

    free_matrix(G);
}