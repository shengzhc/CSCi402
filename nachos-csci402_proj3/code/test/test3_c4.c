#include "syscall.h"
int l1,c1,m1,value;

void main()
{
    Print("\nTest3 is for waiting and broadcasting condition variable.\n", sizeof("\nTest3 is for waiting and broadcasting condition variable.\n")/sizeof(char), 0, 0);
    l1 = CreateLock("l1", 2);
     c1 = CreateCondition("c1", 2);
     m1 = CreateMV("m1", 2, 3);
    
    
    AcquireLock(l1);
    Print("client4 of Test3 has acquired lock1.\n", sizeof("client4 of Test3 has acquired lock1.\n")/sizeof(char), 0, 0);
    SetMV(m1, 1, 10);
    BroadcastCondition(c1,l1);
    Print("client4 of Test3 broadcasts all clients.\n", sizeof("client4 of Test3 broadcasts all clients.\n")/sizeof(char), 0, 0);
    ReleaseLock(l1);
    Print("client4 of Test3 has released lock1.\n", sizeof("client4 of Test3 has released lock1.\n")/sizeof(char), 0, 0);
    Exit(0);
}

