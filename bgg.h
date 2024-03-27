#include "circuit.h"
#include "common.h"
#include "sampling.h"

/*
Given a circuit f (and a sampler s) computes
  - A = [A0, A1, ..., Ak] where Ai in Zq^{n * l}
  - Tf in Z^{l * l} the trap used to compute A0
*/
void BGG_KeyGen(circuit f, sampler s, matrix *A, signed_matrix Tf);

/*
Given a bit to encrypt u, a sampler s and A
      | A0 |
  A = | A1 | in Rq^{(k + 1) * m}
      | .. |
      | Ak |
computes CTf = TODO
*/
void BGG_OfflineEnc(poly_matrix A, bool u, sampler s, poly_matrix CTf);
