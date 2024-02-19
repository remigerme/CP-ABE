#include "common.h"

void TrapGen(poly_matrix B, poly_matrix T);

void TrapSamp(poly_matrix Bx, poly_matrix T, poly_matrix Tx);

// Assigns uniformely a random polynomial in Rq
void Sample_Rq_uniform(poly a);

// Samples from D_{R,sigma} where R = Z[X] / <X^n+1> is isomorphic to Z^n
void Sample_R_centered(signed_poly a, real sigma);

/*
Samples from distribution D_{c,sigma}, ie
Samples an element in Z with probability proportionnal to e^{-(c-x)^2/2*(sigma^2)}
*/
signed_scalar sample_Z(real c, real sigma);
