#include <stdio.h>
#include <stdlib.h>

#include "src/go.h"

uint64_t repeat = 0xbaad;
uint64_t result = 0;

int main() {
    result = (result << 16) | repeat;
    if (repeat == 0xbaad) {
        repeat = 0xc0de;
        again();
        printf("*** we should never get here\n");
    }
    printf("*** 0x%lx\n", result);
    return 42;
}

void ready() {}
void zombie() {}
void finished() {}
void yield() {}
void wrapper() {}