#include "syscall.h"
int l1,l2,m1,c1;
void main()
{
    /*--------------t1 of test1 begins------------------*/
    Print("\nTest1 is for testing lock mechanism.\n", sizeof("\nTest1 is for testing lock mechanism.\n")/sizeof(char), 0, 0);
    Print("t1 of Test1 is for invalid and negative test.\n", sizeof("t1 of Test1 is for invalid and negative test.\n")/sizeof(char), 0, 0);
    Print("create lock1 with invalid name length.\n", sizeof("create lock1 with invalid name length.\n")/sizeof(char), 0, 0);
    l1 = CreateLock("l1", -1);
    Print("create condition1 with invalid name length.\n", sizeof("create condition1 with invalid name length.\n")/sizeof(char), 0, 0);
    c1 = CreateCondition("c1", -1);
    Print("create monitor variable1 with invalid name length.\n", sizeof("create monitor variable1 with invalid name length.\n")/sizeof(char), 0, 0);
    m1 = CreateMV("m1", -1, 3);
    Print("use lock1 with negative index", sizeof("use lock1 with negative index.\n")/sizeof(char), 0, 0);
    Exit(0);
}
