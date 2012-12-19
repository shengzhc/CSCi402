#include "syscall.h"
int l1,l2,m1,c1;
void main()
{
    Print("t3 of Test1 is for releasing lock uncorrectly.\n", sizeof("t3 of Test1 is for releasing lock uncorrectly.\n")/sizeof(char), 0, 0);
    l1 = CreateLock("l1", 2);
    l2 = CreateLock("l2", 2);    
    AcquireLock(l1);
    Print("t3 of Test1 has acquired lock1.\n", sizeof("t3 of Test1 has acquired lock1.\n")/sizeof(char), 0, 0);
    ReleaseLock(l2);
    Exit(0);
    
    
}