#include "common.h"

void Setup(poly_matrix B, poly_matrix T);

void Enc(poly_matrix B, circuit f, bool u, poly_matrix CTf);

void KeyGen(poly_matrix B, poly_matrix T, attribute x, poly_matrix SK_x);

void Dec(attribute x, poly_matrix SK_x, circuit f, poly_matrix CTf, bool u);
