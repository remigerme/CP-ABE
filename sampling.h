#pragma once

#include "matrix.h"
#include "random.h"

/*
void TrapGen(poly_matrix B, poly_matrix T);

void TrapSamp(poly_matrix Bx, poly_matrix T, poly_matrix Tx);
*/

typedef struct _sampler {
    CSPRNG rng;              // for uniform distribution
    dgs_disc_gauss_dp_t *D;  // for gaussian discrete distribution
} sampler;

/* -------------------- */
/* Functions for matrix */
/* -------------------- */

// Create a sampler, used by sampling functions to generate random results
sampler create_sampler(void);

// Assigns uniformely scalars in Zq over a random matrix
void sample_Zq_uniform_matrix(matrix A, sampler s);

// Samples an entire matrix from D_{Zq,sigma}
void sample_Z_centered_matrix(signed_matrix A, sampler s);
