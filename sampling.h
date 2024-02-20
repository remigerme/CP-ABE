#include "common.h"
#include "random.h"

void TrapGen(poly_matrix B, poly_matrix T);

void TrapSamp(poly_matrix Bx, poly_matrix T, poly_matrix Tx);

typedef struct _sampler {
    CSPRNG rng;              // for uniform distribution
    dgs_disc_gauss_dp_t *D;  // for gaussian discrete distribution
} sampler;

// Create a sampler, used by sampling functions to generate random results
sampler create_sampler(void);

// Assigns uniformely a random polynomial in Rq
void Sample_Rq_uniform(poly a, sampler s);

// Assigns uniformely polynomials in Rq over a random matrix of size m * n
void Sample_Rq_uniform_matrix(poly_matrix A, size_t m, size_t n, CSPRNG rng);

// Samples from D_{R,sigma} where R = Z[X] / <X^n+1> is isomorphic to Z^n
void Sample_R_centered(signed_poly a, sampler s);
