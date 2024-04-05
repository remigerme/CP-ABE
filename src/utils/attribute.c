#include "attribute.h"

#include <assert.h>

bool get_xn(attribute x, int n) {
    assert(n > 0);
    assert(n <= PARAM_K);
    return (x >> (n - 1)) & 1;
}
