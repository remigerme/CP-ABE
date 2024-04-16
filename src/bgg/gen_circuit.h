#include "attribute.h"
#include "circuit.h"
#include "common.h"

/*
Returns a circuit such as f(x) = 0
and forall y != x, f(y) = 1.
*/
circuit* gen_circuit(attribute x);
