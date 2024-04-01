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

TODO 4 : maybe encrypt more than just a single bit (a full int ? a sequence of ints representing an ASCII message e.g. ?) - same goes for decrypt of course

TODO 3 : understand why A0 = AfTf (mod q) is a matrix and not just a polynomial of Rq cause : Af in Zq^{n * m} = Rq^m and Tf in Zq^m (just a vector of scalars ! not of polynomials). See 2020-191 p12 -> Everything's fine if Tf lives in Zq^{nm * m} = Rq^{m * m}, so let's assume that for now.

TODO 3bis : understand the size of gadget matrix G (see 2020-191 p11) : we want A_nand in Rq^m so we want G in Rq^m (or at least Zq^{n * m}) and G^-1(A) in Rq^{m * m} (or similar). However, "G−1 : Zn×m
q → {0, 1}^{N ×m}" where N = n * log q = n * k, so it could work if k = m which was not planned (m = k + 2 elsewhere, maybe not relevant for this algorithm but yet to investigate) (investigate about bits decomposition and operations too) 

TODO 2 : Hf,x,A construction for Dec

TODO 1 : check inv_G implem