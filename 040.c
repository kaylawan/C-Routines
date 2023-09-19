#include <stdio.h>
#include <stdlib.h>

#include "src/go.h"

#define EMPTY       20

/*
 * Goal: Test recursion and that send/receive direction works through a partial stress test
 * Implementation: Tower of Hanoi
 */

int count = 0, NUM_RING = 3;

Value stack() {
    int *sizes = calloc(10, sizeof(int));
    int size = 0;
    while (1) {
        if (size >= 10) {
            printf("MASSIVE ERROR LOL\n");
        }
        int code = receive(me()).asInt;
        if (code == 0) {
            if (size == 0)
                send(me(), asInt(EMPTY));
            else
                send(me(), asInt(sizes[size - 1]));
        } else if (code > 0) {
            sizes[size++] = code;
        } else {
            if (size == 0) {
                printf("Attempting to remove rings from empty peg\n");
                continue;
            }
            if (sizes[size - 1] != -code) {
                printf("Attempting to remove wrong ring size\n");
                continue;
            }
            size--;
        }
    }
}

int main() {
    Channel *a = go(stack);
    Channel *b = go(stack);
    Channel *c = go(stack);
    int swap = NUM_RING % 2 == 1;

    for (int i = NUM_RING; i > 0; i--) {
        send(a, asInt(i));
    }


    int pc = 0;

    while (1) {
        send(a, asInt(0)); // Query top ring on each peg
        send(b, asInt(0));
        send(c, asInt(0));
        int a_rings = receive(a).asInt;
        int b_rings = receive(b).asInt;
        int c_rings = receive(c).asInt;
        if (a_rings == EMPTY && ( swap ? b_rings == 1 && c_rings == EMPTY : b_rings == EMPTY && c_rings == 1)) {
            printf("Success in %d moves!\n", count);
            if(NUM_RING >= 8) {
                return 0;
            }
            NUM_RING++;
            count = 0;
            again();
        }

        if (count >= 2 << NUM_RING) {
            printf("Too many moves needed\n");
            return 0;
        }

        printf("A: %d, B: %d, C: %d\n", a_rings, b_rings, c_rings);

        switch (pc % 3) {
            case 0:
                if (a_rings < b_rings && a_rings != 0) {
                    printf("MOVE: A -> B, %d\n", a_rings);
                    send(a, asInt(-a_rings));
                    send(b, asInt(a_rings));
                } else {
                    printf("MOVE: B-> A, %d\n", b_rings);
                    send(a, asInt(b_rings));
                    send(b, asInt(-b_rings));
                }
                break;
            case 1:
                if (a_rings < c_rings && a_rings != 0) {
                    printf("MOVE: A -> C, %d\n", a_rings);
                    send(a, asInt(-a_rings));
                    send(c, asInt(a_rings));
                } else {
                    printf("MOVE: C -> A, %d\n", c_rings);
                    send(a, asInt(c_rings));
                    send(c, asInt(-c_rings));
                }
                break;
            case 2:
                if (b_rings < c_rings && b_rings != 0) {
                    printf("MOVE: B -> C, %d\n", b_rings);
                    send(b, asInt(-b_rings));
                    send(c, asInt(b_rings));
                } else {
                    printf("MOVE: C -> B, %d\n", c_rings);
                    send(b, asInt(c_rings));
                    send(c, asInt(-c_rings));
                }
                break;
        }
        pc++;
        count++;
    }
}
