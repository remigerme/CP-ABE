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
    printf("\tN = %d\n", PARAM_N);
    printf("\tK = %d\n", PARAM_K);
    printf("\tL = %d\n", PARAM_L);
    printf("\tA matrixes are size : N * L = %d\n", PARAM_N * PARAM_L);
    printf("\tT matrixes are size : L * L = %d\n", PARAM_L * PARAM_L);
    printf("\tH matrixes are size : K * L * L = %d\n",
           PARAM_K * PARAM_L * PARAM_L);

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

    matrix* B = new_matrixes(2 * PARAM_K + 1, PARAM_M, PARAM_N);
    for (int i = 0; i < 2 * PARAM_K + 1; i++) sample_Zq_uniform_matrix(B[i], s);

    cp_ciphertext res_0, res_1;
    CHRONO("Computed ciphertext for u = 0 in %fs\n", res_0 = Enc(B, f, 0););
    CHRONO("Computed ciphertext for u = 1 in %fs\n", res_1 = Enc(B, f, 1););

    free_signed_matrix(res_0.Tf);
    free_matrixes(res_0.CTf, 2 * PARAM_K + 1);
    free_signed_matrix(res_1.Tf);
    free_matrixes(res_1.CTf, 2 * PARAM_K + 1);
    free_matrix(G);
}