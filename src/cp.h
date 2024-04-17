#include <stdbool.h>

#include "attribute.h"
#include "bgg.h"
#include "circuit.h"
#include "matrix.h"
#include "sampling.h"

typedef struct {
    matrix* B;
    signed_matrix T;
} cp_keys;

typedef struct {
    matrix* CTf;
    signed_matrix Tf;
    matrix* A;
} cp_ciphertext;

void init_cp(void);

cp_keys Setup(void);

cp_ciphertext Enc(matrix* B, circuit f, bool u);

signed_matrix KeyGen(matrix* B, signed_matrix T, attribute x);

bool Dec(attribute x, circuit f, signed_matrix tx, cp_ciphertext cipher);
