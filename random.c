#include "random.h"

#include <stdint.h>

/*
Uniform mod q

// Interface to the OS Cryptographically-Secure Pseudo-Random Number Generator

// Copyright 2017 Michael Thomas Greer
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file ../../LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt )
*/

// Returns a new CSPRNG object, 0 on error.
CSPRNG csprng_create() {
    CSPRNG_TYPE csprng;
    csprng.urandom = fopen("/dev/urandom", "rb");
    return csprng.object;
}

/*
Fills $dest with $size bytes worth of random data.
Returns 1 on succes, 0 on failure.
*/
int csprng_get(CSPRNG object, void* dest, unsigned long long size) {
    CSPRNG_TYPE csprng;
    csprng.object = object;
    return (csprng.urandom) &&
           (fread((char*)dest, 1, size, csprng.urandom) == size);
}

void uniform_scalar(CSPRNG rng, scalar* s) {
    while (!csprng_get(rng, s, sizeof(scalar)))
        ;
}

scalar uniform_mod_q(CSPRNG rng) {
    scalar r;
    uniform_scalar(rng, &r);
    real rand = (real)r / (real)UINT32_MAX;
    return rand * (PARAM_Q - 1) + 0.5;
}

/*

Discrete gaussian distribution utils functions

*/

dgs_disc_gauss_dp_t* init_dgs() {
    // Cutoff `τ`, samples outside the range `(⌊c⌉-⌈στ⌉,...,⌊c⌉+⌈στ⌉)` are
    // considered to have probability zero. This bound applies to algorithms
    // which sample from the uniform distribution.
    dgs_disc_gauss_dp_t* D = dgs_disc_gauss_dp_init(
        PARAM_SIGMA, 0, PARAM_TAU, DGS_DISC_GAUSS_UNIFORM_TABLE);
    return D;
}

void clear_dgs(dgs_disc_gauss_dp_t* D) { dgs_disc_gauss_dp_clear(D); }
