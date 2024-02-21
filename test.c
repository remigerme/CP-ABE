#include <stdio.h>

#include "common.h"
#include "random.h"

int main() {
    CSPRNG rng = csprng_create();
    dgs_disc_gauss_dp_t* D = init_dgs();
    for (int i = 0; i < 10; i++) {
        scalar r = uniform_mod_q(rng);
        signed_scalar j = (signed_scalar)(D->call(D));
        printf("r : %u\n", r);
        printf("j : %d\n", j);
    }
}