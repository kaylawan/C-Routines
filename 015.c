#include <stdio.h>
#include "src/go.h"

//This is a test to verify that the state of the stack and registers is preserved when switching between coroutines

//This keeps track of whether or not f1 and f2 have been executed 
int counter = 0;
//This channel is used to block main
Channel* to_main;
//This channel is used to context switch between f1 and f2
Channel* context_switch;

Value f1(){
//local vars
    int a = 0;
    int b = 0;
    int c = 0;
    int d = 4;
    int e = 5;

//arithmetic for possible register use
    a = a+1;
    b = a + b;
    b = a + b;
    c = a + b;

//loop counter for possible register use
    for(int i = 0; i < 1000; i++){
        if (i == 500){
            //this forces context switch in the middle of for loop
            send(context_switch, asInt(0));
            receive(context_switch);
        }
        if(i == 510){
            printf("success\n");
        }
    }

//verify that state was preserved
    if (a != 1 || b != 2 || c!= 3 || d!= 4 || e!= 5){
        printf("f1 state is incorrect\n");
    }

//signal main if f1 and f2 are complete
    counter++;
    if (counter == 2){
        send(to_main, asInt(0));
    }

    return asInt(0);
}

Value f2(){
    receive(context_switch);
//create f2 state
    int a = 0;
    int b = 0;
    int c = 0;
    int d = 4;
    int e = 5;
    a = a+1;
    b = a + b;
    b = a + b;
    c = a + b;

//possible context switch
    for(int i = 0; i < 1000; i++){
        if (i == 800){
            send(context_switch, asInt(0));
        }
        if(i == 801){
            printf("success\n");
        }
    }

//verify state
    if (a != 1 || b != 2 || c!= 3 || d!= 4 || e!= 5){
        printf("f2 state is incorrect\n");
    }

//signal main
    counter++;
    if (counter == 2){
        send(to_main, asInt(0));
    }

    return asInt(0);
}

int main(){
    to_main = channel();
    context_switch = channel();

    go(f1);
    go(f2);

//forces f1 and f2 to execute
    receive(to_main);

    return 0;
}

