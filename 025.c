#include <stdio.h>
#include <ctype.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdbool.h>
#include "src/go.h"

uint64_t grapes = 25;
uint64_t stock = 3;
bool lemonade_man_has_grapes = true;

Value got_any_grapes() {
    while(stock > 0) {
        while (lemonade_man_has_grapes) {
            send(me(), asInt(grapes));
            grapes--;
            lemonade_man_has_grapes = grapes > 0;
        }
        stock--;
        again();
    }

    return asInt(100);
}

int main() {
    /**
     * This test checks for returning a value an infinite number of times from a completed channel,
     * It tests specfically for the case when the previously blocked receivers on the completed channel
     * become unblocked and "merge" with the ready queue as unblocked channels.
    */
   Channel *get_grapes = go(got_any_grapes);

    printf("GRAPE PYRAMID\n");
    while(true) {
        uint64_t basket = receive(get_grapes).asI64;

        if (basket == 100) { return 0; }

        for (int i = 0; i < basket; i++) {
            printf("%s", ":O:");
        }
        printf("\n");
    }
}