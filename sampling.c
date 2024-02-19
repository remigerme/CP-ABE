#include "sampling.h"
#include "random.h"
#include "dgs/dgs.h"

void Sample_Rq_uniform(poly a)
{
    for (int i = 0; i < PARAM_N; i++)
    {
        a[i] = uniform_mod_q();
    }
};

void Sample_R_centered(signed_poly a, dgs_disc_gauss_dp_t *D)
{
    for (int i = 0; i < PARAM_N; ++i)
    {
        a[i] = (signed_scalar)(D->call(D));
    }
}
