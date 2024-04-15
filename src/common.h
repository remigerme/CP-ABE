#pragma once

#include <stdint.h>
#include <stdio.h>
#include <time.h>

// Test parameters
#define PARAM_N 16                   // size of polynomials
#define PARAM_Q 53507                // modulus
#define PARAM_K 16                   // attribute length
#define PARAM_L (PARAM_N * PARAM_K)  // KP-ABE matrices dimension
#define PARAM_P 10                   // yet to determine
#define PARAM_M (PARAM_P + 2)        // yet to determine
#define PARAM_SIGMA 7.00             // used for discrete gaussian distribution
#define PARAM_TAU 80                 // used for discrete gaussian distribution

// Real parameters
/*
#define PARAM_N 1024                 // size of polynomials
#define PARAM_Q 1073707009           // modulus
#define PARAM_K 30                   // attribute length
#define PARAM_L (PARAM_N * PARAM_K)  // KP-ABE matrices dimension
#define PARAM_M (PARAM_K + 2)        // yet to determine
#define PARAM_SIGMA 7.00             // used for discrete gaussian distribution
#define PARAM_TAU 80                 // used for discrete gaussian distribution
*/
typedef uint64_t scalar;
typedef int64_t signed_scalar;
typedef double real;

#define SCALAR_MAX UINT64_MAX

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

#if PARAM_K > 32
#error "PARAM_K is too high (strictly over 32)."
#elif PARAM_P > 31
#error "PARAM_P is too high (strictly over 31)."
#endif