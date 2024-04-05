#pragma once

#include "circuit.h"
#include "matrix.h"
#include "sampling.h"

/*
Represents a pair of public / secret keys used in BGG
  - public key : A = [A0, A1, ..., Ak] where Ai in Zq^{n * l}
  - secret key : Tf in Z^{l * l} the trap used to compute A0
*/
typedef struct {
    matrix* A;
    signed_matrix Tf;
} bgg_keys;

/*
Given a circuit f (and a sampler s) computes a new pair of bgg_keys
    - A = [A0, A1, ..., Ak] where Ai in Zq^{n * l}
    - Tf in Z^{l * l} the trap used to compute A0
*/
bgg_keys BGG_KeyGen(circuit f, sampler s);

/*
Given a bit to encrypt u, a sampler s and
A = [A0, A1, ..., Ak] where Ai in Zq^{n * l}
returns CTf = [C0, C1-0, C1-1, ..., Ck-0, Ck-1]
*/
matrix* BGG_OfflineEnc(matrix* A, bool u, sampler s);
