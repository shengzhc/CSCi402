// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"
#include <stdio.h>
#include <iostream>
#ifdef NETWORK
#include "protocol.h"
#include <sys/time.h>
#endif
using namespace std;
extern "C" {
	int bcopy(char *, char *, int);
};

int copyin(unsigned int vaddr, int len, char *buf) {
    // Copy len bytes from the current thread's virtual address vaddr.
    // Return the number of bytes so read, or -1 if an error occors.
    // Errors can generally mean a bad virtual address was passed in.
    bool result;
    int n=0;			// The number of bytes copied in
    int *paddr = new int;

    while ( n >= 0 && n < len) {
      result = machine->ReadMem( vaddr, 1, paddr );
      while(!result) // FALL 09 CHANGES
	  {
   			result = machine->ReadMem( vaddr, 1, paddr ); // FALL 09 CHANGES: TO HANDLE PAGE FAULT IN THE ReadMem SYS CALL
	  }	
      
      buf[n++] = *paddr;
     
      if ( !result ) {
	//translation failed
	return -1;
      }

      vaddr++;
    }

    delete paddr;
    return len;
}

int copyout(unsigned int vaddr, int len, char *buf) {
    // Copy len bytes to the current thread's virtual address vaddr.
    // Return the number of bytes so written, or -1 if an error
    // occors.  Errors can generally mean a bad virtual address was
    // passed in.
    bool result;
    int n=0;			// The number of bytes copied in

    while ( n >= 0 && n < len) {
      // Note that we check every byte's address
      result = machine->WriteMem( vaddr, 1, (int)(buf[n++]) );

      if ( !result ) {
	//translation failed
	return -1;
      }

      vaddr++;
    }

    return n;
}

void info(Thread *t) {
    
    printf("Thread id: %d\n", t->threadinfo->tid);
    printf("Process id: %d\n", t->threadinfo->pid);
    printf("childThreadsCount: %d\n", t->threadinfo->childThreadsCount);
    printf("parentThread id: %d\n", t->threadinfo->parentThread->threadinfo->tid);
    printf("parentThread Process id: %d\n", t->threadinfo->parentThread->threadinfo->pid);
    printf("Is this thread main_thread of this process?%c\n", t == t->threadinfo->parentThread?'Y':'N'); 
    
    
    if (t == t->threadinfo->parentThread) {
        
        int pid = t->threadinfo->pid;
        Process *proc = (Process *)processTable -> Get(pid);
        printf("Process_%d is the Child of Process_%d\n", pid, proc->parentAddrSpace->processId);
        printf("Process_%d has %d Child Process\n", pid, proc->childProcessCount);
    }
    
}

void Create_Syscall(unsigned int vaddr, int len) {
    // Create the file with the name in the user buffer pointed to by
    // vaddr.  The file name is at most MAXFILENAME chars long.  No
    // way to return errors, though...
    char *buf = new char[len+1];	// Kernel buffer to put the name in

    if (!buf) return;

    if( copyin(vaddr,len,buf) == -1 ) {
	printf("%s","Bad pointer passed to Create\n");
	delete buf;
	return;
    }

    buf[len]='\0';

    fileSystem->Create((char*)buf,0);
    delete[] buf;
    return;
}

int Open_Syscall(unsigned int vaddr, int len) {
    // Open the file with the name in the user buffer pointed to by
    // vaddr.  The file name is at most MAXFILENAME chars long.  If
    // the file is opened successfully, it is put in the address
    // space's file table and an id returned that can find the file
    // later.  If there are any errors, -1 is returned.
    char *buf = new char[len+1];	// Kernel buffer to put the name in
    OpenFile *f;			// The new open file
    int id;				// The openfile id

    if (!buf) {
	printf("%s","Can't allocate kernel buffer in Open\n");
	return -1;
    }

    if( copyin(vaddr,len,buf) == -1 ) {
	printf("%s","Bad pointer passed to Open\n");
	delete[] buf;
	return -1;
    }

    buf[len]='\0';

    f = fileSystem->Open((char*)buf);
    delete[] buf;

    if ( f ) {
	if ((id = currentThread->space->fileTable.Put(f)) == -1 )
	    delete f;
	return id;
    }
    else
	return -1;
}

void Write_Syscall(unsigned int vaddr, int len, int id) {
    // Write the buffer to the given disk file.  If ConsoleOutput is
    // the fileID, data goes to the synchronized console instead.  If
    // a Write arrives for the synchronized Console, and no such
    // console exists, create one. For disk files, the file is looked
    // up in the current address space's open file table and used as
    // the target of the write.
    
    char *buf;		// Kernel buffer for output
    OpenFile *f;	// Open file for output

    if ( id == ConsoleInput) return;
    
    if ( !(buf = new char[len]) ) {
	printf("%s","Error allocating kernel buffer for write!\n");
	return;
    } else {
        if ( copyin(vaddr,len,buf) == -1 ) {
	    printf("%s","Bad pointer passed to to write: data not written\n");
	    delete[] buf;
	    return;
	}
    }

    if ( id == ConsoleOutput) {
      for (int ii=0; ii<len; ii++) {
	printf("%c",buf[ii]);
      }

    } else {
	if ( (f = (OpenFile *) currentThread->space->fileTable.Get(id)) ) {
	    f->Write((char*)buf, len);
	} else {
	    printf("%s","Bad OpenFileId passed to Write\n");
	    len = -1;
	}
    }

    delete[] buf;
}

int Read_Syscall(unsigned int vaddr, int len, int id) {
    // Write the buffer to the given disk file.  If ConsoleOutput is
    // the fileID, data goes to the synchronized console instead.  If
    // a Write arrives for the synchronized Console, and no such
    // console exists, create one.    We reuse len as the number of bytes
    // read, which is an unnessecary savings of space.
    char *buf;		// Kernel buffer for input
    OpenFile *f;	// Open file for output

    if ( id == ConsoleOutput) return -1;
    
    if ( !(buf = new char[len]) ) {
	printf("%s","Error allocating kernel buffer in Read\n");
	return -1;
    }

    if ( id == ConsoleInput) {
      //Reading from the keyboard
      scanf("%s", buf);

      if ( copyout(vaddr, len, buf) == -1 ) {
	printf("%s","Bad pointer passed to Read: data not copied\n");
      }
    } else {
	if ( (f = (OpenFile *) currentThread->space->fileTable.Get(id)) ) {
	    len = f->Read((char*)buf, len);
	    if ( len > 0 ) {
	        //Read something from the file. Put into user's address space
  	        if ( copyout(vaddr, len, buf) == -1 ) {
		    printf("%s","Bad pointer passed to Read: data not copied\n");
		}
	    }
	} else {
	    printf("%s","Bad OpenFileId passed to Read\n");
	    len = -1;
	}
    }

    delete[] buf;
    return len;
}

void Close_Syscall(int fd) {
    // Close the file associated with id fd.  No error reporting.
    OpenFile *f = (OpenFile *) currentThread->space->fileTable.Remove(fd);

    if ( f ) {
      delete f;
    } else {
      printf("%s","Tried to close an unopen file\n");
    }
}
// Print msg system call, vaddr and length are the input string, args and value are the number of variables trying to display on the string

int Random_Syscall(int seed) {
    
    int rec;
    rec = rand()%seed;
    return rec;
}

void Print_Syscall(unsigned int vaddr, int length, int args, int value){
    char *buf = new char[length+1];
    if (!buf) 
        return;
    if (copyin(vaddr, length, buf) == -1) {
        printf("copy error.\n");
    }
    buf[length] = '\0';
    int last=0, argIndex=args-1;
    int val[args];
    int v = value;
    for (int i=0; i<args; i++) {
        val[i] = v & 0x000000FF;
        v >>= 8;
    }
    while (last<=length) {
        if (buf[last] == '%') {
            last+=2;
            printf("%d", val[argIndex--]);
        } else {
            printf("%c", buf[last]);
            last++;
        }
    }
    delete buf;
}
// vaddr and length are the string for the lock name, return value is the index of the lock in the lock table
int CreateLock_Syscall(unsigned int vaddr, int length) {
    
    int lockid = -1;
    char *buf = new char[length+1];
    if (copyin(vaddr, length, buf) == -1) {
        cout<<"copy error."<<endl;
        return -1;
    }
    buf[length] = '\0';

#ifdef NETWORK
    int sid = rand()%cap+1;
    char *sendargs = new char[MaxMailSize];
    sendargs[0] = NETWORK_SERVER_CREATELOCK_REQUEST;
    sendargs[1] = length;
    int i;
    for (i=0; i<length; i++) {
        sendargs[2+i] = buf[i];
    }
    client->Send(sendargs, sid, currentThread->space->processId, 2+length);

    delete sendargs;
    
    char *recargs;
    client->Receive(&recargs, currentThread->space->processId);

    ASSERT(recargs != NULL);
    if ((int)recargs[0] != NETWORK_SERVER_CREATELOCK_REPLY) {
        return -1;
    }
    bcopy(&recargs[1], (char*)&lockid, sizeof(int));
    //cout<<"CreateLock: ["<<lockid<<"] with Server["<<sid<<"]"<<endl;
    delete recargs;
    return lockid;
#endif
    return lockid;
    
}
// Destroy lock with lockid in lock table, set the corresponding lock with ToBeDeleted
int DestroyLock_Syscall(int lockid){
    
#ifdef NETWORK
    int sid = rand()%cap+1;
    char *sendargs = new char[MaxMailSize];
    sendargs[0] = NETWORK_SERVER_DESTROYLOCK_REQUEST;
    bcopy((char*)(&lockid), &sendargs[1], sizeof(int));
    client->Send(sendargs, sid, currentThread->space->processId, 1+sizeof(int));
    delete sendargs;
    
    char *recargs;
    client->Receive(&recargs, currentThread->space->processId);
    ASSERT(recargs != NULL);
    if ((int)recargs[0] != NETWORK_SERVER_DESTROYLOCK_REPLY) {
        return -1;
    }
    delete recargs;
    //cout<<"DestroyLock :["<<lockid<<"] with Server["<<sid<<"]"<<endl;
    return 1;
#endif
    return 1;
}
// Acquire a lock for M.E. with lockid
int AcquireLock_Syscall(int lockid){
    
#ifdef NETWORK
    int sid = rand()%cap+1;
    char *sendargs = new char[MaxMailSize];
    sendargs[0] = NETWORK_SERVER_ACQUIRELOCK_REQUEST;
    bcopy((char*)&lockid, &sendargs[1], sizeof(int));
    client->Send(sendargs, sid, currentThread->space->processId, 1+sizeof(int));
    delete sendargs;
    
    char *recargs;
    client->Receive(&recargs, currentThread->space->processId);
    ASSERT(recargs != NULL);
    if ((int)recargs[0] != NETWORK_SERVER_ACQUIRELOCK_REPLY) {
        return -1;
    }
    delete recargs;
    //cout<<"AcquireLock: ["<<lockid<<"] with Server["<<sid<<"]"<<endl;
    return 1;
#endif
    return 1;
}
// Release the lock
int ReleaseLock_Syscall(int lockid) {

#ifdef NETWORK
    int sid = rand()%cap+1;
    char *sendargs = new char[MaxMailSize];
    sendargs[0] = NETWORK_SERVER_RELEASELOCK_REQUEST;
    bcopy((char*)&lockid, &sendargs[1], sizeof(int));
    client->Send(sendargs, sid, currentThread->space->processId, 1+sizeof(int));
    delete sendargs;
    
    char *recargs;
    client->Receive(&recargs, currentThread->space->processId);
    ASSERT(recargs != NULL);
    if ((int)recargs[0] != NETWORK_SERVER_RELEASELOCK_REPLY) {
        return -1;
    }
    //cout<<"ReleaseLock ["<<lockid<<"] with Server["<<sid<<"]"<<endl;
    delete recargs;
    return 1;
#endif
    return 1;
}
// Create a new condition from kernel, returns the index of the condition in condition table
int CreateCondition_Syscall(unsigned int vaddr, int length){
    
    int cvid = -1;
    char *buf = new char[length+1];
    if (!buf)
        return -1;
    if (copyin(vaddr, length, buf) == -1) {
        printf("copy error.\n");
        return -1;
    }
    buf[length] = '\0';
#ifdef NETWORK
    int sid = rand()%cap+1;
    char *sendargs = new char[MaxMailSize];
    sendargs[0] = NETWORK_SERVER_CREATECONDITION_REQUEST;
    sendargs[1] = length;
    int i;
    for (i=0; i<length; i++) {
        sendargs[2+i] = buf[i];
    }
    client->Send(sendargs, sid, currentThread->space->processId, 2+length);
    delete sendargs;
    
    char *recargs;
    client->Receive(&recargs, currentThread->space->processId);
    ASSERT(recargs!=NULL);
    if ((int)recargs[0] != NETWORK_SERVER_CREATECONDITION_REPLY) {
        return -1;
    }
    bcopy(&recargs[1], (char*)&cvid, sizeof(int));
    delete recargs;
    //cout<<"CreateCondition: ["<<cvid<<"] with Server["<<sid<<"]"<<endl;
    return cvid;
#endif
    return cvid;
}
// Destroy the condition in condition table with cvid, set the ToBeDeleted
int DestroyCondition_Syscall(int cvid){

#ifdef NETWORK
    int sid = rand()%cap+1;
    char *sendargs = new char[MaxMailSize];
    sendargs[0] = NETWORK_SERVER_DESTROYCONDITION_REQUEST;
    bcopy((char*)&cvid, &sendargs[1], sizeof(int));
    client->Send(sendargs, sid, currentThread->space->processId, 1+sizeof(int));
    delete sendargs;
    
    char *recargs;
    client->Receive(&recargs, currentThread->space->processId);
    ASSERT(recargs != NULL);
    if ((int)recargs[0] != NETWORK_SERVER_DESTROYCONDITION_REPLY) {
        return -1;
    }
    delete recargs;
    //cout<<"DestroyCondition :["<<cvid<<"] with Server["<<sid<<"]"<<endl;
    return 1;
#endif
    return 1;
}
// Condition Waits for signal
int WaitCondition_Syscall(int cvid, int lockid) {
    
#ifdef NETWORK    
    int sid = rand()%cap+1;
    char *sendargs = new char[MaxMailSize];
    sendargs[0] = NETWORK_SERVER_WAITCONDITION_REQUEST;
    bcopy((char*)&cvid, &sendargs[1], sizeof(int));
    bcopy((char*)&lockid, &sendargs[1+sizeof(int)], sizeof(int));
    client->Send(sendargs, sid, currentThread->space->processId, 1+2*sizeof(int));
    delete sendargs;
    
    char *recargs;
    client->Receive(&recargs, currentThread->space->processId);
    ASSERT(recargs != NULL);
    if ((int)recargs[0] != NETWORK_SERVER_WAITCONDITION_REPLY) {
        return -1;
    }
    delete recargs;
    //cout<<"WaitCondition: "<<cvid<<", "<<lockid<<"] with Server["<<sid<<"]"<<endl;
    return 1;
#endif
    return 1;
}
// Condition signal
int SignalCondition_Syscall(int cvid, int lockid){

#ifdef NETWORK
    int sid = rand()%cap+1;
    char *sendargs = new char[MaxMailSize];
    sendargs[0] = NETWORK_SERVER_SIGNALCONDITION_REQUEST;
    bcopy((char*)&cvid, &sendargs[1], sizeof(int));
    bcopy((char*)&lockid, &sendargs[1+sizeof(int)], sizeof(int));
    client->Send(sendargs, sid, currentThread->space->processId, 1+2*sizeof(int));
    delete sendargs;
    
    char *recargs;
    client->Receive(&recargs, currentThread->space->processId);
    ASSERT(recargs != NULL);
    if ((int)recargs[0] != NETWORK_SERVER_SIGNALCONDITION_REPLY) {
        return -1;
    }
    delete recargs;
    //cout<<"SignalCondition: ["<<cvid<<", "<<lockid<<"] with Server["<<sid<<"]"<<endl;
    return 1;
#endif 
    return 1;
}
// Condition broadcast, invoke all the waiting threads of the condition
int BroadcastCondition_Syscall(int cvid, int lockid){

#ifdef NETWORK
    int sid = rand()%cap+1;
    char *sendargs = new char[MaxMailSize];
    sendargs[0] = NETWORK_SERVER_BROADCASTCONDITION_REQUEST;
    bcopy((char*)&cvid, &sendargs[1], sizeof(int));
    bcopy((char*)&lockid, &sendargs[1+sizeof(int)], sizeof(int));
    client->Send(sendargs, sid, currentThread->space->processId, 1+2*sizeof(int));
    delete sendargs;
    
    char *recargs;
    client->Receive(&recargs, currentThread->space->processId);
    ASSERT(recargs != NULL);
    if ((int)recargs[0] != NETWORK_SERVER_BROADCASTCONDITION_REPLY) {
        return -1;
    }
    delete recargs;
    //cout<<"BroadcastCondition: ["<<cvid<<", "<<lockid<<"] with Server["<<sid<<"]"<<endl;
    return 1;
#endif
    return 1;
}
int CreateMV_Syscall(unsigned int vaddr, int length, int size)
{
    int mvid;
    char *buf = new char[length+1];
    if (!buf)
        return -1;
    if (copyin(vaddr, length, buf) == -1) {
        printf("copy error.\n");
        return -1;
    }
    buf[length] = '\0';
#ifdef NETWORK 
    int sid = rand()%cap+1;
    char *sendargs = new char[MaxMailSize];
    sendargs[0] = NETWORK_SERVER_CREATEMV_REQUEST;
    sendargs[1] = length;
    int i;
    for (i=0; i<length; i++) {
        sendargs[2+i] = buf[i];
    }
    bcopy((char*)&size, &sendargs[2+length], sizeof(int));
    client->Send(sendargs, sid, currentThread->space->processId, 2+length+sizeof(int));
    delete sendargs;
    
    char *recargs;
    client->Receive(&recargs, currentThread->space->processId);
    ASSERT(recargs != NULL);
    if (recargs[0] != NETWORK_SERVER_CREATEMV_REPLY) {
        return -1;
    }
    bcopy(&recargs[1], (char*)&mvid, sizeof(int));

    delete recargs;
    //cout<<"CreateMV: ["<<mvid<<"] with Server["<<sid<<"]"<<endl;
    return mvid;
#endif
    return -1;
}

int SetMV_Syscall(int mvid, int index, int value)
{
#ifdef NETWORK
    int sid = rand()%cap+1;
    char *sendargs = new char[MaxMailSize];
    sendargs[0] = NETWORK_SERVER_SETMV_REQUEST;
    bcopy((char*)&mvid, &sendargs[1], sizeof(int));
    bcopy((char*)&index, &sendargs[1+sizeof(int)], sizeof(int));
    bcopy((char*)&value, &sendargs[1+2*sizeof(int)], sizeof(int));
    client->Send(sendargs, sid, currentThread->space->processId, 1+3*sizeof(int));
    delete sendargs;
    
    char *recargs;
    client->Receive(&recargs, currentThread->space->processId);
    ASSERT(recargs != NULL);
    if (recargs[0] != NETWORK_SERVER_SETMV_REPLY) {
        return -1;
    }
    //cout<<"SetMV :["<<mvid<<", "<<index<<", "<<value<<"] with Server["<<sid<<"]"<<endl;
    delete recargs;
    return 1;
#endif
    return -1;
}

int GetMV_Syscall(int mvid, int index)
{
#ifdef NETWORK
    int sid = rand()%cap+1;
    char *sendargs = new char[MaxMailSize];
    sendargs[0] = NETWORK_SERVER_GETMV_REQUEST;
    bcopy((char*)&mvid, &sendargs[1], sizeof(int));
    bcopy((char*)&index, &sendargs[1+sizeof(int)], sizeof(int));
    client->Send(sendargs, sid, currentThread->space->processId, 1+2*sizeof(int));
    delete sendargs;
    
    char *recargs;
    client->Receive(&recargs, currentThread->space->processId);
    ASSERT(recargs != NULL);
    if (recargs[0] != NETWORK_SERVER_GETMV_REPLY) {
        return -1;
    }
    int rv;
    bcopy(&recargs[1], (char*)&rv, sizeof(int));
    delete recargs;
//  cout<<"GetMV: ["<<mvid<<", "<<index<<"] with Server["<<sid<<"]"<<endl;

    return rv;
#endif
    return -1;
}

int DestroyMV_Syscall(int mvid)
{
#ifdef NETWORK
    int sid = rand()%cap+1;
    char *sendargs = new char[MaxMailSize];
    sendargs[0] = NETWORK_SERVER_DESTROYMV_REQUEST;
    bcopy((char*)&mvid, &sendargs[1], sizeof(int));
    client->Send(sendargs, sid, currentThread->space->processId, 1+sizeof(int));
    delete sendargs;
    
    char *recargs;
    client->Receive(&recargs, currentThread->space->processId);
    ASSERT(recargs != NULL);
    if (recargs[0] != NETWORK_SERVER_DESTROYMV_REPLY) {
        return -1;
    }
    delete recargs;
    //cout<<"DestroyMV"<<endl;
    return 1;
#endif
    return -1;
}

void Fork_Thread(int arg){
    // Get a stack from main memory
    int stack = currentThread->space->AllocateNewStack();
    currentThread->userstackTop = stack;
    cout<<stack<<endl;
    // Write to the Reg the next command
    machine->WriteRegister(PCReg, arg);
    machine->WriteRegister(NextPCReg, arg+4);
    machine->WriteRegister(StackReg, stack);
    // Run command in addspace's code segment
    machine->Run();
}

void Fork_Syscall(unsigned int faddr) {
    
    Thread *t = new Thread("Fork_Thread");
    t->space = currentThread->space;
    t->threadinfo->parentThread = currentThread;
    currentThread->threadinfo->threadLock->Acquire();
    currentThread->threadinfo->childThreadsCount++;
    currentThread->threadinfo->threadLock->Release();
    threadTableLock->Acquire();
    // get a variable tid from thread table
    int tid = threadTable->Put((void*)t);
    t->threadinfo->tid = tid;
    t->threadinfo->pid = currentThread->space->processId;
    threadTableLock->Release();
    t->Fork(Fork_Thread, faddr);
}

void Yield_Syscall() {
    
    currentThread -> Yield();
}

void Exit_Syscall(int status) {
    
    // Wait for child threads to finish
    currentThread->threadinfo->threadLock->Acquire();
    while (currentThread->threadinfo->childThreadsCount>0) {
        currentThread->threadinfo->threadCV->Wait(currentThread->threadinfo->threadLock);
    }
    currentThread->threadinfo->isToExit = true;
    currentThread->threadinfo->threadLock -> Release();
    
    // Revise the parent thread's information
    if (currentThread->threadinfo->parentThread != currentThread) {
        currentThread->threadinfo->parentThread->threadinfo->threadLock->Acquire();
        currentThread->threadinfo->parentThread->threadinfo->childThreadsCount--;
        currentThread->threadinfo->parentThread->threadinfo->threadCV->Signal(currentThread->threadinfo->parentThread->threadinfo->threadLock);
        currentThread->threadinfo->parentThread->threadinfo->threadLock->Release();
        
        if(currentThread->space->DeallocateStack()) {
            threadTableLock->Acquire();
            threadTable->Remove(currentThread->threadinfo->tid);
            threadTableLock->Release();
            delete currentThread->threadinfo;
            currentThread->Finish();
        }

    } 
    else {
        // If it is the main thread of process, revise process table
        processTableLock->Acquire();
        Process *proc = (Process *)processTable->Get(currentThread->space->processId);
        proc->processLock->Acquire();
        while (!proc->childProcessList->IsEmpty()) {
            proc->childProcessCount --;
            ProcessBlock *childBlock = (ProcessBlock *)proc->childProcessList -> Remove();
            Process *childProc = (Process *)processTable -> Get(childBlock->childProcessId);
            childProc->processLock -> Acquire();
            childProc->parentAddrSpace = childBlock -> childAddrSpace;
            childProc->processLock -> Release();
        }
        if (proc->parentAddrSpace != currentThread->space) {
            int parentpid = proc->parentAddrSpace->processId;
            Process *parentProc = (Process*)processTable->Get(parentpid);
            parentProc->processLock->Acquire();
            parentProc->childProcessCount--;
            while (true) {
                ProcessBlock *block = (ProcessBlock *)parentProc->childProcessList -> Remove();
                if (block->childProcessId == proc->processId) {
                    delete block;
                    break;
                }
                parentProc->childProcessList->Append(block);
            }
            parentProc->processLock->Release();
        }
        proc->processLock->Release();
        processTable->Remove(currentThread->space->processId);
        processTableLock->Release();
        threadTable->Remove(currentThread->threadinfo->tid);
        delete proc;
        delete currentThread->threadinfo;
        
        osLockTableLock -> Acquire();
        for (int i=0; i<MAX_OSLOCK; i++) {
            if (osLockTable[i].addrSpace == currentThread->space) {
                if (osLockTable[i].isToBeDeleted) {
                    delete osLockTable[i].lock;
                    osLockTable[i].isDeleted = true;
                    osLockTable[i].isToBeDeleted = false;
                }
            }
        }
        osLockTableLock -> Release();
        osConditionTableLock -> Acquire();
        for (int i=0; i<MAX_OSCONDITION; i++) {
            if (osConditionTable[i].addrSpace == currentThread->space) {
                if (osConditionTable[i].isToBeDeleted) {
                    delete osConditionTable[i].condition;
                    osConditionTable[i].isDeleted = true;
                    osConditionTable[i].isToBeDeleted = false;
                }
            }
        }
        osConditionTableLock->Release();
        delete currentThread->space;
        if (mem->IsOver()) {
            interrupt->Halt();
        }
        currentThread->Finish();
    }    
}
// kernel thread for Exec
void Exec_Thread(int arg) {
    currentThread->space->InitRegisters();
    currentThread->space->RestoreState();
    machine->Run();
}

int Exec_Syscall(unsigned int vaddr, int length) {
    char *filename = new char[length];
    if (!filename)
        return -1;
    if (copyin(vaddr, length, filename) == -1) {
        printf("copy error.\n");
        return -1;
    }
    OpenFile *executable = fileSystem -> Open((char*)filename);
    if (executable == NULL) {
        printf("Unable to open file %s\n", filename);
        return -1;
    }
    AddrSpace *space = new AddrSpace(executable);
    ASSERT(space != NULL);
    
    // Create a new process block and add to process table
    Process *proc = new Process();
    proc->childProcessCount = 0;
    proc->parentAddrSpace = currentThread->space;
    proc->space = space;
    proc->childProcessList = new List();
    proc->processLock = new Lock("");
    processTableLock -> Acquire();
    int pid = processTable->Put((void*)proc);
    ASSERT(pid>=0);
    proc = (Process *)processTable->Get(pid);
    proc->processId = pid;
    Process *parentProc = (Process *)processTable->Get(currentThread->space->processId);
    parentProc->processLock->Acquire();
    processTableLock->Release();
    parentProc->childProcessCount ++;
    ProcessBlock *childBlock = new ProcessBlock();
    childBlock->childProcessId = pid;
    childBlock->childAddrSpace = space;
    parentProc->childProcessList->Append((void*)childBlock);
    parentProc->processLock->Release();
    // Create a main thread for the process
    
    Thread *t = new Thread("Exec_Thread");
    t->space = space;
    t->space->processId = pid;
    threadTableLock -> Acquire();
    int tid = threadTable->Put((void*)t);
    ASSERT(tid>=0);
    t->threadinfo->tid = tid;
    t->threadinfo->pid = pid;
    t->threadinfo->parentThread = t;
    threadTableLock->Release();
    t->Fork(Exec_Thread, 0);
    return pid;
}

// Terminate the process when encounter user-mode problem
void TerminateProcess() {
    
    int pid = currentThread->threadinfo->pid;
    processTableLock -> Acquire();
    Process *proc = (Process *)processTable->Get(currentThread->space->processId);
    // Revise the process table
    proc->processLock->Acquire();
    while (!proc->childProcessList->IsEmpty()) {
        proc->childProcessCount--;
        ProcessBlock *childBlock = (ProcessBlock *)proc->childProcessList->Remove();
        Process *childProc = (Process *)processTable->Get(childBlock->childProcessId);
        childProc->processLock->Acquire();
        childProc->parentAddrSpace = childBlock->childAddrSpace;
        childProc->processLock->Release();
    }
    if (proc->parentAddrSpace != currentThread->space) {
        int parentpid = proc->parentAddrSpace->processId;
        Process *parentProc = (Process*)processTable->Get(parentpid);
        parentProc->processLock->Acquire();
        parentProc->childProcessCount --;
        while (true) {
            ProcessBlock *block = (ProcessBlock *)parentProc->childProcessList->Remove();
            if (block->childProcessId == proc->processId) {
                delete block;
                break;
            }
            parentProc->childProcessList->Append(block);
        }
        parentProc->processLock->Release();
    }
    proc->processLock->Release();
    processTable->Remove(currentThread->space->processId);
    processTableLock->Release();
    threadTable->Remove(currentThread->threadinfo->tid);
    delete proc;
    delete currentThread->threadinfo;
    // Remove the threads within the process from the scheduler
    scheduler->RemoveThread(pid);
    delete currentThread->space;
    currentThread->Finish();
}
// Get a value from cnosole
int GetInput_Syscall() {
    
    int value;
    if(scanf("%d", &value) == 0) {
        printf("Error Input Format.\n");
    }
    fflush(stdin);
    return value;
}


void ExceptionHandler(ExceptionType which) {
    int type = machine->ReadRegister(2); // Which syscall?
    int rv=0; 	// the return value from a syscall

    if ( which == SyscallException ) {
	switch (type) {
	    default:
		DEBUG('m', "Unknown syscall - shutting down.\n");
	    case SC_Halt:
		DEBUG('m', "Shutdown, initiated by user program.\n");
		interrupt->Halt();
		break;
	    case SC_Create:
		DEBUG('m', "Create syscall.\n");
		Create_Syscall(machine->ReadRegister(4), machine->ReadRegister(5));
		break;
	    case SC_Open:
		DEBUG('m', "Open syscall.\n");
		rv = Open_Syscall(machine->ReadRegister(4), machine->ReadRegister(5));
		break;
	    case SC_Write:
		DEBUG('m', "Write syscall.\n");
		Write_Syscall(machine->ReadRegister(4),
			      machine->ReadRegister(5),
			      machine->ReadRegister(6));
		break;
	    case SC_Read:
		DEBUG('m', "Read syscall.\n");
		rv = Read_Syscall(machine->ReadRegister(4),
			      machine->ReadRegister(5),
			      machine->ReadRegister(6));
		break;
	    case SC_Close:
		DEBUG('m', "Close syscall.\n");
		Close_Syscall(machine->ReadRegister(4));
		break;
        case SC_Print:
        DEBUG('m', "Print syscall.\n");
        Print_Syscall(machine->ReadRegister(4), 
                      machine->ReadRegister(5), 
                      machine->ReadRegister(6), 
                      machine->ReadRegister(7));
        break;
        case SC_CreateLock:
        DEBUG('m', "CreateLock syscall.\n");
        rv = CreateLock_Syscall(machine->ReadRegister(4),machine->ReadRegister(5));
        break;
        case SC_DestroyLock:
        DEBUG('m', "DestroyLock syscall.\n");
        rv = DestroyLock_Syscall(machine->ReadRegister(4));
        break;
        case SC_AcquireLock:
        DEBUG('m', "AcquireLock syscall.\n");
        rv = AcquireLock_Syscall(machine->ReadRegister(4));
        break;
        case SC_ReleaseLock:
        DEBUG('m', "ReleaseLock syscall.\n");
        rv = ReleaseLock_Syscall(machine->ReadRegister(4));
        break;
        case SC_CreateCondition:
        DEBUG('m', "CreateCondition syscall.\n");
        rv = CreateCondition_Syscall(machine->ReadRegister(4), machine->ReadRegister(5));
        break;
        case SC_DestroyCondition:
        DEBUG('m', "DestroyCondition syscall.\n");
        rv = DestroyCondition_Syscall(machine->ReadRegister(4));
        break;
        case SC_WaitCondition:
        DEBUG('m', "WaitCondition syscall.\n");
        rv = WaitCondition_Syscall(machine->ReadRegister(4), machine->ReadRegister(5));
        break;
        case SC_SignalCondition:
        DEBUG('m', "SignalCondition syscall.\n");
        rv = SignalCondition_Syscall(machine->ReadRegister(4), machine->ReadRegister(5));
        break;
        case SC_BroadcastCondition:
        DEBUG('m', "BroadcastCondition syscall.\n");
        rv = BroadcastCondition_Syscall(machine->ReadRegister(4), machine->ReadRegister(5));
        break;
        case SC_Fork:
        DEBUG('m', "Fork syscall.\n");
        Fork_Syscall(machine->ReadRegister(4));
        break;
        case SC_Yield:
        DEBUG('m', "Yield syscall.\n");
        Yield_Syscall();
        break;
        case SC_Exit:
        DEBUG('m', "Exit syscall.\n");
        Exit_Syscall(machine->ReadRegister(4));
        break;
        case SC_Exec:
        DEBUG('m', "Exec syscall.\n");
        rv = Exec_Syscall(machine->ReadRegister(4), machine->ReadRegister(5));
        break;
        case SC_GetInput:
        DEBUG('m', "GetInput syscall.\n");
        rv = GetInput_Syscall();
        break;
        case SC_CreateMV:
        DEBUG('m', "CreateMV syscall.\n");
        rv = CreateMV_Syscall(machine->ReadRegister(4), machine->ReadRegister(5), machine->ReadRegister(6));
        break;
        case SC_SetMV:
        DEBUG('m', "SetMV syscall.\n");
        rv = SetMV_Syscall(machine->ReadRegister(4), machine->ReadRegister(5), machine->ReadRegister(6));
        break;
        case SC_GetMV:
        DEBUG('m', "GetMV syscall.\n");
        rv = GetMV_Syscall(machine->ReadRegister(4), machine->ReadRegister(5));
        break;
        case SC_DestroyMV:
        DEBUG('m', "DestroyMV syscall.\n");
        rv = DestroyMV_Syscall(machine->ReadRegister(4));
        case SC_Random:
        DEBUG('m', "Random syscall.\n");
            rv = Random_Syscall(machine->ReadRegister(4));    
        break;
    }

	// Put in the return value and increment the PC
	machine->WriteRegister(2,rv);
	machine->WriteRegister(PrevPCReg,machine->ReadRegister(PCReg));
	machine->WriteRegister(PCReg,machine->ReadRegister(NextPCReg));
	machine->WriteRegister(NextPCReg,machine->ReadRegister(PCReg)+4);
	return;
    } 
    else if( which == PageFaultException) {
    }
    else {
      cout<<"Unexpected user mode exception - which:"<<which<<"  type:"<< type<<endl;
        TerminateProcess();
        return;
    }
}
