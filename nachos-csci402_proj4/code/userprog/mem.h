#ifndef Nachos_mem_h
#define Nachos_mem_h

#include "bitmap.h"
#include "machine.h"

class PhysicalMemory {
    
private:
    BitMap *memMap;
public:
    PhysicalMemory() {memMap = new BitMap(NumPhysPages);}
    ~PhysicalMemory() {delete memMap;}
    int FindPages(int pages);
    void ClearPages(int pid, int pages);
    bool IsOver();
};


#endif
