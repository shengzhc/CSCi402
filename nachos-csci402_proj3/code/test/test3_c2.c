#include "syscall.h"
int l1,c1,m1,value;

void main()
{
    Print("\nTest3 is for waiting and broadcasting condition variable.\n", sizeof("\nTest3 is for waiting and broadcasting condition variable.\n")/sizeof(char), 0, 0);
     l1 = CreateLock("l1", 2);
     c1 = CreateCondition("c1", 2);
     m1 = CreateMV("m1", 2, 3);
        
    AcquireLock(l1);
    Print("client2 of Test3 has acquired lock1.\n", sizeof("client2 of Test3 has acquired lock1.\n")/sizeof(char), 0, 0);
    if (value == 0) {
        Print("client2 is waiting other to signal.\n", sizeof("client2 is waiting other to signal.\n")/sizeof(char), 0, 0);
        WaitCondition(c1, l1);
    }
    value = GetMV(m1, 1);
    Print("The MV value of client2 is [%d].\n", sizeof("The MV value of client2 is [%d].\n")/sizeof(char), 1, value);
    ReleaseLock(l1);
    Print("client2 of Test3 has released lock1.\n", sizeof("client2 of Test3 has released lock1.\n")/sizeof(char), 0, 0);
    Exit(0);
}

