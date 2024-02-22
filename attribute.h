#include "common.h"

typedef scalar attribute;

// Get xk value (0 or 1), using 0-indexing
scalar get_xk(attribute x, int k);

// Guard : we only have 32 bits available
#if PARAM_K > 32
#error "PARAM_K is too high (strictly over 32)."
#endif
