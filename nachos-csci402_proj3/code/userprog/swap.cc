#include <iostream>
#include "swap.h"

Swap::Swap(char *fn, int sfp, int ps, FileSystem *fs) 
{
    this->filename = new char[strlen(fn)+1];
    strcpy(this->filename, fn);
    this->filename[strlen(fn)] = '\0';
    
    this->swapFilePages = sfp;
    this->pageSize = ps;
    this->filesys = fs;
    
    this->swapBufferBitMap = new BitMap(this->swapFilePages);
    this->filesys->Create(this->filename, this->pageSize*this->swapFilePages);
    this->swapBuffer = this->filesys->Open(this->filename);
    
    this->stlb = new SwapTranslationEntry[this->swapFilePages];
    
}

Swap::~Swap()
{
    if (swapBuffer != NULL) 
        delete swapBuffer;
    
    if (swapBufferBitMap != NULL)
        delete swapBufferBitMap;
    
    if (filesys != NULL && filename != NULL)  {
        if (filesys->Remove(filename))
            delete filename;
        filesys = NULL;
    }
    
    if (stlb != NULL) 
        delete stlb;
}

int Swap::GetVirtualPage(int entry)
{
    return stlb[entry].virtualPage;
}

void Swap::SetVirtualPage(int entry, int value)
{
    stlb[entry].virtualPage = value;
}

int Swap::GetPhysicalPage(int entry)
{
    return stlb[entry].physicalPage;
}

void Swap::SetPhysicalPage(int entry, int value)
{
    stlb[entry].physicalPage = value;
}

bool Swap::GetValidBit(int entry)
{
    return stlb[entry].valid;
}

void Swap::SetValidBit(int entry, bool value)
{
    stlb[entry].valid = value;
}

bool Swap::GetDirtyBit(int entry)
{
    return stlb[entry].dirty;
}

void Swap::SetDirtyBit(int entry, bool value)
{
    stlb[entry].dirty = value;
}

bool Swap::GetUseBit(int entry)
{
    return stlb[entry].use;
}

void Swap::SetUseBit(int entry, bool value)
{
    stlb[entry].use = value;
}

bool Swap::GetROBit(int entry)
{
    return stlb[entry].readOnly;
}

void Swap::SetROBit(int entry, bool value)
{
    stlb[entry].readOnly = value;
}

int Swap::GetPid(int entry)
{
    return stlb[entry].processId;
}

void Swap::SetPid(int entry, int value)
{
    stlb[entry].processId = value;
}
// Return the swap open file ptr
OpenFile* Swap::GetSwapBuffer()
{
    return swapBuffer;
}

int Swap::Find()
{
    return swapBufferBitMap->Find();
}
// Find a page is in swap file or not
int Swap::SearchEntry(int vpn, int processId)
{
   
    for (int i=0; i<swapFilePages; i++) {
        if ((swapBufferBitMap->Test(i)) 
            && (stlb[i].virtualPage == vpn) 
            && (stlb[i].processId == processId)) {
            return i;
        }
    }
    return -1;
}
// Clear a page in swap file
void Swap::Clear(int entry)
{
    if (entry>=0 && entry<swapFilePages) {
        swapBufferBitMap->Clear(entry);
    }
}
// Remove all the info from the swap file
void Swap::RemoveInfo(int processId)
{
    for (int i=0; i<swapFilePages; i++) {
        if (stlb[i].processId == processId) {
            Clear(i);
        }
    }
}




