#include "cp.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "bgg.h"
#include "circuit.h"
#include "common.h"
#include "matrix.h"
#include "sampling.h"

/*******************/
/* CP_CIPHER UTILS */
/*******************/

void free_cp_cipher(cp_cipher cipher) {
    for (int i = 0; i < cipher.nbits; i++) {
        free_matrixes(cipher.ciphers[i].CTf, 1 + 2 * PARAMS.K);
        free_signed_matrix(cipher.ciphers[i].Tf);
        free_matrixes(cipher.ciphers[i].A, 1 + PARAMS.K);
    }
}

/********************/
/* CP-ABE FUNCTIONS */
/********************/

void init_cp(scalar N, scalar Q, scalar K, scalar P, real SIGMA) {
    init_params(N, Q, K, P, SIGMA);
    init_sampler();
    init_G();
}

cp_keys Setup(scalar N, scalar Q, scalar K, scalar P, real SIGMA) {
    init_cp(N, Q, K, P, SIGMA);
    cp_keys keys;
    matrix* B = new_matrixes(2 * PARAMS.K + 1, PARAMS.M, PARAMS.N);
    signed_matrix T = new_signed_matrix(PARAMS.P, PARAMS.M);
    TrapGen(B, T);
    keys.B = B;
    keys.T = T;
    return keys;
}

void init_cp_default() {
    init_params_default();
    init_sampler();
    init_G();
}

cp_keys SetupDefault() {
    init_cp_default();
    cp_keys keys;
    matrix* B = new_matrixes(2 * PARAMS.K + 1, PARAMS.M, PARAMS.N);
    signed_matrix T = new_signed_matrix(PARAMS.P, PARAMS.M);
    TrapGen(B, T);
    keys.B = B;
    keys.T = T;
    return keys;
}

cp_cipher_bit EncBit(matrix* B, circuit f, bool u) {
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

    cp_cipher_bit c = {CTf, keys.Tf, keys.A};
    return c;
}

cp_cipher EncStr(matrix* B, circuit f, char* message) {
    int message_size = strlen(message);
    int nbits = 8 * message_size;
    cp_cipher_bit* ciphers = calloc(nbits, sizeof(cp_cipher_bit));
    for (int i = 0; i < message_size; i++) {
        // Considering byte i of message
        for (int b = 0; b < 8; b++) {
            // Considering bit b of message[i]
            bool u = (message[i] >> b) & 1;
            cp_cipher_bit c = EncBit(B, f, u);
            ciphers[8 * i + b] = c;
        }
    }
    cp_cipher c = {nbits, ciphers};
    return c;
}

signed_matrix KeyGen(matrix* B, signed_matrix T, attribute x) {
    return TrapSamp(B, T, x);
}

bool DecBit(attribute x, circuit f, signed_matrix tx, cp_cipher_bit cipher) {
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

// Set (to 1) the kth bit of n (0-indexed)
char set_bit(char n, int k) { return n | (1 << k); }

// Clear (to 0) the kth bit of n (0-indexed)
char clear_bit(char n, int k) { return n & (~(1 << k)); }

char* DecStr(attribute x, circuit f, signed_matrix tx, cp_cipher cipher) {
    assert(cipher.nbits % 8 == 0);
    char* plain = calloc(cipher.nbits / 8 + 1, sizeof(char));
    for (int k = 0; k < cipher.nbits; k++) {
        int i = k / 8;
        int b = k % 8;
        bool u = DecBit(x, f, tx, cipher.ciphers[k]);
        if (u)
            plain[i] = set_bit(plain[i], b);
        else
            plain[i] = clear_bit(plain[i], b);
    }
    plain[cipher.nbits] = '\0';
    return plain;
}