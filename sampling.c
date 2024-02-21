#include "sampling.h"

sampler create_sampler() {
    CSPRNG rng = create_csprng();
    dgs_disc_gauss_dp_t *D = create_dgs();
    sampler s = {rng, D};
    return s;
}

void Sample_Rq_uniform(poly a, sampler s) {
    for (int i = 0; i < PARAM_N; i++) {
        a[i] = uniform_mod_q(s.rng);
    }
}

void Sample_R_centered(signed_poly a, sampler s) {
    for (int i = 0; i < PARAM_N; i++) {
        a[i] = signed_scalar_dgs(s.D);
    }
}
