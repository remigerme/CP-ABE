# CP-ABE

Post-quantum cryptographic scheme based on lattices, CP-ABE (using BGG-lite KP-ABE scheme).
Implementation of scheme proposed in https://eprint.iacr.org/2020/191.

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
DGS is **not random** on its own, it seems to provide only a fully deterministic sequence of integers distributed according to a discrete gaussian (though I'm absolutely no expert). The same goes for Luca's `algorithmF` using aes intrinsics (compile with `-maes` flag).

I need to be careful about stack memory limit, as creating big matrices of polynomials can be very memory-consuming. Prefer heap allocation using `malloc`.