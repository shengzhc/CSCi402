#include "syscall.h"
/*------------------------------------
 part2 test suite
 -------------------------------------*/
/*------------------------------------
 Test 1 -- test Exec syscall
 -------------------------------------*/
void test1(){
Print("Test1 is for Exec syscall.\n", sizeof("Test1 is for multiprocess.\n")/sizeof(char), 0, 0);
Print("Test1:p1 process trying to exec.\n", sizeof("Test1:p1 process tring to exec.\n")/sizeof(char), 0, 0);
Exec("../test/p1", sizeof("../test/p1")/sizeof(char));
Exit(0);
}


/*------------------------------------
 Test 2 -- test Fork syscall
 -------------------------------------*/
void test2(){
    Yield();
    Print("\nTest2 is for Fork syscall.\n", sizeof("\nTest2 is for Fork syscall.\n")/sizeof(char), 0, 0);
    Print("Test2:p2 process trying to exec.\n", sizeof("Test2:ap2 process tring to exec.\n")/sizeof(char), 0, 0);
    Exec("../test/p2", sizeof("../test/p2")/sizeof(char));
    Exit(0);

}
/*------------------------------------
 Test 3 -- test multiprocess
 -------------------------------------*/
void test3(){
    Yield();
    Yield();
    Yield();
    Print("\nTest3 is for multiprocess.\n", sizeof("\nTest3 is for multiprocess.\n")/sizeof(char), 0, 0);
    Exec("../test/p3", sizeof("../test/p3")/sizeof(char));
    Exec("../test/p3", sizeof("../test/p3")/sizeof(char));
    Exec("../test/p3", sizeof("../test/p3")/sizeof(char));
    Exit(0);
    
}

/*-------------------------------------------
 Test 4 -- test multiprocess and multithread
 -------------------------------------------*/
void test4(){
    Yield();
    Yield();
    Yield();
    Yield();
    Print("\nTest4 is for multiprocess and multithread.\n", sizeof("\nTest4 is for multiprocess and multithread.\n")/sizeof(char), 0, 0);
    Exec("../test/p4", sizeof("../test/p4")/sizeof(char));
    Yield();
    Exec("../test/p4", sizeof("../test/p4")/sizeof(char));
    Exit(0);

}
/*---------------------------------------------------
 Test 5 -- test error input for Exec syscall
 --------------------------------------------------*/
void test5(){
    Yield();
    Yield();
    Yield();
    Yield();
    Yield();
    Yield();
    Yield();
    Print("\nTest5 is for Exec syscall error input.\n", sizeof("\nTest6 is for Exec syscall error input.\n")/sizeof(char), 0, 0);
    Exec("../test/error", sizeof("../test/error")/sizeof(char));
        Exit(0);
}
void test6(){
    Yield();
    Yield();
    Yield();
    Yield();
    Yield();
    Yield();
    Yield();
    Yield();
    Print("\nTest6 is for userlevel error.\n", sizeof("\nTest5 is for userlevel error.\n")/sizeof(char), 0, 0);
    Exec("../test/p5", sizeof("../test/p5")/sizeof(char));
    Exit(0);
}
void main()
{
    Fork(test1);
    Fork(test2);
    Fork(test3);
    Fork(test4);
    Fork(test5);
    Fork(test6);
    Exit(0);
}
