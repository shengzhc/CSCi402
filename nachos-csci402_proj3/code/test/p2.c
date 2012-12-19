#include "syscall.h"
/*This file is used as a process to test part2.*/
void t1() {
    
    Print("Test2:t1 thread of p2 process starts.\n", sizeof("Test2:t1 thread of p2 process starts.\n")/sizeof(char), 0, 0);
    Exit(0);
}

void t2() {
    
    
    Print("Test2:t2 thread of p2 process starts.\n", sizeof("Test2:t2 thread of p2 process starts.\n")/sizeof(char), 0, 0);
    Exit(0);
}
void main()
{
    Print("Test2:p2 process execs.\n", sizeof("Test2:p2 process execs.\n")/sizeof(char), 0, 0);
    Fork(t1);
    Fork(t2);
    Exit(0);
}

