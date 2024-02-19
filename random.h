#include "common.h"

// Returns an integer sampled from the uniform distribution over [0, q-1]
scalar uniform_mod_q(void);

// Discrete gaussian distribution utils functions
dgs_disc_gauss_dp_t *init_dgs();
void clear_dgs(dgs_disc_gauss_dp_t *D);
