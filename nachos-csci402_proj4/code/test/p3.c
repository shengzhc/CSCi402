#include "syscall.h"
void main()
{
    int i;
    for (i=0; i<8; i++) {
        Exec("../test/patient", sizeof("../test/patient")/sizeof(char));
    }
    Exit(0);
}

