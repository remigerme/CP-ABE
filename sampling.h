#include "common.h"

void TrapGen(poly_matrix B, poly_matrix T);

void TrapSamp(poly_matrix Bx, poly_matrix T, poly_matrix Tx);

// Assigns uniformely a random polynomial in Rq
void Sample_Rq_uniform(poly a);

// Assigns a polynomial according to discrete gaussian distribution D(R, sigma)
void Sample_D_centered(signed_poly a, real sigma);
