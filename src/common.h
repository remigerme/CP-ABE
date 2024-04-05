#pragma once

#include <stdint.h>
#include <stdio.h>
#include <time.h>

// Test parameters
#define PARAM_N 2                    // size of polynomials
#define PARAM_Q 11                   // modulus
#define PARAM_K 4                    // attribute length
#define PARAM_L (PARAM_N * PARAM_K)  // KP-ABE matrices dimension
#define PARAM_M (PARAM_K + 2)        // yet to determine
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
typedef uint32_t scalar;
typedef int32_t signed_scalar;
typedef double real;

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
