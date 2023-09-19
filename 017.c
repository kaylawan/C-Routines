#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>

#include "src/go.h"

Value r1();
Value r2();
Value r3(); 

int main() {

    //Creating channel = shouldn't override what is in the routine's
    //channel - tests to make sure these channels don't get added
    //to routine's ch
    Channel* channel1 = channel();

    if(me() == channel1)
    {
        printf("You should not store new channels, that don't spawn anything \
                on the co-routine");
    }

    Channel* routine1C = go(r1);
    go(r2);
    go(r3);
    go(r1);

    send(routine1C, asLong(2));

    return 0;
}

//The following methods check whether you can spawn
//routines multiple times and prints out the respective output in r3
Value r1() {
    go(r2);
    go(r2);
    return asLong(0);
}

Value r2() {
    go(r3);
    go(r3);
    return asLong(0);
}

Value r3() {
    printf("r3 successfully called.\n");
    return asLong(0);
}




