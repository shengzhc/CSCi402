#include "syscall.h"
int l1,l2,m1,c1;
void main()
{
   Print("t2 of Test1 is for creating,acquiring and releasing lock correctly.\n", sizeof("t2 of Test1 is for creating,acquiring and releasing lock correctly.\n")/sizeof(char), 0, 0);
    l1 = CreateLock("l1", 2);
    Print("t2 of Test1 has created lock1.\n", sizeof("t2 of Test1 has created lock1.\n")/sizeof(char), 0, 0);
    AcquireLock(l1);
    Print("t2 of Test1 has acquired lock1.\n", sizeof("t2 of Test1 has acquired lock1.\n")/sizeof(char), 0, 0);
    ReleaseLock(l1);
    Print("t2 of Test1 has released lock1.\n", sizeof("t2 of Test1 has released lock1.\n")/sizeof(char), 0, 0);
    Exit(0);


}