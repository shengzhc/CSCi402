#include "syscall.h"
#include "DoctorOffice.h"
void main()
{
    int num;
    int i;
    i=0;
    Print("Please choose how many simulation process you want to exec: ",sizeof("Please choose how many simulation process you want to exec: ")/sizeof(char),0,0);
    num=GetInput();
    if (num>8) {
        Print("The max process number is 8.\n",sizeof("The max process number is 8.\n")/sizeof(char),0,0);
    } else {
        for (;i<num; i++) {
            Exec("../test/DoctorOffice", sizeof("../test/DoctorOffice")/sizeof(char));
        }
    }
    Exit(0);  
}

