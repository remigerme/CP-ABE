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

bool Dec(attribute x, circuit f, bgg_keys keys, signed_matrix tx, matrix* CTf) {
    // Computing the right term HT (without Identity block)
    matrix H = compute_H(keys.A, f, x);
    matrix HT = new_matrix(PARAM_K * PARAM_L, PARAM_L);
    mul_matrix_trap(H, keys.Tf, HT);

    // Computing the relevant CTf
    matrix CTfx = new_matrix(PARAM_M, PARAM_K * PARAM_L);
    for (int k = 0; k < PARAM_K; k++) {
        int b = get_xn(x, k + 1);  // Beware of 1-indexing
        for (int m = 0; m < PARAM_M; m++)
            for (int l = 0; l < PARAM_L; l++)
                matrix_element(CTfx, m, k * PARAM_L + l) =
                    matrix_element(CTf[1 + 2 * k + b], m, l);
    }

    // Computing [C1,x1 | ... | Ck,xk] * HT - C0
    // All C are C with hat for now
    matrix right_res = new_matrix(PARAM_M, PARAM_L);
    mul_matrix(CTfx, HT, right_res);
    sub_matrix(right_res, CTf[0], right_res);

    // Computing tx * (SA[0] - C[0])
    int TODO = 1;  // TODO : determine rows of tx (columns = M)
    matrix res = new_matrix(TODO, PARAM_L);
    mul_matrix_trap(right_res, tx, res);  // TODO : multiply trap left
    bool plain = 0;                       // TODO : is_short(matrix)

    free_matrix(H);
    free_matrix(HT);
    free_matrix(CTfx);
    free_matrix(right_res);
    free_matrix(res);

    return plain;
}