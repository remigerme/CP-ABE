#include "cp.h"

#include <stdbool.h>

#include "bgg.h"
#include "circuit.h"
#include "matrix.h"
#include "sampling.h"

sampler s;

void init() {
    init_G();
    s = create_sampler();
}

cp_ciphertext Enc(matrix* B, circuit f, bool u) {
    bgg_keys keys = BGG_KeyGen(f, s);

    matrix* BGG_CTf = BGG_OfflineEnc(keys.A, u, s);

    matrix* CTf = new_matrixes(2 * PARAM_K + 1, PARAM_M, PARAM_L);

    // First term by hand
    matrix S = new_matrix(PARAM_N, PARAM_L);
    sample_Zq_uniform_matrix(S, s);
    mul_matrix(B[0], S, CTf[0]);
    add_matrix(CTf[0], BGG_CTf[0], CTf[0]);

    for (int k = 0; k < PARAM_K; k++) {
        for (int b = 0; b < 2; b++) {
            sample_Zq_uniform_matrix(S, s);
            int i = 1 + 2 * k + b;
            mul_matrix(B[i], S, CTf[i]);
            add_matrix(CTf[i], BGG_CTf[i], CTf[i]);
        }
    }

    free_matrixes(keys.A, PARAM_K + 1);
    free_matrixes(BGG_CTf, 2 * PARAM_K + 1);
    free_matrix(S);

    cp_ciphertext c = {CTf, keys.Tf};
    return c;
}