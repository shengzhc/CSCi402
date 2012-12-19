#ifndef _DOCTOROFFICE_H_
#define _DOCTOROFFICE_H_

#define MAX_EXAMROOMS 5
#define MIN_EXAMROOMS 2
#define MAX_NURSES 5
#define MIN_NURSES 2
#define MAX_XRAY 2
#define MIN_XRAY 1
#define MAX_DOCTORS 3
#define MIN_DOCTORS 2
#define MAX_CHILD 30
#define MAX_LISTELEMENT 100
#define NULL ((void*)0)
#define true 1
#define false 0


typedef enum {
    WRNFREE = 0,
    WRNBUSY = 1
}WRNSTATUS;
typedef enum {
    NONE = 0,
    GETFORMAP = 1,
    GETFORMCP = 2,
    GIVEFORM = 3
}WRNTASK;
typedef enum {
    ERFREE = 0,
    ERBLOCKED = 1
}ERSTATUS;
typedef enum {
    SYMPAIN = 0,
    SYMNAUSEA = 1,
    SYMHEAR = 2,
    SYMNOTHING = 3
}SYMPTOM;
typedef enum {
    XRAYUNKNOWN = 0,
    XRAYREQUIRED = 1,
    XRAYUNREQUIRED = 2,
    XRAYHALFDONE = 3,
    XRAYDONE = 4
}XRAYSTATUS;
typedef enum {
    SHOTUNKNOWN = 0,
    SHOTREQUIRED = 1,
    SHOTUNREQUIRED = 2,
    SHOTDONE = 3
}SHOTSTATUS;
typedef enum {
    ESUNFINISHED = 0,
    ESFINISHED = 1
}ESSTATUS;
typedef enum {
    NOTHING = 0,
    FRACTURE = 1,
    BREAK = 2,
    UNFILLED = 3
}XRAYRESULT;
typedef enum {
    XRRFREE = 0,
    XRRBUSY = 1,
    XRRDONE = 2
}XRRSTATUS;
typedef enum {
    DOCFREE = 0,
    DOCBUSY = 1
}DOCSTATUS;
typedef enum {
    CASHFREE = 0,
    CASHBUSY = 1
}CASHSTATUS;
typedef enum {
    ESIDLE = 0,
    ESINPROCESS = 1
}ESPROCESS;
typedef enum {
    BROADFREE = 0,
    BROADBUSY = 1
}BROADCASTIDSTATUS;
typedef enum {
    ADULT = 0,
    CHILD = 1
}PATIENTTYPE;
typedef enum {
    PATNOTHING = 0,
    PATGOXRAY = 1,
    PATBACKXRAY = 2,
    PATSHOT = 3,
    PATDIAGNOSE = 4,
    PATDIAGNOSEXRAY = 5,
    PATDONE = 6
}PATIENTTASK;
typedef enum {
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
}CHILDTASK;
typedef struct {
    int name; // 0
    int age; // 1
    int examSheetId; // 2
    int erId; // 3
    int temperature; // 4
    int bloodPressure; // 5
    int nurseId; // 6
    int docId; //7
    PATIENTTYPE patientType; //8
    PATIENTTASK patientTask;//9
    SYMPTOM illness;//10
    XRAYSTATUS xrayStatus;//11
    int xrayImages;//12
    XRAYRESULT xrayResult;//13
    SHOTSTATUS shotStatus;//14
    ESSTATUS esStatus;//15
    ESPROCESS esProcess;//16
    int totalAmount;//17
}ExamSheet;

typedef struct {
    int front;
    int rear;
    ExamSheet *queue[MAX_LISTELEMENT];
    int range;
}Queue;

int waitingRoomLock;
int wrnLock;
int nwrnLock;
int childWaitingLock[MAX_CHILD];
int childExamSheetLock;
int ersStatusLock;
int broadCastIdLock;
int examRoomsLock[MAX_EXAMROOMS];
int doctWaitingLock[MAX_DOCTORS];
int xrayRoomsLock[MAX_XRAY];
int xrayWaitingLock;
int cabinetLock;
int cashWaitingLock;
int cashLock;
int xrayRoomsStatusLock;
int waitingRoomCV;
int wrnCV;
int childWaitingCV[MAX_CHILD];
int nwrnCV;
int nwrnWaitingCV;
int broadCastIdCV;
int bcIdWaitingCV;
int examRoomsCV[MAX_EXAMROOMS];
int doctWaitingCV[MAX_DOCTORS];
int xrayRoomsCV[MAX_XRAY];
int cashWaitingCV;
int cashCV;
int wrWaitingCount;
int nwrnWaitingCount;
WRNSTATUS wrnStatus;
WRNTASK wrnTask;
ExamSheet *wrnExamSheet;
int examRooms;
ERSTATUS examRoomsStatus[MAX_EXAMROOMS];
int broadCastId;
ExamSheet *wallPackets[MAX_EXAMROOMS];
DOCSTATUS docStatus[MAX_DOCTORS];
int ndocESNo[MAX_DOCTORS];
int docCount;
ExamSheet *xrayRoomsES[MAX_XRAY];
XRRSTATUS xrayRoomsStatus[MAX_XRAY];
int xrayRoomsCount;
int cabinetCount;
ExamSheet *cashExamSheet;
int cashWaitingCount;
BROADCASTIDSTATUS bcIdStatus;
int wrnServiceId;
int nwrnId;
ExamSheet *nwrnExamSheet;
ExamSheet *childExamSheet;
CHILDTASK childWaitingTask[MAX_CHILD];
int patients, nurses, doctors, xrays, wrns, cashiers, children;
int glopatid, glodocid, glonurid, gloxrayid, glochid, gloparid;

ExamSheet esStack[MAX_LISTELEMENT];
Queue wrnCompleteESList;
Queue xrayWaitingES;

void Child();
void Parent();
void WaitingRoomNurse();
void Patient();
void Nurse();
void XRayTechnician();
void Doctor();
void Cashier();
void GlobalVariablesInit();
void GlobalVariablesClean();



#endif
