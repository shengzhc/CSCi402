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
int childExamSheetLock;
int paid;
int paidLock;
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
    childExamSheetLock = CreateLock("cesl", sizeof("cesl")/sizeof(char));
    paid = CreateMV("paid", sizeof("paid")/sizeof(char), 1);
    paidLock = CreateLock("pal", sizeof("pal")/sizeof(char));

}

void Parent()
{
    int myExamSheet;
    int mp_Type = 1;
    int erIndex, i;
    int _paid, _childWaitingLock, _childWaitingCV, _broadCastId, _examRoomsLock, _examRoomsCV, _nid, _illness,_patientTask, _docid, _xrayImages, _shotStatus, _totalAmount;
    AcquireLock(paidLock);
    _paid = GetMV(paid, 0);
    _paid += 1;
    SetMV(paid, 0, _paid);
    ReleaseLock(paidLock);
    
    /*-----------------------------------
     // Parent Interacts with WRN
     //-----------------------------------*/
    
    Print("Parent  [%d] has entered the Doctor's Office Waiting Room.\n", sizeof("Parent  [%d] has entered the Doctor's Office Waiting Room.\n")/sizeof(char), 1, _paid);
    _childWaitingLock = GetMV(childWaitingLockMV, _paid);
    _childWaitingCV = GetMV(childWaitingCVMV, _paid);
    AcquireLock(_childWaitingLock);
    SetMV(childWaitingTask, _paid, 1);
    WaitCondition(_childWaitingCV, _childWaitingLock);
    ReleaseLock(_childWaitingLock);
    /*-----------------------------------
     // Parent Wait in the line of WR
     //-----------------------------------*/
    AcquireLock(waitingRoomLock);
    SetMV(wrWaitingCount, 0, GetMV(wrWaitingCount, 0)+1);
    Print("Parent [%d] gets in line of the Waiting Room Nurse to get registration form.\n", sizeof("Parent [%d] gets in line of the Waiting Room Nurse to get registration form.\n")/sizeof(char), 1, _paid);
    WaitCondition(waitingRoomCV, waitingRoomLock);
    /*-----------------------------------
     // Parent gets form from WR
     //-----------------------------------*/
    ReleaseLock(waitingRoomLock);
    AcquireLock(wrnLock);
    SetMV(wrnTask, 0, 2);
    SetMV(wrnServiceId, 0, _paid);
    SignalCondition(wrnCV, wrnLock);
    WaitCondition(wrnCV, wrnLock);
    Print("Parent of child patient [%d] gets the form from the Waiting Room Nurse.\n", sizeof("Parent of child patient [%d] gets the form from the Waiting Room Nurse.\n")/sizeof(char), 1, _paid);
    myExamSheet = GetMV(wrnExamSheet, 0);
    ReleaseLock(wrnLock);
    /*-----------------------------------
     // Parent Fill the Form
     //-----------------------------------*/
    SetMV(esStack, myExamSheet*18+0, _paid);
    SetMV(esStack, myExamSheet*18+1, Random(10)+5);
    SetMV(esStack, myExamSheet*18+2, myExamSheet);
    SetMV(esStack, myExamSheet*18+8, mp_Type);
    SetMV(esStack, myExamSheet*18+9, 0);
    SetMV(esStack, myExamSheet*18+10, Random(3));
    SetMV(esStack, myExamSheet*18+11, 0);
    SetMV(esStack, myExamSheet*18+12, 0);
    SetMV(esStack, myExamSheet*18+13, 3);
    SetMV(esStack, myExamSheet*18+14, 0);
    SetMV(esStack, myExamSheet*18+15, 0);
    SetMV(esStack, myExamSheet*18+16, 0);
    SetMV(esStack, myExamSheet*18+17, 0);
    AcquireLock(waitingRoomLock);
    SetMV(wrWaitingCount, 0, GetMV(wrWaitingCount, 0)+1);
    Print("Parent of child patient [%d] gets in line of the Waiting Room Nurse to submit the registration form.\n", sizeof("Parent of child patient [%d] gets in line of the Waiting Room Nurse to submit the registration form.\n")/sizeof(char), 1, _paid);
    WaitCondition(waitingRoomCV, waitingRoomLock);
    /*-----------------------------------
     // Parent submit the form
     //-----------------------------------*/
    ReleaseLock(waitingRoomLock);
    AcquireLock(wrnLock);
    SetMV(wrnTask, 0, 3);
    SetMV(wrnExamSheet, 1, myExamSheet);
    SetMV(wrnServiceId, 0, _paid);
    SignalCondition(wrnCV, wrnLock);
    Print("Parent of child patient [%d] submits the filled form to the Waiting Room Nurse.\n", sizeof("Parent of child patient [%d] submits the filled form to the Waiting Room Nurse.\n")/sizeof(char), 1, _paid);
    WaitCondition(wrnCV, wrnLock);
    ReleaseLock(wrnLock);
    /*---------------------------------------------
     // Parent Wait for being Called by Nurse
     //--------------------------------------------- */
    AcquireLock(broadCastIdLock);
    _broadCastId = GetMV(_broadCastId, 0);
    while ((_broadCastId-30) != _paid) 
    {
        WaitCondition(broadCastIdCV, broadCastIdLock);
        _broadCastId = GetMV(broadCastId, 0);
    }
    
    SetMV(bcIdStatus, 0, 0);
    SignalCondition(bcIdWaitingCV, broadCastIdLock);
    ReleaseLock(broadCastIdLock);
    
    AcquireLock(_childWaitingLock);
    SetMV(childWaitingTask, _paid, 2);
    AcquireLock(childExamSheetLock);
    SetMV(childExamSheet, 0, myExamSheet);
    Print("Parent [%d] asks Child Patient [%d] to follow them.\n", sizeof("Parent [%d] asks Child Patient [%d] to follow them.\n")/sizeof(char), 2, _paid*256+_paid);
    WaitCondition(_childWaitingCV, _childWaitingLock);
    ReleaseLock(childExamSheetLock);
    ReleaseLock(_childWaitingLock);
    erIndex = GetMV(esStack, myExamSheet*18+3);
    _examRoomsLock = GetMV(examRoomsLockMV, erIndex);
    _examRoomsCV = GetMV(examRoomsCVMV, erIndex);
    AcquireLock(_examRoomsLock);
    SignalCondition(_examRoomsCV, _examRoomsLock);
    WaitCondition(_examRoomsCV, _examRoomsLock);
    _nid = GetMV(esStack, myExamSheet*18+6);
    Print("Parent [%d] is following Nurse [%d] to Examination Room [%d].\n", sizeof("Parent [%d] is following Nurse [%d] to Examination Room [%d].\n")/sizeof(char), 3, _paid*65536+_nid*256+erIndex);
    SignalCondition(_examRoomsCV, _examRoomsLock);
    Print("Parent [%d] has arrived at Examination Room [%d].\n", sizeof("Parent [%d] has arrived at Examination Room [%d].\n")/sizeof(char), 2, _paid*256+erIndex);
    WaitCondition(_examRoomsCV, _examRoomsLock);
    /*-----------------------------------
     // Parent tell the SYMPTOM to Nurse
     //-----------------------------------*/
    SetMV(esStack, myExamSheet*18+10, Random(4));
    _illness = GetMV(esStack, myExamSheet*18+10);
    switch (_illness) 
    {
        case 0:
            Print("Parent [%d] says, \"His symptoms are Pain\".\n", sizeof("Parent [%d] says, \"His symptoms are Pain\".\n")/sizeof(char), 1, _paid);
            break;
        case 1:
            Print("Parent [%d] says, \"His symptoms are Nausea\".\n", sizeof("Parent [%d] says, \"His symptoms are Nausea\".\n")/sizeof(char), 1, _paid);
            break;
        case 2:
            Print("Parent [%d] says, \"His symptoms are Hear Alien Voices\".\n", sizeof("Parent [%d] says, \"His symptoms are Hear Alien Voices\".\n")/sizeof(char), 1, _paid);
            break;
        default:
            break;
    }
    SignalCondition(_examRoomsCV, _examRoomsLock);
    WaitCondition(_examRoomsCV, _examRoomsLock);
    ReleaseLock(_examRoomsLock);
    while (1) 
    {
        AcquireLock(_examRoomsLock);
        /*-----------------------------------
         // All things are Done.
         //-----------------------------------*/
        _patientTask = GetMV(esStack, myExamSheet*18+9);
        _docid = GetMV(esStack, myExamSheet*18+7);
        _nid = GetMV(esStack, myExamSheet*18+6);
        if (_patientTask == 6) 
        {
            AcquireLock(_childWaitingLock);
            SetMV(childWaitingTask, _paid, 12);
            WaitCondition(_childWaitingCV, _childWaitingLock);
            ReleaseLock(_childWaitingLock);
            break;            
        } 
        /*-----------------------------------------------------
         // Stay @ ER and Wait for the 2nd Diagnose after X-ray
         //-----------------------------------------------------*/
        else if (_patientTask == 5)
        {
            SetMV(esStack, myExamSheet*18+9, 0);
            AcquireLock(_childWaitingLock);
            SetMV(childWaitingTask, _paid, 11);
            WaitCondition(_childWaitingCV, _childWaitingLock);
            ReleaseLock(_childWaitingLock);
            SignalCondition(_examRoomsCV, _examRoomsLock);
            WaitCondition(_examRoomsCV, _examRoomsLock);
        }
        /*------------------------------------------
         // Stay @ ER and Wait for the 1st Diagnose  
         //------------------------------------------*/
        else if (_patientTask == 4)
        {
            SetMV(esStack, myExamSheet*18+9, 0);
            AcquireLock(_childWaitingLock);
            SetMV(childWaitingTask, _paid, 3);
            WaitCondition(_childWaitingCV, _childWaitingLock);
            ReleaseLock(_childWaitingLock);
            SignalCondition(_examRoomsCV, _examRoomsLock);
            WaitCondition(_examRoomsCV, _examRoomsLock);
            _xrayImages = GetMV(esStack, myExamSheet*18+12);
            if (_xrayImages > 0) 
            {
                AcquireLock(_childWaitingLock);
                SetMV(childWaitingTask, _paid, 4);
                WaitCondition(_childWaitingCV, _childWaitingLock);
                ReleaseLock(_childWaitingLock);
                SignalCondition(_examRoomsCV, _examRoomsLock);
                WaitCondition(_examRoomsCV, _examRoomsLock);
            }
            _shotStatus = GetMV(esStack, myExamSheet*18+14);        
            
            if (_shotStatus == 1)
            {
                AcquireLock(_childWaitingLock);
                SetMV(childWaitingTask, _paid, 5);
                WaitCondition(_childWaitingCV, _childWaitingLock);
                ReleaseLock(_childWaitingLock);
                SignalCondition(_examRoomsCV, _examRoomsLock);
                WaitCondition(_examRoomsCV, _examRoomsLock);
            }
            AcquireLock(_childWaitingLock);
            SetMV(childWaitingTask, _paid, 6);
            WaitCondition(_childWaitingCV, _childWaitingLock);
            ReleaseLock(_childWaitingLock);
            SignalCondition(_examRoomsCV, _examRoomsLock);
        }
        /*------------------------------------------
         // Stay @ ER and Wait for escort to XRAY
         //------------------------------------------*/
        else if (_patientTask == 1)
        {
            SignalCondition(_examRoomsCV, _examRoomsLock);
            Print("Parent [%d] waits for a Nurse to escort them to the Xray room.\n", sizeof("Parent [%d] waits for a Nurse to escort them to the Xray room.\n")/sizeof(char), 1, _paid);
            SetMV(esStack, myExamSheet*18+9, 0);
            SetMV(esStack, myExamSheet*18+11, 1);
            WaitCondition(_examRoomsCV, _examRoomsLock);
            AcquireLock(_childWaitingLock);
            SetMV(childWaitingTask, _paid, 7);
            WaitCondition(_childWaitingCV, _childWaitingLock);
            ReleaseLock(_childWaitingLock);
            SignalCondition(_examRoomsCV, _examRoomsLock);
            WaitCondition(_examRoomsCV, _examRoomsLock);
            _xrayImages = GetMV(esStack, myExamSheet*18+12);
            for (i =0; i< _xrayImages; i++) 
            {
                AcquireLock(_childWaitingLock);
                SetMV(childWaitingTask, _paid, 8);
                WaitCondition(_childWaitingCV, _childWaitingLock);
                ReleaseLock(_childWaitingLock);
                SignalCondition(_examRoomsCV, _examRoomsLock);
                WaitCondition(_examRoomsCV, _examRoomsLock);
                AcquireLock(_childWaitingLock);
                SetMV(childWaitingTask, _paid, 9);
                WaitCondition(_childWaitingCV, _childWaitingLock);
                ReleaseLock(_childWaitingLock);
                SignalCondition(_examRoomsCV, _examRoomsLock);
                WaitCondition(_examRoomsCV, _examRoomsLock);
            }
            SignalCondition(_examRoomsCV, _examRoomsLock);
            WaitCondition(_examRoomsCV, _examRoomsLock);
            _nid = GetMV(esStack, myExamSheet*18+6);
            Print("Parent [%d] is following Nurse [%d] to Examination Room [%d].\n", sizeof("Parent [%d] is following Nurse [%d] to Examination Room [%d].\n")/sizeof(char), 3, _paid*65536+_nid*256+erIndex);
            SignalCondition(_examRoomsCV, _examRoomsLock);
            Print("Parent [%d] has arrived at Examination Room [%d].\n", sizeof("Parent [%d] has arrived at Examination Room [%d].\n")/sizeof(char), 2, _paid*256+erIndex);
        }
        /*-------------------------------------------------
         // Stay @ ER and Wait for a Nurse to be give a shot
         //-------------------------------------------------*/
        else if (_patientTask == 3)
        {
            SetMV(esStack, myExamSheet*18+9, 0);
            AcquireLock(_childWaitingLock);
            SetMV(childWaitingTask, _paid, 10);
            WaitCondition(_childWaitingCV, _childWaitingLock);
            ReleaseLock(_childWaitingLock);
            Print("Parent [%d] says, \"Yes He is ready for the shot\".\n", sizeof("Parent [%d] says, \"Yes He is ready for the shot\".\n")/sizeof(char), 1, _paid);
            SignalCondition(_examRoomsCV, _examRoomsLock);
            WaitCondition(_examRoomsCV, _examRoomsLock);
            
        }
        else 
        {
            
        }
        ReleaseLock(_examRoomsLock);
        Yield();
    }
    SignalCondition(_examRoomsCV, _examRoomsLock);
    WaitCondition(_examRoomsCV, _examRoomsLock);
    SignalCondition(_examRoomsCV, _examRoomsLock);
    ReleaseLock(_examRoomsLock);
    /*-------------------------------------------------
     // Stay @ Cashier Waiting Room
     //-------------------------------------------------*/
    AcquireLock(cashWaitingLock);
    SetMV(cashWaitingCount, 0, GetMV(cashWaitingCount, 0)+1);
    Print("Parent [%d] enters the queue for Cashier.\n", sizeof("Parent [%d] enters the queue for Cashier.\n")/sizeof(char), 1, _paid);
    WaitCondition(cashWaitingCV, cashWaitingLock);
    ReleaseLock(cashWaitingLock);
    Print("Parent [%d] reaches the Cashier.\n", sizeof("Parent [%d] reaches the Cashier.\n")/sizeof(char), 1, _paid);
    AcquireLock(cashLock);
    SetMV(cashExamSheet, 0, myExamSheet);
    cashExamSheet = myExamSheet;
    Print("Parent [%d] hands over his examination sheet to the Cashier.\n", sizeof("Parent [%d] hands over his examination sheet to the Cashier.\n")/sizeof(char), 1, _paid);
    SignalCondition(cashCV, cashLock);
    WaitCondition(cashCV, cashLock);
    _totalAmount = GetMV(esStack, myExamSheet*18+17);
    Print("Parent [%d] pays the Cashier $%f.\n", sizeof("Parent [%d] pays the Cashier $%f.\n")/sizeof(char), 2, _paid*256+_totalAmount);
    SignalCondition(cashCV, cashLock);
    WaitCondition(cashCV, cashLock);
    Print("Parent [%d] receives a receipt for Child Patient[%d] from the Cashier.\n", sizeof("Parent [%d] receives a receipt for Child Patient[%d] from the Cashier.\n")/sizeof(char), 2, _paid*256+_paid);
    SignalCondition(cashCV, cashLock);
    Print("Parent [%d] leaves the doctor's office with Child Patient [%d].\n", sizeof("Parent [%d] leaves the doctor's office with Child Patient [%d].\n")/sizeof(char), 2, _paid*256+_paid);
    ReleaseLock(cashLock);
}

void main()
{
    doCreate();
    Parent();
    Exit(0);
}

