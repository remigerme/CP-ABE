#include <stdio.h>
#include <time.h>

#include "common.h"
#include "lib/luca/random.h"
#include "poly.h"
#include "sampling.h"

real mean(poly a) {
    real m = 0;
    for (int i = 0; i < PARAM_N; i++) {
        m += (real)a[i];
    }
    return m / PARAM_N;
}

real meanbis(signed_poly a) {
    real m = 0;
    for (int i = 0; i < PARAM_N; i++) {
        m += (real)a[i];
    }
    return m / PARAM_N;
}

real var(signed_poly a) {
    real m = meanbis(a);
    real v = 0;
    for (int i = 0; i < PARAM_N; i++) {
        v += a[i] * a[i];
    }
    return v / PARAM_N - m;
}

int main() {
    sampler s = create_sampler();
    real start, end;

    poly a = malloc(PARAM_N * sizeof(scalar));
    start = (real)clock() / CLOCKS_PER_SEC;
    sample_Rq_uniform(a, s);
    end = (real)clock() / CLOCKS_PER_SEC;
    printf("Mine : %fs mean: %f\n", end - start, mean(a));

    poly d = malloc(PARAM_N * sizeof(scalar));
    start = (real)clock() / CLOCKS_PER_SEC;
    for (int i = 0; i < PARAM_N; i++) {
        d[i] = _uniform_mod_q();
    }
    end = (real)clock() / CLOCKS_PER_SEC;
    printf("Luca : %fs mean: %f\n", end - start, mean(d));

    signed_poly b = malloc(PARAM_N * sizeof(signed_scalar));
    start = (real)clock() / CLOCKS_PER_SEC;
    sample_R_centered(b, s);
    end = (real)clock() / CLOCKS_PER_SEC;
    printf("Mine : %fs mean: %f var: %f\n", end - start, meanbis(b), var(b));

    signed_poly c = malloc(PARAM_N * sizeof(signed_scalar));
    start = (real)clock() / CLOCKS_PER_SEC;
    for (int i = 0; i < PARAM_N; i++) {
        c[i] = algorithmF(0, PARAM_SIGMA);
    }
    end = (real)clock() / CLOCKS_PER_SEC;
    printf("Luca : %fs mean: %f var: %f\n", end - start, meanbis(c), var(c));
}