#include <stdio.h>

#include "common.h"
#include "dgs/dgs.h"

typedef void *CSPRNG;

typedef union {
    CSPRNG object;
    FILE *urandom;
} CSPRNG_TYPE;

CSPRNG csprng_create(void);

// Samples an integer from the uniform distribution over [0, q-1]
scalar uniform_mod_q(CSPRNG rng);

// Discrete gaussian distribution utils functions
dgs_disc_gauss_dp_t *init_dgs();
void clear_dgs(dgs_disc_gauss_dp_t *D);
