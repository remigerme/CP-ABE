#include <stdio.h>

#include "cp.h"
#include "gen_circuit.h"

int main() {
    cp_keys keys = SetupDefault();

    // User related settings
    attribute x = 0b110;
    attribute other_x = 0b011;
    attribute wrong_x = 0b100;
    signed_matrix tx = KeyGen(keys.B, keys.T, x);
    signed_matrix tx_other = KeyGen(keys.B, keys.T, other_x);
    signed_matrix tx_wrong = KeyGen(keys.B, keys.T, wrong_x);

    // User wants to cipher a message
    // He defines a circuit defining the access policy
    // Example hand-crafted circuit : f(x) = not(x1 & (x2 | x3))
    // Could be like checking if user is an admin or
    // is a dev and has the right to access feature
    char message[] = "Hello, world!";
    circuit* f = circuit_not(circuit_or(
        gen_leaf(1, true), circuit_and(gen_leaf(2, true), gen_leaf(3, true))));
    cp_cipher cipher = EncStr(keys.B, *f, message);

    // An authorized user wants to decrypt the message
    char* plain = DecStr(x, *f, tx, cipher);
    printf("Message decrypted by same user : %s\n", plain);

    // Another authorized user wants to decrypt the message
    char* plain_bis = DecStr(other_x, *f, tx_other, cipher);
    printf("Message decrypted by another authorized user : %s\n", plain_bis);

    // Unauthorized user trying to decrypt the message
    // In fact as KeyGen is not implemented
    // He has access to the full trap T
    // But even with that much information, it's not easy !
    char* not_so_plain = DecStr(wrong_x, *f, tx_wrong, cipher);
    printf("Message decrypted by an unauthorized user : %s\n", not_so_plain);
}