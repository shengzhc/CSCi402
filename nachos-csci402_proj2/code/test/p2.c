#include "syscall.h"
/*This file is used as a process to test part2.*/
void t1() {
    
    Print("t1 thread of p2 process starts.\n", sizeof("t1 thread of p2 process starts.\n")/sizeof(char), 0, 0);
    Exit(0);
}

void t2() {
    
    
    Print("t2 thread of p2 process starts.\n", sizeof("t1 thread of p2 process starts.\n")/sizeof(char), 0, 0);
    Exit(0);
}
void main()
{
    Print("p2 process execs.\n", sizeof("p2 process execs.\n")/sizeof(char), 0, 0);
    Fork(t1);
    Fork(t2);
    Exit(0);
}

