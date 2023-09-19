#include <stdio.h>
#include "src/go.h"

int curr = 0;
int A[101];

Value add_arr() {
    Channel *ch = me();
    Value v = receive(ch);
    A[curr++] = v.asInt;
    send(ch, asLong(0));
    return asLong(0);
}

void test_sort() {
    printf("tests send and receive on a sorted array\n");
    for(int i = 0; i < 100; i++) {
        Channel *c = go(add_arr);
        send(c, asInt(i ^ 18)); // random xor to "randomize"
        receive(c);
    }

    // sorts the array according to: https://arxiv.org/pdf/2110.01111.pdf
    for(int i = 0; i < 100; i++) {
        for(int j = 0; j < 100; j++) {
            if(A[i] < A[j]) {
                A[i] ^= A[j];
                A[j] ^= A[i];
                A[i] ^= A[j];
            }
        }
    }

    for(int i = 0; i < 100; i++) {
        printf("%d", A[i]);
        if(i != 100 - 1) printf(" ");
    }
    puts("");
}

int sum = 0;

Value add() {
    Channel *c = me();
    Value v = receive(c);
    sum += v.asInt;
    send(c, asInt(sum));
    return asInt(1);
}

void test_sum() {
    for(int i = 0; i < 100; i++) {
        Channel *c = go(add);
        send(c, asInt(i));
        A[i] = receive(c).asInt;
    }

    // sorts the array according to: https://arxiv.org/pdf/2110.01111.pdf
    for(int i = 0; i < 100; i++) {
        for(int j = 0; j < 100; j++) {
            if(A[i] < A[j]) {
                A[i] ^= A[j];
                A[j] ^= A[i];
                A[i] ^= A[j];
            }
        }
    }

    puts("calculates prefix sum array of 0..99");
    printf("sum: %d\n", sum);
    for(int i = 0; i < 100; i++) {
        printf("%d", A[i]);
        if(i != 100 - 1) printf(" ");
    }
    puts("");
}

int it = 0;

int main() {
    test_sort();
    test_sum();

    if(it == 0) {
        it++;
        puts("\nround 2");
        again(); // tests again() on main
    }

    return 0;
}