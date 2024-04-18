#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef uint32_t attribute;

// Get xn value (0 or 1), USING 1-INDEXING
bool get_xn(attribute x, int n);
