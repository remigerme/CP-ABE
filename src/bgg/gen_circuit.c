#include "gen_circuit.h"

circuit* gen_leaf(int n, bool xn) {
    // n is still 1-indexed
    circuit* f = new_circuit();
    if (xn) {
        f->n = n;
        return f;
    }
    circuit* g = new_circuit();
    f->left = f->right = g;
    g->n = n;
    return f;
}

circuit* circuit_and(circuit* f, circuit* g) {
    // AND(A, B) = NAND(C, C) where C = NAND(A, B)
    circuit* in = new_circuit();
    in->left = f;
    in->right = g;
    circuit* res = new_circuit();
    res->left = in;
    res->right = in;
    return res;
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
    for (int n = 3; n < PARAM_K + 1; n++) {
        circuit* Gn = gen_leaf(n, get_xn(x, n));
        f = circuit_and(f, Gn);
    }

    // NOT gate
    circuit* g = new_circuit();
    g->left = g->right = f;
    return g;
}