#pragma once

#include "attribute.h"
#include "matrix.h"

/*
We store a circuit as a binary tree which indicates us
in which order we should compute the NAND gates.
The field `n` is relevant only if we're located
at a leaf ie left == right == NULL, in which case
it stores the relevant index `n` (n = 1 to consider A1).
*/
typedef struct btree {
    struct btree* left;
    struct btree* right;
    int n;
} circuit;

/*
Given A = [A0, A1, ..., Ak] (A0 is yet to compute)
where Ai in Zq^{n * l}, and a boolean circuit f
composed of NAND gates, we compute Af = f(A1, ..., Ak).
*/
void compute_Af(matrix* A, circuit f, matrix Af);

/*
Given A = [A0, A1, ..., Ak] where Ai in Zq^{n * l},
a boolean circuit f and an attribute x, we compute
the associated Hf,x,A matrix.
*/
void compute_H(matrix* A, circuit f, attribute x, matrix H);
