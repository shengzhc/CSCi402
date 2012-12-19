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
void Cashier()
{
    int holdingES,i;
    int _totalAmount, _cashWaitingCount, _patientType, _pid, _xrayStatus, _xrayImages, _shotStatus;
    while (1) {
        /*-----------------------------------------------------
         // Cashier wait for Nurse bringing patient to his desk
         //-----------------------------------------------------*/
        AcquireLock(cashWaitingLock);
        _cashWaitingCount = GetMV(cashWaitingCount, 0);
        if (_cashWaitingCount > 0) 
        {
            SetMV(cashWaitingCount, 0,_cashWaitingCount-1);
            SignalCondition(cashWaitingCV, cashWaitingLock);
            AcquireLock(cashLock);
            ReleaseLock(cashWaitingLock);
            WaitCondition(cashCV, cashLock);
            /*------------------------------------------
             // Cashier gets the ES and calculate the fee
             //------------------------------------------*/
            
            holdingES = GetMV(cashExamSheet, 0);
            _patientType = GetMV(esStack, holdingES*18+8);
            _pid = GetMV(esStack, holdingES*18+0);
            if(_patientType == 0)
            {
                Print("Cashier receives the examination sheet from Adult Patient [%d].\n", sizeof("Cashier receives the examination sheet from Adult Patient [%d].\n")/sizeof(char), 1, _pid);
            }
            else
            {
                Print("Cashier receives the examination sheet from Child Patient [%d] from Parent [%d].\n", sizeof("Cashier receives the examination sheet from Child Patient [%d] from Parent [%d].\n")/sizeof(char), 2, _pid*256+_pid);
            }
            
            _totalAmount = 20;
            _xrayStatus = GetMV(esStack, holdingES*18+11);
            _xrayImages = GetMV(esStack, holdingES*18+12);
            if (_xrayStatus == 4) {
                for (i=0; i<_xrayImages; i++) {
                    _totalAmount += 10;
                }
                _totalAmount += 20;
                
            }
            _shotStatus = GetMV(esStack, holdingES*18+14);
            if (_shotStatus == 3) {
                _totalAmount += 20;
            }
            SetMV(esStack, holdingES*18+17, _totalAmount);
            if(_patientType == 0)
            {
                Print("Cashier reads the examination sheet of Adult Patient [%d] and asks him to pay $ [%d].\n", sizeof("Cashier reads the examination sheet of Adult Patient [%d] and asks him to pay $ [%d].\n")/sizeof(char), 2, _pid*256+_totalAmount);
            }
            else 
            {
                Print("Cashier reads the examination sheet of Child Patient [%d] and asks Parent [%d] to pay $ [%d].\n", sizeof("Cashier reads the examination sheet of Child Patient [%d] and asks Parent [%d] to pay $ [%d].\n")/sizeof(char), 3, _pid*65536+_pid*256+_totalAmount);
            }
            SignalCondition(cashCV, cashLock);
            WaitCondition(cashCV, cashLock);
            if(_patientType == 0)
            {
                Print("Cashier accepts $[%d] from Adult Patient [%d].\n", sizeof("Cashier accepts $[%d] from Adult Patient [%d].\n")/sizeof(char), 2, _totalAmount*256+_pid);
                Print("Cashier gives a receipt of $[%d] to Adult Patient [%d].\n", sizeof("Cashier gives a receipt of $[%d] to Adult Patient [%d].\n")/sizeof(char), 2, _totalAmount*256+_pid);
                
            }
            else
            {
                Print("Cashier accepts $[%d] from Parent [%d].\n", sizeof("Cashier accepts $[%d] from Parent [%d].\n")/sizeof(char), 2, _totalAmount*256+_pid);
                Print("Cashier gives a receipt of $[%d] to Parent [%d].\n", sizeof("Cashier gives a receipt of $[%d] to Parent [%d].\n")/sizeof(char), 2, _totalAmount*256+_pid);
                
            }
            SignalCondition(cashCV, cashLock);
            WaitCondition(cashCV, cashLock);
            ReleaseLock(cashLock);
        }
        else
        {
            ReleaseLock(cashWaitingLock);
        }
        Yield();
    }
}

void main()
{
    doCreate();
    Cashier();
    Exit(0);
}

