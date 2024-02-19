#include "common.h"
#include "dgs/dgs.h"

dgs_disc_gauss_dp_t *init_dgs()
{
    // Cutoff `τ`, samples outside the range `(⌊c⌉-⌈στ⌉,...,⌊c⌉+⌈στ⌉)` are
    // considered to have probability zero. This bound applies to algorithms
    // which sample from the uniform distribution.
    dgs_disc_gauss_dp_t *D = dgs_disc_gauss_dp_init(PARAM_SIGMA, 0, PARAM_TAU, DGS_DISC_GAUSS_UNIFORM_TABLE);
    return D;
}

void clear_dgs(dgs_disc_gauss_dp_t *D)
{
    dgs_disc_gauss_dp_clear(D);
}
