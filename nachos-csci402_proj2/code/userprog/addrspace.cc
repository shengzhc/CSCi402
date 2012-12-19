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
    fileTable.Put(0);
    fileTable.Put(0);

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

    ASSERT(numPages <= NumPhysPages);		// check we're not trying
						// to run anything too big --
						// at least until we have
                        // virtual memory
    // find a continuous numPages free pages in memory
    int memStartPage = FindPages(numPages);
    if (memStartPage == -1) {
        DEBUG('a', "No Enough Main Memory in Nachos.\n");
        return ;
    }
    DEBUG('a', "Initializing address space, num pages %d, size %d\n", 
					numPages, size);
// first, set up the translation 
    pageTable = new TranslationEntry[numPages];
    
    for (i = 0; i < numPages; i++) {
    
        int nextPage = memStartPage+i;
    
        pageTable[i].virtualPage = i;	// for now, virtual page # = phys page #
        pageTable[i].physicalPage = nextPage;
        pageTable[i].valid = TRUE;
        pageTable[i].use = FALSE;
        pageTable[i].dirty = FALSE;
        pageTable[i].readOnly = FALSE;  // if the code segment was entirely on 
					// a separate page, we could set its 
					// pages to be read-only
        for (int j=0; j<PageSize; j++) {
            
            machine->mainMemory[j+nextPage*PageSize] = 0x00;
        }
    }
   // printf("Allocate addrspace from page_%d to page_%d\n", memStartPage, memStartPage+numPages-1);
    
// zero out the entire address space, to zero the unitialized data segment 
// and the stack segment
    //bzero(machine->mainMemory, size);

// then, copy in the code and data segments into memory
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
    }

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
    for (unsigned int i=0; i<numPages; i++) {
        if (!pageTable[i].dirty) {
            ClearPages(pageTable[i].physicalPage, 1);
        }
    }
    delete pageTable;
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
    int memStartPage = FindPages(addpages);
    if (memStartPage == -1) {
        DEBUG('a', "No Enough Main Memory in Nachos.\n");
        return -1;
    }
    TranslationEntry *newpageTable = new TranslationEntry[numPages+addpages];
    for (unsigned int i=0; i<numPages; i++) {
        newpageTable[i].virtualPage = pageTable[i].virtualPage;
        newpageTable[i].physicalPage = pageTable[i].physicalPage;
        newpageTable[i].valid = pageTable[i].valid;
        newpageTable[i].use = pageTable[i].use;
        newpageTable[i].dirty = pageTable[i].dirty;
        newpageTable[i].readOnly = pageTable[i].readOnly;
    }
    int virPage = newpageTable[numPages].virtualPage+1;
    int nextPage;
    // Append the new pages to pageTable
    for (int i=0; i<addpages; i++) {
            
        nextPage = memStartPage+i;
        newpageTable[i+numPages].virtualPage = virPage+i;
        newpageTable[i+numPages].physicalPage = nextPage;
        newpageTable[i+numPages].valid = TRUE;
        newpageTable[i+numPages].use = FALSE;
        newpageTable[i+numPages].dirty = FALSE;
        newpageTable[i+numPages].readOnly = FALSE;
        for (int j=0; j<PageSize; j++) {
            machine->mainMemory[nextPage*PageSize+j]=0x00;
        }
    }
    
    numPages += addpages;
    delete pageTable;
    pageTable = newpageTable;
    
    //printf("Allocate stack from page_%d to page_%d end with %d.\n", memStartPage, memStartPage+addpages-1, pageTable[numPages-1].physicalPage * PageSize + PageSize);
    return numPages*PageSize-16;
}
// When user thread exits, deallocate the stack in main memory
int AddrSpace::DeallocateStack() {
    
    int stackPtr = currentThread->userstackTop + 16;
    unsigned int stackStartPage = divRoundUp((stackPtr - UserStackSize), PageSize);
    int pages = divRoundUp(UserStackSize, PageSize);
    if (stackStartPage > numPages - pages) {
        printf("Error DeallocateStack %d, %d.\n", stackStartPage, numPages);
        return -1;
    }
    unsigned int i = stackStartPage;
    unsigned int memStartPage = pageTable[i].physicalPage;
    for (int j=0; j<pages; j++) {
        pageTable[j+stackStartPage].dirty = true;
    }/*
    TranslationEntry *newpageTable = new TranslationEntry[numPages-pages];
    for (unsigned int j=0; j<i; j++) {
        newpageTable[j].virtualPage = pageTable[j].virtualPage;
        newpageTable[j].physicalPage = pageTable[j].physicalPage;
        newpageTable[j].valid = pageTable[j].valid;
        newpageTable[j].use = pageTable[j].use;
        newpageTable[j].dirty = pageTable[j].dirty;
        newpageTable[j].readOnly = pageTable[j].readOnly;
    }
    for (unsigned int j=0; j<numPages-i-pages; j++) {
        newpageTable[i+j].virtualPage = pageTable[i+j+pages].virtualPage;
        newpageTable[i+j].physicalPage = pageTable[i+j+pages].physicalPage;
        newpageTable[i+j].valid = pageTable[i+j+pages].valid;
        newpageTable[i+j].use = pageTable[i+j+pages].use;
        newpageTable[i+j].dirty = pageTable[i+j+pages].dirty;
        newpageTable[i+j].readOnly = pageTable[i+j+pages].readOnly;
    }
    delete pageTable;
    pageTable = newpageTable;
    
    numPages -= pages;
    */
   // printf("Deallocate stack from page_%d to page_%d, end with %d.\n", memStartPage,memStartPage+pages-1,(memStartPage+pages)*PageSize);
    RestoreState();
    return ClearPages(memStartPage, pages);
    
}






