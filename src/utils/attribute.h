#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "common.h"

typedef uint32_t attribute;

// Get xn value (0 or 1), USING 1-INDEXING
bool get_xn(attribute x, int n);

// Guard : we only have 32 bits available
#if PARAM_K > 32
#error "PARAM_K is too high (strictly over 32)."
#endif
