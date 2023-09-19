#include <stdio.h>
#include <stdbool.h>
#include "src/go.h"

#define MAX_PRIMES 100000

bool primes[MAX_PRIMES];

Value filter() {
    long prime = receive(me()).asLong;
    while (true) {
        long n = receive(me()).asLong;
        if (n == 0) break;
        if (n % prime != 0) send(me(), asLong(n));
    }
    return asLong(0);
}

int main() {
    long n=20;

    for (long i = 2; i <= n; i++) {
        primes[i] = true;
    }

    for (long i = 2; i <= n; i++) {
        if (primes[i]) {
            Channel* ch = go(filter);
            send(ch, asLong(i));
            for (long j = i * 2; j <= n; j += i) {
                primes[j] = false;
                send(ch, asLong(j));
            }
            send(ch, asLong(0));
        }
    }

    printf("Primes: ");
    for (long i = 2; i <= n; i++) {
        if (primes[i]) {
            printf("%ld ", i);
        }
    }
    printf("\n");

    return 0;
}