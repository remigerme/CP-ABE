#include "gen_circuit.h"

#include "common.h"

circuit* gen_leaf(int n, bool xn) {
    // n is still 1-indexed
    circuit* f = new_circuit();
    f->n = n;
    if (xn) return f;
    return circuit_not(f);
}

circuit* circuit_not(circuit* f) {
    // NOT(A) = NAND(A, A)
    circuit* g = new_circuit();
    g->left = g->right = f;
    return g;
}

circuit* circuit_and(circuit* f, circuit* g) {
    // AND(A, B) = NOT(C) where C = NAND(A, B)
    circuit* c = new_circuit();
    c->left = f;
    c->right = g;
    return circuit_not(c);
}

circuit* circuit_or(circuit* f, circuit* g) {
    // OR(A, B) = NAND(NOT(A), NOT(B))
    circuit* o = new_circuit();
    o->left = circuit_not(f);
    o->right = circuit_not(g);
    return o;
}

circuit* gen_circuit(attribute x) {
    /*
    We denote Gi the logic gate associated to xi.
    If xi = 1 : Gi is a leaf with Gi.n = i
    Else (xi = 0) : Gi = not (i) ie a circuit with
    two children such Gil.n = Gir.n = i (not gate using nand gates).
    Then the final circuit is just NOT( AND(Gi) forall i).
    Beware of NOT gate !
    */
    circuit* f;

    // Forall AND(Gi)
    circuit* G1 = gen_leaf(1, get_xn(x, 1));
    circuit* G2 = gen_leaf(2, get_xn(x, 2));
    f = circuit_and(G1, G2);
    for (int n = 3; n < PARAMS.K + 1; n++) {
        circuit* Gn = gen_leaf(n, get_xn(x, n));
        f = circuit_and(f, Gn);
    }

    // NOT gate
    circuit* g = new_circuit();
    g->left = g->right = f;
    return g;
}