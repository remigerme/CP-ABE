#include <stdio.h>

#include "common.h"
#include "lib/dgs/dgs.h"

typedef void *CSPRNG;

typedef union {
    CSPRNG object;
    FILE *urandom;
} CSPRNG_TYPE;

CSPRNG create_csprng(void);

// Samples an integer from the uniform distribution over [0, q-1]
scalar uniform_mod_q(CSPRNG rng);

// Discrete gaussian distribution utils functions
dgs_disc_gauss_dp_t *create_dgs();                        // Creating sampler
signed_scalar signed_scalar_dgs(dgs_disc_gauss_dp_t *D);  // Get a signed scalar
void clear_dgs(dgs_disc_gauss_dp_t *D);                   // Freeing sampler
