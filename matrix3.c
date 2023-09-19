#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

#include "src/go.h"

// this function will have its args passed in XMM registers
// and will be called from a routine, ensuring stack alignment
double add(double a, double b) {
    return a + b;
}

// this function calculates the nth (received on its channel) fibonacci number
// this tests that you can call go recursively, and that your again works correctly, maintaining alignment
Value fibonacci() {
    uint64_t n = receive(me()).asU64;

    if (n <= 1) {
        send(me(), asU64(n));
        again();
    }

    Channel* recursive = go(fibonacci);
    send(recursive, asU64(n - 1));
    uint64_t n_minus_1 = receive(recursive).asU64;
    send(recursive, asU64(n - 2));
    uint64_t n_minus_2 = receive(recursive).asU64;

    uint64_t result = (uint64_t) add((double) n_minus_1, (double) n_minus_2);
    send(me(), asU64(result));
    again();

    // should never get here
    return asU64(99999);
}

// make sure you don't use macros to do again for main
// again is not acutally called, but if you always define your own main function
// then you will fail to compile with a double definition
#undef main

int main() {
    for (uint64_t n = 0; n < 26; ++n) {
        Channel* result_channel = go(fibonacci);
        send(result_channel, asU64(n));

        printf("%" PRIu64 "\n", receive(result_channel).asU64);
    }

    printf("Passed\n");

    return 0;
}
