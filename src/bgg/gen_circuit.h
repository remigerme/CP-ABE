#include "attribute.h"
#include "circuit.h"
#include "common.h"

circuit* gen_leaf(int n, bool xn);

circuit* circuit_not(circuit* f);

circuit* circuit_and(circuit* f, circuit* g);

circuit* circuit_or(circuit* f, circuit* g);

/*
Returns a circuit such as f(x) = 0
and forall y != x, f(y) = 1.
*/
circuit* gen_circuit(attribute x);
