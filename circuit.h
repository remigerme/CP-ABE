#include "matrix.h"

/*
We store a circuit as a binary tree which indicates us
in which order we should compute the NAND gates.
The field `n` is relevant only if we're located
at a leaf ie left == right == NULL, in which case
it stores the relevant index `n` (n = 1 to consider A1).
*/
typedef struct btree {
    struct btree *left;
    struct btree *right;
    int n;
} circuit;

/*
Given A = [A0, A1, ..., Ak] (A0 is yet to compute)
where Ai in Zq^{n * l}, we compute Af = f(A1, ..., Ak)
where f is a boolean circuit composed of NAND gates.
*/
void compute_Af(matrix *A, circuit f, matrix Af);
