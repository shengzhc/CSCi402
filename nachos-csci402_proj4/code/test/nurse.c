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
void Nurse()
{
    int holdingES;
    int _pid, _nid, _examRooms, _examRoomsStatus, _bcIdStatus, _examRoomsLock, _examRoomsCV, _broadCastId, _patientType, _doctWaitingLock, _docStatus, _front, _rear, _xrayStatus, _esProcess, _xrayRoomsLock, _xrayRoomsCount, _xrayRoomsStatus, _shotStatus, _patientTask, _xrayResult, _esStatus, _docCount, _doctWaitingCV, _xrayRoomsCV;
    int erIndex, docIndex;
    int flag = 0;
    int xrIndex;
    int times = 20;
    AcquireLock(nidLock);
    _nid = GetMV(nid, 0);
    _nid = _nid+1;
    SetMV(nid, 0, _nid);
    ReleaseLock(nidLock);
    _examRooms = GetMV(examRoomsLockMV, 0);
    while (1) 
    {
        /*---------------------------------------------------------------------
         // Check whether there is empty ER so they can go for ask a patient to come
         //------------------------------------------------------------------*/
        
        for (erIndex = 1; erIndex <= _examRooms; erIndex++) 
        {
            AcquireLock(ersStatusLock);
            _examRoomsStatus = GetMV(examRoomsStatus, erIndex);
            if (_examRoomsStatus == 0) 
            {
                SetMV(examRoomsStatus, erIndex, 1);
                ReleaseLock(ersStatusLock);
                AcquireLock(wrnLock);
                SetMV(nwrnWaitingCount, 0, GetMV(nwrnWaitingCount, 0)+1);
                WaitCondition(nwrnWaitingCV, wrnLock);
                ReleaseLock(wrnLock);
                AcquireLock(nwrnLock);
                SetMV(nwrnId, 0, _nid);
                SignalCondition(nwrnCV, nwrnLock);
                WaitCondition(nwrnCV, nwrnLock);
                holdingES = GetMV(nwrnExamSheet, 0);
                if (holdingES == -1) 
                {
                    SignalCondition(nwrnCV, nwrnLock);
                    ReleaseLock(nwrnLock);
                    AcquireLock(ersStatusLock);
                    SetMV(examRoomsStatus, erIndex, 0);
                    break;
                }
                /*---------------------------------------------------------
                 //nurse interacts with the wrn to get the examination sheet
                 //---------------------------------------------------------*/
                
                Print("Nurse [%d] tells Waiting Room Nurse to give a new examination sheet.\n", sizeof("Nurse [%d] tells Waiting Room Nurse to give a new examination sheet.\n")/sizeof(char), 1, _nid);
                
                SignalCondition(nwrnCV, nwrnLock);
                ReleaseLock(nwrnLock);
                SetMV(esStack, holdingES*18+3, erIndex);
                SetMV(esStack, holdingES*18+6, _nid);
                _pid = GetMV(esStack, holdingES*18+0);
                AcquireLock(broadCastIdLock);
                _bcIdStatus = GetMV(bcIdStatus, 0);
                if (_bcIdStatus == 0) {
                    SetMV(bcIdStatus, 0, 1);
                } 
                else {
                    WaitCondition(bcIdWaitingCV, broadCastIdLock);
                    SetMV(bcIdStatus, 0, 1);
                }
                _examRoomsLock = GetMV(examRoomsLockMV, erIndex);
                _examRoomsCV = GetMV(examRoomsCVMV, erIndex);
                AcquireLock(_examRoomsLock);
                _broadCastId = GetMV(esStack, holdingES*18+0);
                _patientType = GetMV(esStack, holdingES*18+8);
                if (_patientType == 0) {
                    SetMV(broadCastId, 0, _broadCastId);
                }
                else {
                    SetMV(broadCastId, 0, _broadCastId+30);
                }
                BroadcastCondition(broadCastIdCV, broadCastIdLock);
                ReleaseLock(broadCastIdLock);
                WaitCondition(_examRoomsCV, _examRoomsLock);
                /*----------------------------------------------------------
                 //nurse escort the patient to enter the examination room.
                 //---------------------------------------------------------*/
                _patientType = GetMV(esStack, holdingES*18+8);
                if (_patientType == 0) {
                    
                    Print("Nurse [%d] escorts Adult Patient [%d] to the examination room [%d].\n", sizeof("Nurse [%d] escorts Adult Patient [%d] to the examination room [%d].\n")/sizeof(char), 3, _nid*65536+_pid*256+erIndex);
                }
                else {
                    
                    Print("Nurse [%d] escorts Parent [%d] to the examination room [%d].\n", sizeof("Nurse [%d] escorts Parent [%d] to the examination room [%d].\n")/sizeof(char), 3, _nid*65536+_pid*256+erIndex);
                }
                SignalCondition(_examRoomsCV, _examRoomsLock);
                WaitCondition(_examRoomsCV, _examRoomsLock);
                /*------------------------------------------------------
                 //nurse takes the temperature and blood pressure in exmination room
                 //-------------------------------------------------------*/
                
                if (_patientType == 0)
                {
                    Print("Nurse [%d] takes the temperature and blood pressure of Adult Patient [%d].\n", sizeof("Nurse [%d] takes the temperature and blood pressure of Adult Patient [%d].\n")/sizeof(char), 2, _nid*256+_pid);
                }
                else
                {
                    
                    Print("Nurse [%d] takes the temperature and blood pressure of Parent [%d].\n", sizeof("Nurse [%d] takes the temperature and blood pressure of Parent [%d].\n")/sizeof(char), 2, _nid*256+_pid);
                }
                SetMV(esStack, holdingES*18+4, Random(10)+32);
                SetMV(esStack, holdingES*18+5, Random(80)+70);
                if (_patientType == 0)
                {
                    Print("Nurse [%d] ask Adult Patient [%d] \"What Symptoms do you have?\".\n", sizeof("Nurse [%d] ask Adult Patient [%d] \"What Symptoms do you have?\".\n")/sizeof(char), 2, _nid*256+_pid);
                }
                else
                {
                    Print("Nurse [%d] ask Parent [%d] \"What Symptoms do you have?\".\n", sizeof("Nurse [%d] ask Parent [%d] \"What Symptoms do you have?\".\n")/sizeof(char), 2, _nid*256+_pid);
                }
                SignalCondition(_examRoomsCV, _examRoomsLock);
                WaitCondition(_examRoomsCV, _examRoomsLock);
                /*------------------------------------------------------
                 //nurse records the information on examination sheet
                 //-------------------------------------------------------*/
                
                if (_patientType == 0)
                {
                    Print("Nurse [%d] writes all the information of Adult Patient [%d] in his examination sheet.\n", sizeof("Nurse [%d] writes all the information of Adult Patient [%d] in his examination sheet.\n")/sizeof(char), 2, _nid*256+_pid);
                }
                else
                {
                    Print("Nurse [%d] writes all the information of Parent [%d] in his examination sheet.\n", sizeof("Nurse [%d] writes all the information of Parent [%d] in his examination sheet.\n")/sizeof(char), 2, _nid*256+_pid);
                }
                /*------------------------------------------------------
                 //nurse inform the doctor to diagnose
                 //-------------------------------------------------------*/
                _docCount = GetMV(doctWaitingLockMV, 0);
                for (docIndex = 1; docIndex <= _docCount ; docIndex ++) 
                {
                    _doctWaitingLock = GetMV(doctWaitingLockMV, docIndex);
                    _doctWaitingCV = GetMV(doctWaitingCVMV, docIndex);
                    AcquireLock(_doctWaitingLock);
                    _docStatus = GetMV(docStatus, docIndex);
                    if (_docStatus == 0) 
                    { 
                        SetMV(docStatus, docIndex, 1);
                        SetMV(ndocESNo, docIndex, erIndex);
                        SignalCondition(_doctWaitingCV, _doctWaitingLock);
                        SetMV(esStack, holdingES*18+16, 1);
                        ReleaseLock(_doctWaitingLock);
                        if (_patientType == 0)
                        {
                            Print("Nurse [%d] informs Doctor [%d] that Adult Patient [%d] is waiting in the examination room [%d].\n", sizeof("Nurse [%d] informs Doctor [%d] that Adult Patient [%d] is waiting in the examination room [%d].\n")/sizeof(char), 4, _nid*16777216+docIndex*65536+_pid*256+erIndex);
                            Print("Nurse [%d] hands over to the Doctor [%d] the examination sheet of Adult Patient [%d].\n", sizeof("Nurse [%d] hands over to the Doctor [%d] the examination sheet of Adult Patient [%d].\n")/sizeof(char), 3, _nid*65536+docIndex*256+erIndex);
                        }
                        else
                        {
                            Print("Nurse [%d] informs Doctor [%d] that Child Patient [%d] is waiting in the examination room [%d].\n", sizeof("Nurse [%d] informs Doctor [%d] that Child Patient [%d] is waiting in the examination room [%d].\n")/sizeof(char), 4, _nid*16777216+docIndex*65536+_pid*256+erIndex);
                            Print("Nurse [%d] hands over to the Doctor [%d] the examination sheet of Child Patient [%d].\n", sizeof("Nurse [%d] hands over to the Doctor [%d] the examination sheet of Child Patient [%d].\n")/sizeof(char), 3, _nid*65536+docIndex*256+_pid);
                        }
                        break;
                        
                    } 
                    else 
                    {
                        ReleaseLock(_doctWaitingLock);
                    }
                }
                SignalCondition(_examRoomsCV, _examRoomsLock);
                SetMV(wallPackets, erIndex, holdingES);
                holdingES = -1;
                ReleaseLock(_examRoomsLock);
                break;                
            } 
            else 
            {
                ReleaseLock(ersStatusLock);
            }
        }
        Yield();
        
        
        /*-----------------------------------------------------
         // Check whether Patient is Already done in X-Ray Room
         //-----------------------------------------------------*/
        AcquireLock(xrayWaitingLock);
        _front = GetMV(xrayWaitingES, 0);
        _rear = GetMV(xrayWaitingES, 1);
        if (_front != 0) 
        {
            holdingES = GetMV(xrayWaitingES, _front);
            if (_front == _rear) {
                SetMV(xrayWaitingES, 0, 0);
                SetMV(xrayWaitingES, 1, 0);
            }
            else {
                SetMV(xrayWaitingES, 0, _front+1);
            }
            ReleaseLock(xrayWaitingLock);
            erIndex = GetMV(esStack, holdingES*18+3);
            _patientType = GetMV(esStack, holdingES*18+8);
            _pid = GetMV(esStack, holdingES*18+0);
            if (_patientType == 0)
            {
                Print("Nurse [%d] gets examination sheet for Adult Patient [%d] in Xray waiting room.\n", sizeof("Nurse [%d] gets examination sheet for Adult Patient [%d] in Xray waiting room.\n")/sizeof(char), 2, _nid*256+_pid);
            }
            else
            {
                Print("Nurse [%d] gets examination sheet for Parent [%d] in Xray waiting room.\n", sizeof("Nurse [%d] gets examination sheet for Parent [%d] in Xray waiting room.\n")/sizeof(char), 2, _nid*256+_pid);
            }
            _examRoomsLock = GetMV(examRoomsLockMV, erIndex);
            _examRoomsCV = GetMV(examRoomsCVMV, erIndex);
            AcquireLock(_examRoomsLock);
            SetMV(esStack, holdingES*18+6, _nid);
            SetMV(esStack, holdingES*18+9, 2);
            /*------------------------------------------------------
             //nurse escort the patient to examination room after xray
             //-------------------------------------------------------*/
            if (_patientType == 0)
            {
                Print("Nurse [%d] escorts Adult Patient [%d] to the examination room [%d].\n", sizeof("Nurse [%d] escorts Adult Patient [%d] to the examination room [%d].\n")/sizeof(char), 3, _nid*65536+_pid*256+erIndex);
            }
            else
            {
                Print("Nurse [%d] escorts Parent [%d] to the examination room [%d].\n", sizeof("Nurse [%d] escorts Parent [%d] to the examination room [%d].\n")/sizeof(char), 3, _nid*65536+_pid*256+erIndex);
            }
            SignalCondition(_examRoomsCV, _examRoomsLock);
            WaitCondition(_examRoomsCV, _examRoomsLock);
            SetMV(esStack, holdingES*18+16, 0);
            SetMV(wallPackets, erIndex, holdingES);
            ReleaseLock(_examRoomsLock);
            holdingES = -1;
        }    
        else 
        {
            ReleaseLock(xrayWaitingLock);
        }
        Yield();
        
        /*-------------------------------------
         // Nurse Checks the wallpacket
         //-------------------------------------*/
        for (erIndex = 1; erIndex<=_examRooms; erIndex++) 
        {
            _examRoomsLock = GetMV(examRoomsLockMV, erIndex);
            _examRoomsCV = GetMV(examRoomsCVMV, erIndex);
            AcquireLock(_examRoomsLock);
            holdingES = GetMV(wallPackets, erIndex);
            if (holdingES <= 0) 
            {
                ReleaseLock(_examRoomsLock);
                continue;
            }
            
            Print("Nurse [%d] checks the wall box of examination room [%d].\n", sizeof("Nurse [%d] checks the wall box of examination room [%d].\n")/sizeof(char), 2, _nid*256+erIndex);
            SetMV(wallPackets, erIndex, -1);
            /*---------------------------------------------
             // Patient needs the doctor to service 2nd
             //---------------------------------------------*/
            _xrayStatus = GetMV(esStack, holdingES*18+11);
            _esProcess = GetMV(esStack, holdingES*18+16);
            _patientType = GetMV(esStack, holdingES*18+8);
            _shotStatus = GetMV(esStack, holdingES*18+14);
            _pid = GetMV(esStack, holdingES*18+0);
            _xrayResult = GetMV(esStack, holdingES*18+13);
            if (_xrayStatus == 3 &&
                _esProcess != 1) 
            {  
                _docCount = GetMV(doctWaitingLockMV, 0);
                for (docIndex = 1; docIndex <= _docCount; docIndex ++) 
                {
                    _doctWaitingLock = GetMV(doctWaitingLockMV, docIndex);
                    _doctWaitingCV = GetMV(doctWaitingCVMV, docIndex);
                    AcquireLock(_doctWaitingLock);
                    _docStatus = GetMV(docStatus, docIndex);
                    _pid = GetMV(esStack, holdingES*18+0);
                    
                    if (_docStatus == 0) 
                    {
                        SetMV(docStatus, docIndex, 1);
                        SetMV(ndocESNo, docIndex, erIndex);
                        SetMV(esStack, holdingES*18+16, 1);
                        SignalCondition(_doctWaitingCV, _doctWaitingLock);
                        ReleaseLock(_doctWaitingLock);
                        Print("Nurse [%d] informs Doctor [%d] that Adult/Child Patient [%d] is waiting in the examination room [%d].\n", sizeof("Nurse [%d] informs Doctor [%d] that Adult/Child Patient [%d] is waiting in the examination room [%d].\n")/sizeof(char), 4, _nid*16777216+docIndex*65536+_pid*256+erIndex);
                        Print("Nurse [%d] hands over to the Doctor [%d] the examination sheet of Adult/Child Patient [%d].\n", sizeof("Nurse [%d] hands over to the Doctor [%d] the examination sheet of Adult/Child Patient [%d].\n")/sizeof(char), 3, _nid*65536+docIndex*256+_pid);
                        break;
                    } 
                    else 
                    {
                        ReleaseLock(_doctWaitingLock);
                    }
                }
                SetMV(wallPackets, erIndex, holdingES);
                holdingES = -1;
                ReleaseLock(_examRoomsLock);
            }
            /*------------------------------------------------
             // Patients need to be sent to XRAYTechnician
             //------------------------------------------------*/
            else if (_xrayStatus == 1 && 
                     _esProcess != 1) 
            {                       
                SetMV(esStack, holdingES*18+16, 1);
                AcquireLock(xrayRoomsStatusLock);
                _xrayRoomsCount = GetMV(xrayRoomsLockMV, 0);
                flag = 0;
                for (xrIndex = 1; xrIndex <= _xrayRoomsCount; xrIndex++) 
                {
                    _xrayRoomsStatus = GetMV(xrayRoomsStatus, xrIndex);
                    if (_xrayRoomsStatus == 0) 
                    {   
                        SetMV(xrayRoomsStatus, xrIndex, 1);
                        flag = 1;
                        break;
                    } 
                }
                ReleaseLock(xrayRoomsStatusLock);
                if (flag == 1) 
                {
                    SetMV(esStack, holdingES*18+9, 1);
                    WaitCondition(_examRoomsCV, _examRoomsLock);
                    _xrayRoomsLock = GetMV(xrayRoomsLockMV, xrIndex);
                    _xrayRoomsCV = GetMV(xrayRoomsCVMV, xrIndex);
                    AcquireLock(_xrayRoomsLock);
                    SetMV(xrayRoomsES, xrIndex, holdingES);
                    SignalCondition(_xrayRoomsCV, _xrayRoomsLock);
                    ReleaseLock(_xrayRoomsLock);
                    if (_patientType == 0)
                    {
                        Print("Nurse [%d] escorts Adult Patient [%d] to the X-ray room [%d].\n", sizeof("Nurse [%d] escorts Adult Patient [%d] to the X-ray room [%d].\n")/sizeof(char), 3, _nid*65536+_pid*256+xrIndex);
                        Print("Nurse [%d] informs X-Ray Technician [%d] about Adult Patient [%d] and hands over the examination sheet.\n", sizeof("Nurse [%d] informs X-Ray Technician [%d] about Adult Patient [%d] and hands over the examination sheet.\n")/sizeof(char), 3, _nid*65536+xrIndex*256+_pid);
                    }
                    else
                    {
                        Print("Nurse [%d] escorts Parent [%d] to the X-ray room [%d].\n", sizeof("Nurse [%d] escorts Parent [%d] to the X-ray room [%d].\n")/sizeof(char), 3, _nid*65536+_pid*256+xrIndex);
                        Print("Nurse [%d] informs X-Ray Technician [%d] about Child Patient [%d] and hands over the examination sheet.\n", sizeof("Nurse [%d] informs X-Ray Technician [%d] about Child Patient [%d] and hands over the examination sheet.\n")/sizeof(char), 3, _nid*65536+xrIndex*256+_pid);
                    }
                    Print("Nurse [%d] leaves the X-ray Room [%d].\n", sizeof("Nurse [%d] leaves the X-ray Room [%d].\n")/sizeof(char), 2, _nid*256+xrIndex);
                    holdingES = -1;
                    SetMV(wallPackets, erIndex, -1);
                    ReleaseLock(_examRoomsLock);
                }
                else 
                {
                    SetMV(esStack, holdingES*18+16, 0);
                    SetMV(wallPackets, erIndex, holdingES);
                    holdingES = -1;
                    ReleaseLock(_examRoomsLock);
                }
            } 
            else if (_shotStatus == 1 && 
                     _esProcess != 1 &&
                     _patientTask != 1) 
            {
                /*------------------------------
                 // Patient needs a shot
                 //------------------------------*/
                
                if (_patientType == 0) 
                {
                    Print("Nurse [%d] goes to supply cabinet to give to take medicine for Adult Patient [%d].\n", sizeof("Nurse [%d] goes to supply cabinet to give to take medicine for Adult Patient [%d].\n")/sizeof(char), 2, _nid*256+_pid);
                }
                else
                {
                    Print("Nurse [%d] goes to supply cabinet to give to take medicine for Parent [%d].\n", sizeof("Nurse [%d] goes to supply cabinet to give to take medicine for Parent [%d].\n")/sizeof(char), 2, _nid*256+_pid);
                }
                SetMV(esStack, holdingES*18+16, 1);
                AcquireLock(cabinetLock);
                SetMV(cabinetCount, 0, GetMV(cabinetCount, 0)+1);
                ReleaseLock(cabinetLock);
                SetMV(esStack, holdingES*18+9, 3);
                if (_patientType == 0)
                {
                    Print("Nurse [%d] asks Adult Patient [%d] \"Are you ready for the shot?\".\n", sizeof("Nurse [%d] asks Adult Patient [%d] \"Are you ready for the shot?\".\n")/sizeof(char), 2, _nid*256+_pid);
                }
                else
                {
                    Print("Nurse [%d] asks Parent [%d] \"Are you ready for the shot?\".\n", sizeof("Nurse [%d] asks Parent [%d] \"Are you ready for the shot?\".\n")/sizeof(char), 2, _nid*256+_pid);
                }
                WaitCondition(_examRoomsCV, _examRoomsLock);
                SetMV(esStack, holdingES*18+14, 3);
                SetMV(esStack, holdingES*18+16, 0);
                if (_patientType == 0)
                {
                    Print("Nurse [%d] tells Adult Patient [%d] \"Your shot is over.\".\n", sizeof("Nurse [%d] tells Adult Patient [%d] \"Your shot is over.\".\n")/sizeof(char), 2, _nid*256+_pid);
                }
                else
                {
                    Print("Nurse [%d] tells Parent [%d] \"Your shot is over.\".\n", sizeof("Nurse [%d] tells Parent [%d] \"Your shot is over.\".\n")/sizeof(char), 2, _nid*256+_pid);
                }
                SignalCondition(_examRoomsCV, _examRoomsLock);
                SetMV(wallPackets, erIndex, holdingES);
                holdingES = -1;
                ReleaseLock(_examRoomsLock);
                
            } 
            else if (_xrayStatus == 0 && 
                     _shotStatus == 0 &&
                     _esProcess != 1) 
            {
                /*-----------------------------------------------
                 // Patient needs a doctor for 1st diagnose
                 //-----------------------------------------------*/
                
                /*Print("D\n", sizeof("A\n")/sizeof(char), 0, 0);*/
                _docCount = GetMV(doctWaitingLockMV, 0);
                for (docIndex = 1; docIndex <= _docCount; docIndex ++) 
                {
                    _doctWaitingLock = GetMV(doctWaitingLockMV, docIndex);
                    _doctWaitingCV = GetMV(doctWaitingCVMV, docIndex);
                    AcquireLock(_doctWaitingLock);
                    _docStatus = GetMV(docStatus, docIndex);
                    if (_docStatus == 0) 
                    {
                        SetMV(docStatus, docIndex, 1);
                        SetMV(ndocESNo, docIndex, erIndex);
                        SignalCondition(_doctWaitingCV, _doctWaitingLock);
                        ReleaseLock(_doctWaitingLock);
                        if (_patientType == 0)
                        {
                            Print("Nurse [%d] informs Doctor [%d] that Adult Patient [%d] is waiting in the examination room [%d].\n", sizeof("Nurse [%d] informs Doctor [%d] that Adult Patient [%d] is waiting in the examination room [%d].\n")/sizeof(char), 4, _nid*16777216+docIndex*65536+_pid*256+erIndex);
                            Print("Nurse [%d] hands over to the Doctor [%d] the examination sheet of Adult Patient [%d].\n", sizeof("Nurse [%d] hands over to the Doctor [%d] the examination sheet of Adult Patient [%d].\n")/sizeof(char), 3, _nid*65536+docIndex*256+erIndex);
                        }
                        else
                        {
                            Print("Nurse [%d] informs Doctor [%d] that Child Patient [%d] is waiting in the examination room [%d].\n", sizeof("Nurse [%d] informs Doctor [%d] that Child Patient [%d] is waiting in the examination room [%d].\n")/sizeof(char), 4, _nid*16777216+docIndex*65536+_pid*256+erIndex);
                            Print("Nurse [%d] hands over to the Doctor [%d] the examination sheet of Child Patient [%d].\n", sizeof("Nurse [%d] hands over to the Doctor [%d] the examination sheet of Child Patient [%d].\n")/sizeof(char), 3, _nid*65536+docIndex*256+erIndex);
                        }
                        SetMV(esStack, holdingES*18+16, 1);
                        break;
                    } 
                    else 
                    {
                        ReleaseLock(_doctWaitingLock);
                    }
                }
                SetMV(wallPackets, erIndex, holdingES);
                holdingES = -1;
                ReleaseLock(_examRoomsLock);
            } 
            else 
            {
                
               /* Print("E[%d] [%d]\n", sizeof("E[%d] [%d]\n")/sizeof(char), 2, _xrayStatus*256+_shotStatus);*/

                if (_xrayStatus == 4 && 
                    _shotStatus == 3 &&
                    _xrayResult != 3)
                {
                    SetMV(esStack, holdingES*18+15, 1);
                }
                if (_xrayStatus == 2 ) 
                {   
                    if (_shotStatus == 3 || 
                        _shotStatus == 2)
                    {
                        SetMV(esStack, holdingES*18+15, 1);
                    }
                }
                if (_shotStatus == 2) 
                {   
                    if ((_xrayStatus == 4 &&
                         _xrayResult != 3) ||
                        _xrayStatus == 2) 
                    {
                        SetMV(esStack, holdingES*18+15, 1);
                    }
                }
                _esStatus = GetMV(esStack, holdingES*18+15);
                _esProcess = GetMV(esStack, holdingES*18+16);
                if (_esStatus == 1 && 
                    _esProcess != 1) 
                {   
                    SetMV(esStack, holdingES*18+16, 1);
                    SetMV(esStack, holdingES*18+9, 6);
                    WaitCondition(_examRoomsCV, _examRoomsLock);
                    if (_patientType == 0)
                    {
                        Print("Nurse [%d] escorts Adult Patient [%d] to Cashier.\n", sizeof("Nurse [%d] escorts Adult Patient [%d] to Cashier.\n")/sizeof(char), 2, _nid*256+_pid);
                    }
                    else
                    {
                        Print("Nurse [%d] escorts Parent [%d] to Cashier.\n", sizeof("Nurse [%d] escorts Parent [%d] to Cashier.\n")/sizeof(char), 2, _nid*256+_pid);
                    }
                    SetMV(esStack, holdingES*18+16, 0);
                    holdingES = -1;
                    SetMV(wallPackets, erIndex, -1);
                    SignalCondition(_examRoomsCV, _examRoomsLock);
                    WaitCondition(_examRoomsCV, _examRoomsLock);
                    ReleaseLock(_examRoomsLock);
                    AcquireLock(ersStatusLock);
                    SetMV(examRoomsStatus, erIndex, 0);
                    ReleaseLock(ersStatusLock);
                } 
                else 
                {
                    SetMV(wallPackets, erIndex, holdingES);
                    holdingES = -1;
                    ReleaseLock(_examRoomsLock);
                }
            }
            Yield();
        }
    }
}

void main()
{
    doCreate();
    Nurse();
    Exit(0);
}
