#include <stdio.h>
#include "src/go.h"

// Used for the pseudo rng
int x = 1284930576;

// Generates a pseudo random number
int prng(void) {
    x *= 2016473895;
    return x;
}

// Tests alignment with floating point operations (and sending, receiving, and
// global variables)
Value testAlign(void) {
    float x = (float) prng();
    double y = (double) prng();
    printf("%f %f\n", x + y, x / y);
    send(me(), asU64(9876543210));
    return asU8(0);
}

// Tests preservation of variable values after sending (and channel sending
// over channels)
Value testSend(void) {
    Channel* ch = receive(me()).asChannel;
    int a = 12345, b = 67890, c = 13579, d = 24680;
    send(ch, asU8(0));
    if (a != 12345 || b != 67890 || c != 13579 || d != 24680) {
        puts("Values not preserved");
    }
    return asU8(0);
}

// Tests preservation of variable values after receiving (and channel sending
// over channels)
Value testReceive(void) {
    Channel* ch = receive(me()).asChannel;
    int a = 54321, b = 98760, c = 97531, d = 86420;
    Value v = receive(ch);
    if (v.asU8 != 0 || a != 54321 || b != 98760 || c != 97531 || d != 86420) {
        puts("Values not preserved");
    }
    return asU8(0);
}

// Runs tests
int main() {
    // Tests alignment
    puts("TEST 1: alignment");
    Channel* channels[10];
    for (int i = 0; i < 10; i++) {
        channels[i] = go(testAlign);
    }
    for (int i = 9; i >= 0; i--) {
        Value value = receive(channels[i]);
        if (value.asU64 != 9876543210) {
            puts("Invalid value received");
        }
    }
    puts("END TEST 1");

    // Tests variable preservation
    puts("TEST 2: preservation");
    for (int i = 0; i < 10; i++) {
        Channel* ch = go(testSend);
        send(ch, asChannel(ch));
        ch = go(testReceive);
        send(ch, asChannel(ch));
    }
    puts("END TEST 2 (ALMOST)");

    // Finishes all ready routines and exits
    receive(me());
    puts("INVALID");
    return 0;
}