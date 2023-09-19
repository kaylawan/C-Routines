#include <stdio.h>

#include "src/go.h"

Value factorials() {
    long f = 1;
    long n = 0;

    while (1) {
        send(me(),asLong(f));
        n++;
        f = f * n;
    }
    return asLong(0xdeadbeef);
}

Value sumOfFactorials() {
    Channel* facts = go(factorials);

    long sum = 0;
    while (1) {
        send(me(), asLong(sum += receive(facts).asLong));
    }
    return asLong(0xdeadbeef);
}

int main() {
    Channel* ch1 = go(factorials);
    Channel* ch2 = go(sumOfFactorials);

    for (long i = 0; i<20; i++) {
        long f = receive(ch1).asLong;
        long s = receive(ch2).asLong;
        printf("i=%ld, f=%ld, s=%ld\n",i,f,s);
    }
    return 0;
}
