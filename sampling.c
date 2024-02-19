#include "sampling.h"
#include "random.h"

void Sample_Rq_uniform(poly a)
{
    for (int i = 0; i < PARAM_N; i++)
        a[i] = uniform_mod_q();
}

/*
Sample_R_centerd, Sample_Z and algorithmF copy-pasted from Luca Prabel github's implem
https://github.com/lucasprabel/module_gaussian_lattice/blob/main/ROM_GPV/sampling.c
*/

void Sample_R_centered(signed_poly a, real sigma)
{
    for (int i = 0; i < PARAM_N; ++i)
    {
        a[i] = SampleZ(0, sigma);
    }
}

signed_scalar Sample_Z(real c, real sigma)
{
    return algorithmF(c, sigma);
}