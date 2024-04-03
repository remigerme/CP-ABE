#include <stdbool.h>

#include "attribute.h"
#include "circuit.h"
#include "matrix.h"
#include "sampling.h"

extern sampler s;

void init(void);

void Setup(matrix* B, matrix T);

void Enc(matrix* B, circuit f, bool u, matrix* CTf, signed_matrix Tf);

void KeyGen(matrix* B, matrix T, attribute x, matrix Tx);

bool Dec(attribute x, matrix Tx, circuit f, matrix* CTf);
