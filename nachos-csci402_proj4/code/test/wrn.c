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

void doCreate() 
{
    int t;
    wrWaitingCount = CreateMV("wrwc", sizeof("wrwc")/sizeof(char), 1);
    wrnServiceId = CreateMV("wsid", sizeof("wsid")/sizeof(char), 1);
    wrnExamSheet = CreateMV("wes", sizeof("wes")/sizeof(char), 2);
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
}

void WaitingRoomNurse()
{
    int _wrWaitingCount;
    int _wrnExamSheet;
    int _name;
    int _age;
    int _eid;
    int _front;
    int _rear;
    int _nwrnWaitingCount;
    int _nwrnExamSheet;
    int _nwrnId;
    int _wrnTask;
    int _patientType;
    int _wrnServiceId;
    while (1) {
         /*--------------------------------------------------------      
         // WRN Service the Patient
         //--------------------------------------------------------*/
        
        AcquireLock(waitingRoomLock);
        _wrWaitingCount = GetMV(wrWaitingCount, 0);
        if (_wrWaitingCount> 0) 
        {
            SetMV(wrWaitingCount, 0, _wrWaitingCount-1);
            SignalCondition(waitingRoomCV, waitingRoomLock);
            AcquireLock(wrnLock);
            ReleaseLock(waitingRoomLock);
            WaitCondition(wrnCV, wrnLock);
            _wrnTask = GetMV(wrnTask, 0);
            _wrnServiceId = GetMV(wrnServiceId, 0);
            /*------------------------------------------
             // WRN gives the form to the Adult
             //------------------------------------------*/
            if (_wrnTask == 1) 
            {
                _wrnExamSheet = GetMV(wrnExamSheet, 0);
                _wrnExamSheet = _wrnExamSheet+1;
                SetMV(wrnExamSheet, 0, _wrnExamSheet);

                Print("Waiting Room nurse gives a form to Adult patient [%d].\n", sizeof("Waiting Room nurse gives a form to Adult patient [%d].\n")/sizeof(char), 1, _wrnServiceId);
                SetMV(wrnTask, 0, 0);
                SetMV(wrnServiceId, 0, -1);
                SignalCondition(wrnCV, wrnLock);
                ReleaseLock(wrnLock);
            }
            /*------------------------------------------
            // WRN gives the form to the Parent
            //------------------------------------------*/
            else if (_wrnTask == 2)
            {
                
                _wrnExamSheet = GetMV(wrnExamSheet, 0);
                _wrnExamSheet = _wrnExamSheet+1;
                SetMV(wrnExamSheet, 0, _wrnExamSheet);
                Print("Waiting Room nurse gives a form[%d] to the Parent of Child patient [%d].\n", sizeof("Waiting Room nurse gives a form[%d] to the Parent of Child patient [%d].\n")/sizeof(char), 2, _wrnExamSheet*256+_wrnServiceId);
                SetMV(wrnServiceId, 0, -1);
                SetMV(wrnTask, 0, 0);
                SignalCondition(wrnCV, wrnLock);
                ReleaseLock(wrnLock);
            }
            /*------------------------------------------
             // WRN receives the form from patient
             //------------------------------------------*/
            else if (_wrnTask == 3) 
            {
                _wrnExamSheet = GetMV(wrnExamSheet, 1);
                _patientType = GetMV(esStack, _wrnExamSheet*18+8);
                _name = GetMV(esStack, _wrnExamSheet*18+0);
                _age = GetMV(esStack, _wrnExamSheet*18+1);
                _eid = GetMV(esStack, _wrnExamSheet*18+3);
                if (_patientType == 0)
                {
                    Print("Waiting Room nurse accepts the form from Adult Patient [%d] and age [%d].\n", sizeof("Waiting Room nurse accepts the form from Adult Patient [%d] and age [%d].\n")/sizeof(char), 2, _name*256+_age);
                    Print("Waiting Room nurse creates an examination sheet for Adult patient with name [%d] and age [%d].\n", sizeof("Waiting Room nurse creates an examination sheet for Adult patient with name [%d] and age [%d].\n")/sizeof(char), 2, _name*256+_age);
                    Print("Waiting Room nurse tells the Adult Patient [%d] to wait in the waiting room for a nurse.\n", sizeof("Waiting Room nurse tells the Adult Patient [%d] to wait in the waiting room for a nurse.\n")/sizeof(char), 1, _name);
                }
                else
                {
                    Print("Waiting Room nurse accepts the form from Parent with name [%d] and age [%d].\n", sizeof("Waiting Room nurse accepts the form from Parent with name [%d] and age [%d].\n")/sizeof(char), 2, _name*256+_age);
                    Print("Waiting Room nurse creates an examination sheet for Child patient with name [%d] and age [%d].\n", sizeof("Waiting Room nurse creates an examination sheet for Child patient with name [%d] and age [%d].\n")/sizeof(char), 2, _name*256+_age);
                    Print("Waiting Room nurse tells Parent [%d] to wait in the waiting room for a nurse.\n", sizeof("Waiting Room nurse tells Parent [%d] to wait in the waiting room for a nurse.\n")/sizeof(char), 1, _name);
                }
                _front = GetMV(wrnCompleteESList, 0);
                _rear = GetMV(wrnCompleteESList, 1);
                if (_front == 0) {
                    SetMV(wrnCompleteESList, 0, 2);
                    SetMV(wrnCompleteESList, 1, 2);
                    SetMV(wrnCompleteESList, 2, _wrnExamSheet);
                }
                else {
                
                    SetMV(wrnCompleteESList, _rear+1, _wrnExamSheet);
                    SetMV(wrnCompleteESList, 1, _rear+1);
                }
                SignalCondition(wrnCV, wrnLock);
                SetMV(wrnTask, 0, 0);
                ReleaseLock(wrnLock);
            } 
            else 
            {
                
                Print("Error: Do nothing while patients are waiting in the WR.\n", sizeof("Error: Do nothing while patients are waiting in the WR.\n")/sizeof(char), 0, 0);
                ReleaseLock(wrnLock);
            }
        } 
        else 
        {   /*----------------------------------------- 
             // WRN Service the Nurse and give the form
             //-----------------------------------------*/
            ReleaseLock(waitingRoomLock);
            AcquireLock(wrnLock);
            _nwrnWaitingCount = GetMV(nwrnWaitingCount, 0);
            if (_nwrnWaitingCount > 0) 
            {
                SetMV(nwrnWaitingCount, 0, _nwrnWaitingCount-1);
                SignalCondition(nwrnWaitingCV, wrnLock);
                AcquireLock(nwrnLock);
                ReleaseLock(wrnLock);
                WaitCondition(nwrnCV, nwrnLock);
                
                _front = GetMV(wrnCompleteESList, 0);
                _rear = GetMV(wrnCompleteESList, 1);
                
                if (_front != 0) 
                {
                    _nwrnExamSheet = GetMV(wrnCompleteESList, _front);
                    if (_front == _rear) {
                        SetMV(wrnCompleteESList, 0, 0);
                        SetMV(wrnCompleteESList, 1, 0);
                    }
                    else {
                        SetMV(wrnCompleteESList, 0, _front+1);
                    }
                    
                    SetMV(nwrnExamSheet, 0, _nwrnExamSheet);
                    SignalCondition(nwrnCV, nwrnLock);
                    
                    _nwrnId = GetMV(nwrnId, 0);
                    _name = GetMV(esStack, _nwrnExamSheet*18+0);
                    Print("Waiting Room nurse gives examination sheet of patient [%d] to Nurse [%d].\n", sizeof("Waiting Room nurse gives examination sheet of patient [%d] to Nurse [%d].\n")/sizeof(char), 2, _name*256+_nwrnId);
                    WaitCondition(nwrnCV, nwrnLock);
                    
                    SetMV(nwrnId, 0, -1);
                    ReleaseLock(nwrnLock);
                }
                else
                {
                    SetMV(nwrnExamSheet, 0, -1);
                    SignalCondition(nwrnCV, nwrnLock);
                    WaitCondition(nwrnCV, nwrnLock);
                    ReleaseLock(nwrnLock);
                }
            }
            else
            {
                ReleaseLock(wrnLock);
            }
        }
    }    
}

void main()
{
    doCreate();
    WaitingRoomNurse();
    
    Exit(0);
}

