#include "syscall.h"
int l1,c1,m1,value;
void main()
{
    Print("\nTest2 is for waiting and signaling condition variable.\n", sizeof("\nTest2 is for waiting and signaling condition variable.\n")/sizeof(char), 0, 0);
    l1 = CreateLock("l1", 2);
    c1 = CreateCondition("c1", 2);
    m1 = CreateMV("m1", 2, 3);
    
    AcquireLock(l1);
    Print("client1 of Test2 has acquired lock1.\n", sizeof("client1 of Test2 has acquired lock1.\n")/sizeof(char), 0, 0);
    if (value == 0) {
        Print("client1 is waiting other to signal.\n", sizeof("client1 is waiting other to signal.\n")/sizeof(char), 0, 0);
        WaitCondition(c1, l1);
    }
    value = GetMV(m1, 1);
    SetMV(m1, 1, 5);
    ReleaseLock(l1);
    Print("client1 of Test2 has released lock1.\n", sizeof("client1 of Test2 has released lock1.\n")/sizeof(char), 0, 0);
    Exit(0);
}

