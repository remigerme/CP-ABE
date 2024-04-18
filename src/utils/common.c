#include "common.h"

#include <assert.h>
#include <inttypes.h>
#include <math.h>
#include <stdbool.h>

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
    assert((scalar)ceil((Q) / log(10)) == K);
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
