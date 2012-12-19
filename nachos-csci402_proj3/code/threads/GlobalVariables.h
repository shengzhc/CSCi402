//
//  GlobalVariables.h
//  Nachos
//
//  Created by Shengzhe Chen on 1/28/12.
//  Copyright 2012 University of Southern California. All rights reserved.
//

#ifndef Nachos_GlobalVariables_h
#define Nachos_GlobalVariables_h

#include "synch.h"
#include "system.h"
#include "copyright.h"

#define MAX_EXAMROOMS 5
#define MIN_EXAMROOMS 2
#define MAX_NURSES 5
#define MIN_NURSES 2
#define MAX_XRAY 2
#define MIN_XRAY 1
#define MAX_DOCTORS 3
#define MIN_DOCTORS 2
#define MAX_CHILD 1000

//--------------------------------
// Data Structure
//--------------------------------

enum WRNSTATUS {
    WRNFREE = 0,
    WRNBUSY = 1
};

enum WRNTASK {
    NONE = 0,
    GETFORMAP = 1,
    GETFORMCP = 2,
    GIVEFORM = 3
};

enum ERSTATUS {
    ERFREE = 0,
    ERBLOCKED = 1
};

enum SYMPTOM {
    SYMPAIN = 0,
    SYMNAUSEA = 1,
    SYMHEAR = 2,
    SYMNOTHING = 3
};

enum XRAYSTATUS {
    XRAYUNKNOWN = 0,
    XRAYREQUIRED = 1,
    XRAYUNREQUIRED = 2,
    XRAYHALFDONE = 3,
    XRAYDONE = 4
};

enum SHOTSTATUS {
    SHOTUNKNOWN = 0,
    SHOTREQUIRED = 1,
    SHOTUNREQUIRED = 2,
    SHOTDONE = 3
};

enum ESSTATUS {
    ESUNFINISHED = 0,
    ESFINISHED = 1
};

enum XRAYRESULT {
    NOTHING = 0,
    FRACTURE = 1,
    BREAK = 2,
    UNFILLED = 3
};

enum XRRSTATUS {
    XRRFREE = 0,
    XRRBUSY = 1,
    XRRDONE = 2
};

enum DOCSTATUS {
    DOCFREE = 0,
    DOCBUSY = 1
};

enum CASHSTATUS {
    CASHFREE = 0,
    CASHBUSY = 1
};

enum ESPROCESS {
    ESIDLE = 0,
    ESINPROCESS = 1
};

enum BROADCASTIDSTATUS {
    BROADFREE = 0,
    BROADBUSY = 1
};

enum PATIENTTYPE {
    ADULT = 0,
    CHILD = 1
};

enum PATIENTTASK {
    PATNOTHING = 0,
    PATGOXRAY = 1,
    PATBACKXRAY = 2,
    PATSHOT = 3,
    PATDIAGNOSE = 4,
    PATDIAGNOSEXRAY = 5,
    PATDONE = 6
};

enum CHILDTASK {
    CHILDNONE = 0,
    CHILDWR = 1,
    CHILDGOES = 2,
    FDUNDERGO = 3,
    INFORMXRAY = 4,
    INFORMSHOT = 5,
    FDDONE = 6,
    GETONTABLE = 7,
    XRAYMOVE = 8,
    XRAYONE = 9,
    NURSESHOT = 10,
    CLDIAXRAY = 11,
    CHILDOVER = 12
};

struct ExamSheet {
    int name;
    int age;
    int examSheetId;
    int erId;
    int temperature;
    int bloodPressure;
    int nurseId;
    int docId;
    
    PATIENTTYPE patientType;
    PATIENTTASK patientTask;
    
    SYMPTOM illness;
    XRAYSTATUS xrayStatus;
    int xrayImages;
    XRAYRESULT xrayResult;
    SHOTSTATUS shotStatus;
    ESSTATUS esStatus;
    ESPROCESS esProcess;
    float totalAmount;
};


//--------------------------------
// Locks
//--------------------------------

// Lock for waitingRoom
Lock *waitingRoomLock;

// Lock for WaitingRoomNurse
Lock *wrnLock;

// Lock for N & WRN interaction
Lock *nwrnLock;

// Lock for each child
Lock *childWaitingLock[MAX_CHILD];

// Lock for swaping the Parent ES to Child ES
Lock *childExamSheetLock;

// Lock for Examination-Rooms Status to Check the availiability
Lock *ersStatusLock;

// Lock that is used for N & P in calling
Lock *broadCastIdLock;

// Locks that are used for Locking each room, in another saying each patient
Lock *examRoomsLock[MAX_EXAMROOMS];

// Lock for Doctors 
Lock *doctWaitingLock[MAX_DOCTORS];

// Lock for XRayTechnician
Lock *xrayRoomsLock[MAX_XRAY];

// Lock for patients that waiting for escort outside the xrayroom
Lock *xrayWaitingLock;

// Lock that restricts number of entering cabinet 
Lock *cabinetLock;

// Lock that is used for interaction between C & N
Lock *cashWaitingLock;

// Lock that is used for swaping Patient ES to Cashier
Lock *cashLock;

// Lock that limits the access to the xray rooms status
Lock *xrayRoomsStatusLock;






//--------------------------------
// Condition
//--------------------------------

// Condition that is used for WRN calling Patient
Condition *waitingRoomCV;

// Condition that is used for WRN interaction with Patient
Condition *wrnCV;

// Conditions that are used for sequencing between Child and Parent
Condition *childWaitingCV[MAX_CHILD];

// Condition that is used for WRN and Nurse interaction, through WRN to invoke Nurse
Condition *nwrnCV;

// Condition that is used for nurses who are waiting in line for getting a form from WRN
Condition *nwrnWaitingCV;

// Condition that is used for patients who are waited for signaled by the Nurse who has gotten the completed form
Condition *broadCastIdCV;

// Condition that is used by Nurse to wait in line for the usage of broad casting
Condition *bcIdWaitingCV;

// Conditions that are used by P, D, and N in interaction with WallPackets
Condition *examRoomsCV[MAX_EXAMROOMS];

// Condtion that Doctors are waiting for calling by Nurse
Condition *doctWaitingCV[MAX_DOCTORS];

// Conditions that Control the sequence of X-Ray Technician Actions
Condition *xrayRoomsCV[MAX_XRAY];

// Condition that Controls the sequence of Nurse and Cashier
Condition *cashWaitingCV;

// Condition that is used for the sequence between Cashier and Patient
Condition *cashCV;


//--------------------------------
// Monitor Variables
//--------------------------------

// Count the number of Patients who are waiting for the WRN
int wrWaitingCount;

// Count the number of Nurses that are waiting to get a form from WRN
int nwrnWaitingCount = 0;

// Indicate the Status of WRN
WRNSTATUS wrnStatus;

// Contains the WaitingRoomNurse Task when Service the Patient
WRNTASK wrnTask;

// Contains the ExamSheet which is held by the WRN
ExamSheet *wrnExamSheet;

// Contains the whole Completed ExamSheet that Patients had submitted
List *wrnCompleteESList;

// Total number of examination rooms. Required to be inputed by user
int examRooms = 2;

// Recording each examroom status to be gotten by nurse to decide whether to get form
ERSTATUS examRoomsStatus[MAX_EXAMROOMS];

// Variable that is used by Nurse to broadcast the # of specific person, Each person has an unique esId
int broadCastId = -1;

// Variables array that Store the ExamSheet of a given room with a given patient 
ExamSheet *wallPackets[MAX_EXAMROOMS];

// Variable that indicate how many available doctors are @ office
DOCSTATUS docStatus[MAX_DOCTORS];

// Variable that is used between N and D to know which ER should the D go to
int ndocESNo[MAX_DOCTORS];

// Doctors Count
int docCount = 2;

// Variables that stores each XrayTechnician ES which points to patient ES
ExamSheet *xrayRoomsES[MAX_XRAY];

// Variables indicate the Status of xrayRooms
XRRSTATUS xrayRoomsStatus[MAX_XRAY];

// Patient ES that is waiting to be escorted by Nurse outside the XRay Room
List *xrayWaitingES;

// Variables that bound the quantity of xray rooms
int xrayRoomsCount = 2;

// Variable that counts the visits to cabinet
int cabinetCount = 0;

// Variable that Nurse passes the patient ES to cashier
ExamSheet *cashExamSheet;

// Variable that counts the Patients who are waiting for Cashier
int cashWaitingCount = 0;

// Variable that indicate the BUSY/FREE status of cashier, it is very useful here
BROADCASTIDSTATUS bcIdStatus;

// Variable that contains the Patient ID which the WRN is servicing right now
int wrnServiceId = -1;

// Variable that contains the ID of Nurse that WRN is servicing right now
int nwrnId = -1;

ExamSheet *nwrnExamSheet;

ExamSheet *childExamSheet = NULL;

CHILDTASK childWaitingTask[MAX_CHILD];

int patients=5, nurses=5, doctors=2, xrays=2, wrns=1, cashiers=1, children=5;

#endif

