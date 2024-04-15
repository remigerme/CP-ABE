# CP-ABE

Post-quantum cryptographic scheme based on lattices, CP-ABE (using BGG-lite KP-ABE scheme).
Implementation of scheme proposed in https://eprint.iacr.org/2020/191.

## Setup

See [PSC-debug-config](https://github.com/remigerme/PSC-debug-config).

Set formatter on distant machine (SSH) : search for `C_Cpp: Clang_format_fallback Style` in VSCode > settings `{ BasedOnStyle: Google, IndentWidth: 4 }`. The formatter should apply on save, if that's not the case check online how to configure it.

## Contribute

Commit on your own branch (with an explicit feature name if possible), then merge request.

If you have any question ask me !

## How to use
First, let's build the external libs using
```
make libs
```
Then you can run `test` for example :
```
make test
./test
```

## Comments
[DGS](https://github.com/malb/dgs/tree/master) is **not random** on its own, it seems to provide only a fully deterministic sequence of integers distributed according to a discrete gaussian (though I'm absolutely no expert). The same goes for [Luca's](https://github.com/lucasprabel/module_gaussian_lattice/tree/main/ROM_GPV) `algorithmF` using aes intrinsics (compile with `-maes` flag).

I need to be careful about stack memory limit, as creating big matrices of polynomials can be very memory-consuming. Prefer heap allocation using `malloc`.

### TODO
- [x] BGG
    - [x] BGG.KeyGen implem
    - [x] BGG.OfflineEnc implem
    - [x] compute_H implem
    - [x] BGG tests
    - [x] BGG benchmark
- [ ] CP
    - [x] Setup
        - [x] Trapgen
        - [x] Benchmark
    - [x] Enc
        - [x] Implem
        - [x] Benchmark
    - [ ] KeyGen
        - [ ] Implem
        - [ ] Benchmark
    - [ ] Dec
        - [ ] Implem
            - [ ] is_short
            - [x] H issue
            - [x] algo
        - [x] Benchmark
- [ ] statistical analysis of CP
