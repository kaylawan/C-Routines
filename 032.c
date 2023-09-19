#include <stdio.h>
#include "src/go.h"

//test1: all of these coroutines should block and exit besides c2 with stuffing
//main should never reach the return statement -> this ensures that channels are independent of each other
//the again() calls in multiple functions should ensure that you have a unique jmpbuf per routine
int k = 0;

Value stuffing() {
    return asInt(666);
}

Value dumpling_skin() {
    Channel* c2 = go(stuffing);
    send(c2, asInt(3));
    again();
    puts("this is really bad - you shouldn't return in dumpling_skin");
    return asLong(666);
}

Value dimsum() {
    Channel* c1 = go(dumpling_skin);
    receive(me());
    send(c1, asInt(2));
    again();
    receive(c1);
    puts("this is really bad - you shouldn't return in dimsum");
    return asLong(666);
}

int main() {    
    k += 1;

    if (k == 10) {
        puts("good job resetting rsp for again() on main!");
        Channel *chan = go(dimsum);
        send(chan, asInt(1));
        puts("congrats, you've passed again() tests!");
        receive(chan);
        puts("this is really bad - you shouldn't be here");
    }

    //test2: again() on main resets rsp -> fill stack make sure the stack is reset
    uint64_t arr[1000000];
    for (int i = 1; i < 1000000; i++) {
        arr[i] += arr[i-1];
    }
    again();

    return 0;
}