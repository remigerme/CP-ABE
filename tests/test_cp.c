#include <stdio.h>
#include <time.h>

#include "cp.h"
#include "gen_circuit.h"

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_RESET "\x1b[0m"

int main() {
    real start, end;
    cp_keys keys = SetupDefault();

    // Users related settings
    attribute x_min = 0;
    attribute x_max = 0b1111;  // included
    printf("Testing CP\n");
    print_params();
    printf("\tattributes within range [%u, %u]\n", x_min, x_max);

    // User wants to cipher a message
    // He defines a circuit defining the access policy
    // Example hand-crafted circuit : f(x) = not(x1 | (x2 & x3) | (x2 & x4))
    // Could be like checking if user is an admin or
    // is a dev and has the right to access feature
    char message[] = "Hello, world!";
    printf("Original message : %s\n", message);
    circuit* f = circuit_not(circuit_or(
        gen_leaf(1, true),
        circuit_or(circuit_and(gen_leaf(2, true), gen_leaf(3, true)),
                   circuit_and(gen_leaf(2, true), gen_leaf(4, true)))));

    // Same circuit but optimized
    // f(x) = (!1 ^ !2) ^ (!1 ^ !3 ^ !4)
    // where ^ is a nand gate
    // and .^.^. is a 3-input nand gate
    // nand(a, b, c) = nand(and(a, b), c)
    circuit g;
    circuit g1;
    g1.left = gen_leaf(1, false);
    g1.right = gen_leaf(2, false);
    g.left = &g1;
    circuit g2;
    circuit* g21 = circuit_and(gen_leaf(1, false), gen_leaf(3, false));
    g2.left = g21;
    g2.right = gen_leaf(4, false);
    g.right = &g2;

    // Encrypting the message
    start = (real)clock() / CLOCKS_PER_SEC;
    cp_cipher cipher = EncStr(keys.B, *f, message);
    end = (real)clock() / CLOCKS_PER_SEC;
    printf("Cipher generated in %.4gs\n", end - start);

    // Trying to decrypt with different attributes
    printf("Message decrypted by an :\n");
    for (attribute x = x_min; x < x_max + 1; x++) {
        // Computing the private key of the user
        signed_matrix tx = KeyGen(keys, x);

        // Trying to decrypt...
        start = (real)clock() / CLOCKS_PER_SEC;
        char* plain = DecStr(x, *f, tx, cipher);
        end = (real)clock() / CLOCKS_PER_SEC;

        if (!compute_f(*f, x)) {
            // An authorized user wants to decrypt the message
            printf(ANSI_COLOR_GREEN "  - authorized user :");
        } else {
            // Unauthorized user trying to decrypt the message
            // In fact as KeyGen is not implemented
            // He has access to the full trap T
            // But even with that much information, it's not easy !
            printf(ANSI_COLOR_RED "  - unauthorized user :");
        }

        printf(" %s" ANSI_COLOR_RESET " in %.4gs\n", plain, end - start);
    }
}