# CP-ABE

Post-quantum cryptographic scheme based on lattices, CP-ABE [_Ciphertext Policy - Attribute Based Encryption_](using BGG-lite<sup>+</sup> KP-ABE scheme [_Key Policy_]).
Implementation of scheme proposed in https://eprint.iacr.org/2020/191.

> [!CAUTION]
> Do not use for real cryptographic applications. This project was made for educational purpose only.

Made by Rémi Germe.

## Useful resources

Custom `TrapGen` procedure derived from the one proposed for polynomials in https://eprint.iacr.org/2017/601.pdf.

Using `random.h` and `random.c` from Lucas's implementation of another scheme : https://github.com/lucasprabel/module_gaussian_lattice.

## Setup

See [PSC-debug-config](https://github.com/remigerme/PSC-debug-config).

Set formatter on distant machine (SSH) : search for `C_Cpp: Clang_format_fallback Style` in VSCode > settings `{ BasedOnStyle: Google, IndentWidth: 4 }`. The formatter should apply on save, if that's not the case check online how to configure it.

You can also install the `ANSI Colors` extension on VSCode to display properly `demo/test_cp.ans` for example.

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

## TODO

> [!WARNING]
> `TrapSamp` procedure needed for `KeyGen` was not implemented yet.

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
    - [x] CP for strings
- [x] "benchmark" of CP
    - [x] is short
    - [x] enc and dec rate

## Limits and comments
- limited by the size / depth of the circuits f used in practice, cause it tends the norm to get bigger and bigger (to the point we can't rule that a matrix which should be short is or not) : problem even with simple circuits of depth 4 and more
- difficult to use big value of `P` (and so `M`) in practice as it causes supposedly short matrixes to have their norm wildly increased
- | Rate   | Enc   | Dec  |
  |--------|-------|------|
  | bit/s  | ≈ 600 | ≈ 24 |
  | byte/s | ≈ 75  | ≈ 3  |
- the value of `P` doesn't seem to have an influence on `Dec` rate, which is by far the slowest between `Enc` (fast - compared to `Dec`) and `Dec` (slow, extremely slow as `N` grows...)
