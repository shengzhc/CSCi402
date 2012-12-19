#include "syscall.h"
int l1,c1,m1,value;
void main()
{
    Print("\nTest2 is for waiting and signaling condition variable.\n", sizeof("\nTest2 is for waiting and signaling condition variable.\n")/sizeof(char), 0, 0);
     l1 = CreateLock("l1", 2);
     c1 = CreateCondition("c1", 2);
     m1 = CreateMV("m1", 2, 3);
    
    AcquireLock(l1);
    Print("client2 of Test2 has acquired lock1.\n", sizeof("client2 of Test2 has acquired lock1.\n")/sizeof(char), 0, 0);
    SetMV(m1, 1, 10);
    SignalCondition(c1, l1);
    Print("client2 of Test2 signals client1.\n", sizeof("client2 of Test2 signals client1.\n")/sizeof(char), 0, 0);
    ReleaseLock(l1);
    Print("client2 of Test2 has released lock1.\n", sizeof("client2 of Test2 has released lock1.\n")/sizeof(char), 0, 0);
    Exit(0);
}
