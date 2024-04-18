#include "cp.h"

#include <stdbool.h>

#include "bgg.h"
#include "circuit.h"
#include "common.h"
#include "matrix.h"
#include "sampling.h"

void init_cp() {
    init_params_default();
    init_sampler();
    init_G();
}

cp_keys Setup() {
    cp_keys keys;
    matrix* B = new_matrixes(2 * PARAMS.K + 1, PARAMS.M, PARAMS.N);
    signed_matrix T = new_signed_matrix(PARAMS.P, PARAMS.M);
    TrapGen(B, T);
    keys.B = B;
    keys.T = T;
    return keys;
}

cp_ciphertext Enc(matrix* B, circuit f, bool u) {
    bgg_keys keys = BGG_KeyGen(f);

    matrix* BGG_CTf = BGG_OfflineEnc(keys.A, u);

    matrix* CTf = new_matrixes(2 * PARAMS.K + 1, PARAMS.M, PARAMS.L);

    // First term by hand
    matrix S = new_matrix(PARAMS.N, PARAMS.L);
    sample_Zq_uniform_matrix(S);
    mul_matrix(B[0], S, CTf[0]);
    add_matrix(CTf[0], BGG_CTf[0], CTf[0]);

    for (int k = 0; k < PARAMS.K; k++) {
        for (int b = 0; b < 2; b++) {
            sample_Zq_uniform_matrix(S);
            int i = 1 + 2 * k + b;
            mul_matrix(B[i], S, CTf[i]);
            add_matrix(CTf[i], BGG_CTf[i], CTf[i]);
        }
    }

    free_matrixes(BGG_CTf, 2 * PARAMS.K + 1);
    free_matrix(S);

    cp_ciphertext c = {CTf, keys.Tf, keys.A};
    return c;
}

signed_matrix KeyGen(matrix* B, signed_matrix T, attribute x) {
    return TrapSamp(B, T, x);
}

bool Dec(attribute x, circuit f, signed_matrix tx, cp_ciphertext cipher) {
    // Computing the right term HT (without Identity block)
    matrix H = compute_H(cipher.A, f, x);
    matrix HT = new_matrix(PARAMS.K * PARAMS.L, PARAMS.L);
    mul_matrix_trap(H, cipher.Tf, HT);

    // Computing the relevant CTf
    matrix CTfx = new_matrix(PARAMS.M, PARAMS.K * PARAMS.L);
    for (int k = 0; k < PARAMS.K; k++) {
        int b = get_xn(x, k + 1);  // Beware of 1-indexing
        for (int m = 0; m < PARAMS.M; m++)
            for (int l = 0; l < PARAMS.L; l++)
                matrix_element(CTfx, m, k * PARAMS.L + l) =
                    matrix_element(cipher.CTf[1 + 2 * k + b], m, l);
    }

    // Computing [C1,x1 | ... | Ck,xk] * HT - C0
    // All C are C with hat for now
    matrix right_res = new_matrix(PARAMS.M, PARAMS.L);
    mul_matrix(CTfx, HT, right_res);
    sub_matrix(right_res, cipher.CTf[0], right_res);

    // Computing tx * (-E0 + [E1,x1 | ... | E2,x2] * HT)
    // (if x is the correct attribute, else this is a
    // nightmarish computation and we don't care about it)
    matrix res = new_matrix(PARAMS.P, PARAMS.L);
    mul_matrix_trap_left(tx, right_res, res);

    // Computing decoded bit
    // Warning it is 0 if res is short
    // else 1 if it is random
    bool plain = !is_short(res);

    free_matrix(H);
    free_matrix(HT);
    free_matrix(CTfx);
    free_matrix(right_res);
    free_matrix(res);

    return plain;
}