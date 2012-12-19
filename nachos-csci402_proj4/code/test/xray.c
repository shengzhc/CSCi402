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
void XRayTechnician() 
{
    
    int erIndex;
    int holdingES;
    int i;
    int _xid, _xrayRoomsLock, _xrayRoomsCV, _patientType, _pid, _examRoomsLock, _examRoomsCV, _xrayImages, _xrayResult, _front, _rear;
    AcquireLock(xidLock);
    _xid = GetMV(xid, 0);
    _xid = _xid+1;
    SetMV(xid, 0, _xid);
    ReleaseLock(xidLock);
    _xrayRoomsLock = GetMV(xrayRoomsLockMV, _xid);
    _xrayRoomsCV = GetMV(xrayRoomsCVMV, _xid);
    while (1) 
    {   
        /*------------------------------------------------------
         //xray technician enters the xray room and waits for nurse signal
         //-------------------------------------------------------*/

        AcquireLock(_xrayRoomsLock);
        WaitCondition(_xrayRoomsCV, _xrayRoomsLock);
        /*------------------------------------------------------
         //one patient comes to xray room and begins to take xray
         //-------------------------------------------------------*/
        holdingES = GetMV(xrayRoomsES, _xid);
        SetMV(xrayRoomsES, _xid, -1);
        ReleaseLock(_xrayRoomsLock);
        erIndex = GetMV(esStack, holdingES*18+3);
        _examRoomsLock = GetMV(examRoomsLockMV, erIndex);
        _examRoomsCV = GetMV(examRoomsCVMV, erIndex);
        AcquireLock(_examRoomsLock);
        _patientType = GetMV(esStack, holdingES*18+8);
        _pid = GetMV(esStack, holdingES*18+0);
        if (_patientType == 0) 
        {
            Print("Xray technician [%d] asks Adult Patient [%d] to get on the table.\n", sizeof("Xray technician [%d] asks Adult Patient [%d] to get on the table.\n")/sizeof(char), 3, _xid*65536+_pid*256+erIndex);
        }
        else
        {
            Print("Xray technician [%d] asks Child Patient [%d] to get on the table.\n", sizeof("Xray technician [%d] asks Child Patient [%d] to get on the table.\n")/sizeof(char), 2, _xid*256+_pid);
        }
        /*------------------------------------------------------
         //wait patient to prepare
         //-------------------------------------------------------*/
        SignalCondition(_examRoomsCV, _examRoomsLock);
        WaitCondition(_examRoomsCV, _examRoomsLock);
        /*------------------------------------------------------
         //take xray images repetablely
         //-------------------------------------------------------*/
        _xrayImages = GetMV(esStack, holdingES*18+12);
        for (i = 0; i<_xrayImages; i++) 
        {
            if (_patientType == 0)
            {
                Print("Xray Technician [%d] asks Adult Patient [%d] to move.\n", sizeof("Xray Technician [%d] asks Adult Patient [%d] to move.\n")/sizeof(char), 2, _xid*256+_pid);
            }
            else
            {
                Print("Xray Technician [%d] asks Child Patient [%d] to move.\n", sizeof("Xray Technician [%d] asks Child Patient [%d] to move.\n")/sizeof(char), 2, _xid*256+_pid);
            }
            SignalCondition(_examRoomsCV, _examRoomsLock);
            WaitCondition(_examRoomsCV, _examRoomsLock);
            if (_patientType == 0)
            {
                Print("Xray Technician [%d] takes an Xray Image of Adult Patient [%d].\n", sizeof("Xray Technician [%d] takes an Xray Image of Adult Patient [%d].\n")/sizeof(char), 2, _xid*256+_pid);
            }
            else
            {
                Print("Xray Technician [%d] takes an Xray Image of Child Patient [%d].\n", sizeof("Xray Technician [%d] takes an Xray Image of Child Patient [%d].\n")/sizeof(char), 2, _xid*256+_pid);
            }
            SignalCondition(_examRoomsCV, _examRoomsLock);
            WaitCondition(_examRoomsCV, _examRoomsLock);
        }
        SetMV(esStack, holdingES*18+11, 3);
        SetMV(esStack, holdingES*18+13, Random(3));
        /*------------------------------------------------------
         //xray writes the xray result on exmination sheet
         //-------------------------------------------------------*/
 
        _xrayResult = GetMV(esStack, holdingES*18+13);
        switch (_xrayResult) 
        {
            case 0:
                if (_patientType == 0)
                {
                    Print("Xray Technician [%d] records nothing on Adult Patient [%d]'s examination sheet.\n", sizeof("Xray Technician [%d] records nothing on Adult Patient [%d]'s examination sheet.\n")/sizeof(char), 2, _xid*256+_pid);
                }
                else
                {
                    Print("Xray Technician [%d] records nothing on Child Patient [%d]'s examination sheet.\n", sizeof("Xray Technician [%d] records nothing on Child Patient [%d]'s examination sheet.\n")/sizeof(char), 2, _xid*256+_pid);
                }
                break;
            case 1:
                if (_patientType == 0)
                {
                    Print("Xray Technician [%d] records compound fracture on Adult Patient [%d]'s examination sheet.\n", sizeof("Xray Technician [%d] records compound fracture on Adult Patient [%d]'s examination sheet.\n")/sizeof(char), 2, _xid*256+_pid);
                }
                else
                {
                    Print("Xray Technician [%d] records compound fracture on Child Patient [%d]'s examination sheet.\n", sizeof("Xray Technician [%d] records compound fracture on Child Patient [%d]'s examination sheet.\n")/sizeof(char), 2, _xid*256+_pid);
                }
                break;
                
            case 2:
                if (_patientType == 0)
                {
                    Print("Xray Technician [%d] records break on Adult Patient [%d]'s examination sheet.\n", sizeof("Xray Technician [%d] records break on Adult Patient [%d]'s examination sheet.\n")/sizeof(char), 2, _xid*256+_pid);
                }
                else
                {
                    Print("Xray Technician [%d] records break on Child Patient [%d]'s examination sheet.\n", sizeof("Xray Technician [%d] records break on Child Patient [%d]'s examination sheet.\n")/sizeof(char), 2, _xid*256+_pid);
                }
                break;
            default:
                break;
        }
        /*------------------------------------------------------
         //tells the patient to wait nurse in xray waiting room 
         //-------------------------------------------------------*/
        if (_patientType == 0)
        {
            Print("X-ray Technician [%d] tells Adult Patient [%d] to wait in Xray waiting room.\n", sizeof("X-ray Technician [%d] tells Adult Patient [%d] to wait in Xray waiting room.\n")/sizeof(char), 2, _xid*256+_pid);
        }
        else
        {
            Print("X-ray Technician [%d] tells Parent [%d] to wait in Xray waiting room.\n", sizeof("X-ray Technician [%d] tells Parent [%d] to wait in Xray waiting room.\n")/sizeof(char), 2, _xid*256+_pid);
        }
        SignalCondition(_examRoomsCV, _examRoomsLock);
        WaitCondition(_examRoomsCV, _examRoomsLock);
        AcquireLock(xrayWaitingLock);
        _front = GetMV(xrayWaitingES, 0);
        _rear = GetMV(xrayWaitingES, 1);
        if (_front == 0) {
            SetMV(xrayWaitingES, 0, 2);
            SetMV(xrayWaitingES, 1, 2);
            SetMV(xrayWaitingES, 2, holdingES);
        }
        else {
            SetMV(xrayWaitingES, _rear+1, holdingES);
            SetMV(xrayWaitingES, 1, _rear+1);
        }
        ReleaseLock(xrayWaitingLock);
        /*------------------------------------------------------
         //xray puts the patient's examination sheet into wall pocket for nurse to take away
         //-------------------------------------------------------*/
        if (_patientType == 0) 
        {
            Print("X-ray Technician [%d] puts Adult Patient [%d] in Xray waiting room wall pocket.\n", sizeof("X-ray Technician [%d] puts Adult Patient [%d] in Xray waiting room wall pocket.\n")/sizeof(char), 2, _xid*256+_pid);
        }
        else
        {
            Print("X-ray Technician [%d] puts Parent [%d] in Xray waiting room wall pocket.\n", sizeof("X-ray Technician [%d] puts Parent [%d] in Xray waiting room wall pocket.\n")/sizeof(char), 2, _xid*256+_pid);
        }
        holdingES = -1;
        ReleaseLock(_examRoomsLock);
        AcquireLock(xrayRoomsStatusLock);
        SetMV(xrayRoomsStatus, _xid, 0);
        ReleaseLock(xrayRoomsStatusLock);
        Yield();
    }
    Exit(0);
}
void main()
{
    doCreate();    
    XRayTechnician();
    
    Exit(0);
}