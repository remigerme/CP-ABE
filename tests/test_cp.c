#include <stdio.h>

#include "cp.h"
#include "gen_circuit.h"

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_RESET "\x1b[0m"

int main() {
    cp_keys keys = SetupDefault();

    // Users related settings
    attribute x_min = 0;
    attribute x_max = 0b1111;  // included
    printf("Testing CP\n");
    print_params();
    printf("\tattributes within range [%u, %u]\n", x_min, x_max);

    // User wants to cipher a message
    // He defines a circuit defining the access policy
    // Example hand-crafted circuit : f(x) = not(x1 | (x2 & x3))
    // Could be like checking if user is an admin or
    // is a dev and has the right to access feature
    char message[] = "Hello, world!";
    circuit* f = circuit_not(circuit_or(
        gen_leaf(1, true), circuit_and(gen_leaf(2, true), gen_leaf(3, true))));
    cp_cipher cipher = EncStr(keys.B, *f, message);

    // Trying to decrypt
    printf("Original message : %s\n", message);
    printf("Message decrypted by an :\n");
    for (attribute x = x_min; x < x_max + 1; x++) {
        // Computing the private key of the user
        signed_matrix tx = KeyGen(keys, x);

        if (!compute_f(*f, x)) {
            // An authorized user wants to decrypt the message
            char* plain = DecStr(x, *f, tx, cipher);
            printf(ANSI_COLOR_GREEN "  - authorized user : %s", plain);
        } else {
            // Unauthorized user trying to decrypt the message
            // In fact as KeyGen is not implemented
            // He has access to the full trap T
            // But even with that much information, it's not easy !
            char* not_so_plain = DecStr(x, *f, tx, cipher);
            printf(ANSI_COLOR_RED "  - unauthorized user : %s", not_so_plain);
        }

        printf(ANSI_COLOR_RESET "\n");
    }
}