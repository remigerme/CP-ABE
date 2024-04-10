#include <assert.h>
#include <stdio.h>

#include "attribute.h"
#include "bgg.h"
#include "circuit.h"
#include "cp.h"
#include "matrix.h"
#include "sampling.h"

int main() {
    init_cp();
    real start, end;

    // Printing parameters
    printf("Testing CP with parameters\n");
    printf("\tQ = %d\n", PARAM_Q);
    printf("\tN = %d\n", PARAM_N);
    printf("\tK = %d\n", PARAM_K);
    printf("\tL = %d\n", PARAM_L);
    printf("\tP = %d\n", PARAM_P);
    printf("\tM = P + 2 = %d", PARAM_M);
    printf("\tA matrixes are size : N * L = %d\n", PARAM_N * PARAM_L);
    printf("\tTf matrixes are size : L * L = %d\n", PARAM_L * PARAM_L);
    printf("\tH matrixes are size : K * L * L = %d\n",
           PARAM_K * PARAM_L * PARAM_L);
    printf("\tB matrixes are size : M * N = %d\n", PARAM_M * PARAM_N);
    printf("\tB (big) matrix is size : (2 * K + 1) * M * N = %d\n",
           (2 * PARAM_K + 1) * PARAM_M * PARAM_N);
    printf("\tT matrix is size : P * M = %d\n", PARAM_P * PARAM_M);

    circuit f;
    circuit g;
    circuit h;
    f.left = &g;
    f.right = &h;
    g.left = g.right = NULL;
    h.left = h.right = NULL;
    g.n = 1;
    h.n = 2;

    printf("Circuit : ");
    print_circuit(f);
    printf("\n");

    matrix ZERO = new_matrix(PARAM_P, PARAM_N);
    matrix res = new_matrix(PARAM_P, PARAM_N);
    cp_keys keys;
    CHRONO("TrapGen generated in %fs\n", keys = Setup(););
    for (int k = 0; k < 2 * PARAM_K + 1; k++) {
        mul_matrix_trap_left(keys.T, keys.B[k], res);
        assert(equals(res, ZERO));
    }
    printf("Checked TrapGen for all B matrixes.\n");

    cp_ciphertext res_0, res_1;
    CHRONO("Computed ciphertext for u = 0 in %fs\n",
           res_0 = Enc(keys.B, f, 0););
    CHRONO("Computed ciphertext for u = 1 in %fs\n",
           res_1 = Enc(keys.B, f, 1););

    free_signed_matrix(res_0.Tf);
    free_matrixes(res_0.CTf, 2 * PARAM_K + 1);
    free_signed_matrix(res_1.Tf);
    free_matrixes(res_1.CTf, 2 * PARAM_K + 1);
    free_matrixes(keys.B, 2 * PARAM_K + 1);
    free_signed_matrix(keys.T);
    free_matrix(G);
}