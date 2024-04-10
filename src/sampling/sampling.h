#pragma once

#include "attribute.h"
#include "matrix.h"
#include "random.h"

typedef struct _sampler {
    CSPRNG rng;              // for uniform distribution
    dgs_disc_gauss_dp_t* D;  // for gaussian discrete distribution
} sampler;

/*
Given a sampler s, computes B = [B0 | B1,0 | B1,1 | ... | Bk,1]
and its trap T such TBi,b = 0 where Bi,b in Zq^{m * n} and T in Z^{m * m}.
Warning, some dimensions are hardcoded in TrapGen and we use the gadget vector
(with g=2).
*/
void TrapGen(sampler s, matrix* B, signed_matrix T);

/*
Given B = [B0 | B1,0 | B1,1 | ... | Bk,1], its trap T such as TB0=TBi,b=0, an
attribute x and a sampler s, returns trap Tx (in Z^{P * M}) for the attribute :
- TxB0 = 0
- TxBi,xk = 0
- TxBi,(1-xk) is uniformely distributed
Warning dimensions are hardcoded in TrapGen and we use the gadget vector (g=2).
*/
signed_matrix TrapSamp(matrix* B, signed_matrix T, attribute x, sampler s);

/* -------------------- */
/* Functions for matrix */
/* -------------------- */

// Create a sampler, used by sampling functions to generate random results
sampler create_sampler(void);

// Assigns uniformely scalars in Zq over a random matrix
void sample_Zq_uniform_matrix(matrix A, sampler s);

// Samples an entire matrix from D_{Zq,sigma}
void sample_Z_centered_matrix(signed_matrix A, sampler s);
