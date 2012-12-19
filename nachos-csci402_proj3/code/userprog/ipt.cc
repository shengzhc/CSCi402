#include <iostream>
#include "ipt.h"
#include "machine.h"
#include "system.h"

InvertedPageTable::InvertedPageTable() 
{
    InvertedPageTable(NumPhysPages, PageSize);
}

InvertedPageTable::InvertedPageTable(int ps, int pe)
{
    this->pages = ps;
    this->pageSize = pe;
    ipt = new IPTTranslationEntry[this->pages];
    iptBitmap = new BitMap(this->pages);
    for (int i=0; i<this->pages; i++) {
        ipt[i].physicalPage = i;
    }
    queue = new List();
}

InvertedPageTable::~InvertedPageTable()
{
    if (ipt != NULL)
        delete ipt;
    
    if (iptBitmap != NULL) 
        delete iptBitmap;
    
    delete queue;
}

int InvertedPageTable::GetVirtualPage(int entry)
{
    return ipt[entry].virtualPage;
}

void InvertedPageTable::SetVirtualPage(int entry, int value)
{
    ipt[entry].virtualPage = value;
}

int InvertedPageTable::GetPhysicalPage(int entry)
{
    return ipt[entry].physicalPage;
}

void InvertedPageTable::SetPhysicalPage(int entry, int value)
{
    ipt[entry].physicalPage = value;
}

bool InvertedPageTable::GetValidBit(int entry)
{
    return ipt[entry].valid;
}

void InvertedPageTable::SetValidBit(int entry, bool value)
{
    ipt[entry].valid = value;
}

bool InvertedPageTable::GetDirtyBit(int entry)
{
    return ipt[entry].dirty;
}

void InvertedPageTable::SetDirtyBit(int entry, bool value)
{
    ipt[entry].dirty = value;
}

bool InvertedPageTable::GetUseBit(int entry)
{
    return ipt[entry].use;
}

void InvertedPageTable::SetUseBit(int entry, bool value)
{
    ipt[entry].use = value;
}

bool InvertedPageTable::GetROBit(int entry)
{
    return ipt[entry].readOnly;
}

void InvertedPageTable::SetROBit(int entry, bool value)
{
    ipt[entry].readOnly = value;
}

int InvertedPageTable::GetPid(int entry)
{
    return ipt[entry].processId;
}

void InvertedPageTable::SetPid(int entry, int value)
{
    ipt[entry].processId = value;
}

// Check whether the page is in ipt
int InvertedPageTable::PageInIpt(int vaddr, int processId)
{   
    int virPage = (int)(vaddr/pageSize);
    for (int i=0; i<pages; i++) {
        if (ipt[i].processId == processId &&
            ipt[i].virtualPage == virPage &&
            iptBitmap->Test(i)) {
            return i;
        }
    }
    return -1;
}
// Find a free entry in ipt
int InvertedPageTable::Find()
{
    int pno = iptBitmap->Find();
    if (pno != -1) {        
        int *p = new int[1];
        p[0] = pno;
        queue->Append((void *)p);
    }    
    return pno;
}
// Evict a page out
int InvertedPageTable::AllocatePage(Swap *swap)
{
    int pno = -1;
    int *p;
#ifdef USE_TLB
    if (swapFlag == 2) {
        while (true) {
            pno = rand()%pages;
            if (!ipt[pno].valid) {
                if (ipt[pno].use) {
                    continue;
                }
                if (ipt[pno].dirty) {
                    // Save to SwapFile
                    int swapEntry = swap->Find();
                    if (swapEntry == -1) 
                        return -1;
                    swap->SetVirtualPage(swapEntry, ipt[pno].virtualPage);
                    swap->SetPhysicalPage(swapEntry, -1);
                    swap->SetValidBit(swapEntry, false);
                    swap->SetDirtyBit(swapEntry, ipt[pno].dirty);
                    swap->SetUseBit(swapEntry, false);
                    swap->SetROBit(swapEntry, ipt[pno].readOnly);
                    swap->SetPid(swapEntry, ipt[pno].processId);
                    OpenFile *to = swap->GetSwapBuffer();
                    to->WriteAt(&(machine->mainMemory[pno*PageSize]), this->pageSize, swapEntry*this->pageSize);
                    // cout<<"Save to "<<swap->GetVirtualPage(swapEntry)<<endl;
                    Process *proc = (Process*)processTable->Get(ipt[pno].processId);
                    proc->space->locTable[ipt[pno].virtualPage] = 1;
                }
                return pno;
            }
        }

    }
    else if (swapFlag == 1) {
        while (true) {
            p = (int *)queue->Remove();
            pno = p[0];
            if (ipt[pno].valid) {
                p = new int[1];
                p[0] = pno;
                queue->Append(p);
                continue;
            }
            else {
                if (ipt[pno].use) {
                    p = new int[1];
                    p[0] = pno;
                    queue->Append((void*)p);
                    continue;
                }
                if (ipt[pno].dirty) {
                    ipt[pno].use = true;
                    int swapEntry = swap->Find();
                    ASSERT(swapEntry != -1);
                    swap->SetVirtualPage(swapEntry, ipt[pno].virtualPage);
                    swap->SetPhysicalPage(swapEntry, -1);
                    swap->SetValidBit(swapEntry, false);
                    swap->SetDirtyBit(swapEntry, ipt[pno].dirty);
                    swap->SetUseBit(swapEntry, false);
                    swap->SetROBit(swapEntry, ipt[pno].readOnly);
                    swap->SetPid(swapEntry, ipt[pno].processId);
                    OpenFile *to = swap->GetSwapBuffer();
                    to->WriteAt(&(machine->mainMemory[pno*PageSize]), this->pageSize, swapEntry*this->pageSize);
                    // cout<<"Save to "<<swap->GetVirtualPage(swapEntry)<<endl;
                    Process *proc = (Process*)processTable->Get(ipt[pno].processId);
                    proc->space->locTable[ipt[pno].virtualPage] = 1;
                    ipt[pno].use = false;
                }
                return pno;
            }
        }

    }
    else {        
        return pno;
    }
#endif
    return pno;
}
// modify the ipt while executing contextswitch
void InvertedPageTable::ContextSwitch()
{
    for (int i=0; i<TLBSize; i++) {
        machine->tlb[i].valid = false;
        ipt[machine->tlb[i].physicalPage].valid = machine->tlb[i].valid;
        ipt[machine->tlb[i].physicalPage].dirty = machine->tlb[i].dirty;
    }
}
// update the ipt while encountering page fault
void InvertedPageTable::UpdateIPT()
{
    for (int i=0; i<TLBSize; i++) {
        if (machine->tlb[i].valid) {
            ipt[machine->tlb[i].physicalPage].valid = machine->tlb[i].valid;
            if (machine->tlb[i].dirty) {
                ipt[machine->tlb[i].physicalPage].dirty = machine->tlb[i].dirty;
            }
        }
    }
}
// Count on the free page in main memory
int InvertedPageTable::FreeCount()
{
    int count = 0;
    for (int i=0; i<pages; i++) {
        if (ipt[i].use) {
            continue;
        }
        count++;
    }
    return count;
}
// Delete all the info in ipt
void InvertedPageTable::RemoveInfo(int processId)
{
    for (int i=0; i<pages; i++) {
        if (ipt[i].processId == processId) {
            iptBitmap->Clear(i);
            ipt[i].use = false;
            ipt[i].dirty = false;
        }
    }
}






















