#include <stdio.h>
#include <inttypes.h>

#include "src/go.h"

static Channel* make_child(uint64_t depth);

Value common() {
    uint64_t depth = receive(me()).asU64;
    Channel* parent = receive(me()).asChannel;
    Channel* child = 0;
    if (depth > 0) {
        child = (Channel*) make_child(depth-1);
    }
    while (1) {
        uint64_t v = receive(me()).asU64;
        if (child != 0) {
            send(child,asU64(v+1));
            v = receive(me()).asU64;
        }
        send(parent,asU64(v+1));
    }
   
    printf("the impossible has happened\n");
    return asLong(666);
}

static Channel* make_child(uint64_t depth) {
    Channel* child = go(common);
    send(child,asU64(depth));
    send(child,asChannel(me()));
    return child;
}
    
int main() {
    Channel* child = make_child(100000);

    for (uint64_t i=0; i<12; i++) {
        send(child,asU64(i));
        uint64_t v = receive(me()).asU64;
        printf("%"PRIu64"\n",v);
    }

    printf("done\n");

    return 0;
}
