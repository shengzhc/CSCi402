#include "syscall.h"
void main()
{
    int t;
    for (t=0; t<5; t++) {
        Exec("../test/parent", sizeof("../test/parent")/sizeof(char));
        Exec("../test/child", sizeof("../test/child")/sizeof(char));
    }
    Exit(0);
}