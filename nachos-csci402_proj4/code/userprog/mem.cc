#include <iostream>
#include <assert.h>
#include "mem.h"
using namespace std;
int PhysicalMemory::FindPages(int pages) {
    
    assert(pages>0);
    assert(memMap!=NULL);
    for (int i=0; i<NumPhysPages; i++) {
        if (!memMap->Test(i)) {
            int j;
            for (j=0; j<pages; j++) {
                if (memMap->Test(i+j)) {
                    break;
                }
            }
            if (j>=pages) {
                for (j=0; j<pages; j++) {
                    memMap->Mark(j+i);
                }
                return i;
            }
        }
    }
    return -1;
}

void PhysicalMemory::ClearPages(int pid, int pages) {
    
    assert(pid>=0);
    assert(pages>=0);
    assert(memMap!=NULL);
    for (int i=0; i<pages; i++) {
        memMap->Clear(i+pid);
    }
    return;
    
}

bool PhysicalMemory::IsOver() {

    for (int i=0; i<NumPhysPages; i++) {
        if (memMap->Test(i)) {
            return false;
        }
    }
    return true;
}
