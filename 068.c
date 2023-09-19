#include <stdio.h>
#include <inttypes.h>

#include "src/go.h"

#define NUM_TESTS (100)

static uint64_t num = 0;
static Channel * done;

// this test ping pongs across channels to test sending and receiving in various contexts

Value func() {
    for (uint64_t i=0; i<NUM_TESTS; i++) {
        Channel* ch = receive(me()).asChannel;
        send(ch, asChannel(me()));
        num+=i;
    }
    send(done, asChannel(me()));
    return asChannel(me());
}

Value func2() {
    for (uint64_t i=0; i<NUM_TESTS; i++) {
        Channel* ch = receive(me()).asChannel;
        send(ch, asChannel(me()));
        num+=i;
    }
    send(done, asChannel(me()));
    return asChannel(me());
}

int main() {
    done = channel();

    Channel* ch = go(func);
    Channel* ch2 = go(func2);

    send(ch, asChannel(ch2));
    receive(done);

    uint64_t ans = (NUM_TESTS-1) * (NUM_TESTS - 1);
    if(ans == num)
        printf("passed\n"); 
    else
        printf("failed got %lu instead of %lu\n", num, ans); 

    return 0;
}
