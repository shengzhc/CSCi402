// addrspace.cc 
//	Routines to manage address spaces (executing user programs).
//
//	In order to run a user program, you must:
//
//	1. link with the -N -T 0 option 
//	2. run coff2noff to convert the object file to Nachos format
//		(Nachos object code format is essentially just a simpler
//		version of the UNIX executable object code format)
//	3. load the NOFF file into the Nachos file system
//		(if you haven't implemented the file system yet, you
//		don't need to do this last step)
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "addrspace.h"
#include "noff.h"
#include "table.h"
#include "synch.h"

extern "C" { int bzero(char *, int); };

Table::Table(int s) : map(s), table(0), lock(0), size(s) {
    table = new void *[size];
    lock = new Lock("TableLock");
}

Table::~Table() {
    if (table) {
	delete table;
	table = 0;
    }
    if (lock) {
	delete lock;
	lock = 0;
    }
}

void *Table::Get(int i) {
    // Return the element associated with the given if, or 0 if
    // there is none.

    return (i >=0 && i < size && map.Test(i)) ? table[i] : 0;
}

int Table::Put(void *f) {
    // Put the element in the table and return the slot it used.  Use a
    // lock so 2 files don't get the same space.
    int i;	// to find the next slot

    lock->Acquire();
    i = map.Find();
    lock->Release();
    if ( i != -1)
	table[i] = f;
    return i;
}

void *Table::Remove(int i) {
    // Remove the element associated with identifier i from the table,
    // and return it.

    void *f =0;

    if ( i >= 0 && i < size ) {
	lock->Acquire();
	if ( map.Test(i) ) {
	    map.Clear(i);
	    f = table[i];
	    table[i] = 0;
	}
	lock->Release();
    }
    return f;
}

//----------------------------------------------------------------------
// SwapHeader
// 	Do little endian to big endian conversion on the bytes in the 
//	object file header, in case the file was generated on a little
//	endian machine, and we're now running on a big endian machine.
//----------------------------------------------------------------------

static void 
SwapHeader (NoffHeader *noffH)
{
	noffH->noffMagic = WordToHost(noffH->noffMagic);
	noffH->code.size = WordToHost(noffH->code.size);
	noffH->code.virtualAddr = WordToHost(noffH->code.virtualAddr);
	noffH->code.inFileAddr = WordToHost(noffH->code.inFileAddr);
	noffH->initData.size = WordToHost(noffH->initData.size);
	noffH->initData.virtualAddr = WordToHost(noffH->initData.virtualAddr);
	noffH->initData.inFileAddr = WordToHost(noffH->initData.inFileAddr);
	noffH->uninitData.size = WordToHost(noffH->uninitData.size);
	noffH->uninitData.virtualAddr = WordToHost(noffH->uninitData.virtualAddr);
	noffH->uninitData.inFileAddr = WordToHost(noffH->uninitData.inFileAddr);
}

//----------------------------------------------------------------------
// AddrSpace::AddrSpace
// 	Create an address space to run a user program.
//	Load the program from a file "executable", and set everything
//	up so that we can start executing user instructions.
//
//	Assumes that the object code file is in NOFF format.
//
//	"executable" is the file containing the object code to load into memory
//
//      It's possible to fail to fully construct the address space for
//      several reasons, including being unable to allocate memory,
//      and being unable to read key parts of the executable.
//      Incompletely consretucted address spaces have the member
//      constructed set to false.
//----------------------------------------------------------------------

AddrSpace::AddrSpace(OpenFile *executable) : fileTable(MaxOpenFiles) {
    NoffHeader noffH;
    unsigned int i, size;

    // Don't allocate the input or output to disk files
    //fileTable.Put(0);
    //fileTable.Put(0);

    executable->ReadAt((char *)&noffH, sizeof(noffH), 0);
    if ((noffH.noffMagic != NOFFMAGIC) && 
		(WordToHost(noffH.noffMagic) == NOFFMAGIC))
    	SwapHeader(&noffH);
    ASSERT(noffH.noffMagic == NOFFMAGIC);

    size = noffH.code.size + noffH.initData.size + noffH.uninitData.size ;
    numPages = divRoundUp(size, PageSize) + divRoundUp(UserStackSize,PageSize);
                                                // we need to increase the size
						// to leave room for the stack
    size = numPages * PageSize;
    
    DEBUG('a', "Initializing address space, num pages %d, size %d\n", 
					numPages, size);
// first, set up the translation 
    //printf("Initializing address space, num pages %d, size %d\n", numPages, size);
    pageTable = new TranslationEntry[numPages];
#ifdef USE_TLB
    locTable = new int[numPages];
    for (i = 0; i < numPages; i++) {
        
        pageTable[i].virtualPage = i;	// for now, virtual page # = phys page #
        pageTable[i].physicalPage = -1;
        pageTable[i].valid = FALSE;
        pageTable[i].use = FALSE;
        pageTable[i].dirty = FALSE;
        pageTable[i].readOnly = FALSE;  // if the code segment was entirely on 
					// a separate page, we could set its 
					// pages to be read-only
        locTable[i] = 0;
    }
    codeInFileAddr = noffH.code.inFileAddr;
    dataInFileAddr = noffH.initData.inFileAddr;
    initialPages = numPages;
#endif
    // printf("Allocate addrspace from page_%d to page_%d\n", memStartPage, memStartPage+numPages-1);
    
    /*
    if (noffH.code.size > 0) {
        DEBUG('a', "Initializing code segment, at 0x%x, size %d\n", 
			noffH.code.virtualAddr, noffH.code.size);
        executable->ReadAt(&(machine->mainMemory[noffH.code.virtualAddr+memStartPage*PageSize]),
			noffH.code.size, noffH.code.inFileAddr);
    }
    if (noffH.initData.size > 0) {
        DEBUG('a', "Initializing data segment, at 0x%x, size %d\n", 
			noffH.initData.virtualAddr, noffH.initData.size);
        executable->ReadAt(&(machine->mainMemory[noffH.initData.virtualAddr+memStartPage*PageSize]),
			noffH.initData.size, noffH.initData.inFileAddr);
    }*/
}

//----------------------------------------------------------------------
// AddrSpace::~AddrSpace
//
// 	Dealloate an address space.  release pages, page tables, files
// 	and file tables
//----------------------------------------------------------------------

AddrSpace::~AddrSpace()
{
    // Deallocate the pages from main memory
    //printf("Deallocate pages from page_%d to page_%d\n", pageTable[0].physicalPage, pageTable[numPages-1].physicalPage);
    delete pageTable;
#ifdef USE_TLB
    delete locTable;
#endif
}

//----------------------------------------------------------------------
// AddrSpace::InitRegisters
// 	Set the initial values for the user-level register set.
//
// 	We write these directly into the "machine" registers, so
//	that we can immediately jump to user code.  Note that these
//	will be saved/restored into the currentThread->userRegisters
//	when this thread is context switched out.
//----------------------------------------------------------------------

void
AddrSpace::InitRegisters()
{
    int i;

    for (i = 0; i < NumTotalRegs; i++)
	machine->WriteRegister(i, 0);

    // Initial program counter -- must be location of "Start"

    machine->WriteRegister(PCReg, 0);	

    // Need to also tell MIPS where next instruction is, because
    // of branch delay possibility
    
    machine->WriteRegister(NextPCReg, 4);

   // Set the stack register to the end of the address space, where we
   // allocated the stack; but subtract off a bit, to make sure we don't
   // accidentally reference off the end!

    machine->WriteRegister(StackReg, numPages * PageSize - 16);
    DEBUG('a', "Initializing stack register to %x\n", numPages * PageSize - 16);
}

//----------------------------------------------------------------------
// AddrSpace::SaveState
// 	On a context switch, save any machine state, specific
//	to this address space, that needs saving.
//
//	For now, nothing!
//----------------------------------------------------------------------

void AddrSpace::SaveState() 
{}

//----------------------------------------------------------------------
// AddrSpace::RestoreState
// 	On a context switch, restore the machine state so that
//	this address space can run.
//
//      For now, tell the machine where to find the page table.
//----------------------------------------------------------------------

void AddrSpace::RestoreState() 
{
    machine->pageTable = pageTable;
    machine->pageTableSize = numPages;
}


// Allocate a new stack for a Fork thread
int AddrSpace::AllocateNewStack() {
    
    int addpages = divRoundUp(UserStackSize, PageSize);
    TranslationEntry *newpageTable = new TranslationEntry[numPages+addpages];
    int *newLocTable = new int[numPages+addpages];
    for (unsigned int i=0; i<numPages; i++) {
        newpageTable[i].virtualPage = pageTable[i].virtualPage;
        newpageTable[i].physicalPage = pageTable[i].physicalPage;
        newpageTable[i].valid = pageTable[i].valid;
        newpageTable[i].use = pageTable[i].use;
        newpageTable[i].dirty = pageTable[i].dirty;
        newpageTable[i].readOnly = pageTable[i].readOnly;
#ifdef USE_TLB
        newLocTable[i] = locTable[i];
#endif
    }
    int virPage = newpageTable[numPages-1].virtualPage+1;
    int nextPage;
    // Append the new pages to pageTable
    for (int i=0; i<addpages; i++) {
        
        newpageTable[i+numPages].virtualPage = virPage+i;
        newpageTable[i+numPages].physicalPage = -1;
        newpageTable[i+numPages].valid = false;
        newpageTable[i+numPages].use = false;
        newpageTable[i+numPages].dirty = false;
        newpageTable[i+numPages].readOnly = false;
#ifdef USE_TLB
        newLocTable[i+numPages] = 0;
#endif
    }
    
    numPages += addpages;
    delete pageTable;
    pageTable = newpageTable;
    
#ifdef USE_TLB
    delete locTable;
    locTable = newLocTable;
#endif
    //printf("Allocate stack from page_%d to page_%d end with %d.\n", virPage, virPage+addpages-1, (virPage+addpages-1)*PageSize);
    
    return numPages*PageSize-16;
}
// When user thread exits, deallocate the stack in main memory
int AddrSpace::DeallocateStack() {
    
    int stackPtr = currentThread->userstackTop + 16;
    unsigned int stackStartPage = divRoundUp((stackPtr - UserStackSize), PageSize);
    int pages = divRoundUp(UserStackSize, PageSize);
    //printf("%d %d A\n", stackStartPage, numPages-pages);
    ASSERT(stackStartPage<=(numPages-pages));
    for (unsigned int i=stackStartPage; i<stackStartPage+pages; i++) {
        for (int j=0; j<TLBSize; j++) {
            if (machine->tlb[j].valid &&
                machine->tlb[j].virtualPage == (int)i) {
                machine->tlb[j].valid = false;
                if (machine->tlb[j].use) {
                    ipt->SetUseBit(machine->tlb[j].physicalPage, false);
                }
                if (ipt->GetDirtyBit(machine->tlb[j].physicalPage)) {
                    int entry = swapBuffer->SearchEntry(i, processId);
                    if (i>=0) 
                        swapBuffer->Clear(entry);
                    ipt->SetDirtyBit(machine->tlb[j].physicalPage, false);
                }
            }
            
        }
    }
    // printf("Deallocate stack from page_%d to page_%d, end with %d.\n", memStartPage,memStartPage+pages-1,(memStartPage+pages)*PageSize);
    return 1;
}

TranslationEntry *AddrSpace::SearchEntry(int vaddr) {
    
    unsigned int page = divRoundDown(vaddr, PageSize);
    ASSERT(page<=numPages);
    TranslationEntry *entry = new TranslationEntry();
    entry->virtualPage = pageTable[page].virtualPage;
    entry->physicalPage = pageTable[page].physicalPage;
    entry->valid = pageTable[page].valid;
    entry->use = pageTable[page].use;
    entry->dirty = pageTable[page].dirty;
    entry->readOnly = pageTable[page].readOnly;
    return entry;
}







