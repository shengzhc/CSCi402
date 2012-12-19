#ifndef Nachos_IPT_h
#define Nachos_IPT_h

#include "translate.h"
#include "bitmap.h"
#include "swap.h"
#include "list.h"
#include "synch.h"
// Define the ipt entry 
class IPTTranslationEntry {

public:
    int virtualPage;  	
    int physicalPage;  	
    bool valid;         
    bool readOnly;	
    bool use;          
    bool dirty;     
    int processId;
public:
    IPTTranslationEntry(){ processId=-1;virtualPage=-1;physicalPage=-1;valid=false;readOnly=true;use=false;dirty=false;}
};

// Define the table struct to manage ipt entry
class InvertedPageTable {

public:
    InvertedPageTable();
    InvertedPageTable(int ps, int pe);
    ~InvertedPageTable();
    
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
    
    int PageInIpt(int vaddr, int processId);
    int Find();
    int AllocatePage(Swap *swap);
    void ContextSwitch();
    void UpdateIPT();
    int FreeCount();
    void RemoveInfo(int processId);
    
private:
    int pages; // define the number of entries in a ipt
    int pageSize; // define each page size
    IPTTranslationEntry *ipt; // define the ipt
    BitMap *iptBitmap; // use as the free ptr find tool
    List *queue; // used when apply a FIFO swapping approach
};


#endif
