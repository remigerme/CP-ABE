#include "circuit.h"
#include "common.h"
#include "sampling.h"

/*
Given a circuit f (and a sampler s) computes
      | A0 |
  A = | A1 | in Rq^{(k + 1) * m}
      | .. |
      | Ak |
  Tf in R^{m*m} the trap used to compute A0
*/
void BGG_KeyGen(circuit f, sampler s, poly_matrix A, signed_poly_matrix Tf);

/*
Given a sampler s and A
      | A0 |
  A = | A1 | in Rq^{(k + 1) * m}
      | .. |
      | Ak |
computes CTf = TODO
*/
void BGG_OfflineEnc(poly_matrix A, sampler s, poly_matrix CTf);
