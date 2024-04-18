#pragma once

#include <stdint.h>
#include <stdio.h>
#include <time.h>

// Defining alias types
typedef uint64_t scalar;
typedef int64_t signed_scalar;
typedef double real;

// Struct containing all parameters of the CP-ABE
typedef struct _cp_params {
    scalar N;    // should be 1 for now - degree of polynomials
    scalar Q;    // Q <= 2^K - modulus
    scalar K;    // K = ceil(logQ) - attribute length and boolean circuits arity
    scalar L;    // L = (N * K) - KP-ABE matrices dimension
    scalar P;    // CP trap size
    scalar M;    // M = (P + 2) - artificial CP trap size for computation
    real SIGMA;  // used for discrete gaussian distribution
    real SHORT_THRESHOLD;  // threshold used in is_short
} cp_params;

// Global params globally available
extern cp_params PARAMS;

void init_params(scalar N, scalar Q, scalar K, scalar P, real SIGMA,
                 real SHORT_THRESHOLD);

/*
Example parameters
N = 1
Q = 1073707009
K = 30
=> L = N * K = 30
P = 10
=> M = P + 2 = 12
SIGMA = 7.00
SHORT_THRESHOLD = P * L * Q / 100 (arbitrarily set)
*/

/*
double start and double end need to be defined before !
comment need to include %f specifier to include duration
*/
#define CHRONO(comment, code)                        \
    do {                                             \
        start = (real)clock() / CLOCKS_PER_SEC;      \
        {code} end = (real)clock() / CLOCKS_PER_SEC; \
        printf(comment, end - start);                \
    } while (0)

// Debug parameters
// #define DEBUG_NORM
