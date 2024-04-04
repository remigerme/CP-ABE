#include <stdbool.h>

#include "attribute.h"
#include "circuit.h"
#include "matrix.h"
#include "sampling.h"

extern sampler s;

typedef struct {
    matrix* CTf;
    signed_matrix Tf;
} cp_ciphertext;

void init(void);

void Setup(matrix* B, matrix T);

cp_ciphertext Enc(matrix* B, circuit f, bool u);

void KeyGen(matrix* B, matrix T, attribute x, matrix Tx);

bool Dec(attribute x, matrix Tx, circuit f, matrix* CTf);
