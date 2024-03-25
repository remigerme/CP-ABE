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
void sample_Rq_uniform(poly a, sampler s);

// Assigns uniformely scalars in Zq over a random matrix of size d1 * d2
void sample_Zq_uniform_matrix(matrix A, int d1, int d2, sampler s);

// Assigns uniformely polynomials in Rq over a random matrix of size d1 * d2
void sample_Rq_uniform_matrix(poly_matrix A, int d1, int d2, sampler s);

// Samples from D_{R,sigma} where R = Z[X] / <X^n+1> is isomorphic to Z^n
void sample_R_centered(signed_poly a, sampler s);

// Samples an entire matrix of size d1 * d2 from D_{Zq,sigma}
void sample_Zq_centered_matrix(signed_matrix A, int d1, int d2, sampler s);

/*
Samples an entire matrix of size d1 * d2 from D_{R,sigma}
where R = Z[X] / <X^n+1> is isomorphic to Z^n
*/
void sample_R_centered_matrix(signed_poly_matrix A, int d1, int d2, sampler s);
