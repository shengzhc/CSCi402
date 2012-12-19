// system.h 
//	All global variables used in Nachos are defined here.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#ifndef SYSTEM_H
#define SYSTEM_H

#include "copyright.h"
#include "utility.h"
#include "thread.h"
#include "scheduler.h"
#include "interrupt.h"
#include "stats.h"
#include "timer.h"
#include "synch.h"

// Initialization and cleanup routines
extern void Initialize(int argc, char **argv); 	// Initialization,
						// called before anything else
extern void Cleanup();				// Cleanup, called when
						// Nachos is done.

extern Thread *currentThread;			// the thread holding the CPU
extern Thread *threadToBeDestroyed;  		// the thread that just finished
extern Scheduler *scheduler;			// the ready list
extern Interrupt *interrupt;			// interrupt status
extern Statistics *stats;			// performance metrics
extern Timer *timer;				// the hardware alarm clock

#ifdef USER_PROGRAM
#include "machine.h"

#define MAX_OSLOCK 1024
#define MAX_OSCONDITION 1024
#define MAX_THREADS 1024
#define MAX_PROCESSES 8

struct KernelLock {
    Lock *lock;
    AddrSpace *addrSpace;
    bool isDeleted;
    bool isToBeDeleted;
};

struct KernelCondition {
    Condition *condition;
    AddrSpace *addrSpace;
    bool isDeleted;
    bool isToBeDeleted;
};

struct ProcessBlock {
    
    int childProcessId;
    AddrSpace *childAddrSpace;
};

struct Process {
    int childProcessCount;
    AddrSpace *parentAddrSpace;
    int processId;
    List *childProcessList;
    Lock *processLock;
};



extern Machine* machine;	// user program memory and registers

extern KernelLock osLockTable[MAX_OSLOCK];
extern Lock *osLockTableLock;

extern KernelCondition osConditionTable[MAX_OSCONDITION];
extern Lock *osConditionTableLock;

extern Table *processTable;
extern Lock *processTableLock;

extern Table *threadTable;
extern Lock *threadTableLock;
extern int FindPages(int pages);
extern int ClearPages(int start, int pages);
extern BitMap *freeMemMap;
#endif

#ifdef FILESYS_NEEDED 		// FILESYS or FILESYS_STUB 
#include "filesys.h"
extern FileSystem  *fileSystem;
#endif

#ifdef FILESYS
#include "synchdisk.h"
extern SynchDisk   *synchDisk;
#endif

#ifdef NETWORK
#include "post.h"
extern PostOffice* postOffice;
#endif

#endif // SYSTEM_H
