#include "syscall.h"

typedef enum {
    TRUE, FALSE
}BP;
struct ABC {
    int a;
    int b;
    BP c;
};

void t1() {
    
    int erIndex;
    int i=0;
    for (; i<5; i++) {
        erIndex=1;
        Yield();
        Print("t1_%d.\n", sizeof("t1_%d.\n")/sizeof(char), 1, erIndex);
    }
    Exit(0);
}

void t2() {
    
    int erIndex=0;
    int i=0;
    for (; i<5; i++) {
        Yield();
        Print("t2_%d.\n", sizeof("t2_%d.\n")/sizeof(char), 1, erIndex);
        erIndex=2;
    }
    Exit(0);
}

void p1() {
    
    Print("Start p1.\n", sizeof("Start p1.\n")/sizeof(char), 0, 0);
    Fork(t1);
    Fork(t2);
    Print("End p1.\n", sizeof("End p1.\n")/sizeof(char), 0, 0);
    Exit(0);
}

void p2() {
    
    Print("Start p2.\n", sizeof("Start p2.\n")/sizeof(char), 0, 0);
    Fork(t1);
    Fork(t2);
    Print("End p2.\n", sizeof("End p2.\n")/sizeof(char), 0, 0);
    Exit(0);
}

void main()
{
    int a, b;
    a = GetInput();
    b = GetInput();
    Print("%d\n", 3, 1, a);
    Print("%d\n", 3, 1, b);
    Exit(0);
}
