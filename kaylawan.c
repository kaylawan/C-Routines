#include <stdio.h>
#include <inttypes.h>

#include "src/go.h"

//Test 1
//This test tests thee ability to call other methods from inside one method as well as returning AsInt values and adding 1 to them sequentially.
//It checks that the routine calls each action in order as well as returning from main.

int num = 0;

void JThroughZ(){
    printf("this should never be called \n");
}

void a() {
    printf("made it to a \n");
    send(me(),asInt(num));
    if (num < 20) {
        num +=1;
        again();
    }
}
void b() {
    a();
}
void c() {
    b();
}
void d() {
    c();
}
void e() {
    d();
}
void f() {
    e();
}
void g() {
    f();
}
void h() {
    g();
}
Value i(){
    printf("in i \n");
    h();
    return asInt(1800);
}

int main() {
    //tests channel creation
    Channel* ch = go(i);
    if(ch==NULL){
        printf("incorrect channel creation implementation \n");
    }
    while (true) {
        int x = receive(ch).asInt;
        if (x == 1800){
            return false;
        }
        else{
            printf("%d\n",x);
            printf("%d\n",x+1);
        }
    }
    printf("this should never be printed out\n");
    printf("this should never be printed out either\n");
    return 0;
    printf("this should never be printed out\n");
}
/*#include <stdio.h>

#include "src/go.h"

Value pointer(){
    Value val = receive(me());
    return asPointer(val.asPointer);
}

Value testShort(){
    Value val = receive(me());
    return asShort(val.asShort);
}

int main(){
    Channel * ch1 = go(pointer);
    int integer = 20;
    int * p = &integer;
    send(ch1, asPointer(p));
    if(receive(ch1).asPointer == NULL){
        printf("channel 1 receive should not be null");
        return 0;
    }

    Channel * ch2 = go(testShort);
    send(ch2, asShort(32767));
    printf("%hu\n", receive(ch2).asShort);

    
    send(ch2, asInt(10));
    printf("%u\n", receive(ch2).asInt); //this should not print
}*/