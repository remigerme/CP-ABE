#include "common.h"

/*
We store a circuit as a binary tree which indicates us
in which order we should compute the NAND gates.
The field `n` is relevant only if we're located
at a leaf ie left == right == NULL, in which case
it stores the relevant index `n` (using 0-indexing).
*/
typedef struct btree {
    struct btree *left;
    struct btree *right;
    int n;
} circuit;

/*
Given A = [A1 | ... | Ak] we compute
Af = f(A1, ..., Ak) where f is a boolean circuit
composed of NAND gates.
*/
void compute_Af(poly_matrix A, circuit f, poly_matrix Af);
