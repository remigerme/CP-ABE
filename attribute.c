#include "attribute.h"

scalar get_xk(attribute x, int k) { return (x >> k) & 1; }
