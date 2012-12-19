#include "syscall.h"
/*----------------------------
 part1 test suite
 ----------------------------*/
int lock1,lock2,cv1;
/*-------------------------------------------------
 Test 1 --test acquirelock and releaselock syscall
 -------------------------------------------------*/
/*----------------------------
t1_t1()--test1 thread1
 This is the rightful lock owner
 ----------------------------*/
void t1_t1(){
    int i=0;
    Print("\nThe t1_t1 test is for the rightful lock owner.\n", sizeof("\nThe t1_t1 test is for the rightful lock owner.\n")/sizeof(char), 0, 0);
    if(AcquireLock(lock1))
        Print("t1_t1 has acquired lock1 successfully.\n", sizeof("t1_t1 has acquired lock1 successfully.\n")/sizeof(char), 0, 0);
    Yield();
    if(ReleaseLock(lock1))
        Print("t1_t1 has released lock1 successfully.\n", sizeof("t1_t1 has released lock1 successfully.\n")/sizeof(char), 0, 0);
    
    Exit(0);

}
/*----------------------------------------
 t1_t2()--test1 thread2
 This thread will wait on the held lock.
 -----------------------------------------*/
void t1_t2(){
    Print("\nThe t1_t2 test is for the thread waiting on the held lock.\n", sizeof("\nThe t1_t2 test is for the thread waiting on the held lock.\n")/sizeof(char), 0, 0);
    Print("t1_t2 trying to acquire lock1.\n", sizeof("t1_t2 trying to acquire lock1.\n")/sizeof(char), 0, 0);
    if(AcquireLock(lock1))
        Print("t1_t2 has acquired lock1 successfully.\n", sizeof("t1_t2 has acquired lock1 successfully.\n")/sizeof(char), 0, 0);
    if(ReleaseLock(lock1))
        Print("t1_t2 has released lock1 successfully.\n", sizeof("t1_t2 has released lock1 successfully.\n")/sizeof(char), 0, 0);
    
    Exit(0);
    
}
/*--------------------------------------------------
 t1_t3() -- test1 thread 3
 This thread will try to release the lock illegally
--------------------------------------------------*/
void t1_t3(){
    Print("\nThe t1_t3 test is for the illegal lock release.\n", sizeof("\nThe t1_t1 test is for the illegal lock release.\n")/sizeof(char), 0, 0);
    Print("t1_t3 trying to release lock1.\n", sizeof("t1_t3 trying to release lock1.\n")/sizeof(char), 0, 0);
    ReleaseLock(lock1);
    Exit(0);

}
/*------------------------------------------------------
 Test 2 --test signalcondition and waitcondition syscall
 -------------------------------------------------------*/
/*--------------------------------------------------
 t2_t1() -- test2 thread 1
 This thread will wait other thread to wake up
 --------------------------------------------------*/

void t2_t1(){
    Print("\nThe t2_t1 test is for the thread waiting to be waken up.\n", sizeof("\nThe t1_t1 test is for the thread waiting to be waken up.\n")/sizeof(char), 0, 0);
    if(AcquireLock(lock1))
        Print("t2_t1 has acquired lock1.\n", sizeof("t2_t1 has acquired lock1.\n")/sizeof(char), 0, 0);
    
     Print("t2_t1 is waiting for signal.\n", sizeof("t2_t1 is waiting for signal.\n")/sizeof(char), 0, 0);
    
    if (WaitCondition(cv1, lock1))
        Print("t2_t1 is signaled.\n", sizeof("t2_t1 is signaled.\n")/sizeof(char), 0, 0);
    
    if(ReleaseLock(lock1))
        Print("t2_t1 release lock1.\n", sizeof("t2_t1 release lock1.\n")/sizeof(char), 0, 0);
    
    Exit(0);
}
/*---------------------------------------------------------
 t2_t2() -- test2 thread 2
 This thread is trying to wake up other thread with wrong lock 
 ---------------------------------------------------------*/
void t2_t2(){
    Print("\nThe t2_t2 test is for signaling t2_t1 using wrong lock.\n", sizeof("\nThe t2_t2 test is for signaling t2_t1 using wrong lock.\n")/sizeof(char), 0, 0);
    if(AcquireLock(lock1))
        Print("t2_t2 has acquired lock1.\n", sizeof("t2_t2 has acquired lock1.\n")/sizeof(char), 0, 0);
    Print("t2_t2 trying to wake thread with lock2.\n", sizeof("t2_t2 trying to wake thread with lock2.\n")/sizeof(char), 0, 0);
    SignalCondition(cv1, lock2);
    if(ReleaseLock(lock1))
        Print("t2_t2 release lock1.\n", sizeof("t2_t2 release lock1.\n")/sizeof(char), 0, 0);
    
    Exit(0);
}
/*--------------------------------------------------
 t2_t3() -- test2 thread 3
 This thread will signal the waiting thread.
 --------------------------------------------------*/
void t2_t3(){
    Print("\nThe t2_t3 test is for signaling t2_t1 correctly.\n", sizeof("\nThe t2_t2 test is for signaling t2_t1 correctly.\n")/sizeof(char), 0, 0);
    if(AcquireLock(lock1))
        Print("t2_t3 has acquired lock1.\n", sizeof("t2_t3 has acquired lock1.\n")/sizeof(char), 0, 0);
    
    if (SignalCondition(cv1, lock1))
        Print("t2_t3 trying to signal t2_t1.\n", sizeof("t2_t3 trying to signal t2_t1.\n")/sizeof(char), 0, 0);
    
    if(ReleaseLock(lock1))
        Print("t2_t3 release lock1.\n", sizeof("t2_t3 release lock1.\n")/sizeof(char), 0, 0);
    
    Exit(0);
}
/*------------------------------------------------------
 Test 3 --test broadcastcondition syscall
 -------------------------------------------------------*/

/*--------------------------------------------------
 t3_t1() & t3_t2() -- test3 thread 1 and thread 2
 Both thread will wait other thread to wake them up
 --------------------------------------------------*/
void t3_t1(){
    Print("\nThe t3_t1 and t3_t2 test is for other thread to broadcast.\n", sizeof("\nThe t3_t1 and t3_t2 test is for other thread to broadcast.\n")/sizeof(char), 0, 0);
    if(AcquireLock(lock1))
        Print("t3_t1 has acquired lock1.\n", sizeof("t2_t1 has acquired lock1.\n")/sizeof(char), 0, 0);
    
    Print("t3_t1 is waiting for signal.\n", sizeof("t2_t1 is waiting for signal.\n")/sizeof(char), 0, 0);
    
    if (WaitCondition(cv1, lock1))
        Print("t3_t1 is signaled.\n", sizeof("t2_t1 is signaled.\n")/sizeof(char), 0, 0);
    
    if(ReleaseLock(lock1))
        Print("t3_t1 release lock1.\n", sizeof("t2_t1 release lock1.\n")/sizeof(char), 0, 0);
    
    Exit(0);
}
void t3_t2(){
    if(AcquireLock(lock1))
        Print("t2_t2 has acquired lock1.\n", sizeof("t2_t1 has acquired lock1.\n")/sizeof(char), 0, 0);
    
    Print("t3_t2 is waiting for signal.\n", sizeof("t2_t1 is waiting for signal.\n")/sizeof(char), 0, 0);
    
    if (WaitCondition(cv1, lock1))
        Print("t3_t2 is signaled.\n", sizeof("t2_t1 is signaled.\n")/sizeof(char), 0, 0);
    
    if(ReleaseLock(lock1))
        Print("t3_t2 release lock1.\n", sizeof("t2_t1 release lock1.\n")/sizeof(char), 0, 0);
    
    Exit(0);
}
/*--------------------------------------------------
 t3_t3() -- test3 thread 3
 This thread will broadcast the waiting threads
 --------------------------------------------------*/
void t3_t3() {
    Print("\nThe t3_t3 test is to broadcast other waiting threads.\n", sizeof("\nThe t3_t3 test is to broadcast other waiting threads.\n")/sizeof(char), 0, 0);
    if (AcquireLock(lock1)) {
        Print("t3_t3 has acquired lock1.\n", sizeof("t3_t3 has acquired lock1.\n")/sizeof(char), 0, 0);
    }
    
    if (BroadcastCondition(cv1, lock1)) {
        
        Print("t3_t3 Signal all.\n", sizeof("t3_t3 Signal all.\n")/sizeof(char), 0, 0);
    }
    
    if (ReleaseLock(lock1)) {
        
        Print("t3_t3 release lock1.\n", sizeof("t3_t3 release lock1.\n")/sizeof(char), 0, 0);
    }
    
    Exit(0);
}
/*--------------------------------------------------
 Test 4 --- test line buffering behavior
 -------------------------------------------------*/
/*--------------------------------------------------
 t4_t1() -- test4 thread 1
 This thread will test the output line buffering
 --------------------------------------------------*/
void t4_t1(){
    Print("\nThe t4_t1 test is to test the output line buffering.\n", sizeof("\nThe t4_t1 test is to test the output line buffering.\n")/sizeof(char), 0, 0);
    Print("If you can see this,the output line buffering is correct.\n", sizeof("If you can see this,the output line buffering is correct.\n")/sizeof(char), 0, 0);
    Exit(0);

}
/*--------------------------------------------------
 t4_t2() -- test4 thread 2
 This thread will test the input line buffering
 --------------------------------------------------*/
void t4_t2(){
    int num;
    Print("\nThe t4_t2 test is to test the input line buffering.\n", sizeof("\nThe t4_t1 test is to test the input line buffering.\n")/sizeof(char), 0, 0);
    Print("Please enter a int number: \n", sizeof("Please enter a int number: \n")/sizeof(char), 0, 0);
    num=GetInput();
    Print("The number you enter is:", sizeof("The number you enter is:\n")/sizeof(char), 0, 0);
    Print("%d.\n", sizeof("%d.\n")/sizeof(char), 1, num);
    Exit(0);
}
int main(){
    
    int i=0;
 
    lock1 = CreateLock("lock1", sizeof("lock1")/sizeof(char));
    lock2 = CreateLock("lock2", sizeof("lock2")/sizeof(char));
    
    cv1 = CreateCondition("cv1", sizeof("cv1")/sizeof("cv1"));
    /*--------------------------------------------------
     The Fork syscall can be tested through using it correctly
     --------------------------------------------------*/
    Fork(t1_t1);
    
    Fork(t1_t2);
    
    for(;i<3;i++)
        
    {Yield();}
    
    Fork(t1_t3);
    /*--------------------------------------------------
     The Yield syscall can be tested through using it correctly
     --------------------------------------------------*/
    Yield();
    
    Fork(t2_t1);
    
    Fork(t2_t2);
    
    Fork(t2_t3);
    
    Yield();
    
    Fork(t3_t1);
    
    Fork(t3_t2);
    
    Fork(t3_t3);

    /*--------------------------------------------------
     The Exit syscall can be tested through using it correctly
     --------------------------------------------------*/
    Yield();
    Fork(t4_t1);
    Fork(t4_t2);
    Exit(0);
    
}