#include "common.h"

#include <assert.h>
#include <inttypes.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

cp_params PARAMS;

static bool initialized = false;

void init_params(int32_t N, uint32_t Q, int32_t K, int32_t P, real SIGMA,
                 real SHORT_THRESHOLD) {
    // We can initialize parameters only once
    assert(!initialized);

    // Checking parameters
    assert(0 < N && N <= UINT32_MAX);
    assert(0 < Q && Q <= UINT32_MAX);
    assert(0 < K && K <= 32);
    assert((uint32_t)ceil(log(Q) / log(2)) == K);
    assert(0 < P && P <= 30);
    assert(0 < SIGMA);
    assert(0 < SHORT_THRESHOLD);

    // Assigning parameters
    PARAMS.N = N;
    PARAMS.Q = Q;
    PARAMS.K = K;
    PARAMS.L = N * K;
    PARAMS.P = P;
    PARAMS.M = P + 2;
    PARAMS.SIGMA = SIGMA;
    PARAMS.SHORT_THRESHOLD = SHORT_THRESHOLD;

    // Not possible to initialize it again
    initialized = true;
}

void init_params_default() {
    int32_t N = 1;
    uint32_t Q = 1073707009;
    int32_t K = 30;
    int32_t P = 10;
    real SIGMA = 7.00;
    // Empirical formula from is_short tests
    real SHORT_THRESHOLD = (real)N * (Q / 2) * pow(P, 1.5) * pow(SIGMA, 2);
    // Arbitrary factor to compensate reality so it works better in practice
    // ie for a wider range of parameters
    SHORT_THRESHOLD /= 100;
    init_params(N, Q, K, P, SIGMA, SHORT_THRESHOLD);
}

void print_params() {
    printf("Printing parameters...\n");
    printf("\tN = %d\n", PARAMS.N);
    printf("\tQ = %u\n", PARAMS.Q);
    printf("\tK = %d\n", PARAMS.K);
    printf("\tL = %d\n", PARAMS.L);
    printf("\tP = %d\n", PARAMS.P);
    printf("\tM = P + 2 = %d\n", PARAMS.M);
    printf("\tSIGMA = %.2f\n", PARAMS.SIGMA);
    printf("\tSHORT_THRESHOLD = %.2g\n", PARAMS.SHORT_THRESHOLD);
}
