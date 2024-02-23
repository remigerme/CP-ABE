#include "poly.h"

void copy_poly(poly a, poly r) {
    for (int i = 0; i < PARAM_N; i++) {
        r[i] = a[i];
    }
}

void add_poly(poly a, poly b, poly r) {
    for (int i = 0; i < PARAM_N; i++) {
        r[i] = (a[i] + b[i]) % PARAM_Q;
    }
}

void sub_poly(poly a, poly b, poly r) {
    for (int i = 0; i < PARAM_N; i++) {
        // We artificially add PARAM_Q to be sure
        // we don't underflow (cause we're using uint32_t)
        r[i] = (PARAM_Q + a[i] - b[i]) % PARAM_Q;
    }
}

void mul_poly(poly a, poly b, poly r) {
    // TODO
}