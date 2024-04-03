#include <stdbool.h>

#include "attribute.h"
#include "circuit.h"
#include "matrix.h"

void Setup(matrix* B, matrix T);

matrix* Enc(matrix* B, circuit f, bool u);

void KeyGen(matrix* B, matrix T, attribute x, matrix Tx);

bool Dec(attribute x, matrix Tx, circuit f, matrix* CTf);
