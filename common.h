#pragma once

#include <stdint.h>

// Dummy parameters
#define PARAM_N 1024                 // size of polynomials
#define PARAM_Q 1073707009           // modulus
#define PARAM_K 30                   // attribute length
#define PARAM_L (PARAM_N * PARAM_K)  // KP-ABE matrices dimension
#define PARAM_M (PARAM_K + 2)        // yet to determine
#define PARAM_SIGMA 7.00             // used for discrete gaussian distribution
#define PARAM_TAU 80                 // used for discrete gaussian distribution

typedef uint32_t scalar;
typedef int32_t signed_scalar;
typedef double real;
