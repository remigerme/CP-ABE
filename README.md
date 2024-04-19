# CP-ABE

Post-quantum cryptographic scheme based on lattices, CP-ABE (using BGG-lite KP-ABE scheme).
Implementation of scheme proposed in https://eprint.iacr.org/2020/191.
Using `random.h` and `random.c` from Lucas's implementation of another scheme : https://github.com/lucasprabel/module_gaussian_lattice.

## Setup

See [PSC-debug-config](https://github.com/remigerme/PSC-debug-config).

Set formatter on distant machine (SSH) : search for `C_Cpp: Clang_format_fallback Style` in VSCode > settings `{ BasedOnStyle: Google, IndentWidth: 4 }`. The formatter should apply on save, if that's not the case check online how to configure it.

## Contribute

Commit on your own branch (with an explicit feature name if possible), then merge request.

If you have any question ask me !

## How to use
First, let's build the external libs and the project using
```
make libs
make
```
Then you can run one of the tests, `test_sampling` for example :
```
./build/test_sampling
```

## Comments

### TODO
- [x] BGG
    - [x] BGG.KeyGen implem
    - [x] BGG.OfflineEnc implem
    - [x] compute_H implem
    - [x] BGG tests
    - [x] BGG benchmark
- [ ] CP
    - [x] Setup
    - [x] Enc
    - [ ] KeyGen
    - [x] Dec
    - [ ] CP for strings
- [ ] statistical analysis of CP
    - [x] is short
    - [ ] enc and dec rate

### Limits
- probably limited by the size of the circuits f used in practice, cause it tends the norm to get bigger and bigger (to the point we can't rule that a matrix which should be short is or not)
