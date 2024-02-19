#include <inttypes.h>
#include <stdbool.h>

typedef uint32_t scalar;
typedef int32_t signed_scalar;
typedef double real;

// Polynomial in standard (coefficients) representation
typedef scalar *poly;
typedef signed_scalar *signed_poly;

// Matrix of polynomials (single pointer on scalars, everything is accessed through macros)
typedef scalar *poly_matrix;
typedef signed_scalar *signed_poly_matrix;
