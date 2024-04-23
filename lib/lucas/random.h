#include <inttypes.h>

/*******************/
/* By Lucas Prabel */
/* See https://github.com/lucasprabel/module_gaussian_lattice */
/*******************/

uint32_t uniform_mod_n(uint32_t n);

/*
    Code from random_aesni.c
*/

void random_bytes_init(void);

void random_bytes(uint8_t *data);

/*
    Code from exp_aes.cpp
*/

double algorithm_EA(uint64_t *n);

/*
    Code from algoF_aes.cpp
*/

// Discrete Gaussian Distribution
int algorithmF(const double mu, const double sigma);

void salt(uint8_t *r);
