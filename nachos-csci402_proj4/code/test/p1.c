#include "syscall.h"
void main()
{
    int i;
    Exec("../test/wrn", sizeof("../test/wrn")/sizeof(char));
    
    for (i=0; i<5; i++) {
        Exec("../test/nurse", sizeof("../test/nurse")/sizeof(char));
    }
    
    for (i=0; i<2; i++) {
        Exec("../test/doctor", sizeof("../test/doctor")/sizeof(char));
    }

    for (i=0; i<2; i++) {
        Exec("../test/xray", sizeof("../test/xray")/sizeof(char));
    }
    Exec("../test/cashier", sizeof("../test/cashier")/sizeof(char));
    Exit(0);
}