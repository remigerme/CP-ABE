#include "common.h"

/*
Returns an integer sampled from the uniform distribution over [0, q-1]
using the uniform distribution over [0, 2^32 - 1] provided by random_bytes
*/
scalar uniform_mod_q(void);

/*
Samples from distribution D_{mu,sigma}, ie
Samples an element in Z with probability proportionnal to e^{-(mu-x)^2/2*(sigma^2)}
*/
signed_scalar algorithmF(const real mu, const real sigma);
