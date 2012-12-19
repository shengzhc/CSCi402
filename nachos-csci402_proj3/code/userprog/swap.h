#ifndef Nachos_swap_h
#define Nachos_swap_h

#include "translate.h"
#include "filesys.h"
#include "bitmap.h"
#include "synch.h"
#include "openfile.h"

// Define the swap entry
class SwapTranslationEntry {
    
public:
    int virtualPage;  	
    int physicalPage;  	
    bool valid;         
    bool readOnly;	
    bool use;          
    bool dirty;     
    int processId;
public:
    SwapTranslationEntry() { processId=-1;virtualPage=-1;physicalPage=-1;valid=false;readOnly=true;use=false;dirty=false;}
};

class Swap {
    
public:
    Swap(char *fn, int sfp, int ps, FileSystem *fs);
    ~Swap();
    
    int GetVirtualPage(int entry);
    void SetVirtualPage(int entry, int value);
    int GetPhysicalPage(int entry);
    void SetPhysicalPage(int entry, int value);
    bool GetValidBit(int entry);
    void SetValidBit(int entry, bool value);
    bool GetDirtyBit(int entry);
    void SetDirtyBit(int entry, bool value);
    bool GetUseBit(int entry);
    void SetUseBit(int entry, bool value);
    bool GetROBit(int entry);
    void SetROBit(int entry, bool value);
    int GetPid(int entry);
    void SetPid(int entry, int value);
    int GetSwapFilePages() {return swapFilePages;}
    int GetPageSize() {return pageSize;}
    
    OpenFile* GetSwapBuffer();
    int Find();
    int SearchEntry(int vpn, int processId);
    void Clear(int entry);
    void RemoveInfo(int processId);
        
private:
    char *filename;
    int swapFilePages; // define the number of pages in swap file
    int pageSize; // define the page size
    
    BitMap *swapBufferBitMap;
    OpenFile *swapBuffer;
    FileSystem *filesys;
    SwapTranslationEntry *stlb;    
};

#endif
