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
    int32_t N;   // should be 1 for now - degree of polynomials
    uint32_t Q;  // Q <= 2^K - modulus
    int32_t K;   // K = ceil(logQ) - attribute length and boolean circuits arity
    int32_t L;   // L = (N * K) - KP-ABE matrices dimension
    int32_t P;   // CP trap size
    int32_t M;   // M = (P + 2) - artificial CP trap size for computation
    real SIGMA;  // used for discrete gaussian distribution
    real SHORT_THRESHOLD;  // threshold used in is_short
} cp_params;

// Global params globally available
extern cp_params PARAMS;

void init_params(int32_t N, uint32_t Q, int32_t K, int32_t P, real SIGMA);

// Init params with default values, check common.c for values
void init_params_default(void);

// Nicely print current parameters
void print_params(void);

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
