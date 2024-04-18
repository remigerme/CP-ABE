#include "common.h"

#include <assert.h>
#include <inttypes.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

cp_params PARAMS;

static bool initialized = false;

void init_params(scalar N, scalar Q, scalar K, scalar P, real SIGMA,
                 real SHORT_THRESHOLD) {
    // We can initialize parameters only once
    assert(!initialized);

    // Checking parameters
    assert(0 < N && N <= UINT32_MAX);
    assert(0 < Q && Q <= UINT32_MAX);
    assert(0 < K && K <= 32);
    assert((scalar)ceil(log(Q) / log(2)) == K);
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
    scalar N = 1;
    scalar Q = 1073707009;
    scalar K = 30;
    scalar P = 10;
    real SIGMA = 7.00;
    real SHORT_THRESHOLD = P * N * K * Q / 100;
    init_params(N, Q, K, P, SIGMA, SHORT_THRESHOLD);
}

void print_params() {
    printf("Printing parameters...\n");
    printf("\tN = %lu\n", PARAMS.N);
    printf("\tQ = %lu\n", PARAMS.Q);
    printf("\tK = %lu\n", PARAMS.K);
    printf("\tL = %lu\n", PARAMS.L);
    printf("\tP = %lu\n", PARAMS.P);
    printf("\tM = P + 2 = %lu\n", PARAMS.M);
    printf("\tSIGMA = %.2f\n", PARAMS.SIGMA);
    printf("\tSHORT_THRESHOLD = %.2g\n", PARAMS.SHORT_THRESHOLD);
}
