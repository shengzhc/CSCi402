#include <iostream>
#include "sersynch.h"
#include "system.h"

using namespace std;

           
SLock::SLock(char *name)
{
    lockname = name;
    waitingList = new List();
    owner = -1;
    status = false;
}

SLock::~SLock()
{
    if (lockname != NULL) {
        delete lockname;
    }
    if (waitingList != NULL) {
        delete waitingList;
    }
}

// Acquire the lock, parameter is the count owner value
int SLock::Acquire(int presentowner)
{   
    if (!status) {
        owner = presentowner;
        status = true;
    }
    if (status && owner == presentowner) {
        return 1;
    }
    if (status && owner != presentowner) {
        return -1;
    }
    return -1;
}
// Release the lock while validating the owner
int SLock::Release(int presentowner)
{
    if (!status) {
        cout<<"No owner"<<endl;
        return 0;
    }
    if (owner != presentowner) {
        cout<<"It is not your lock"<<endl;
        return 0;
    }
    if (!waitingList->IsEmpty()) {
        return 2;
    }
    else {
        status = false;
        owner = -1;
    }
    return 1;
}
// Pend the msg to SLock's waiting list
void SLock::Pend(Message *msg)
{
    waitingList->Append((void*)msg);
}
// Get one msg from the lock's waiting list
Message* SLock::GetMsg()
{
    if (waitingList->IsEmpty()) {
        return NULL;
    }
    Message *msg = (Message *)waitingList->Remove();
    owner = msg->pktHdr.to*100+msg->mailHdr.to;
    return msg;
}

SLockTable::SLockTable()
{
    sltable = new SLock*[SL_MAX];
    slmap = new BitMap(SL_MAX);
}

SLockTable::~SLockTable()
{
    delete sltable;
    delete slmap;
}
// Get the lock ptr from lock table
SLock* SLockTable::GetLock(int lockid)
{
    return sltable[lockid];
}
// Pend one msg to the specific slock
void SLockTable::Pend(int lockid, Message *msg)
{
    sltable[lockid]->Pend(msg);
}
// Acquire a lock from a lock table
int SLockTable::Acquire(int lockid, int owner)
{
    if (sltable[lockid] == NULL) {
        return 0;
    }
    return sltable[lockid]->Acquire(owner);
}
// Release a lock from a lock table
int SLockTable::Release(int lockid, int owner)
{
    if (sltable[lockid] == NULL) {
        return 0;
    }
    return sltable[lockid]->Release(owner);
}
// Get a msg from a lock within lock table
Message* SLockTable::GetMsg(int lockid)
{
    Message *msg = sltable[lockid]->GetMsg();
    return msg;
}
// Search for the lock in lock table
int SLockTable::SearchName(char *name)
{
    for (int i=0; i<SL_MAX; i++) {
        if (slmap->Test(i) && !strcmp(name, sltable[i]->GetName())) {
            return i;
        }
    }
    return -1;
}
// Assign a new lock in lock table
int SLockTable::AssignLock(char *name)
{
    int lockid = slmap->Find();
    sltable[lockid] = new SLock(name);
    return lockid;
}
// Destroy a lock
int SLockTable::Destroy(int lockid)
{
    if (sltable[lockid] == NULL && slmap->Test(lockid)) {
        return 0;
    } 
    if (sltable[lockid] == NULL && !slmap->Test(lockid)) {
        return -1;
    }
    else {
        delete sltable[lockid];
        slmap->Clear(lockid);
        return 1;
    }
}
// Set the lock owner value
void SLockTable::SetLockOwner(int lockid, int owner)
{
    sltable[lockid]->SetOwner(owner);
}

SCondition::SCondition(char *name)
{
    scname = name;
    waitingList = new List();
    waitingLock = NULL;
}
 
SCondition::~SCondition()
{
    if (scname != NULL) {
        delete scname;
    }
    if (waitingList != NULL) {
        delete waitingList;
    }
    waitingLock = NULL;
}
// Suspend a msg in condition's waiting list
void SCondition::Pend(Message *msg)
{
    waitingList->Append((void*)msg);
}
// Condition wait
int SCondition::Wait(int lockid)
{
    if (waitingLock == NULL) {
        waitingLock = slt->GetLock(lockid);
    }
    if (waitingLock != slt->GetLock(lockid)) {
        cout<<"Error Wait\n"<<endl;
        return 0;
    }
    return 1;
}
// Condition Signal
int SCondition::Signal(int lockid)
{
    if (waitingLock == NULL) {
        return 1;
    }
    if (waitingLock != slt->GetLock(lockid)) {
        return 0;
    }
    if (waitingList->IsEmpty()) {
        return 1;
    }
    else {
        return 2;
    }
}
// Get a msg from condition's waiting list
Message* SCondition::GetMsg()
{
    if (waitingList->IsEmpty()) {
        return NULL;
    }
    Message *msg = (Message*)waitingList->Remove();
    if (waitingList->IsEmpty()) {
        waitingLock = NULL;
    }
    return msg;
}

SCTable::SCTable()
{
    scmap = new BitMap(SC_MAX);
    sctable = new SCondition*[SC_MAX];
}

SCTable::~SCTable()
{
    delete scmap;
    delete sctable;
}
// Get a msg from the sc table
Message* SCTable::GetMsg(int cvid)
{
    return sctable[cvid]->GetMsg();
}
// Suspend a msg to a sc
void SCTable::Pend(int cvid, Message *msg)
{
    ASSERT(sctable[cvid] != NULL);
    sctable[cvid]->Pend(msg);
}
// Search for the sc with the name
int SCTable::SearchName(char *name)
{
    for (int i=0; i<SC_MAX; i++) {
        if (scmap->Test(i) && !strcmp(name, sctable[i]->GetName())) {
            return i;
        }
    }
    return -1;   
}
// Assign a new sc 
int SCTable::AssignCondition(char *name)
{
    int cvid = scmap->Find();
    sctable[cvid] = new SCondition(name);
    return cvid;
}
// Destroy a sc
int SCTable::Destroy(int cvid)
{
    if (sctable[cvid] == NULL && !scmap->Test(cvid)) {
        return 0;
    }
    if (sctable[cvid] == NULL && scmap->Test(cvid)) {
        return -1;
    }
    else {
        delete sctable[cvid];
        scmap->Clear(cvid);
        return 1;
    }
}
// Wait
int SCTable::Wait(int cvid, int lockid)
{
    if (sctable[cvid] == NULL) {
        return 0;
    }
    return sctable[cvid]->Wait(lockid);
}
// Signal
int SCTable::Signal(int cvid, int lockid)
{
    if (sctable[cvid] == NULL) {
        return 0;
    }
    return sctable[cvid]->Signal(lockid);
}
// Set the MV with value
int MV::SetValue(int index, int value)
{
    if (mv == NULL) {
        return -1;
    }
    if (index>=length) {
        return -1;
    }
    mv[index] = value;
    return mv[index];
}
// Retrieve the MV 
int MV::GetValue(int index)
{
    ASSERT(mv != NULL);
    ASSERT(index<length);
    return mv[index];
}

MVTable::MVTable()
{
    mvmap = new BitMap(MV_MAX);
    mvtable = new MV*[MV_MAX];
}

MVTable::~MVTable()
{
    delete mvmap;
    delete mvtable;
}
// Search the name in MV table
int MVTable::SearchName(char *name)
{
    for (int i=0; i<MV_MAX; i++) {
        if (mvmap->Test(i) && !strcmp(name, mvtable[i]->GetName())) {
            return i;
        }
    }
    return -1;   
}
// Assign a MV
int MVTable::AssignMV(char *name, int size)
{
    int mvid = mvmap->Find();
    if (mvid != -1) {
        mvtable[mvid] = new MV(name, size);
    }
    return mvid;
}
// Set the MV value with mvid and inner index
int MVTable::SetMV(int mvid, int index, int value)
{
    if (mvtable[mvid] == NULL) {
        return -1;
    }
    return mvtable[mvid]->SetValue(index, value);
}
// Get the mv value
int MVTable::GetMV(int mvid, int index)
{
    if (mvtable[mvid] == NULL) {
        return -1;
    }
    else {
        return mvtable[mvid]->GetValue(index);
    }
}
// Destroy a mv
int MVTable::DestroyMV(int mvid)
{
    if (mvtable[mvid] != NULL) {
        delete mvtable[mvid];
        mvmap->Clear(mvid);
    }
    return 1;
}


