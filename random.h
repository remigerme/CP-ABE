#include <stdio.h>

#include "common.h"

typedef void *CSPRNG;

typedef union {
    CSPRNG object;
    FILE *urandom;
} CSPRNG_TYPE;

// Samples an integer from the uniform distribution over [0, q-1]
scalar uniform_mod_q(CSPRNG rng);

// Discrete gaussian distribution utils functions
dgs_disc_gauss_dp_t *init_dgs();
void clear_dgs(dgs_disc_gauss_dp_t *D);
