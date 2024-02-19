#include "common.h"

void TrapGen(poly_matrix B, poly_matrix T);

void TrapSamp(poly_matrix Bx, poly_matrix T, poly_matrix Tx);

// Assigns uniformely a random polynomial in Rq
void Sample_Rq_uniform(poly a, CSPRNG rng);

// Samples from D_{R,sigma} where R = Z[X] / <X^n+1> is isomorphic to Z^n
void Sample_R_centered(signed_poly a, dgs_disc_gauss_dp_t *D);
