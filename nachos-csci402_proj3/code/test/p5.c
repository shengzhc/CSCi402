#include "syscall.h"
/*This file is used for user level error.The exact error is the float data type.*/
void t1(){
    float a=3.0;
    Print("%d",sizeof("%d")/sizeof(char),1,a);
    
}
void main()
{
    
    Print("Test6:p6 process execs.\n", sizeof("Test6:p6 process execs.\n")/sizeof(char), 0, 0);
    Fork(t1);
    Exit(0);
}