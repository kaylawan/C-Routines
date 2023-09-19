#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdbool.h>

#include "src/go.h"

// Tests core functionality of c-routines through code that mimics the workings of a D Flip-flop component
//    ____________
// -->|    \/    |
// D  |   DFF    |
//    |__________|-->Q

Value currentFrontVal;
Value currentBackVal;
Channel* frontChannel;
Channel* backChannel;

void updateGate(Value* ptr1, Value* ptr2, bool inc) {
    uint64_t input = receive(me()).asU64;

    // Signal to maintain value
    if (input == 0) {
    } 
    // Signal to accept new val
    else {
        // If the adder should be applied (only for front gate)
        if (inc) {
            *ptr1 = asU64((*ptr2).asU64 + 1);
        } else {
            *ptr1 = asU64((*ptr2).asU64);
        }
    }
}

Value frontGate() {
    updateGate(&currentFrontVal, &currentBackVal, true);
    send(frontChannel, asU64(0));
    again();
    printf("Should never get here.\n");
    return asU64(0);
}

Value backGate() {
    updateGate(&currentBackVal, &currentFrontVal, false);
    send(backChannel, asU64(0));
    again();
    printf("Oooops should not be here.\n");
    return asU64(0);
}

int main() {
    currentFrontVal = asU64(0);
    currentBackVal = asU64(0);
    frontChannel = channel();
    backChannel = channel();

    Channel* frontSig = go(frontGate);
    Channel* backSig = go(backGate);

    uint64_t totalIterations = 10;
    uint64_t iterations = 0;
    bool signal = false;
    while (iterations < totalIterations) {
        // First gate passing of signal
        send(frontSig, asU64((uint64_t) signal));
        receive(frontChannel);

        // Inverting the signal for the second gate
        send(backSig, asU64((uint64_t) !signal));
        receive(backChannel);

        printf("Iteration %lu, ", iterations);
        printf("observable output: %lu\n", currentBackVal.asU64);
        signal = !signal;

        iterations++;
    } 

    printf("END!\n");
    return 0;
}