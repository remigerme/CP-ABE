#include <stdbool.h>

#include "attribute.h"
#include "bgg.h"
#include "circuit.h"
#include "matrix.h"
#include "sampling.h"

/*
Contains both MPK and MSK for CP-ABE :
MPK = [B0 | B1,0 | B1,1 | ... | Bk,0, Bk,1], B0, Bi,b uniform over Zq^{m * n}
MSK = T in Z^{p * m} with short entries where m = p + 2
such as TB0 = TBi,b = 0 mod q
*/
typedef struct _cp_keys {
    matrix* B;
    signed_matrix T;
} cp_keys;

/*
CP whole ciphertext for a single bit. Contains :
cipher CTf = [C0 | C1,0 | C1,1 | ... | Ck,0, Ck,1], C0, Ci,b in Zq^{m * l}
bgg_keys (A = [A0 | A1 | ... Ak], Tf) used for BGG subroutine
*/
typedef struct _cp_cipher_bit {
    matrix* CTf;
    bgg_keys keys;
} cp_cipher_bit;

/*
CP ciphertext for a whole string
Each bit is ciphered separately
*/
typedef struct _cp_cipher {
    size_t nbits;
    cp_cipher_bit* ciphers;
} cp_cipher;

// Clean free of matrixes used by cp_cipher_bit
void free_cp_cipher_bit(cp_cipher_bit cipher);

// Clean free of matrixes used by cp_cipher
void free_cp_cipher(cp_cipher cipher);

/*
Setup the CP-ABE with given security parameters
In practice, N must be small : N < 5.
Must have K = ceil(log2(Q)) ie K = number of bits of Q
Sets parameter L = N * K
In theory : P < 30, in practice, better use P around 10.
Sets parameter M = P + 2.
Use discrete gaussian sampling over Z centered in 0 with variance SIGMA
Calls mandatory functions used to initialize the program
Returns cp_keys containing (MPK, MSK)
*/
cp_keys Setup(scalar N, scalar Q, scalar K, scalar P, real SIGMA);

/*
Setup the CP-ABE with default parameters set at compile time
In practice, N must be small : N < 5.
Must have K = ceil(log2(Q)) ie K = number of bits of Q
Sets parameter L = N * K
In theory : P < 30, in practice, better use P around 10.
Sets parameter M = P + 2.
Use discrete gaussian sampling over Z centered in 0 with variance SIGMA
Calls mandatory functions used to initialize the program
Returns cp_keys containing (MPK, MSK)
*/
cp_keys SetupDefault(void);

// Encrypts a given bit u according to MPK B and circuit f (access policy)
cp_cipher_bit EncBit(matrix* B, circuit f, bool u);

// Encrypts a full message according to MPK B and circuit f (access policy)
cp_cipher EncStr(matrix* B, circuit f, char* message);

/*
WARNING : NOT IMPLEMENTED

Samples private key tx associated with attribute x from CP keys MPK and MSK
such as txB0 = txBi,xi = 0 mod q and txBi,1-xi is uniform over Zq^{m * n}
*/
signed_matrix KeyGen(cp_keys keys, attribute x);

// Decrypts a single bit, works fine if f(x) = 0 else returns garbage
bool DecBit(attribute x, circuit f, signed_matrix tx, cp_cipher_bit cipher_bit);

// Decrypts a full message, works fine if f(x) = 0 else returns garbage
char* DecStr(attribute x, circuit f, signed_matrix tx, cp_cipher cipher);
