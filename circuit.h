#pragma once

#include <stdbool.h>

#include "attribute.h"
#include "matrix.h"

// Needs to be initialized through init_G !
extern matrix G;

// Initialize gadget matrix
void init_G(void);

// R <- G^-1(A)
void inv_G(matrix A, matrix R);

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
composed of NAND gates, we return Af = f(A1, ..., Ak).
*/
matrix compute_Af(matrix* A, circuit f);

/*
Given A = [A0, A1, ..., Ak] where Ai in Zq^{n * l},
a boolean circuit f and an attribute x, we return
the associated Hf,x,A matrix in Zq^{kl * l}.
*/
matrix compute_H(matrix* A, circuit f, attribute x);

// Returns f(x1, ..., xk)
bool compute_f(circuit f, attribute x);
