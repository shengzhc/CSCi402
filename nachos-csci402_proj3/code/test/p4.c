#include "syscall.h"
/*This file is used as a process to test part2.*/
void t1() {
    
    Print("Test4:t1 thread of p4 process starts.\n", sizeof("Test4:t1 thread of p4 process starts.\n")/sizeof(char), 0, 0);
    Exit(0);
}

void t2() {
    
    
    Print("Test4:t2 thread of p4 process starts.\n", sizeof("Test4:t2 thread of p4 process starts.\n")/sizeof(char), 0, 0);
    Exit(0);
}
void main()
{
    Print("Test4:p4 process execs.\n", sizeof("Test4:p4 process execs.\n")/sizeof(char), 0, 0);
    Fork(t1);
    Fork(t2);
    Exit(0);
}

