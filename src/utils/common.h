#pragma once

#include <stdint.h>
#include <stdio.h>
#include <time.h>

// Test parameters
#define PARAM_N 1                    // yet to determine
#define PARAM_Q 1073707009           // modulus
#define PARAM_K 30                   // attribute length
#define PARAM_L (PARAM_N * PARAM_K)  // KP-ABE matrices dimension
#define PARAM_P 10                   // CP trap size
#define PARAM_M (PARAM_P + 2)        // artificial CP trap size for computation
#define PARAM_SIGMA 7.00             // used for discrete gaussian distribution
#define PARAM_TAU 40                 // used for discrete gaussian distribution

/*
Threshold used to determine if a vector is short in is_short
Is heavily dimension dependant and fixed a bit arbitrarily manually
Dependance in Q not studied yet
*/
/*
#define SHORT_THRESHOLD                                                  \
     (PARAM_M * PARAM_K * PARAM_L * PARAM_L * PARAM_SIGMA * PARAM_SIGMA * \
      PARAM_SIGMA)
*/
#define SHORT_THRESHOLD ((double)PARAM_P * PARAM_L * PARAM_Q / 100)

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

// Debug parameters
#define DEBUG_NORM
