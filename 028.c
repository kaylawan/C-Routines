#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "src/go.h"

/*
 * This test case checks that sends before infinite return values
 * are received first.
 *
 * If you are failing this test case, make sure to receive the senders first,
 * before receiving the infinite return values.
 *
 * The second part of this test case checks that the more rare types
 * of Value are returned correctly.
 *
 * Due to the 2000 characters limit of the test, I can't test integer types.
 * I wish I could though :(
 */

int testInt;

Value ret() {
    for (int i = 1; i <= 100000; ++i)
        send(me(), asLongLong(14000605));
    return asLongLong(1);
}

Value retFunc() {
    return asFunc(ret);
}

Value retPointer() {
    return asPointer(&testInt);
}

Value retChannel() {
    return asChannel(me());
}

Value retString() {
    return asString("P5 is cool!");
}

int main() {
    Channel *channel = go(ret);
    bool matched = true;

    // Receive the senders
    for (int i = 1; i <= 100000; ++i)
        if (receive(channel).asLongLong != 14000605)
            matched = false;

    // Receive the infinite return values
    for (int i = 1; i <= 100000; ++i)
        if (receive(channel).asLongLong != 1)
            matched = false;

    // Check that all the values match, for all types
    Channel *FuncCh = go(retFunc);
    if (receive(FuncCh).asFunc != ret)
        matched = false;

    Channel *PointerCh = go(retPointer);
    if (receive(PointerCh).asPointer != &testInt)
        matched = false;

    Channel *ChannelCh = go(retChannel);
    if (receive(ChannelCh).asChannel != ChannelCh)
        matched = false;

    Channel *StringCh = go(retString);
    if (strcmp(receive(StringCh).asString, "P5 is cool!") != 0)
        matched = false;

    // Check that everything matched
    if (matched) {
        puts("Proud of you! :)");
    } else {
        puts("Make sure you are matching with senders before infinite return values.");
        puts("Also make sure to handle all Value types correctly.");
        puts("Try again! I believe in you!");
    }
}
