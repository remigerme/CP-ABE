#include <stdbool.h>

#include "attribute.h"
#include "bgg.h"
#include "circuit.h"
#include "matrix.h"
#include "sampling.h"

typedef struct _cp_keys {
    matrix* B;
    signed_matrix T;
} cp_keys;

typedef struct _cp_cipher_bit {
    matrix* CTf;
    signed_matrix Tf;
    matrix* A;
} cp_cipher_bit;

typedef struct _cp_cipher {
    size_t nbits;
    cp_cipher_bit* ciphers;
} cp_cipher;

cp_keys Setup(scalar N, scalar Q, scalar K, scalar P, real SIGMA);

cp_keys SetupDefault(void);

cp_cipher_bit EncBit(matrix* B, circuit f, bool u);

cp_cipher EncStr(matrix* B, circuit f, char* message);

signed_matrix KeyGen(matrix* B, signed_matrix T, attribute x);

bool Dec(attribute x, circuit f, signed_matrix tx, cp_cipher_bit cipher_bit);

char* DecStr(attribute x, circuit f, signed_matrix tx, cp_cipher cipher);
