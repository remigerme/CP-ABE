#pragma once

#include "circuit.h"
#include "matrix.h"
#include "sampling.h"

/*
Given a circuit f (and a sampler s) computes
  - A = [A0, A1, ..., Ak] where Ai in Zq^{n * l}
  - Tf in Z^{l * l} the trap used to compute A0
*/
void BGG_KeyGen(circuit f, sampler s, matrix* A, signed_matrix Tf);

/*
Given a bit to encrypt u, a sampler s and
A = [A0, A1, ..., Ak] where Ai in Zq^{n * l}
returns CTf = [C0, C1-0, C1-1, ..., Ck-0, Ck-1]
*/
matrix* BGG_OfflineEnc(matrix* A, bool u, sampler s);
