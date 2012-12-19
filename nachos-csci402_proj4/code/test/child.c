#include "syscall.h"
int wrWaitingCount;
int wrnServiceId;
int wrnExamSheet;
int wrnCompleteESList;
int nwrnWaitingCount;
int nwrnExamSheet;
int nwrnId;
int pid;
int wrnTask;
int esStack;
int broadCastId;
int bcIdStatus;
int examRoomsLockMV;
int examRoomsCVMV; 
int doctWaitingLockMV; 
int docStatus; 
int doctWaitingCVMV; 
int ndocESNo;
int wallPackets;
int docid;
int xid;   
int xrayRoomsLockMV; 
int xrayRoomsCVMV;
int xrayRoomsES; 
int xrayRoomsStatus; 
int xrayWaitingES; 
int cashWaitingCount;
int cashExamSheet; 
int nid;
int examRoomsStatus;
int cabinetCount;
int waitingRoomLock;
int wrnLock;
int nwrnLock;
int broadCastIdLock;
int cashWaitingLock;
int cashLock;
int pidLock;
int didLock;
int xidLock;
int nidLock;
int ersStatusLock;
int xrayWaitingLock;
int xrayRoomsStatusLock;
int cabinetLock;
int waitingRoomCV;
int nwrnCV;
int wrnCV;
int nwrnWaitingCV;
int cashWaitingCV;
int cashCV;
int broadCastIdCV;
int bcIdWaitingCV;
int cid;
int cidLock;
int childWaitingLockMV;
int childWaitingCVMV;
int childWaitingTask;
int childExamSheet;

void doCreate() 
{
    int t;
    wrWaitingCount = CreateMV("wrwc", sizeof("wrwc")/sizeof(char), 1);
    wrnServiceId = CreateMV("wsid", sizeof("wsid")/sizeof(char), 1);
    wrnExamSheet = CreateMV("wes", sizeof("wes")/sizeof(char), 1);
    wrnCompleteESList = CreateMV("wrnces", sizeof("wrnces")/sizeof(char), 64);
    nwrnWaitingCount = CreateMV("nwwc", sizeof("nwwc")/sizeof(char), 1);
    nwrnExamSheet = CreateMV("nwes", sizeof("nwes")/sizeof(char), 1);
    nwrnId = CreateMV("nwid", sizeof("nwid")/sizeof(char), 1);
    pid = CreateMV("pid", sizeof("pid")/sizeof(char), 1);
    wrnTask = CreateMV("wt", sizeof("wt")/sizeof(char), 1);
    esStack = CreateMV("ess", sizeof("ess")/sizeof(char), 18*256);
    broadCastId = CreateMV("bcid", sizeof("bcid")/sizeof(char), 1);
    bcIdStatus = CreateMV("bcis", sizeof("bcis")/sizeof(char), 1);
    examRoomsLockMV = CreateMV("erlmv", sizeof("erlmv")/sizeof(char), 6);
    SetMV(examRoomsLockMV, 0, 5);
    examRoomsCVMV = CreateMV("ercmv", sizeof("ercmv")/sizeof(char), 6);
    SetMV(examRoomsCVMV, 0, 5);
    doctWaitingLockMV = CreateMV("dwlm", sizeof("dwlm")/sizeof(char), 3);
    SetMV(doctWaitingLockMV, 0, 2);
    docStatus = CreateMV("ds", sizeof("ds")/sizeof(char), 3);
    SetMV(docStatus, 0, 2);
    doctWaitingCVMV = CreateMV("dwcm", sizeof("dwcm")/sizeof(char), 3);
    SetMV(doctWaitingCVMV, 0, 2);
    ndocESNo = CreateMV("nesn", sizeof("nesn")/sizeof(char), 3);
    SetMV(ndocESNo, 0, 2);
    wallPackets = CreateMV("wp", sizeof("wp")/sizeof(char), 6);
    SetMV(wallPackets, 0, 5);
    docid = CreateMV("di", sizeof("di")/sizeof(char), 1);
    xid = CreateMV("xid", sizeof("xid")/sizeof(char), 1);    
    xrayRoomsLockMV = CreateMV("xrlmv", sizeof("xrlmv")/sizeof(char), 3);
    SetMV(xrayRoomsLockMV, 0, 2);
    xrayRoomsCVMV = CreateMV("xrcmv", sizeof("xrcmv")/sizeof(char), 3);
    SetMV(xrayRoomsCVMV, 0, 2);
    xrayRoomsES = CreateMV("xres", sizeof("xres")/sizeof(char), 3);
    SetMV(xrayRoomsES, 0, 2);
    xrayRoomsStatus = CreateMV("xrs", sizeof("xrs")/sizeof(char), 3);
    SetMV(xrayRoomsStatus, 0, 2);
    xrayWaitingES = CreateMV("xwes", sizeof("xwes")/sizeof(char), 64);
    cashWaitingCount = CreateMV("cwc", sizeof("cwc")/sizeof(char), 1);
    cashExamSheet = CreateMV("ces", sizeof("ces")/sizeof(char), 1);
    nid = CreateMV("nid", sizeof("nid")/sizeof(char), 1);
    examRoomsStatus = CreateMV("ers", sizeof("ers")/sizeof(char), 6);
    SetMV(examRoomsStatus, 0, 5);
    cabinetCount = CreateMV("cac", sizeof("cac")/sizeof(char), 1);
    waitingRoomLock = CreateLock("wrl", sizeof("wrl")/sizeof(char));
    wrnLock = CreateLock("wrnl", sizeof("wrnl")/sizeof(char));
    nwrnLock = CreateLock("nwrnl", sizeof("nwrnl")/sizeof(char));
    t = CreateLock("erl1", sizeof("erl1")/sizeof(char));
    SetMV(examRoomsLockMV, 1, t);
    t = CreateLock("erl2", sizeof("erl2")/sizeof(char));
    SetMV(examRoomsLockMV, 2, t);
    t = CreateLock("erl3", sizeof("erl3")/sizeof(char));
    SetMV(examRoomsLockMV, 3, t);
    t = CreateLock("erl4", sizeof("erl4")/sizeof(char));
    SetMV(examRoomsLockMV, 4, t);
    t = CreateLock("erl5", sizeof("erl5")/sizeof(char));
    SetMV(examRoomsLockMV, 5, t);
    broadCastIdLock = CreateLock("bcil", sizeof("bcil")/sizeof(char));
    cashWaitingLock = CreateLock("cwl", sizeof("cwl"));
    cashLock = CreateLock("cl", sizeof("cl")/sizeof(char));
    pidLock = CreateLock("pl", sizeof("pl")/sizeof(char));
    t = CreateLock("dwl1", sizeof("dwl1")/sizeof(char));
    SetMV(doctWaitingLockMV, 1, t);
    t = CreateLock("dwl2", sizeof("dwl2")/sizeof(char));
    SetMV(doctWaitingLockMV, 2, t);
    didLock = CreateLock("dl", sizeof("dl")/sizeof(char));
    t = CreateLock("xrl1", sizeof("xrl1")/sizeof(char));
    SetMV(xrayRoomsLockMV, 1, t);
    t = CreateLock("xrl2", sizeof("xrl2")/sizeof(char));
    SetMV(xrayRoomsLockMV, 2, t);
    xidLock = CreateLock("xl", sizeof("xl")/sizeof(char));
    nidLock = CreateLock("nl", sizeof("nl")/sizeof(char));
    ersStatusLock = CreateLock("ersl", sizeof("ersl")/sizeof(char));
    xrayWaitingLock = CreateLock("xwl", sizeof("xwl")/sizeof(char));
    xrayRoomsStatusLock = CreateLock("xrsl", sizeof("xrsl")/sizeof(char));
    cabinetLock = CreateLock("cl", sizeof("cl")/sizeof(char));
    waitingRoomCV = CreateCondition("wrc", sizeof("wrc")/sizeof(char));
    nwrnCV = CreateCondition("nwrnc", sizeof("nwrnc")/sizeof(char));
    wrnCV = CreateCondition("wrnc", sizeof("wrnc")/sizeof(char));
    nwrnWaitingCV = CreateCondition("nwc", sizeof("nwc")/sizeof(char));
    t = CreateCondition("erc1", sizeof("erc1")/sizeof(char));
    SetMV(examRoomsCVMV, 1, t);
    t = CreateCondition("erc2", sizeof("erc2")/sizeof(char));
    SetMV(examRoomsCVMV, 2, t);
    t = CreateCondition("erc3", sizeof("erc3")/sizeof(char));
    SetMV(examRoomsCVMV, 3, t);
    t = CreateCondition("erc4", sizeof("erc4")/sizeof(char));
    SetMV(examRoomsCVMV, 4, t);
    t = CreateCondition("erc5", sizeof("erc5")/sizeof(char));
    SetMV(examRoomsCVMV, 5, t);    
    t = CreateCondition("dwc1", sizeof("dwc1")/sizeof(char));
    SetMV(doctWaitingCVMV, 1, t);
    t = CreateCondition("dwc2", sizeof("dwc2")/sizeof(char));
    SetMV(doctWaitingCVMV, 2, t);
    t = CreateCondition("xrc1", sizeof("xrc1")/sizeof(char));
    SetMV(xrayRoomsCVMV, 1, t);
    t = CreateCondition("xrc2", sizeof("xrc2")/sizeof(char));
    SetMV(xrayRoomsCVMV, 2, t);
    cashWaitingCV = CreateCondition("cwc", sizeof("cwc")/sizeof(char));
    cashCV = CreateCondition("cc", sizeof("cc")/sizeof(char));
    broadCastIdCV = CreateCondition("bcicv", sizeof("bcicv")/sizeof(char));
    bcIdWaitingCV = CreateCondition("bwc", sizeof("bwc")/sizeof(char));

    cid = CreateMV("cid", sizeof("cid")/sizeof(char), 1);
    cidLock = CreateLock("cil", sizeof("cil")/sizeof(char));
    childWaitingLockMV = CreateMV("cwlmv", sizeof("cwlmv")/sizeof(char), 11);
    SetMV(childWaitingLockMV, 0, 10);
    t = CreateLock("cwl1", sizeof("cwl1")/sizeof(char));
    SetMV(childWaitingLockMV, 1, t);;
    t = CreateLock("cwl2", sizeof("cwl2")/sizeof(char));
    SetMV(childWaitingLockMV, 2, t);;
    t = CreateLock("cwl3", sizeof("cwl3")/sizeof(char));
    SetMV(childWaitingLockMV, 3, t);;
    t = CreateLock("cwl4", sizeof("cwl4")/sizeof(char));
    SetMV(childWaitingLockMV, 4, t);;
    t = CreateLock("cwl5", sizeof("cwl5")/sizeof(char));
    SetMV(childWaitingLockMV, 5, t);;
    t = CreateLock("cwl6", sizeof("cwl6")/sizeof(char));
    SetMV(childWaitingLockMV, 6, t);;
    t = CreateLock("cwl7", sizeof("cwl7")/sizeof(char));
    SetMV(childWaitingLockMV, 7, t);;
    t = CreateLock("cwl8", sizeof("cwl8")/sizeof(char));
    SetMV(childWaitingLockMV, 8, t);;
    t = CreateLock("cwl9", sizeof("cwl9")/sizeof(char));
    SetMV(childWaitingLockMV, 9, t);;
    t = CreateLock("cwl0", sizeof("cwl0")/sizeof(char));
    SetMV(childWaitingLockMV, 10, t);;
    
    childWaitingCVMV = CreateMV("cwcmv", sizeof("cwcmv")/sizeof(char), 11);
    SetMV(childWaitingCVMV, 0, 10);
    t = CreateCondition("cwc1", sizeof("cwc1")/sizeof(char));
    SetMV(childWaitingCVMV, 1, t);
    t = CreateCondition("cwc2", sizeof("cwc2")/sizeof(char));
    SetMV(childWaitingCVMV, 2, t);
    t = CreateCondition("cwc3", sizeof("cwc3")/sizeof(char));
    SetMV(childWaitingCVMV, 3, t);
    t = CreateCondition("cwc4", sizeof("cwc4")/sizeof(char));
    SetMV(childWaitingCVMV, 4, t);
    t = CreateCondition("cwc5", sizeof("cwc5")/sizeof(char));
    SetMV(childWaitingCVMV, 5, t);
    t = CreateCondition("cwc6", sizeof("cwc6")/sizeof(char));
    SetMV(childWaitingCVMV, 6, t);
    t = CreateCondition("cwc7", sizeof("cwc7")/sizeof(char));
    SetMV(childWaitingCVMV, 7, t);
    t = CreateCondition("cwc8", sizeof("cwc8")/sizeof(char));
    SetMV(childWaitingCVMV, 8, t);
    t = CreateCondition("cwc9", sizeof("cwc9")/sizeof(char));
    SetMV(childWaitingCVMV, 9, t);
    t = CreateCondition("cwc0", sizeof("cwc0")/sizeof(char));
    SetMV(childWaitingCVMV, 10, t);
    childWaitingTask = CreateMV("cwt", sizeof("cwt")/sizeof(char), 11);
    SetMV(childWaitingTask, 0, 10);
    childExamSheet = CreateMV("ches", sizeof("ches")/sizeof(char), 1);
}

void Child()
{
    
    int myExamSheet;
    int _cid, _childWaitingLock, _childWaitingCV, _childWaitingTask, _docid;
    AcquireLock(cidLock);
    _cid = GetMV(cid, 0)+1;
    SetMV(cid, 0, _cid);
    ReleaseLock(cidLock);
    _childWaitingLock = GetMV(childWaitingLockMV, _cid);
    _childWaitingCV = GetMV(childWaitingCVMV, _cid);
    while (1) 
    {
        AcquireLock(_childWaitingLock);
        /*-------------------------------------------------
         // Child is informed to Enter the WR
         //-------------------------------------------------*/
        _childWaitingTask = GetMV(childWaitingTask, _cid);
        if (_childWaitingTask == 1)
        {
            SetMV(childWaitingTask, _cid, 0);
            Print("Child Patient [%d] has entered the Doctor's Office Waiting Room with Parent [%d].\n", sizeof("Child Patient [%d] has entered the Doctor's Office Waiting Room with Parent [%d].\n")/sizeof(char), 2, _cid*256+_cid);
            SignalCondition(_childWaitingCV, _childWaitingLock);
            ReleaseLock(_childWaitingLock);
        }
        /*------------------------------
         // Child is informed to Follow 
         //------------------------------*/
        else if (_childWaitingTask == 2)
        {
            SetMV(childWaitingTask, _cid, 0);
            myExamSheet = GetMV(childExamSheet, 0);
            SignalCondition(_childWaitingCV, _childWaitingLock);
            ReleaseLock(_childWaitingLock);
        }
        /*-------------------------------------------------------------
         // Child is informed that he/she is undergoing the 1st diagnose
         //-------------------------------------------------------------*/
        else if (_childWaitingTask == 3)
        {
            SetMV(childWaitingTask, _cid, 0);
            SignalCondition(_childWaitingCV, _childWaitingLock);
            ReleaseLock(_childWaitingLock);
        }
        /*-------------------------------------------------------------
         // Child is informed that he/she should take X-Ray
         //-------------------------------------------------------------*/
        else if (_childWaitingTask == 4)
        {
            SetMV(childWaitingTask, _cid, 0);
            _docid = GetMV(esStack, myExamSheet*18+7);
            Print("Child Patient [%d] has been informed by Doctor [%d] that he needs an Xray.\n", sizeof("Child Patient [%d] has been informed by Doctor [%d] that he needs an Xray.\n")/sizeof(char), 2, _cid*256+_docid);
            SignalCondition(_childWaitingCV, _childWaitingLock);
            ReleaseLock(_childWaitingLock);
        }
        /*-------------------------------------------------------------
         // Child is informed that he/she should take a shot
         //-------------------------------------------------------------*/
        else if (_childWaitingTask == 5)
        {
            SetMV(childWaitingTask, _cid, 0);
            _docid = GetMV(esStack, myExamSheet*18+7);
            Print("Child Patient [%d] has been informed by Doctor [%d] that he will be administered a shot.\n", sizeof("Child Patient [%d] has been informed by Doctor [%d] that he will be administered a shot.\n")/sizeof(char), 2, _cid*256+_docid);
            SignalCondition(_childWaitingCV, _childWaitingLock);
            ReleaseLock(_childWaitingLock);
        }
        /*-------------------------------------------------------------
         // Child is informed that 1st diagnose is done
         //-------------------------------------------------------------*/
        else if (_childWaitingTask == 6)
        {
            SetMV(childWaitingTask, _cid, 0);
            _docid = GetMV(esStack, myExamSheet*18+7);
            Print("Child Patient [%d] has been diagnosed by Doctor [%d].\n", sizeof("Child Patient [%d] has been diagnosed by Doctor [%d].\n")/sizeof(char), 2, _cid*256+_docid);
            SignalCondition(_childWaitingCV, _childWaitingLock);
            ReleaseLock(_childWaitingLock);
        }
        /*-------------------------------------------------------------
         // Child is informed that he/she should get to the  x-ray table
         //-------------------------------------------------------------*/
        
        else if (_childWaitingTask == 7)
        {
            SetMV(childWaitingTask, _cid, 0);
            Print("Child Patient [%d] gets on the table.\n", sizeof("Child Patient [%d] gets on the table.\n")/sizeof(char), 1, _cid);
            SignalCondition(_childWaitingCV, _childWaitingLock);
            ReleaseLock(_childWaitingLock);
        }
        /*-------------------------------------------------------------
         // Child is informed that he/she should move to take x-ray
         //-------------------------------------------------------------*/
        else if (_childWaitingTask == 8)
        {
            SetMV(childWaitingTask, _cid, 0);
            Print("Child Patienht [%d] moves for the next Xray.\n", sizeof("Child Patienht [%d] moves for the next Xray.\n")/sizeof(char), 1, _cid);
            SignalCondition(_childWaitingCV, _childWaitingLock);
            ReleaseLock(_childWaitingLock);
        } 
        /*-------------------------------------------------------------
         // Child is informed that he/she has taken a x-ray image
         //-------------------------------------------------------------*/
        else if (_childWaitingTask == 9)
        {
            SetMV(childWaitingTask, _cid, 0);
            Print("Child Patient [%d] has been asked to take an Xray.\n", sizeof("Child Patient [%d] has been asked to take an Xray.\n")/sizeof(char), 1, _cid);
            SignalCondition(_childWaitingCV, _childWaitingLock);
            ReleaseLock(_childWaitingLock);
        }
        /*-------------------------------------------------------------
         // Child is informed that nures is coming for a shot
         //-------------------------------------------------------------*/
        else if (_childWaitingTask == 10)
        {
            SetMV(childWaitingTask, _cid, 0);
            SignalCondition(_childWaitingCV, _childWaitingLock);
            ReleaseLock(_childWaitingLock);
        }
        /*-------------------------------------------------------------
         // Child is informed that he/she is diagnosed by doctor 2nd time
         //-------------------------------------------------------------*/
        else if (_childWaitingTask == 11)
        {
            SetMV(childWaitingTask, _cid, 0);
            _docid = GetMV(esStack, myExamSheet*18+7);
            Print("Child Patient [%d] has been diagnosed by Doctor [%d].\n", sizeof("Child Patient [%d] has been diagnosed by Doctor [%d].\n")/sizeof(char), 2, _cid*256+_docid);
            SignalCondition(_childWaitingCV, _childWaitingLock);
            ReleaseLock(_childWaitingLock);
        }
        /*-------------------------------------------------------------
         // Child is informed that he/she should leave the hospital
         //-------------------------------------------------------------*/
        
        else if (_childWaitingTask == 12)
        {
            SetMV(childWaitingTask, _cid, 0);
            SignalCondition(_childWaitingCV, _childWaitingLock);
            ReleaseLock(_childWaitingLock);
            break;
        }        
        else
        {
            ReleaseLock(_childWaitingLock);
        }
        Yield();
    }
}
void main()
{
    doCreate();
    Child();
    Exit(0);
}

