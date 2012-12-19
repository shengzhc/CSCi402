#include "syscall.h"
#include "DoctorOffice.h"

/* Insert Operation to Queue*/
void Insert(Queue *q, ExamSheet *es)
{
    q->queue[q->rear] = es;
    q->rear++;
    if (q->rear == q->range) {
        q->rear %= q->range;
    }
}
/* Remove Operation to Queue */
void Remove(Queue *q, ExamSheet **es)
{
    if (q->front != q->rear) {
        *es = q->queue[q->front];
        q->front++;
        if (q->front == q->range) {
            q->front %= q->range;
        }
    }
}
void Parent()
{
    ExamSheet *myExamSheet = NULL;
    int mp_Type = 1;
    int erIndex, i;
    int pid = gloparid++;
    Fork(Child);
    /*
    DEBUG('p', "Parent  [%d] has entered the Doctor's Office Waiting Room.\n", pid);
    */
    
     /*-----------------------------------
     // Parent Interacts with WRN
     //-----------------------------------*/
    
    Print("Parent  [%d] has entered the Doctor's Office Waiting Room.\n", sizeof("Parent  [%d] has entered the Doctor's Office Waiting Room.\n")/sizeof(char), 1, pid);
    AcquireLock(childWaitingLock[pid-1]);
    childWaitingTask[pid-1] = (CHILDTASK)CHILDWR;
    WaitCondition(childWaitingCV[pid-1], childWaitingLock[pid-1]);
    /*-----------------------------------
     // Parent Wait in the line of WR
     //-----------------------------------*/
    AcquireLock(waitingRoomLock);
    wrWaitingCount ++;
    /*
    DEBUG('p', "Parent [%d] gets in line of the Waiting Room Nurse to get registration form.\n", pid);
    */
    Print("Parent [%d] gets in line of the Waiting Room Nurse to get registration form.\n", sizeof("Parent [%d] gets in line of the Waiting Room Nurse to get registration form.\n")/sizeof(char), 1, pid);
    WaitCondition(waitingRoomCV, waitingRoomLock);
    /*-----------------------------------
    // Parent gets form from WR
    //-----------------------------------*/
    wrnStatus = (WRNSTATUS)WRNBUSY;
    ReleaseLock(waitingRoomLock);
    AcquireLock(wrnLock);
    wrnTask = GETFORMCP;
    wrnServiceId = pid;
    SignalCondition(wrnCV, wrnLock);
    WaitCondition(wrnCV, wrnLock);
    /*
    DEBUG('p', "Parent of child patient [%d] gets the form from the Waiting Room Nurse.\n", pid);
    */
    Print("Parent of child patient [%d] gets the form from the Waiting Room Nurse.\n", sizeof("Parent of child patient [%d] gets the form from the Waiting Room Nurse.\n")/sizeof(char), 1, pid);
    myExamSheet = wrnExamSheet;
    SignalCondition(wrnCV, wrnLock);
    ReleaseLock(wrnLock);
    /*-----------------------------------
    // Parent Fill the Form
    //-----------------------------------*/
    myExamSheet->name = pid;
    myExamSheet->age = Random(10) + 5;
    myExamSheet -> examSheetId = pid;
    myExamSheet -> patientType = (PATIENTTYPE)mp_Type;
    myExamSheet -> patientTask = (PATIENTTASK)PATNOTHING;
    myExamSheet -> illness = (SYMPTOM)SYMNOTHING;
    myExamSheet -> xrayStatus = (XRAYSTATUS)XRAYUNKNOWN;
    myExamSheet -> xrayImages = 0;
    myExamSheet -> xrayResult = (XRAYRESULT)UNFILLED;
    myExamSheet -> shotStatus = (SHOTSTATUS)SHOTUNKNOWN;
    myExamSheet -> esStatus = (ESSTATUS)ESUNFINISHED;
    myExamSheet -> esProcess = (ESPROCESS)ESIDLE;
    myExamSheet -> totalAmount = 0;
    Yield();
    
    AcquireLock(waitingRoomLock);
    wrWaitingCount ++;
    /*
    DEBUG('p', "Parent of child patient [%d] gets in line of the Waiting Room Nurse to submit the registration form.\n", pid);
    */
    Print("Parent of child patient [%d] gets in line of the Waiting Room Nurse to submit the registration form.\n", sizeof("Parent of child patient [%d] gets in line of the Waiting Room Nurse to submit the registration form.\n")/sizeof(char), 1, pid);
    WaitCondition(waitingRoomCV, waitingRoomLock);
    /*-----------------------------------
    // Parent submit the form
    //-----------------------------------*/
    ReleaseLock(waitingRoomLock);
    AcquireLock(wrnLock);
    wrnTask = GIVEFORM;
    wrnExamSheet = myExamSheet;
    SignalCondition(wrnCV, wrnLock);
    /*
    DEBUG('p', "Parent of child patient [%d] submits the filled form to the Waiting Room Nurse.\n", pid);
    */
    Print("Parent of child patient [%d] submits the filled form to the Waiting Room Nurse.\n", sizeof("Parent of child patient [%d] submits the filled form to the Waiting Room Nurse.\n")/sizeof(char), 1, pid);
    ReleaseLock(wrnLock);
    /*---------------------------------------------
    // Parent Wait for being Called by Nurse
    //--------------------------------------------- */
    AcquireLock(broadCastIdLock);
    while (broadCastId != pid) 
    {
        WaitCondition(broadCastIdCV, broadCastIdLock);
    }
    bcIdStatus = (BROADCASTIDSTATUS)BROADFREE;
    SignalCondition(bcIdWaitingCV, broadCastIdLock);
    ReleaseLock(broadCastIdLock);
    erIndex = myExamSheet -> erId;
    AcquireLock(childWaitingLock[pid-1]);
    childWaitingTask[pid-1] = (CHILDTASK)CHILDGOES;
    AcquireLock(childExamSheetLock);
    childExamSheet = myExamSheet;
    /*
    DEBUG('p', "Parent [%d] asks Child Patient [%d] to follow them.\n", pid, pid);
    */
    Print("Parent [%d] asks Child Patient [%d] to follow them.\n", sizeof("Parent [%d] asks Child Patient [%d] to follow them.\n")/sizeof(char), 2, pid*256+pid);
    WaitCondition(childWaitingCV[pid-1], childWaitingLock[pid-1]);
    ReleaseLock(childExamSheetLock);
    ReleaseLock(childWaitingLock[pid-1]);
    AcquireLock(examRoomsLock[erIndex]);
    SignalCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
    WaitCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
    /*
    DEBUG('p', "Parent [%d] is following Nurse [%d] to Examination Room [%d].\n", pid, myExamSheet -> nurseId, erIndex+1);
    */
    Print("Parent [%d] is following Nurse [%d] to Examination Room [%d].\n", sizeof("Parent [%d] is following Nurse [%d] to Examination Room [%d].\n")/sizeof(char), 3, pid*65536+myExamSheet->nurseId*256+(erIndex+1));
    SignalCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
    /*
    DEBUG('p', "Parent [%d] has arrived at Examination Room [%d].\n", pid, erIndex+1);
    */
    Print("Parent [%d] has arrived at Examination Room [%d].\n", sizeof("Parent [%d] has arrived at Examination Room [%d].\n")/sizeof(char), 2, pid*256+(erIndex+1));
    WaitCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
    /*-----------------------------------
     // Parent tell the SYMPTOM to Nurse
     //-----------------------------------*/
    myExamSheet -> illness = (SYMPTOM)(Random(3));
    switch (myExamSheet -> illness) 
    {
        case (SYMPTOM)SYMPAIN:
            /*
            DEBUG('p', "Parent [%d] says, \"His symptoms are Pain\".\n", pid);
            */
            Print("Parent [%d] says, \"His symptoms are Pain\".\n", sizeof("Parent [%d] says, \"His symptoms are Pain\".\n")/sizeof(char), 1, pid);
            break;
        case (SYMPTOM)SYMNAUSEA:
            /*
            DEBUG('p', "Parent [%d] says, \"His symptoms are Nausea\".\n", pid);*/
            Print("Parent [%d] says, \"His symptoms are Nausea\".\n", sizeof("Parent [%d] says, \"His symptoms are Nausea\".\n")/sizeof(char), 1, pid);
            break;
        case (SYMPTOM)SYMHEAR:
            /*
            DEBUG('p', "Parent [%d] says, \"His symptoms are Hear Alien Voices\".\n", pid);
            */
            Print("Parent [%d] says, \"His symptoms are Hear Alien Voices\".\n", sizeof("Parent [%d] says, \"His symptoms are Hear Alien Voices\".\n")/sizeof(char), 1, pid);
            break;
        default:
            break;
    }
    SignalCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
    WaitCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
    ReleaseLock(examRoomsLock[erIndex]);
    while (true) 
    {
        AcquireLock(examRoomsLock[erIndex]);
        /*-----------------------------------
         // All things are Done.
         //-----------------------------------*/
        
        if (myExamSheet -> patientTask == (PATIENTTASK)PATDONE) 
        {
            AcquireLock(childWaitingLock[pid-1]);
            childWaitingTask[pid-1] = (CHILDTASK)CHILDOVER;
            WaitCondition(childWaitingCV[pid-1], childWaitingLock[pid-1]);
            ReleaseLock(childWaitingLock[pid-1]);
            break;            
        } 
        /*-----------------------------------------------------
        // Stay @ ER and Wait for the 2nd Diagnose after X-ray
        //-----------------------------------------------------*/
        else if (myExamSheet -> patientTask == (PATIENTTASK)PATDIAGNOSEXRAY)
        {
            myExamSheet -> patientTask = (PATIENTTASK)PATNOTHING;
            AcquireLock(childWaitingLock[pid-1]);
            childWaitingTask[pid-1] = (CHILDTASK)CLDIAXRAY;
            WaitCondition(childWaitingCV[pid-1], childWaitingLock[pid-1]);
            ReleaseLock(childWaitingLock[pid-1]);
            SignalCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
            WaitCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
        }
        /*------------------------------------------
        // Stay @ ER and Wait for the 1st Diagnose  
        //------------------------------------------*/
        else if (myExamSheet -> patientTask == (PATIENTTASK)PATDIAGNOSE)
        {
            myExamSheet -> patientTask = (PATIENTTASK)PATNOTHING;
            AcquireLock(childWaitingLock[pid-1]);
            childWaitingTask[pid-1] = (CHILDTASK)FDUNDERGO;
            WaitCondition(childWaitingCV[pid-1], childWaitingLock[pid-1]);
            ReleaseLock(childWaitingLock[pid-1]);
            SignalCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
            WaitCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
            if (myExamSheet -> xrayImages > 0) 
            {
                AcquireLock(childWaitingLock[pid-1]);
                childWaitingTask[pid-1] = (CHILDTASK)INFORMXRAY;
                WaitCondition(childWaitingCV[pid-1], childWaitingLock[pid-1]);
                ReleaseLock(childWaitingLock[pid-1]);
                SignalCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
                WaitCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
            }
            if (myExamSheet -> shotStatus == (SHOTSTATUS)SHOTREQUIRED)
            {
                AcquireLock(childWaitingLock[pid-1]);
                childWaitingTask[pid-1] = (CHILDTASK)INFORMSHOT;
                WaitCondition(childWaitingCV[pid-1], childWaitingLock[pid-1]);
                ReleaseLock(childWaitingLock[pid-1]);
                SignalCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
                WaitCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
            }
            AcquireLock(childWaitingLock[pid-1]);
            childWaitingTask[pid-1] = (CHILDTASK)FDDONE;
            WaitCondition(childWaitingCV[pid-1], childWaitingLock[pid-1]);
            ReleaseLock(childWaitingLock[pid-1]);
            SignalCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
        }
        /*------------------------------------------
        // Stay @ ER and Wait for escort to XRAY
        //------------------------------------------*/
        else if ((myExamSheet -> patientTask == (PATIENTTASK)PATGOXRAY))
        {
            /*
            DEBUG('p', "Parent [%d] waits for a Nurse to escort them to the Xray room.\n", pid);
            */
            Print("Parent [%d] waits for a Nurse to escort them to the Xray room.\n", sizeof("Parent [%d] waits for a Nurse to escort them to the Xray room.\n")/sizeof(char), 1, pid);
            myExamSheet -> patientTask = (PATIENTTASK)PATNOTHING;
            myExamSheet -> xrayStatus = (XRAYSTATUS)XRAYREQUIRED;
            WaitCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
            AcquireLock(childWaitingLock[pid-1]);
            childWaitingTask[pid-1] = (CHILDTASK)GETONTABLE;
            WaitCondition(childWaitingCV[pid-1], childWaitingLock[pid-1]);
            ReleaseLock(childWaitingLock[pid-1]);
            SignalCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
            WaitCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
            for (i =0; i< myExamSheet -> xrayImages; i++) 
            {
                AcquireLock(childWaitingLock[pid-1]);
                childWaitingTask[pid-1] = (CHILDTASK)XRAYMOVE;
                WaitCondition(childWaitingCV[pid-1], childWaitingLock[pid-1]);
                ReleaseLock(childWaitingLock[pid-1]);
                SignalCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
                WaitCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
                AcquireLock(childWaitingLock[pid-1]);
                childWaitingTask[pid-1] = (CHILDTASK)XRAYONE;
                WaitCondition(childWaitingCV[pid-1], childWaitingLock[pid-1]);
                ReleaseLock(childWaitingLock[pid-1]);
                SignalCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
                WaitCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
            }
            SignalCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
            WaitCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
            /*
            DEBUG('p', "Parent [%d] is following Nurse [%d] to Examination Room [%d].\n", pid, myExamSheet -> nurseId, erIndex+1);
            */
            Print("Parent [%d] is following Nurse [%d] to Examination Room [%d].\n", sizeof("Parent [%d] is following Nurse [%d] to Examination Room [%d].\n")/sizeof(char), 3, pid*65536+myExamSheet->nurseId*256+(erIndex+1));
            SignalCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
            /*
            DEBUG('p', "Parent [%d] has arrived at Examination Room [%d].\n", pid, erIndex+1);
            */
            Print("Parent [%d] has arrived at Examination Room [%d].\n", sizeof("Parent [%d] has arrived at Examination Room [%d].\n")/sizeof(char), 2, pid*256+(erIndex+1));
        }
        /*-------------------------------------------------
        // Stay @ ER and Wait for a Nurse to be give a shot
        //-------------------------------------------------*/
        else if (myExamSheet -> patientTask == (PATIENTTASK)PATSHOT)
        {
            myExamSheet -> patientTask = (PATIENTTASK)PATNOTHING;
            AcquireLock(childWaitingLock[pid-1]);
            childWaitingTask[pid-1] = (CHILDTASK)NURSESHOT;
            WaitCondition(childWaitingCV[pid-1], childWaitingLock[pid-1]);
            ReleaseLock(childWaitingLock[pid-1]);
            /*
            DEBUG('p', "Parent [%d] says, \"Yes He is ready for the shot\".\n", pid);
            */
            Print("Parent [%d] says, \"Yes He is ready for the shot\".\n", sizeof("Parent [%d] says, \"Yes He is ready for the shot\".\n")/sizeof(char), 1, pid);
            SignalCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
            WaitCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
            
        }
        else 
        {
            
        }
        ReleaseLock(examRoomsLock[erIndex]);
        Yield();
    }
    SignalCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
    WaitCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
    SignalCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
    ReleaseLock(examRoomsLock[erIndex]);
    /*-------------------------------------------------
    // Stay @ Cashier Waiting Room
    //-------------------------------------------------*/
    AcquireLock(cashWaitingLock);
    cashWaitingCount ++;
    /*
    DEBUG('p', "Parent [%d] enters the queue for Cashier.\n", pid);
    */
    Print("Parent [%d] enters the queue for Cashier.\n", sizeof("Parent [%d] enters the queue for Cashier.\n")/sizeof(char), 1, pid);
    WaitCondition(cashWaitingCV, cashWaitingLock);
    ReleaseLock(cashWaitingLock);
    /*
    DEBUG('p', "Parent [%d] reaches the Cashier.\n", pid);
    */
    Print("Parent [%d] reaches the Cashier.\n", sizeof("Parent [%d] reaches the Cashier.\n")/sizeof(char), 1, pid);
    AcquireLock(cashLock);
    cashExamSheet = myExamSheet;
    /*
    DEBUG('p', "Parent [%d] hands over his examination sheet to the Cashier.\n", pid);
    */
    Print("Parent [%d] hands over his examination sheet to the Cashier.\n", sizeof("Parent [%d] hands over his examination sheet to the Cashier.\n")/sizeof(char), 1, pid);
    SignalCondition(cashCV, cashLock);
    WaitCondition(cashCV, cashLock);
    /*
    DEBUG('p', "Parent [%d] pays the Cashier $%f.\n", pid, myExamSheet -> totalAmount);
    */
    Print("Parent [%d] pays the Cashier $%f.\n", sizeof("Parent [%d] pays the Cashier $%f.\n")/sizeof(char), 2, pid*256+myExamSheet->totalAmount);
    SignalCondition(cashCV, cashLock);
    WaitCondition(cashCV, cashLock);
    /*
    DEBUG('p', "Parent [%d] receives a receipt for Child Patient[%d] from the Cashier.\n", pid, pid);*/
    Print("Parent [%d] receives a receipt for Child Patient[%d] from the Cashier.\n", sizeof("Parent [%d] receives a receipt for Child Patient[%d] from the Cashier.\n")/sizeof(char), 2, pid*256+pid);
    SignalCondition(cashCV, cashLock);
    /*
    DEBUG('p', "Parent [%d] leaves the doctor's office with Child Patient [%d].\n", pid, pid);*/
    Print("Parent [%d] leaves the doctor's office with Child Patient [%d].\n", sizeof("Parent [%d] leaves the doctor's office with Child Patient [%d].\n")/sizeof(char), 2, pid*256+pid);
    ReleaseLock(cashLock);
    Exit(0);
}
void Child()
{
    
    ExamSheet *myExamSheet = NULL;
    int pid = glochid++;
    while (true) 
    {
        AcquireLock(childWaitingLock[pid-1]);
        /*-------------------------------------------------
         // Child is informed to Enter the WR
         //-------------------------------------------------*/
        if (childWaitingTask[pid-1] == (CHILDTASK)CHILDWR)
        {
            childWaitingTask[pid-1] = (CHILDTASK)CHILDNONE;
            /*
             DEBUG('h', "Child Patient [%d] has entered the Doctor's Office Waiting Room with Parent [%d].\n", pid, pid);
             */
            Print("Child Patient [%d] has entered the Doctor's Office Waiting Room with Parent [%d].\n", sizeof("Child Patient [%d] has entered the Doctor's Office Waiting Room with Parent [%d].\n")/sizeof(char), 2, pid*256+pid);
            SignalCondition(childWaitingCV[pid-1], childWaitingLock[pid-1]);
            ReleaseLock(childWaitingLock[pid-1]);
        }
        /*------------------------------
         // Child is informed to Follow 
         //------------------------------*/
        else if (childWaitingTask[pid-1] == (CHILDTASK)CHILDGOES)
        {
            childWaitingTask[pid-1] = (CHILDTASK)CHILDNONE;
            myExamSheet = childExamSheet;
            SignalCondition(childWaitingCV[pid-1], childWaitingLock[pid-1]);
            ReleaseLock(childWaitingLock[pid-1]);
        }
        /*-------------------------------------------------------------
         // Child is informed that he/she is undergoing the 1st diagnose
         //-------------------------------------------------------------*/
        else if (childWaitingTask[pid-1] == (CHILDTASK)FDUNDERGO)
        {
            childWaitingTask[pid-1]  = (CHILDTASK)CHILDNONE;
            SignalCondition(childWaitingCV[pid-1], childWaitingLock[pid-1]);
            ReleaseLock(childWaitingLock[pid-1]);
        }
        /*-------------------------------------------------------------
         // Child is informed that he/she should take X-Ray
         //-------------------------------------------------------------*/
        else if (childWaitingTask[pid-1] == (CHILDTASK)INFORMXRAY)
        {
            childWaitingTask[pid-1] = (CHILDTASK)CHILDNONE;
            /*
             DEBUG('h', "Child Patient [%d] has been informed by Doctor [%d] that he needs an Xray.\n", pid, myExamSheet -> docId);
             */
            Print("Child Patient [%d] has been informed by Doctor [%d] that he needs an Xray.\n", sizeof("Child Patient [%d] has been informed by Doctor [%d] that he needs an Xray.\n")/sizeof(char), 2, pid*256+myExamSheet->docId);
            SignalCondition(childWaitingCV[pid-1], childWaitingLock[pid-1]);
            ReleaseLock(childWaitingLock[pid-1]);
        }
        /*-------------------------------------------------------------
         // Child is informed that he/she should take a shot
         //-------------------------------------------------------------*/
        else if (childWaitingTask[pid-1] == (CHILDTASK)INFORMSHOT)
        {
            childWaitingTask[pid-1] = (CHILDTASK)CHILDNONE;
            /*
             DEBUG('h', "Child Patient [%d] has been informed by Doctor [%d] that he will be administered a shot.\n", pid, myExamSheet -> docId);
             */
            Print("Child Patient [%d] has been informed by Doctor [%d] that he will be administered a shot.\n", sizeof("Child Patient [%d] has been informed by Doctor [%d] that he will be administered a shot.\n")/sizeof(char), 2, pid*256+myExamSheet->docId);
            SignalCondition(childWaitingCV[pid-1], childWaitingLock[pid-1]);
            ReleaseLock(childWaitingLock[pid-1]);
        }
        /*-------------------------------------------------------------
         // Child is informed that 1st diagnose is done
         //-------------------------------------------------------------*/
        else if (childWaitingTask[pid-1] == (CHILDTASK)FDDONE)
        {
            childWaitingTask[pid-1] = (CHILDTASK)CHILDNONE;
            /*
             DEBUG('h', "Child Patient [%d] has been diagnosed by Doctor [%d].\n", pid, myExamSheet -> docId);
             */
            Print("Child Patient [%d] has been diagnosed by Doctor [%d].\n", sizeof("Child Patient [%d] has been diagnosed by Doctor [%d].\n")/sizeof(char), 2, pid*256+myExamSheet->docId);
            SignalCondition(childWaitingCV[pid-1], childWaitingLock[pid-1]);
            ReleaseLock(childWaitingLock[pid-1]);
        }
        /*-------------------------------------------------------------
         // Child is informed that he/she should get to the  x-ray table
         //-------------------------------------------------------------*/
        
        else if (childWaitingTask[pid-1] == (CHILDTASK)GETONTABLE)
        {
            childWaitingTask[pid-1] = (CHILDTASK)CHILDNONE;
            /*
             DEBUG('h', "Child Patient [%d] gets on the table.\n", pid);
             */
            Print("Child Patient [%d] gets on the table.\n", sizeof("Child Patient [%d] gets on the table.\n")/sizeof(char), 1, pid);
            SignalCondition(childWaitingCV[pid-1], childWaitingLock[pid-1]);
            ReleaseLock(childWaitingLock[pid-1]);
        }
        /*-------------------------------------------------------------
         // Child is informed that he/she should move to take x-ray
         //-------------------------------------------------------------*/
        else if (childWaitingTask[pid-1] == (CHILDTASK)XRAYMOVE)
        {
            childWaitingTask[pid-1] = (CHILDTASK)CHILDNONE;
            /*
             DEBUG('h', "Child Patienht [%d] moves for the next Xray.\n", pid);
             */
            Print("Child Patienht [%d] moves for the next Xray.\n", sizeof("Child Patienht [%d] moves for the next Xray.\n")/sizeof(char), 1, pid);
            SignalCondition(childWaitingCV[pid-1], childWaitingLock[pid-1]);
            ReleaseLock(childWaitingLock[pid-1]);
        } 
        /*-------------------------------------------------------------
         // Child is informed that he/she has taken a x-ray image
         //-------------------------------------------------------------*/
        else if (childWaitingTask[pid-1] == (CHILDTASK)XRAYONE)
        {
            childWaitingTask[pid-1] = (CHILDTASK)CHILDNONE;
            /*
             DEBUG('h', "Child Patient [%d] has been asked to take an Xray.\n", pid);
             */
            Print("Child Patient [%d] has been asked to take an Xray.\n", sizeof("Child Patient [%d] has been asked to take an Xray.\n")/sizeof(char), 1, pid);
            SignalCondition(childWaitingCV[pid-1], childWaitingLock[pid-1]);
            ReleaseLock(childWaitingLock[pid-1]);
        }
        /*-------------------------------------------------------------
         // Child is informed that nures is coming for a shot
         //-------------------------------------------------------------*/
        else if (childWaitingTask[pid-1] == (CHILDTASK)NURSESHOT)
        {
            childWaitingTask[pid-1] = (CHILDTASK)CHILDNONE;
            SignalCondition(childWaitingCV[pid-1], childWaitingLock[pid-1]);
            ReleaseLock(childWaitingLock[pid-1]);
        }
        /*-------------------------------------------------------------
         // Child is informed that he/she is diagnosed by doctor 2nd time
         //-------------------------------------------------------------*/
        else if (childWaitingTask[pid-1] == (CHILDTASK)CLDIAXRAY)
        {
            childWaitingTask[pid-1] = (CHILDTASK)CHILDNONE;
            /*
             DEBUG('h', "Child Patient [%d] has been diagnosed by Doctor [%d].\n", pid, myExamSheet -> docId);
             */
            Print("Child Patient [%d] has been diagnosed by Doctor [%d].\n", sizeof("Child Patient [%d] has been diagnosed by Doctor [%d].\n")/sizeof(char), 2, pid*256+myExamSheet->docId);
            SignalCondition(childWaitingCV[pid-1], childWaitingLock[pid-1]);
            ReleaseLock(childWaitingLock[pid-1]);
        }
        /*-------------------------------------------------------------
         // Child is informed that he/she should leave the hospital
         //-------------------------------------------------------------*/
        
        else if (childWaitingTask[pid-1] == (CHILDTASK)CHILDOVER)
        {
            childWaitingTask[pid-1] = (CHILDTASK)CHILDNONE;
            SignalCondition(childWaitingCV[pid-1], childWaitingLock[pid-1]);
            ReleaseLock(childWaitingLock[pid-1]);
            break;
        }        
        else
        {
            ReleaseLock(childWaitingLock[pid-1]);
        }
        Yield();
    }
    Exit(0);
}
void WaitingRoomNurse()
{
    int pid = 1;
    int times = 1000;
    while (times-- > 0) {
        /*--------------------------------------------------------      
         // WRN Service the Patient
         //--------------------------------------------------------*/
        
        AcquireLock(waitingRoomLock);
        wrnStatus = (WRNSTATUS)WRNFREE;
        if (wrWaitingCount > 0) 
        {
            wrWaitingCount --;
            SignalCondition(waitingRoomCV, waitingRoomLock);
            wrnStatus = (WRNSTATUS)WRNBUSY;
            AcquireLock(wrnLock);
            ReleaseLock(waitingRoomLock);
            WaitCondition(wrnCV, wrnLock);
            /*------------------------------------------
             // WRN gives the form to the Adult
             //------------------------------------------*/
            if (wrnTask == (WRNTASK)GETFORMAP) 
            {
                wrnExamSheet = &esStack[wrnServiceId];
                
                /*DEBUG('w', "Waiting Room nurse gives a form to Adult patient [%d].\n", wrnServiceId);
                 */
                Print("Waiting Room nurse gives a form to Adult patient [%d].\n", sizeof("Waiting Room nurse gives a form to Adult patient [%d].\n")/sizeof(char), 1, wrnServiceId);
                wrnTask = (WRNTASK)NONE;
                wrnServiceId = -1;
                SignalCondition(wrnCV, wrnLock);
                WaitCondition(wrnCV, wrnLock);
                ReleaseLock(wrnLock);
                Yield();
            }
            /*------------------------------------------
             // WRN gives the form to the Parent
             //------------------------------------------*/
            else if (wrnTask == GETFORMCP)
            {
                
                wrnExamSheet = &esStack[wrnServiceId];
                
                /*DEBUG('w', "Waiting Room nurse gives a form to the Parent of Child patient [%d].\n", wrnServiceId);
                 */
                Print("Waiting Room nurse gives a form to the Parent of Child patient [%d].\n", sizeof("Waiting Room nurse gives a form to the Parent of Child patient [%d].\n")/sizeof(char), 1, wrnServiceId);
                wrnServiceId = -1;
                wrnTask = (WRNTASK)NONE;
                SignalCondition(wrnCV, wrnLock);
                ReleaseLock(wrnLock);
                Yield();
            }
            /*------------------------------------------
             // WRN receives the form from patient
             //------------------------------------------*/
            else if (wrnTask == GIVEFORM) 
            {
                if (wrnExamSheet->patientType == (PATIENTTYPE)ADULT)
                {
                    /*DEBUG('w', "Waiting Room nurse accepts the form from Adult Patient [%d] and age [%d].\n", wrnExamSheet -> name, wrnExamSheet -> age);*/
                    Print("Waiting Room nurse accepts the form from Adult Patient [%d] and age [%d].\n", sizeof("Waiting Room nurse accepts the form from Adult Patient [%d] and age [%d].\n")/sizeof(char), 2, wrnExamSheet->name*256+wrnExamSheet->age);
                }
                else
                {/*
                  DEBUG('w', "Waiting Room nurse accepts the form from Parent with name [%d] and age [%d].\n", wrnExamSheet -> name, wrnExamSheet -> age);*/
                    Print("Waiting Room nurse accepts the form from Parent with name [%d] and age [%d].\n", sizeof("Waiting Room nurse accepts the form from Parent with name [%d] and age [%d].\n")/sizeof(char), 2, wrnExamSheet->name*256+wrnExamSheet->age);
                }
                Insert(&wrnCompleteESList, wrnExamSheet);
                if (wrnExamSheet->patientType == (PATIENTTYPE)ADULT)
                {
                    /*DEBUG('w', "Waiting Room nurse creates an examination sheet for Adult patient with name [%d] and age [%d].\n", wrnExamSheet -> name, wrnExamSheet -> age);
                     
                     DEBUG('w', "Waiting Room nurse tells the Adult Patient [%d] to wait in the waiting room for a nurse.\n", wrnExamSheet -> examSheetId);*/
                    Print("Waiting Room nurse creates an examination sheet for Adult patient with name [%d] and age [%d].\n", sizeof("Waiting Room nurse creates an examination sheet for Adult patient with name [%d] and age [%d].\n")/sizeof(char), 2, wrnExamSheet->name*256+wrnExamSheet->age);
                    Print("Waiting Room nurse tells the Adult Patient [%d] to wait in the waiting room for a nurse.\n", sizeof("Waiting Room nurse tells the Adult Patient [%d] to wait in the waiting room for a nurse.\n")/sizeof(char), 1, wrnExamSheet->examSheetId);
                }
                else
                {
                    /*
                     DEBUG('w', "Waiting Room nurse creates an examination sheet for Child patient with name [%d] and age [%d].\n", wrnExamSheet -> name, wrnExamSheet -> age);
                     
                     DEBUG('w', "Waiting Room nurse tells Parent [%d] to wait in the waiting room for a nurse.\n", wrnExamSheet -> examSheetId);*/
                    Print("Waiting Room nurse creates an examination sheet for Child patient with name [%d] and age [%d].\n", sizeof("Waiting Room nurse creates an examination sheet for Child patient with name [%d] and age [%d].\n")/sizeof(char), 2, wrnExamSheet->name*256+wrnExamSheet->age);
                    Print("Waiting Room nurse tells Parent [%d] to wait in the waiting room for a nurse.\n", sizeof("Waiting Room nurse tells Parent [%d] to wait in the waiting room for a nurse.\n")/sizeof(char), 1, wrnExamSheet->examSheetId);
                }
                SignalCondition(wrnCV, wrnLock);
                wrnExamSheet = NULL;
                wrnTask = NONE;
                ReleaseLock(wrnLock);
            } 
            else 
            {
                /*
                 printf("Error: Do nothing while patients are waiting in the WR.\n");
                 */
                Print("Error: Do nothing while patients are waiting in the WR.\n", sizeof("Error: Do nothing while patients are waiting in the WR.\n")/sizeof(char), 0, 0);
                ReleaseLock(wrnLock);
            }
        } 
        else 
        {   /*----------------------------------------- 
             // WRN Service the Nurse and give the form
             //-----------------------------------------*/
            wrnStatus = (WRNSTATUS)WRNFREE;
            ReleaseLock(waitingRoomLock);
            AcquireLock(wrnLock);
            if (nwrnWaitingCount > 0) 
            {
                nwrnWaitingCount --;
                SignalCondition(nwrnWaitingCV, wrnLock);
                AcquireLock(nwrnLock);
                ReleaseLock(wrnLock);
                WaitCondition(nwrnCV, nwrnLock);
                /*
                Print("%d, %d\n", 7, 2, wrnCompleteESList.front*256+wrnCompleteESList.rear); 
                */
                if (wrnCompleteESList.front != wrnCompleteESList.rear) 
                {

                    Remove(&wrnCompleteESList, &nwrnExamSheet);
                    SignalCondition(nwrnCV, nwrnLock);
                    /*DEBUG('w', "Waiting Room nurse gives examination sheet of patient [%d] to Nurse [%d].\n", nwrnExamSheet->examSheetId, nwrnId);
                     */
                    Print("Waiting Room nurse gives examination sheet of patient [%d] to Nurse [%d].\n", sizeof("Waiting Room nurse gives examination sheet of patient [%d] to Nurse [%d].\n")/sizeof(char), 2, nwrnExamSheet->examSheetId*256+nwrnId);
                    WaitCondition(nwrnCV, nwrnLock);
                    nwrnId = -1;
                    ReleaseLock(nwrnLock);
                }
                else
                {
                    nwrnExamSheet = NULL;
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
        Yield();
    }    
    Exit(0);
}
void Patient()
{
    ExamSheet *myExamSheet = NULL;
    int mp_Type = 0;
    int pid, erIndex, i;
    pid = glopatid++;
    /*
     DEBUG('l', "Adult Patient [%d] has entered the Doctor's Office Waiting Room.\n", pid);
     */
    /*-----------------------------------
     // Adult Patient Interacts with WRN
     //-----------------------------------*/
    Print("Adult Patient [%d] has entered the Doctor's Office Waiting Room.\n", sizeof("Adult Patient [%d] has entered the Doctor's Office Waiting Room.\n")/sizeof(char), 1, pid);
    AcquireLock(waitingRoomLock);
    wrWaitingCount ++;
    /*
     DEBUG('l', "Adult Patient [%d] gets in line of the Waiting Room Nurse to get registration form.\n", pid);
     */
    Print("Adult Patient [%d] gets in line of the Waiting Room Nurse to get registration form.\n", sizeof("Adult Patient [%d] gets in line of the Waiting Room Nurse to get registration form.\n")/sizeof(char), 1, pid);
    WaitCondition(waitingRoomCV, waitingRoomLock);
    wrnStatus = (WRNSTATUS)WRNBUSY;
    ReleaseLock(waitingRoomLock);
    AcquireLock(wrnLock);
    wrnTask = GETFORMAP;
    wrnServiceId = pid;
    SignalCondition(wrnCV, wrnLock);
    WaitCondition(wrnCV, wrnLock);
    /*
     DEBUG('l', "Adult patient [%d] gets the form from the Waiting Room Nurse.\n", pid);
     */
    Print("Adult patient [%d] gets the form from the Waiting Room Nurse.\n", sizeof("Adult patient [%d] gets the form from the Waiting Room Nurse.\n")/sizeof(char), 1, pid);
    myExamSheet = wrnExamSheet;
    SignalCondition(wrnCV, wrnLock);
    ReleaseLock(wrnLock);
    myExamSheet->name = pid;
    myExamSheet->age = Random(60)+15;
    myExamSheet->examSheetId = pid;
    myExamSheet->patientType = (PATIENTTYPE)mp_Type;
    myExamSheet->patientTask = (PATIENTTASK)PATNOTHING;
    myExamSheet->illness = (SYMPTOM)(Random(3));
    myExamSheet->xrayStatus = (XRAYSTATUS)XRAYUNKNOWN;
    myExamSheet->xrayImages = 0;
    myExamSheet->xrayResult = (XRAYRESULT)UNFILLED;
    myExamSheet->shotStatus = (SHOTSTATUS)SHOTUNKNOWN;
    myExamSheet->esStatus = (ESSTATUS)ESUNFINISHED;
    myExamSheet->esProcess = (ESPROCESS)ESIDLE;
    myExamSheet->totalAmount = 0;
    Yield();
    AcquireLock(waitingRoomLock);
    wrWaitingCount ++;
    /*
     DEBUG('l', "Adult Patient [%d] gets in line of the Waiting Room Nurse to submit the registration form.\n", pid);
     */
    Print("Adult Patient [%d] gets in line of the Waiting Room Nurse to submit the registration form.\n", sizeof("Adult Patient [%d] gets in line of the Waiting Room Nurse to submit the registration form.\n")/sizeof(char), 1, pid);
    WaitCondition(waitingRoomCV, waitingRoomLock);
    ReleaseLock(waitingRoomLock);
    AcquireLock(wrnLock);
    wrnTask = GIVEFORM;
    wrnExamSheet = myExamSheet;
    SignalCondition(wrnCV, wrnLock);
    /*
     DEBUG('l', "Adult patient [%d] submits the filled form to the Waiting Room Nurse.\n", pid);
     */
    Print("Adult patient [%d] submits the filled form to the Waiting Room Nurse.\n", sizeof("Adult patient [%d] submits the filled form to the Waiting Room Nurse.\n")/sizeof(char), 1, pid);
    WaitCondition(wrnCV, wrnLock);
    ReleaseLock(wrnLock);
    /*-------------------------------------------------
     // Adult Patient Wait for being Called by Nurse
     //------------------------------------------------- */   
    
    AcquireLock(broadCastIdLock);
    while (broadCastId != pid) 
    {
        WaitCondition(broadCastIdCV, broadCastIdLock);
        
    }
    bcIdStatus = (BROADCASTIDSTATUS)BROADFREE;
    SignalCondition(bcIdWaitingCV, broadCastIdLock);
    ReleaseLock(broadCastIdLock);
    /*-------------------------------------------------------------------------
     // Paitent Wait for the Nurse to take the temperature
     //---------------------------------------------------------------------*/
    
    erIndex = myExamSheet -> erId;
    AcquireLock(examRoomsLock[erIndex]);
    SignalCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
    WaitCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
    /*
     DEBUG('l', "Adult Patient [%d] is following Nurse [%d] to Examination Room [%d].\n", pid, myExamSheet -> nurseId, erIndex+1);
     */
    Print("Adult Patient [%d] is following Nurse [%d] to Examination Room [%d].\n", sizeof("Adult Patient [%d] is following Nurse [%d] to Examination Room [%d].\n")/sizeof(char), 3, pid*65536+myExamSheet->nurseId*256+(erIndex+1));
    SignalCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
    /*
     DEBUG('l', "Adult Patient [%d] has arrived at Examination Room [%d].\n", pid, erIndex+1);
     */
    Print("Adult Patient [%d] has arrived at Examination Room [%d].\n", sizeof("Adult Patient [%d] has arrived at Examination Room [%d].\n")/sizeof(char), 2, pid*256+(erIndex+1));
    WaitCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
    myExamSheet->illness = (SYMPTOM)(1);
    switch (myExamSheet->illness) {
        case (SYMPTOM)SYMPAIN:
            /*
             DEBUG('l', "Adult Patient [%d] says, \"My symptoms are Pain\".\n", pid);
             */
            Print("Adult Patient [%d] says, \"My symptoms are Pain\".\n", sizeof("Adult Patient [%d] says, \"My symptoms are Pain\".\n")/sizeof(char), 1, pid);
            break;
        case (SYMPTOM)SYMNAUSEA:
            /*  
             DEBUG('l', "Adult Patient [%d] says, \"My symptoms are Nausea\".\n", pid);
             */
            Print("Adult Patient [%d] says, \"My symptoms are Nausea\".\n", sizeof("Adult Patient [%d] says, \"My symptoms are Nausea\".\n")/sizeof(char), 1, pid);
            break;
        case (SYMPTOM)SYMHEAR:
            /*
             DEBUG('l', "Adult Patient [%d] says, \"My symptoms are Hear Alien Voices\".\n", pid);
             */
            Print("Adult Patient [%d] says, \"My symptoms are Hear Alien Voices\".\n", sizeof("Adult Patient [%d] says, \"My symptoms are Hear Alien Voices\".\n")/sizeof(char), 1, pid);
            break;
        default:
            break;
    }
    SignalCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
    WaitCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
    ReleaseLock(examRoomsLock[erIndex]);
    while (true) 
    { 
        AcquireLock(examRoomsLock[erIndex]);
        /*-----------------------------------
         // All things are Done.
         //-----------------------------------*/
        if (myExamSheet->patientTask == (PATIENTTASK)PATDONE) 
        {
            break;            
        }
        /*-----------------------------------------------------
         // Stay @ ER and Wait for the 2nd Diagnose after X-ray
         //-----------------------------------------------------*/
        else if (myExamSheet->patientTask == (PATIENTTASK)PATDIAGNOSEXRAY)
        {
            myExamSheet->patientTask = (PATIENTTASK)PATNOTHING;
            SignalCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
            /*
             DEBUG('l', "Adult Patient [%d] has been diagnosed by Doctor [%d].\n", pid, myExamSheet -> docId);
             */
            Print("Adult Patient [%d] has been diagnosed by Doctor [%d].\n", sizeof("Adult Patient [%d] has been diagnosed by Doctor [%d].\n")/sizeof(char), 2, pid*256+myExamSheet->docId);
            WaitCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
        }
        /*------------------------------------------
         // Stay @ ER and Wait for the 1st Diagnose  
         //------------------------------------------*/
        else if (myExamSheet->patientTask == (PATIENTTASK)PATDIAGNOSE)
        {
            myExamSheet->patientTask = (PATIENTTASK)PATNOTHING;
            SignalCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
            WaitCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
            if (myExamSheet->xrayImages > 0) 
            {
                /*DEBUG('l', "Adult Patient [%d] has been informed by Doctor [%d] that he needs an Xray.\n", pid, myExamSheet -> docId);
                 */
                Print("Adult Patient [%d] has been informed by Doctor [%d] that he needs an Xray.\n", sizeof("Adult Patient [%d] has been informed by Doctor [%d] that he needs an Xray.\n")/sizeof(char), 2, pid*256+myExamSheet->docId);
                SignalCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
                WaitCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
            }
            if (myExamSheet->shotStatus == (SHOTSTATUS)SHOTREQUIRED)
            {
                /*DEBUG('l', "Adult Patient [%d] has been informed by Doctor [%d] that he will be administered a shot.\n", pid, myExamSheet -> docId);
                 */
                Print("Adult Patient [%d] has been informed by Doctor [%d] that he will be administered a shot.\n", sizeof("Adult Patient [%d] has been informed by Doctor [%d] that he will be administered a shot.\n")/sizeof(char), 2, pid*256+myExamSheet->docId);
                SignalCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
                WaitCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
            }
            /*
             DEBUG('l', "Adult Patient [%d] has been diagnosed by Doctor [%d].\n", pid, myExamSheet -> docId);
             */
            Print("Adult Patient [%d] has been diagnosed by Doctor [%d].\n", sizeof("Adult Patient [%d] has been diagnosed by Doctor [%d].\n")/sizeof(char), 2, pid*256+myExamSheet->docId);
            SignalCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
        }
        /*------------------------------------------
         // Stay @ ER and Wait for escort to XRAY
         //------------------------------------------*/
        else if ((myExamSheet -> patientTask == (PATIENTTASK)PATGOXRAY))
        {
            /*
             DEBUG('l', "Adult Patient [%d] waits for a Nurse to escort them to the Xray room.\n", pid);
             */
            Print("Adult Patient [%d] waits for a Nurse to escort them to the Xray room.\n", sizeof("Adult Patient [%d] waits for a Nurse to escort them to the Xray room.\n")/sizeof(char), 1, pid);
            myExamSheet->patientTask = (PATIENTTASK)PATNOTHING;
            myExamSheet->xrayStatus = (XRAYSTATUS)XRAYREQUIRED;
            WaitCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
            /*
             DEBUG('l', "Adult Patient [%d] gets on the table.\n", pid);
             */
            Print("Adult Patient [%d] gets on the table.\n", sizeof("Adult Patient [%d] gets on the table.\n")/sizeof(char), 1, pid);
            SignalCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
            WaitCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
            for (i =0; i< myExamSheet -> xrayImages; i++) 
            {
                /*
                 DEBUG('l', "Adult Patient [%d] has been asked to take an Xray.\n", pid);
                 */
                Print("Adult Patient [%d] has been asked to take an Xray.\n", sizeof("Adult Patient [%d] has been asked to take an Xray.\n")/sizeof(char), 1, pid);
                SignalCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
                WaitCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
                SignalCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
                WaitCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
            }
            SignalCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
            WaitCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
            /*
             DEBUG('l', "Adult Patient [%d] is following Nurse [%d] to Examination Room [%d].\n", pid, myExamSheet -> nurseId, erIndex+1);
             */
            Print("Adult Patient [%d] is following Nurse [%d] to Examination Room [%d].\n", sizeof("Adult Patient [%d] is following Nurse [%d] to Examination Room [%d].\n")/sizeof(char), 3, pid*65536+myExamSheet->nurseId*256+(erIndex+1));
            SignalCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
            /*
             DEBUG('l', "Adult Patient [%d] has arrived at Examination Room [%d].\n", pid, erIndex+1);
             */
            Print("Adult Patient [%d] has arrived at Examination Room [%d].\n", sizeof("Adult Patient [%d] has arrived at Examination Room [%d].\n")/sizeof(char), 2, pid*256+(erIndex+1));
        }
        /*-------------------------------------------------
         // Stay @ ER and Wait for a Nurse to be give a shot
         //-------------------------------------------------*/
        else if (myExamSheet->patientTask == (PATIENTTASK)PATSHOT)
        {
            myExamSheet->patientTask = (PATIENTTASK)PATNOTHING;
            /*
             DEBUG('l', "Adult Patient [%d] says, \"Yes I am/He is ready for the shot\".\n", pid);
             */
            Print("Adult Patient [%d] says, \"Yes I am/He is ready for the shot\".\n", sizeof("Adult Patient [%d] says, \"Yes I am/He is ready for the shot\".\n")/sizeof(char), 1, pid);
            SignalCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
            WaitCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
        }
        else 
        {
        }
        ReleaseLock(examRoomsLock[erIndex]);
        Yield();
    }
    SignalCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
    WaitCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
    SignalCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
    ReleaseLock(examRoomsLock[erIndex]);
    /*-------------------------------------------------
    // Stay @ Cashier Waiting Room
    -------------------------------------------------*/
    AcquireLock(cashWaitingLock);
    cashWaitingCount ++;
    /*
     DEBUG('l', "Adult Patient [%d] enters the queue for Cashier.\n", pid);
     */
    Print("Adult Patient [%d] enters the queue for Cashier.\n", sizeof("Adult Patient [%d] enters the queue for Cashier.\n")/sizeof(char), 1, pid);
    WaitCondition(cashWaitingCV, cashWaitingLock);
    ReleaseLock(cashWaitingLock);
    /*
     DEBUG('l', "Adult Patient [%d] reaches the Cashier.\n", pid);
     */
    Print("Adult Patient [%d] reaches the Cashier.\n", sizeof("Adult Patient [%d] reaches the Cashier.\n")/sizeof(char), 1, pid);
    AcquireLock(cashLock);
    cashExamSheet = myExamSheet;
    /*
     DEBUG('l', "Adult Patient [%d] hands over his examination sheet to the Cashier.\n", pid);
     */
    Print("Adult Patient [%d] hands over his examination sheet to the Cashier.\n", sizeof("Adult Patient [%d] hands over his examination sheet to the Cashier.\n")/sizeof(char), 1, pid);
    SignalCondition(cashCV, cashLock);
    WaitCondition(cashCV, cashLock);
    /*
     DEBUG('l', "Adult Patient [%d] pays the Cashier $%f.\n", pid, myExamSheet -> totalAmount);
     */
    Print("Adult Patient [%d] pays the Cashier $%d.\n", sizeof("Adult Patient [%d] pays the Cashier $%d.\n")/sizeof(char), 2, pid*256+myExamSheet->totalAmount);
    SignalCondition(cashCV, cashLock);
    WaitCondition(cashCV, cashLock);
    /*
     DEBUG('l', "Adult Patient [%d] receives a receipt from the Cashier.\n", pid);
     */
    Print("Adult Patient [%d] receives a receipt from the Cashier.\n", sizeof("Adult Patient [%d] receives a receipt from the Cashier.\n")/sizeof(char), 1, pid);
    SignalCondition(cashCV, cashLock);
    /*
     DEBUG('l', "Adult Patient [%d] leaves the doctor's office.\n", pid);
     */
    Print("Adult Patient [%d] leaves the doctor's office.\n", sizeof("Adult Patient [%d] leaves the doctor's office.\n")/sizeof(char), 1, pid);
    ReleaseLock(cashLock);
    Exit(0);
}
void Nurse()
{
    ExamSheet *holdingES;
    int patientId, pid;
    int erIndex, docIndex;
    int flag = false;
    int xrIndex;
    int times = 600;
    pid = glonurid++;
    while (times-- > 0) 
    {
        /*---------------------------------------------------------------------
         // Check whether there is empty ER so they can go for ask a patient to come
         //------------------------------------------------------------------*/
        
        for (erIndex = 0; erIndex < examRooms; erIndex++) 
        {
            AcquireLock(ersStatusLock);
            if (examRoomsStatus[erIndex] == (ERSTATUS)ERFREE) 
            {
                examRoomsStatus[erIndex] = (ERSTATUS)ERBLOCKED;
                ReleaseLock(ersStatusLock);
                AcquireLock(wrnLock);
                nwrnWaitingCount ++;
                WaitCondition(nwrnWaitingCV, wrnLock);
                ReleaseLock(wrnLock);
                AcquireLock(nwrnLock);
                nwrnId = pid;
                SignalCondition(nwrnCV, nwrnLock);
                WaitCondition(nwrnCV, nwrnLock);
                holdingES = nwrnExamSheet;
                if (holdingES == NULL) 
                {
                    SignalCondition(nwrnCV, nwrnLock);
                    ReleaseLock(nwrnLock);
                    AcquireLock(ersStatusLock);
                    examRoomsStatus[erIndex] = (ERSTATUS)ERFREE;
                    break;
                }
                /*---------------------------------------------------------
                 //nurse interacts with the wrn to get the examination sheet
                 //---------------------------------------------------------*/
                
                Print("Nurse [%d] tells Waiting Room Nurse to give a new examination sheet.\n", sizeof("Nurse [%d] tells Waiting Room Nurse to give a new examination sheet.\n")/sizeof(char), 1, pid);
                
                /*
                 DEBUG('u',"Nurse [%d] tells Waiting Room Nurse to give a new examination sheet.\n", pid);
                 */
                
                SignalCondition(nwrnCV, nwrnLock);
                ReleaseLock(nwrnLock);
                holdingES->erId = erIndex;
                holdingES->nurseId = pid;
                AcquireLock(broadCastIdLock);
                if (bcIdStatus == (BROADCASTIDSTATUS)BROADFREE) { 
                    bcIdStatus = (BROADCASTIDSTATUS)BROADBUSY;
                } else {
                    WaitCondition(bcIdWaitingCV, broadCastIdLock);
                    bcIdStatus = (BROADCASTIDSTATUS)BROADBUSY;
                }
                AcquireLock(examRoomsLock[erIndex]);
                broadCastId = holdingES->examSheetId;
                BroadcastCondition(broadCastIdCV, broadCastIdLock);
                ReleaseLock(broadCastIdLock);
                WaitCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
                /*----------------------------------------------------------
                 //nurse escort the patient to enter the examination room.
                 //---------------------------------------------------------*/
                if (holdingES -> patientType == (PATIENTTYPE)ADULT)
                {
                    /*
                     DEBUG('u', "Nurse [%d] escorts Adult Patient [%d] to the examination room [%d].\n", pid, holdingES -> examSheetId, erIndex+1);
                     */
                    Print("Nurse [%d] escorts Adult Patient [%d] to the examination room [%d].\n", sizeof("Nurse [%d] escorts Adult Patient [%d] to the examination room [%d].\n")/sizeof(char), 3, pid*65536+holdingES->examSheetId*256+(erIndex+1));
                }
                else
                {/*
                  DEBUG('u', "Nurse [%d] escorts Parent [%d] to the examination room [%d].\n", pid, holdingES -> examSheetId, erIndex+1);
                  */
                    Print("Nurse [%d] escorts Parent [%d] to the examination room [%d].\n", sizeof("Nurse [%d] escorts Parent [%d] to the examination room [%d].\n")/sizeof(char), 3, pid*65536+holdingES->examSheetId*256+(erIndex+1));
                }
                SignalCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
                WaitCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
                /*------------------------------------------------------
                 //nurse takes the temperature and blood pressure in exmination room
                 //-------------------------------------------------------*/
                
                if (holdingES -> patientType == (PATIENTTYPE)ADULT)
                {/*
                  DEBUG('u', "Nurse [%d] takes the temperature and blood pressure of Adult Patient [%d].\n", pid, holdingES -> examSheetId);
                  */
                    Print("Nurse [%d] takes the temperature and blood pressure of Adult Patient [%d].\n", sizeof("Nurse [%d] takes the temperature and blood pressure of Adult Patient [%d].\n")/sizeof(char), 2, pid*256+holdingES->examSheetId);
                }
                else
                {
                    /*
                     DEBUG('u', "Nurse [%d] takes the temperature and blood pressure of Parent [%d].\n", pid, holdingES -> examSheetId);
                     */
                    Print("Nurse [%d] takes the temperature and blood pressure of Parent [%d].\n", sizeof("Nurse [%d] takes the temperature and blood pressure of Parent [%d].\n")/sizeof(char), 2, pid*256+holdingES->examSheetId);
                }
                holdingES->temperature = Random(10)+32;
                holdingES->bloodPressure = Random(80)+70;
                if (holdingES -> patientType == (PATIENTTYPE)ADULT)
                {/*
                  DEBUG('u', "Nurse [%d] ask Adult Patient [%d] \"What Symptoms do you have?\".\n", pid, holdingES -> examSheetId);
                  */
                    Print("Nurse [%d] ask Adult Patient [%d] \"What Symptoms do you have?\".\n", sizeof("Nurse [%d] ask Adult Patient [%d] \"What Symptoms do you have?\".\n")/sizeof(char), 2, pid*256+holdingES->examSheetId);
                }
                else
                {/*
                  DEBUG('u', "Nurse [%d] ask Parent [%d] \"What Symptoms do you have?\".\n", pid, holdingES -> examSheetId);
                  */
                    Print("Nurse [%d] ask Parent [%d] \"What Symptoms do you have?\".\n", sizeof("Nurse [%d] ask Parent [%d] \"What Symptoms do you have?\".\n")/sizeof(char), 2, pid*256+holdingES->examSheetId);
                }
                SignalCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
                WaitCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
                /*------------------------------------------------------
                 //nurse records the information on examination sheet
                 //-------------------------------------------------------*/
                
                if (holdingES -> patientType == (PATIENTTYPE)ADULT)
                {/*
                  DEBUG('u', "Nurse [%d] writes all the information of Adult Patient [%d] in his examination sheet.\n", pid, holdingES -> examSheetId);
                  */
                    Print("Nurse [%d] writes all the information of Adult Patient [%d] in his examination sheet.\n", sizeof("Nurse [%d] writes all the information of Adult Patient [%d] in his examination sheet.\n")/sizeof(char), 2, pid*256+holdingES->examSheetId);
                }
                else
                {/*
                  DEBUG('u', "Nurse [%d] writes all the information of Parent [%d] in his examination sheet.\n", pid, holdingES -> examSheetId);
                  */
                    Print("Nurse [%d] writes all the information of Parent [%d] in his examination sheet.\n", sizeof("Nurse [%d] writes all the information of Parent [%d] in his examination sheet.\n")/sizeof(char), 2, pid*256+holdingES->examSheetId);
                }
                /*------------------------------------------------------
                 //nurse inform the doctor to diagnose
                 //-------------------------------------------------------*/
                for (docIndex = 0; docIndex < docCount ; docIndex ++) 
                {
                    AcquireLock(doctWaitingLock[docIndex]);
                    if (docStatus[docIndex] == (DOCSTATUS)DOCFREE) 
                    { 
                        docStatus[docIndex] = (DOCSTATUS)DOCBUSY;
                        ndocESNo[docIndex] = erIndex;
                        SignalCondition(doctWaitingCV[docIndex], doctWaitingLock[docIndex]);
                        holdingES -> esProcess = (ESPROCESS)ESINPROCESS;
                        ReleaseLock(doctWaitingLock[docIndex]);
                        if (holdingES -> patientType == (PATIENTTYPE)ADULT)
                        {/*
                          
                          DEBUG('u', "Nurse [%d] informs Doctor [%d] that Adult Patient [%d] is waiting in the examination room [%d].\n", pid, docIndex+1, holdingES -> examSheetId,erIndex+1);
                          
                          DEBUG('u', "Nurse [%d] hands over to the Doctor [%d] the examination sheet of Adult Patient [%d].\n", pid, docIndex+1, erIndex+1);
                          */
                            Print("Nurse [%d] informs Doctor [%d] that Adult Patient [%d] is waiting in the examination room [%d].\n", sizeof("Nurse [%d] informs Doctor [%d] that Adult Patient [%d] is waiting in the examination room [%d].\n")/sizeof(char), 4, pid*16777216+(docIndex+1)*65536+holdingES->examSheetId*256+(erIndex+1));
                            Print("Nurse [%d] hands over to the Doctor [%d] the examination sheet of Adult Patient [%d].\n", sizeof("Nurse [%d] hands over to the Doctor [%d] the examination sheet of Adult Patient [%d].\n")/sizeof(char), 3, pid*65536+(docIndex+1)*256+(erIndex+1));
                        }
                        else
                        {/*
                          DEBUG('u', "Nurse [%d] informs Doctor [%d] that Child Patient [%d] is waiting in the examination room [%d].\n", pid, docIndex+1, holdingES -> examSheetId,erIndex+1);
                          
                          DEBUG('u', "Nurse [%d] hands over to the Doctor [%d] the examination sheet of Child Patient [%d].\n", pid, docIndex+1, erIndex+1);
                          */
                            Print("Nurse [%d] informs Doctor [%d] that Child Patient [%d] is waiting in the examination room [%d].\n", sizeof("Nurse [%d] informs Doctor [%d] that Child Patient [%d] is waiting in the examination room [%d].\n")/sizeof(char), 4, pid*16777216+(docIndex+1)*65536+holdingES->examSheetId*256+(erIndex+1));
                            Print("Nurse [%d] hands over to the Doctor [%d] the examination sheet of Child Patient [%d].\n", sizeof("Nurse [%d] hands over to the Doctor [%d] the examination sheet of Child Patient [%d].\n")/sizeof(char), 3, pid*65536+(docIndex+1)*256+(erIndex+1));
                        }
                        break;
                        
                    } 
                    else 
                    {
                        ReleaseLock(doctWaitingLock[docIndex]);
                    }
                }
                SignalCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
                wallPackets[erIndex] = holdingES;
                holdingES = NULL;
                ReleaseLock(examRoomsLock[erIndex]);
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
        if (xrayWaitingES.front != xrayWaitingES.rear) 
        {
            Remove(&xrayWaitingES, &holdingES);
            ReleaseLock(xrayWaitingLock);
            erIndex = holdingES->erId;
            if (holdingES -> patientType == (PATIENTTYPE)ADULT)
            {/* 
              DEBUG('u', "Nurse [%d] gets examination sheet for Adult Patient [%d] in Xray waiting room.\n", pid, holdingES -> examSheetId);
              */
                Print("Nurse [%d] gets examination sheet for Adult Patient [%d] in Xray waiting room.\n", sizeof("Nurse [%d] gets examination sheet for Adult Patient [%d] in Xray waiting room.\n")/sizeof(char), 2, pid*256+holdingES->examSheetId);
            }
            else
            {/*
              DEBUG('u', "Nurse [%d] gets examination sheet for Parent [%d] in Xray waiting room.\n", pid, holdingES -> examSheetId);
              */
                Print("Nurse [%d] gets examination sheet for Parent [%d] in Xray waiting room.\n", sizeof("Nurse [%d] gets examination sheet for Parent [%d] in Xray waiting room.\n")/sizeof(char), 2, pid*256+holdingES->examSheetId);
            }
            AcquireLock(examRoomsLock[erIndex]);
            holdingES->nurseId = pid;
            holdingES->patientTask = (PATIENTTASK)PATBACKXRAY;
            /*------------------------------------------------------
             //nurse escort the patient to examination room after xray
             //-------------------------------------------------------*/
            if (holdingES -> patientType == (PATIENTTYPE)ADULT)
            {/* 
              DEBUG('u', "Nurse [%d] escorts Adult Patient [%d] to the examination room [%d].\n", pid, holdingES -> examSheetId, erIndex+1);
              
              */
                Print("Nurse [%d] escorts Adult Patient [%d] to the examination room [%d].\n", sizeof("Nurse [%d] escorts Adult Patient [%d] to the examination room [%d].\n")/sizeof(char), 3, pid*65536+holdingES->examSheetId*256+(erIndex+1));
            }
            else
            {/* 
              DEBUG('u', "Nurse [%d] escorts Parent [%d] to the examination room [%d].\n", pid, holdingES -> examSheetId, erIndex+1);
              */
                Print("Nurse [%d] escorts Parent [%d] to the examination room [%d].\n", sizeof("Nurse [%d] escorts Parent [%d] to the examination room [%d].\n")/sizeof(char), 3, pid*65536+holdingES->examSheetId*256+(erIndex+1));
            }
            SignalCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
            WaitCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
            holdingES -> esProcess = (ESPROCESS)ESIDLE;
            wallPackets[erIndex] = holdingES;
            ReleaseLock(examRoomsLock[erIndex]);
            holdingES = NULL;
        }    
        else 
        {
            ReleaseLock(xrayWaitingLock);
        }
        Yield();
        
        /*-------------------------------------
         // Nurse Checks the wallpacket
         //-------------------------------------*/
        for (erIndex = 0; erIndex<examRooms; erIndex++) 
        {
            
            AcquireLock(examRoomsLock[erIndex]);
            /*
             DEBUG('u', "Nurse [%d] checks the wall box of examination room [%d].\n", pid, erIndex+1);
             */
            
            holdingES = wallPackets[erIndex];
            if (holdingES == NULL) 
            {
                ReleaseLock(examRoomsLock[erIndex]);
                continue;
            }
            
            Print("Nurse [%d] checks the wall box of examination room [%d].\n", sizeof("Nurse [%d] checks the wall box of examination room [%d].\n")/sizeof(char), 2, pid*256+(erIndex+1));
            wallPackets[erIndex] = NULL;
            /*---------------------------------------------
             // Patient needs the doctor to service 2nd
             //---------------------------------------------*/
            if (holdingES -> xrayStatus == (XRAYSTATUS)XRAYHALFDONE &&
                holdingES -> esProcess != (ESPROCESS)ESINPROCESS) 
            {  
                for (docIndex = 0; docIndex < docCount; docIndex ++) 
                {
                    AcquireLock(doctWaitingLock[docIndex]);
                    if (docStatus[docIndex] == (DOCSTATUS)DOCFREE) 
                    {
                        docStatus[docIndex] = (DOCSTATUS)DOCBUSY;
                        ndocESNo[docIndex] = erIndex;
                        holdingES -> esProcess = (ESPROCESS)ESINPROCESS;
                        SignalCondition(doctWaitingCV[docIndex], doctWaitingLock[docIndex]);
                        ReleaseLock(doctWaitingLock[docIndex]);
                        /*
                         DEBUG('u', "Nurse [%d] informs Doctor [%d] that Adult/Child Patient [%d] is waiting in the examination room [%d].\n", pid, docIndex+1, holdingES -> examSheetId,erIndex+1);
                         
                         DEBUG('u', "Nurse [%d] hands over to the Doctor [%d] the examination sheet of Adult/Child Patient [%d].\n", pid, docIndex+1, erIndex+1);
                         */
                        Print("Nurse [%d] informs Doctor [%d] that Adult/Child Patient [%d] is waiting in the examination room [%d].\n", sizeof("Nurse [%d] informs Doctor [%d] that Adult/Child Patient [%d] is waiting in the examination room [%d].\n")/sizeof(char), 4, pid*16777216+(docIndex+1)*65536+holdingES->examSheetId*256+(erIndex+1));
                        Print("Nurse [%d] hands over to the Doctor [%d] the examination sheet of Adult/Child Patient [%d].\n", sizeof("Nurse [%d] hands over to the Doctor [%d] the examination sheet of Adult/Child Patient [%d].\n")/sizeof(char), 3, pid*65536+(docIndex+1)*256+(erIndex+1));
                        break;
                    } 
                    else 
                    {
                        ReleaseLock(doctWaitingLock[docIndex]);
                    }
                }
                wallPackets[erIndex] = holdingES;
                holdingES = NULL;
                ReleaseLock(examRoomsLock[erIndex]);
            }
            /*------------------------------------------------
             // Patients need to be sent to XRAYTechnician
             //------------------------------------------------*/
            else if (holdingES -> xrayStatus == (XRAYSTATUS)XRAYREQUIRED && 
                     holdingES -> esProcess != (ESPROCESS)ESINPROCESS) 
            {   
                AcquireLock(xrayRoomsStatusLock);
                for (xrIndex = 0; xrIndex < xrayRoomsCount; xrIndex++) 
                {
                    if (xrayRoomsStatus[xrIndex] == (XRRSTATUS)XRRFREE) 
                    {   
                        xrayRoomsStatus[xrIndex] = (XRRSTATUS)XRRBUSY;
                        flag = true;
                        break;
                    } 
                }
                ReleaseLock(xrayRoomsStatusLock);
                if (flag) 
                {
                    holdingES -> esProcess = (ESPROCESS)ESINPROCESS;
                    AcquireLock(xrayRoomsLock[xrIndex]);
                    xrayRoomsES[xrIndex] = holdingES;
                    SignalCondition(xrayRoomsCV[xrIndex], xrayRoomsLock[xrIndex]);
                    ReleaseLock(xrayRoomsLock[xrIndex]);
                    if (holdingES -> patientType == (PATIENTTYPE)ADULT)
                    {/* 
                      DEBUG('u', "Nurse [%d] escorts Adult Patient [%d] to the X-ray room [%d].\n", pid, holdingES -> examSheetId, xrIndex+1);
                      
                      DEBUG('u', "Nurse [%d] informs X-Ray Technician [%d] about Adult Patient [%d] and hands over the examination sheet.\n", pid, xrIndex+1, holdingES -> examSheetId);
                      */
                        Print("Nurse [%d] escorts Adult Patient [%d] to the X-ray room [%d].\n", sizeof("Nurse [%d] escorts Adult Patient [%d] to the X-ray room [%d].\n")/sizeof(char), 3, pid*65536+holdingES->examSheetId*256+(xrIndex+1));
                        Print("Nurse [%d] informs X-Ray Technician [%d] about Adult Patient [%d] and hands over the examination sheet.\n", sizeof("Nurse [%d] informs X-Ray Technician [%d] about Adult Patient [%d] and hands over the examination sheet.\n")/sizeof(char), 3, pid*65536+(xrIndex+1)*256+holdingES->examSheetId);
                    }
                    else
                    {/*
                      DEBUG('u', "Nurse [%d] escorts Parent [%d] to the X-ray room [%d].\n", pid, holdingES -> examSheetId, xrIndex+1);
                      
                      DEBUG('u', "Nurse [%d] informs X-Ray Technician [%d] about Child Patient [%d] and hands over the examination sheet.\n", pid, xrIndex+1, holdingES -> examSheetId);
                      */
                        Print("Nurse [%d] escorts Parent [%d] to the X-ray room [%d].\n", sizeof("Nurse [%d] escorts Parent [%d] to the X-ray room [%d].\n")/sizeof(char), 3, pid*65536+holdingES->examSheetId*256+(xrIndex+1));
                        Print("Nurse [%d] informs X-Ray Technician [%d] about Child Patient [%d] and hands over the examination sheet.\n", sizeof("Nurse [%d] informs X-Ray Technician [%d] about Child Patient [%d] and hands over the examination sheet.\n")/sizeof(char), 3, pid*65536+(xrIndex+1)*256+holdingES->examSheetId);
                    }
                    /*
                     DEBUG('u', "Nurse [%d] leaves the X-ray Room [%d].\n", pid, xrIndex+1);
                     */
                    Print("Nurse [%d] leaves the X-ray Room [%d].\n", sizeof("Nurse [%d] leaves the X-ray Room [%d].\n")/sizeof(char), 2, pid*256+(xrIndex+1));
                    holdingES = NULL;
                    wallPackets[erIndex] = NULL;
                    ReleaseLock(examRoomsLock[erIndex]);
                }
                else 
                {
                    wallPackets[erIndex] = holdingES;
                    holdingES = NULL;
                    ReleaseLock(examRoomsLock[erIndex]);
                }
            } 
            else if (holdingES -> shotStatus == (SHOTSTATUS)SHOTREQUIRED && 
                     holdingES -> esProcess != (ESPROCESS)ESINPROCESS &&
                     holdingES -> patientTask != (PATIENTTASK)PATGOXRAY) 
            {
                /*------------------------------
                 // Patient needs a shot
                 //------------------------------*/
                
                if (holdingES -> patientType == (PATIENTTYPE)ADULT) 
                {/*
                  DEBUG('u', "Nurse [%d] goes to supply cabinet to give to take medicine for Adult Patient [%d].\n", pid, holdingES -> examSheetId);
                  */
                    
                    Print("Nurse [%d] goes to supply cabinet to give to take medicine for Adult Patient [%d].\n", sizeof("Nurse [%d] goes to supply cabinet to give to take medicine for Adult Patient [%d].\n")/sizeof(char), 2, pid*256+holdingES->examSheetId);
                }
                else
                {/*
                  DEBUG('u', "Nurse [%d] goes to supply cabinet to give to take medicine for Parent [%d].\n", pid, holdingES -> examSheetId);
                  */
                    Print("Nurse [%d] goes to supply cabinet to give to take medicine for Parent [%d].\n", sizeof("Nurse [%d] goes to supply cabinet to give to take medicine for Parent [%d].\n")/sizeof(char), 2, pid*256+holdingES->examSheetId);
                }
                holdingES -> esProcess = (ESPROCESS)ESINPROCESS;
                AcquireLock(cabinetLock);
                cabinetCount ++;
                ReleaseLock(cabinetLock);
                holdingES -> patientTask = (PATIENTTASK)PATSHOT;
                if (holdingES -> patientType == (PATIENTTYPE)ADULT)
                {/*
                  DEBUG('u', "Nurse [%d] asks Adult Patient [%d] \"Are you ready for the shot?\".\n", pid, holdingES -> examSheetId);
                  */
                    Print("Nurse [%d] asks Adult Patient [%d] \"Are you ready for the shot?\".\n", sizeof("Nurse [%d] asks Adult Patient [%d] \"Are you ready for the shot?\".\n")/sizeof(char), 2, pid*256+holdingES->examSheetId);
                }
                else
                {/*
                  DEBUG('u', "Nurse [%d] asks Parent [%d] \"Are you ready for the shot?\".\n", pid, holdingES -> examSheetId);
                  
                  */
                    Print("Nurse [%d] asks Parent [%d] \"Are you ready for the shot?\".\n", sizeof("Nurse [%d] asks Parent [%d] \"Are you ready for the shot?\".\n")/sizeof(char), 2, pid*256+holdingES->examSheetId);
                }
                WaitCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
                holdingES -> shotStatus = (SHOTSTATUS)SHOTDONE;
                holdingES -> esProcess = (ESPROCESS)ESIDLE;
                if (holdingES -> patientType == (PATIENTTYPE)ADULT)
                {/*
                  DEBUG('u', "Nurse [%d] tells Adult Patient [%d] \"Your shot is over.\".\n", pid, holdingES -> examSheetId);
                  */
                    Print("Nurse [%d] tells Adult Patient [%d] \"Your shot is over.\".\n", sizeof("Nurse [%d] tells Adult Patient [%d] \"Your shot is over.\".\n")/sizeof(char), 2, pid*256+holdingES->examSheetId);
                }
                else
                {/*
                  DEBUG('u', "Nurse [%d] tells Parent [%d] \"Your shot is over.\".\n", pid, holdingES -> examSheetId);
                  */
                    Print("Nurse [%d] tells Parent [%d] \"Your shot is over.\".\n", sizeof("Nurse [%d] tells Parent [%d] \"Your shot is over.\".\n")/sizeof(char), 2, pid*256+holdingES->examSheetId);
                }
                SignalCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
                wallPackets[erIndex] = holdingES;
                holdingES = NULL;
                ReleaseLock(examRoomsLock[erIndex]);
                
            } 
            else if (holdingES -> xrayStatus == (XRAYSTATUS)XRAYUNKNOWN && 
                     holdingES -> shotStatus == (SHOTSTATUS)SHOTUNKNOWN &&
                     holdingES -> esProcess != (ESPROCESS)ESINPROCESS) 
            {
                /*-----------------------------------------------
                 // Patient needs a doctor for 1st diagnose
                 //-----------------------------------------------*/
                
                for (docIndex = 0; docIndex < docCount; docIndex ++) 
                {
                    AcquireLock(doctWaitingLock[docIndex]);
                    if (docStatus[docIndex] == (DOCSTATUS)DOCFREE) 
                    {
                        docStatus[docIndex] = (DOCSTATUS)DOCBUSY;
                        ndocESNo[docIndex] = erIndex;
                        SignalCondition(doctWaitingCV[docIndex], doctWaitingLock[docIndex]);
                        ReleaseLock(doctWaitingLock[docIndex]);
                        if (holdingES -> patientType == (PATIENTTYPE)ADULT)
                        {/*
                          DEBUG('u', "Nurse [%d] informs Doctor [%d] that Adult Patient [%d] is waiting in the examination room [%d].\n", pid, docIndex+1, holdingES -> examSheetId,erIndex+1);
                          DEBUG('u', "Nurse [%d] hands over to the Doctor [%d] the examination sheet of Adult Patient [%d].\n", pid, docIndex+1, erIndex+1);
                          
                          */
                            Print("Nurse [%d] informs Doctor [%d] that Adult Patient [%d] is waiting in the examination room [%d].\n", sizeof("Nurse [%d] informs Doctor [%d] that Adult Patient [%d] is waiting in the examination room [%d].\n")/sizeof(char), 4, pid*16777216+(docIndex+1)*65536+holdingES->examSheetId*256+(erIndex+1));
                            Print("Nurse [%d] hands over to the Doctor [%d] the examination sheet of Adult Patient [%d].\n", sizeof("Nurse [%d] hands over to the Doctor [%d] the examination sheet of Adult Patient [%d].\n")/sizeof(char), 3, pid*65536+(docIndex+1)*256+(erIndex+1));
                        }
                        else
                        {/*
                          DEBUG('u', "Nurse [%d] informs Doctor [%d] that Child Patient [%d] is waiting in the examination room [%d].\n", pid, docIndex+1, holdingES -> examSheetId,erIndex+1);
                          DEBUG('u', "Nurse [%d] hands over to the Doctor [%d] the examination sheet of Child Patient [%d].\n", pid, docIndex+1, erIndex+1);
                          */
                            Print("Nurse [%d] informs Doctor [%d] that Child Patient [%d] is waiting in the examination room [%d].\n", sizeof("Nurse [%d] informs Doctor [%d] that Child Patient [%d] is waiting in the examination room [%d].\n")/sizeof(char), 4, pid*16777216+(docIndex+1)*65536+holdingES->examSheetId*256+(erIndex+1));
                            Print("Nurse [%d] hands over to the Doctor [%d] the examination sheet of Child Patient [%d].\n", sizeof("Nurse [%d] hands over to the Doctor [%d] the examination sheet of Child Patient [%d].\n")/sizeof(char), 3, pid*65536+(docIndex+1)*256+(erIndex+1));
                        }
                        holdingES -> esProcess = (ESPROCESS)ESINPROCESS;
                        break;
                    } 
                    else 
                    {
                        ReleaseLock(doctWaitingLock[docIndex]);
                    }
                }
                wallPackets[erIndex] = holdingES;
                holdingES = NULL;
                ReleaseLock(examRoomsLock[erIndex]);
            } 
            else 
            {
                if (holdingES -> xrayStatus == (XRAYSTATUS)XRAYDONE && 
                    holdingES -> shotStatus == (SHOTSTATUS)SHOTDONE &&
                    holdingES -> xrayResult != (XRAYRESULT)UNFILLED)
                {
                    holdingES -> esStatus = (ESSTATUS)ESFINISHED;
                }
                if (holdingES -> xrayStatus == (XRAYSTATUS)XRAYUNREQUIRED ) 
                {   
                    if (holdingES -> shotStatus == (SHOTSTATUS)SHOTDONE || 
                        holdingES -> shotStatus == (SHOTSTATUS)SHOTUNREQUIRED)
                    {
                        holdingES -> esStatus = (ESSTATUS)ESFINISHED;
                    }
                }
                if (holdingES -> shotStatus == (SHOTSTATUS)SHOTUNREQUIRED) 
                {   
                    if ((holdingES -> xrayStatus == (XRAYSTATUS)XRAYDONE &&
                         holdingES -> xrayResult != (XRAYRESULT)UNFILLED) ||
                        holdingES -> xrayStatus == (XRAYSTATUS)XRAYUNREQUIRED) 
                    {
                        holdingES -> esStatus = (ESSTATUS)ESFINISHED;
                    }
                }
                if (holdingES->esStatus == (ESSTATUS)ESFINISHED && 
                    holdingES->esProcess != (ESPROCESS)ESINPROCESS) 
                {   
                    
                    holdingES->esProcess = (ESPROCESS)ESINPROCESS;
                    holdingES->patientTask = (PATIENTTASK)PATDONE;
                    
                    WaitCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
                    
                    if (holdingES -> patientType == (PATIENTTYPE)ADULT)
                    {/*
                      DEBUG('u', "Nurse [%d] escorts Adult Patient [%d] to Cashier.\n", pid, holdingES -> examSheetId);
                      */
                        Print("Nurse [%d] escorts Adult Patient [%d] to Cashier.\n", sizeof("Nurse [%d] escorts Adult Patient [%d] to Cashier.\n")/sizeof(char), 2, pid*256+holdingES->examSheetId);
                    }
                    else
                    {/*
                      DEBUG('u', "Nurse [%d] escorts Parent [%d] to Cashier.\n", pid, holdingES -> examSheetId);
                      */
                        Print("Nurse [%d] escorts Parent [%d] to Cashier.\n", sizeof("Nurse [%d] escorts Parent [%d] to Cashier.\n")/sizeof(char), 2, pid*256+holdingES->examSheetId);
                    }
                    holdingES -> esProcess = (ESPROCESS)ESIDLE;
                    holdingES = NULL;
                    wallPackets[erIndex] = NULL;
                    SignalCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
                    WaitCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
                    ReleaseLock(examRoomsLock[erIndex]);
                    AcquireLock(ersStatusLock);
                    examRoomsStatus[erIndex] = (ERSTATUS)ERFREE;
                    ReleaseLock(ersStatusLock);
                } 
                else 
                {
                    wallPackets[erIndex] = holdingES;
                    holdingES = NULL;
                    ReleaseLock(examRoomsLock[erIndex]);
                }
            }
            Yield();
        }
        
    }
    Exit(0);
}
void Doctor()
{
    int erIndex;
    ExamSheet *holdingES;
    int pid = glodocid++;
    int possibility;
    int times = 1000;
    while (times -- > 0) 
    {
        /*-------------------------------------------------------------------
         // Doctor @ Office and Wait for the Nurse to Signal them
         //-------------------------------------------------------------------*/
        AcquireLock(doctWaitingLock[pid-1]);
        docStatus[pid-1] = (DOCSTATUS)DOCFREE;
        /*----------------------------------------------------------
         //doctor is waked up by nurse and go to the examination room
         //-------------------------------------------------------*/
        WaitCondition(doctWaitingCV[pid-1], doctWaitingLock[pid-1]);
        /*
         DEBUG('o', "Doctor [%d] is leaving their office.\n", pid);
         */
        Print("Doctor [%d] is leaving their office.\n", sizeof("Doctor [%d] is leaving their office.\n")/sizeof(char), 1, pid);
        erIndex = ndocESNo[pid-1];
        ReleaseLock(doctWaitingLock[pid-1]);
        AcquireLock(examRoomsLock[erIndex]);
        if (wallPackets[erIndex] != NULL) {
            holdingES = wallPackets[erIndex];
            holdingES -> docId = pid;
            wallPackets[erIndex] = NULL;
            if (holdingES -> patientType == 0) {
                
                /*
                 DEBUG('o', "Doctor [%d] is reading the examination sheet of Adult Patient [%d] in Examination room [%d].\n", pid, holdingES -> examSheetId, erIndex+1);
                 */
                Print("Doctor [%d] is reading the examination sheet of Adult Patient [%d] in Examination room [%d].\n", sizeof("Doctor [%d] is reading the examination sheet of Adult Patient [%d] in Examination room [%d].\n")/sizeof(char), 3, pid*65536+holdingES->examSheetId*256+(erIndex+1));
                
            } else {
                /*DEBUG('o', "Doctor [%d] is reading the examination sheet of Child Patient [%d] in Examination room [%d].\n", pid, holdingES -> examSheetId, erIndex+1);
                 */
                Print("Doctor [%d] is reading the examination sheet of Child Patient [%d] in Examination room [%d].\n", sizeof("Doctor [%d] is reading the examination sheet of Child Patient [%d] in Examination room [%d].\n")/sizeof(char), 3, pid*65536+holdingES->examSheetId*256+(erIndex+1));
            }
            /*---------------------------------------
             // First Diagnose of Patient
             //---------------------------------------*/
            if (holdingES -> xrayStatus ==(XRAYSTATUS)XRAYUNKNOWN && 
                holdingES -> shotStatus == (SHOTSTATUS)SHOTUNKNOWN ) 
            {
                /*------------------------------------------------------
                 //determine whether a patient needs xray
                 //-------------------------------------------------------*/
                
                holdingES -> patientTask = (PATIENTTASK)PATDIAGNOSE;
                WaitCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
                possibility = Random(4);
                if (possibility != 0) {
                    holdingES -> xrayStatus = (XRAYSTATUS)XRAYUNREQUIRED;
                    holdingES -> xrayImages = 0;
                } else {
                    holdingES -> xrayImages = Random(3)+1;
                    if (holdingES -> patientType == 0) {
                        /*
                         DEBUG('o', "Doctor [%d] notes down in the sheet that Xray is needed for Adult Patient [%d] in Examination room [%d].\n", pid, holdingES -> examSheetId, erIndex+1);
                         */
                        Print("Doctor [%d] notes down in the sheet that Xray is needed for Adult Patient [%d] in Examination room [%d].\n", sizeof("Doctor [%d] notes down in the sheet that Xray is needed for Adult Patient [%d] in Examination room [%d].\n")/sizeof(char), 3, pid*65536+holdingES->examSheetId*256+(erIndex+1));
                    }
                    else
                    {/*
                      DEBUG('o', "Doctor [%d] notes down in the sheet that Xray is needed for Child Patient [%d] in Examination room [%d].\n", pid, holdingES -> examSheetId, erIndex+1);
                      */
                        Print("Doctor [%d] notes down in the sheet that Xray is needed for Child Patient [%d] in Examination room [%d].\n", sizeof("Doctor [%d] notes down in the sheet that Xray is needed for Child Patient [%d] in Examination room [%d].\n")/sizeof(char), 3, pid*65536+holdingES->examSheetId*256+(erIndex+1));
                    }
                    SignalCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
                    WaitCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
                    holdingES->patientTask = (PATIENTTASK)PATGOXRAY;
                }
                /*------------------------------------------------------
                 //determine whether a patient needs a shot
                 //-------------------------------------------------------*/
                possibility = Random(4);
                
                if (possibility != 0) {
                    holdingES->shotStatus = (SHOTSTATUS)SHOTUNREQUIRED;
                } else {
                    holdingES->shotStatus = (SHOTSTATUS)SHOTREQUIRED;
                    if (holdingES -> patientType == 0) {
                        /*
                         DEBUG('o', "Doctor [%d] notes down in the sheet that Adult Patient [%d] needs to be given a shot in Examination room [%d].\n", pid, holdingES -> examSheetId, erIndex+1);
                         */
                        Print("Doctor [%d] notes down in the sheet that Adult Patient [%d] needs to be given a shot in Examination room [%d].\n", sizeof("Doctor [%d] notes down in the sheet that Adult Patient [%d] needs to be given a shot in Examination room [%d].\n")/sizeof(char), 3, pid*65536+holdingES->examSheetId*256+(erIndex+1));
                    } else {
                        /*
                         DEBUG('o', "Doctor [%d] notes down in the sheet that Child Patient [%d] needs to be given a shot in Examination room [%d].\n", pid, holdingES -> examSheetId, erIndex+1);
                         */
                        Print("Doctor [%d] notes down in the sheet that Child Patient [%d] needs to be given a shot in Examination room [%d].\n", sizeof("Doctor [%d] notes down in the sheet that Child Patient [%d] needs to be given a shot in Examination room [%d].\n")/sizeof(char), 3, pid*65536+holdingES->examSheetId*256+(erIndex+1));
                    }
                    SignalCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
                    WaitCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
                }
                if (holdingES -> xrayStatus == (XRAYSTATUS)XRAYUNREQUIRED &&
                    holdingES -> shotStatus == (SHOTSTATUS)SHOTUNREQUIRED)
                {
                    if (holdingES -> patientType == 0) {
                        /*
                         DEBUG('o', "Doctor [%d] diagnoses Adult Patient [%d] to be fine and is leaving Examination Room [%d].\n", pid, holdingES -> examSheetId, erIndex+1);
                         */
                        Print("Doctor [%d] diagnoses Adult Patient [%d] to be fine and is leaving Examination Room [%d].\n", sizeof("Doctor [%d] diagnoses Adult Patient [%d] to be fine and is leaving Examination Room [%d].\n")/sizeof(char), 3, pid*65536+holdingES->examSheetId*256+(erIndex+1));
                    } else {
                        /*
                         DEBUG('o', "Doctor [%d] diagnoses Child Patient [%d] to be fine and is leaving Examination Room [%d].\n", pid, holdingES -> examSheetId, erIndex+1);
                         */
                        Print("Doctor [%d] diagnoses Child Patient [%d] to be fine and is leaving Examination Room [%d].\n", sizeof("Doctor [%d] diagnoses Child Patient [%d] to be fine and is leaving Examination Room [%d].\n")/sizeof(char), 3, pid*65536+holdingES->examSheetId*256+(erIndex+1));
                    }
                }
                holdingES -> esProcess = (ESPROCESS)ESIDLE;
                wallPackets[erIndex] = holdingES;
                holdingES = NULL;
                /*------------------------------------------------------
                 //after diagnose,patient is waiting for nurse to escort to go proper place
                 //-------------------------------------------------------*/
                
                SignalCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
                WaitCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
                ReleaseLock(examRoomsLock[erIndex]);
                /*
                 DEBUG('o', "Doctor [%d] has left Examination Room [%d].\n", pid, erIndex+1);
                 */
                Print("Doctor [%d] has left Examination Room [%d].\n", sizeof("Doctor [%d] has left Examination Room [%d].\n")/sizeof(char), 2, pid*256+(erIndex+1));
            } else {
                /*-------------------------------------------
                 // 2nd Diagnose of Patient With XRAY Finished
                 //-------------------------------------------*/
                if (holdingES -> xrayStatus == (XRAYSTATUS)XRAYHALFDONE) {
                    if (holdingES -> patientType == (PATIENTTYPE)ADULT) {
                        /*
                         DEBUG('o', "Doctor [%d] is examining the Xrays of Adult Patient [%d] in Examination room [%d].\n", pid, holdingES -> examSheetId, erIndex+1);
                         */
                        Print("Doctor [%d] is examining the Xrays of Adult Patient [%d] in Examination room [%d].\n", sizeof("Doctor [%d] is examining the Xrays of Adult Patient [%d] in Examination room [%d].\n")/sizeof(char), 3, pid*65536+holdingES->examSheetId*256+(erIndex+1));
                    } else {
                        /*
                         DEBUG('o', "Doctor [%d] is examining the Xrays of Child Patient [%d] in Examination room [%d].\n", pid, holdingES -> examSheetId, erIndex+1);
                         */
                        Print("Doctor [%d] is examining the Xrays of Child Patient [%d] in Examination room [%d].\n", sizeof("Doctor [%d] is examining the Xrays of Child Patient [%d] in Examination room [%d].\n")/sizeof(char), 3, pid*65536+holdingES->examSheetId*256+(erIndex+1));
                    }
                    holdingES -> patientTask = (PATIENTTASK)PATDIAGNOSEXRAY;
                    holdingES -> docId = pid;
                    WaitCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
                    holdingES -> xrayStatus = (XRAYSTATUS)XRAYDONE;
                    holdingES -> esProcess = (ESPROCESS)ESIDLE;
                    SignalCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
                    wallPackets[erIndex] = holdingES;
                    holdingES = NULL;
                    ReleaseLock(examRoomsLock[erIndex]);
                    /*
                     DEBUG('o', "Doctor [%d] has left Examination Room [%d].\n", pid, erIndex+1);
                     */
                    Print("Doctor [%d] has left Examination Room [%d].\n", sizeof("Doctor [%d] has left Examination Room [%d].\n")/sizeof(char), 2, pid*256+(erIndex+1));
                } else {
                    ReleaseLock(examRoomsLock[erIndex]);
                    Print("Error!!!!!!!.\n", sizeof("Error!!!!!!!.\n")/sizeof(char), 0, 0);
                }
            }
        } else {
            ReleaseLock(examRoomsLock[erIndex]);
            Print("Error?!.\n", sizeof("Error?!.\n")/sizeof(char), 0, 0);
        }
        /*
         DEBUG('o', "Doctor [%d] is going to their office.\n", pid);
         */
        /*------------------------------------------------------
         //after diagnose, doctor goes back to office
         //-------------------------------------------------------*/
        Print("Doctor [%d] is going to their office.\n", sizeof("Doctor [%d] is going to their office.\n")/sizeof(char), 1, pid);
        Yield();
    }
    Exit(0);
}
void XRayTechnician() 
{
    
    int erIndex;
    ExamSheet *holdingES;
    int i;
    int pid = gloxrayid++;
    int times = 1000;
    while (times-- > 0) 
    {   
        /*------------------------------------------------------
         //check whether the user input number is valid
         //-------------------------------------------------------*/
        if (pid > xrayRoomsCount) 
        {
            Print("ERROR X-Ray Rooms Count.\n", sizeof("ERROR X-Ray Rooms Count.\n")/sizeof(char), 0, 0);
            break;
        }
        /*------------------------------------------------------
         //xray technician enters the xray room and waits for nurse signal
         //-------------------------------------------------------*/
        
        AcquireLock(xrayRoomsLock[pid-1]);
        WaitCondition(xrayRoomsCV[pid-1], xrayRoomsLock[pid-1]);
        /*------------------------------------------------------
         //one patient comes to xray room and begins to take xray
         //-------------------------------------------------------*/
        holdingES = xrayRoomsES[pid-1];
        xrayRoomsES[pid-1] = NULL;
        ReleaseLock(xrayRoomsLock[pid-1]);
        erIndex = holdingES -> erId;
        AcquireLock(examRoomsLock[erIndex]);
        if (holdingES -> patientType == (PATIENTTYPE)ADULT) 
        {/*
          DEBUG('x', "Xray technician [%d] asks Adult Patient [%d] to get on the table.\n", pid, holdingES -> examSheetId);
          */
            Print("Xray technician [%d] asks Adult Patient [%d] to get on the table.\n", sizeof("Xray technician [%d] asks Adult Patient [%d] to get on the table.\n")/sizeof(char), 2, pid*256+holdingES->examSheetId);
        }
        else
        {/* 
          DEBUG('x', "Xray technician [%d] asks Child Patient [%d] to get on the table.\n", pid, holdingES -> examSheetId);
          */
            Print("Xray technician [%d] asks Child Patient [%d] to get on the table.\n", sizeof("Xray technician [%d] asks Child Patient [%d] to get on the table.\n")/sizeof(char), 2, pid*256+holdingES->examSheetId);
        }
        /*------------------------------------------------------
         //wait patient to prepare
         //-------------------------------------------------------*/
        SignalCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
        WaitCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
        /*------------------------------------------------------
         //take xray images repetablely
         //-------------------------------------------------------*/
        for (i = 0; i<holdingES -> xrayImages; i++) 
        {
            if (holdingES -> patientType == (PATIENTTYPE)ADULT)
            {/*   
              DEBUG('x', "Xray Technician [%d] asks Adult Patient [%d] to move.\n", pid, 
              holdingES -> examSheetId);
              */
                Print("Xray Technician [%d] asks Adult Patient [%d] to move.\n", sizeof("Xray Technician [%d] asks Adult Patient [%d] to move.\n")/sizeof(char), 2, pid*256+holdingES->examSheetId);
            }
            else
            {/*
              DEBUG('x', "Xray Technician [%d] asks Child Patient [%d] to move.\n", pid, 
              holdingES -> examSheetId);
              */
                Print("Xray Technician [%d] asks Child Patient [%d] to move.\n", sizeof("Xray Technician [%d] asks Child Patient [%d] to move.\n")/sizeof(char), 2, pid*256+holdingES->examSheetId);
            }
            SignalCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
            WaitCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
            if (holdingES -> patientType == (PATIENTTYPE)ADULT)
            {/*
              DEBUG('x', "Xray Technician [%d] takes an Xray Image of Adult Patient [%d].\n", pid, holdingES -> examSheetId);*/
                Print("Xray Technician [%d] takes an Xray Image of Adult Patient [%d].\n", sizeof("Xray Technician [%d] takes an Xray Image of Adult Patient [%d].\n")/sizeof(char), 2, pid*256+holdingES->examSheetId);
            }
            else
            {/*
              DEBUG('x', "Xray Technician [%d] takes an Xray Image of Child Patient [%d].\n", pid, holdingES -> examSheetId);*/
                Print("Xray Technician [%d] takes an Xray Image of Child Patient [%d].\n", sizeof("Xray Technician [%d] takes an Xray Image of Child Patient [%d].\n")/sizeof(char), 2, pid*256+holdingES->examSheetId);
            }
            SignalCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
            WaitCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
        }
        holdingES -> xrayStatus = (XRAYSTATUS)XRAYHALFDONE;
        holdingES -> xrayResult = (XRAYRESULT)(Random(3));
        /*------------------------------------------------------
         //xray writes the xray result on exmination sheet
         //-------------------------------------------------------*/
        
        switch (holdingES -> xrayResult) 
        {
            case (XRAYRESULT)NOTHING:
                if (holdingES -> patientType == (PATIENTTYPE)ADULT)
                {/*
                  DEBUG('x', "Xray Technician [%d] records nothing on Adult Patient [%d]'s examination sheet.\n", pid, holdingES -> examSheetId);*/
                    Print("Xray Technician [%d] records nothing on Adult Patient [%d]'s examination sheet.\n", sizeof("Xray Technician [%d] records nothing on Adult Patient [%d]'s examination sheet.\n")/sizeof(char), 2, pid*256+holdingES->examSheetId);
                }
                else
                {/*
                  DEBUG('x', "Xray Technician [%d] records nothing on Child Patient [%d]'s examination sheet.\n", pid, holdingES -> examSheetId);*/
                    Print("Xray Technician [%d] records nothing on Child Patient [%d]'s examination sheet.\n", sizeof("Xray Technician [%d] records nothing on Child Patient [%d]'s examination sheet.\n")/sizeof(char), 2, pid*256+holdingES->examSheetId);
                }
                break;
            case (XRAYRESULT)FRACTURE:
                if (holdingES -> patientType == (PATIENTTYPE)ADULT)
                {/*
                  DEBUG('x', "Xray Technician [%d] records compound fracture on Adult Patient [%d]'s examination sheet.\n", pid, holdingES -> examSheetId);*/
                    Print("Xray Technician [%d] records compound fracture on Adult Patient [%d]'s examination sheet.\n", sizeof("Xray Technician [%d] records compound fracture on Adult Patient [%d]'s examination sheet.\n")/sizeof(char), 2, pid*256+holdingES->examSheetId);
                }
                else
                {/*
                  DEBUG('x', "Xray Technician [%d] records compound fracture on Child Patient [%d]'s examination sheet.\n", pid, holdingES -> examSheetId);
                  */
                    Print("Xray Technician [%d] records compound fracture on Child Patient [%d]'s examination sheet.\n", sizeof("Xray Technician [%d] records compound fracture on Child Patient [%d]'s examination sheet.\n")/sizeof(char), 2, pid*256+holdingES->examSheetId);
                }
                break;
                
            case (XRAYRESULT)BREAK:
                if (holdingES -> patientType == (PATIENTTYPE)ADULT)
                {/*
                  DEBUG('x', "Xray Technician [%d] records break on Adult Patient [%d]'s examination sheet.\n", pid, holdingES -> examSheetId);*/
                    Print("Xray Technician [%d] records break on Adult Patient [%d]'s examination sheet.\n", sizeof("Xray Technician [%d] records break on Adult Patient [%d]'s examination sheet.\n")/sizeof(char), 2, pid*256+holdingES->examSheetId);
                }
                else
                {/*
                  DEBUG('x', "Xray Technician [%d] records break on Child Patient [%d]'s examination sheet.\n", pid, holdingES -> examSheetId);*/
                    Print("Xray Technician [%d] records break on Child Patient [%d]'s examination sheet.\n", sizeof("Xray Technician [%d] records break on Child Patient [%d]'s examination sheet.\n")/sizeof(char), 2, pid*256+holdingES->examSheetId);
                }
                break;
            default:
                break;
        }
        /*------------------------------------------------------
         //tells the patient to wait nurse in xray waiting room 
         //-------------------------------------------------------*/
        if (holdingES -> patientType == (PATIENTTYPE)ADULT)
        {/* 
          DEBUG('x', "X-ray Technician [%d] tells Adult Patient [%d] to wait in Xray waiting room.\n", pid, holdingES -> examSheetId);*/
            Print("X-ray Technician [%d] tells Adult Patient [%d] to wait in Xray waiting room.\n", sizeof("X-ray Technician [%d] tells Adult Patient [%d] to wait in Xray waiting room.\n")/sizeof(char), 2, pid*256+holdingES->examSheetId);
        }
        else
        {/* 
          DEBUG('x', "X-ray Technician [%d] tells Parent [%d] to wait in Xray waiting room.\n", pid, holdingES -> examSheetId);
          */
            Print("X-ray Technician [%d] tells Parent [%d] to wait in Xray waiting room.\n", sizeof("X-ray Technician [%d] tells Parent [%d] to wait in Xray waiting room.\n")/sizeof(char), 2, pid*256+holdingES->examSheetId);
        }
        SignalCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
        WaitCondition(examRoomsCV[erIndex], examRoomsLock[erIndex]);
        AcquireLock(xrayWaitingLock);
        Insert(&xrayWaitingES, holdingES);
        ReleaseLock(xrayWaitingLock);
        /*------------------------------------------------------
         //xray puts the patient's examination sheet into wall pocket for nurse to take away
         //-------------------------------------------------------*/
        if (holdingES -> patientType == (PATIENTTYPE)ADULT) 
        {/*
          DEBUG('x', "X-ray Technician [%d] puts Adult Patient [%d] in Xray waiting room wall pocket.\n", pid, holdingES -> examSheetId);*/
            Print("X-ray Technician [%d] puts Adult Patient [%d] in Xray waiting room wall pocket.\n", sizeof("X-ray Technician [%d] puts Adult Patient [%d] in Xray waiting room wall pocket.\n")/sizeof(char), 2, pid*256+holdingES->examSheetId);
        }
        else
        {/*
          DEBUG('x', "X-ray Technician [%d] puts Parent [%d] in Xray waiting room wall pocket.\n", pid, holdingES -> examSheetId);*/
            Print("X-ray Technician [%d] puts Parent [%d] in Xray waiting room wall pocket.\n", sizeof("X-ray Technician [%d] puts Parent [%d] in Xray waiting room wall pocket.\n")/sizeof(char), 2, pid*256+holdingES->examSheetId);
        }
        holdingES = NULL;
        ReleaseLock(examRoomsLock[erIndex]);
        AcquireLock(xrayRoomsStatusLock);
        xrayRoomsStatus[pid-1] = (XRRSTATUS)XRRFREE;
        ReleaseLock(xrayRoomsStatusLock);
    }
    Exit(0);
}
void Cashier()
{
    ExamSheet *holdingES;
    int totalAmount;
    int i;
    int times = 1000;
    while (times-- > 0) {
        /*-----------------------------------------------------
         // Cashier wait for Nurse bringing patient to his desk
         //-----------------------------------------------------*/
        AcquireLock(cashWaitingLock);
        if (cashWaitingCount > 0) 
        {
            cashWaitingCount --;
            SignalCondition(cashWaitingCV, cashWaitingLock);
            AcquireLock(cashLock);
            ReleaseLock(cashWaitingLock);
            WaitCondition(cashCV, cashLock);
            /*------------------------------------------
             // Cashier gets the ES and calculate the fee
             //------------------------------------------*/
            
            holdingES = cashExamSheet;
            if(holdingES -> patientType == 0)
            {/*
              DEBUG('c', "Cashier receives the examination sheet from Adult Patient [%d].\n",holdingES -> examSheetId);*/
                Print("Cashier receives the examination sheet from Adult Patient [%d].\n", sizeof("Cashier receives the examination sheet from Adult Patient [%d].\n")/sizeof(char), 1, holdingES->examSheetId);
            }
            else
            {/*
              DEBUG('c', "Cashier receives the examination sheet from Child Patient [%d] from Parent [%d].\n",holdingES -> examSheetId,holdingES -> examSheetId);*/
                Print("Cashier receives the examination sheet from Child Patient [%d] from Parent [%d].\n", sizeof("Cashier receives the examination sheet from Child Patient [%d] from Parent [%d].\n")/sizeof(char), 2, holdingES->examSheetId*256+holdingES->examSheetId);
            }
            
            totalAmount = 20;
            if (holdingES -> xrayStatus == (XRAYSTATUS)XRAYDONE) {
                for (i=0; i<holdingES->xrayImages; i++) {
                    totalAmount += 10;
                }
                totalAmount += 20;
                
            }
            if (holdingES -> shotStatus == (SHOTSTATUS)SHOTDONE) {
                totalAmount += 20;
            }
            holdingES -> totalAmount = totalAmount;
            if(holdingES -> patientType == 0)
            {/* 
              DEBUG('c', "Cashier reads the examination sheet of Adult Patient [%d] and asks him to pay $ [%f].\n", holdingES -> examSheetId,holdingES -> totalAmount);
              */
                Print("Cashier reads the examination sheet of Adult Patient [%d] and asks him to pay $ [%d].\n", sizeof("Cashier reads the examination sheet of Adult Patient [%d] and asks him to pay $ [%d].\n")/sizeof(char), 2, holdingES->examSheetId*256+holdingES->totalAmount);
            }
            else 
            {/* 
              DEBUG('c', "Cashier reads the examination sheet of Child Patient [%d] and asks Parent [%d] to pay $ [%f].\n", holdingES -> examSheetId,holdingES -> examSheetId,holdingES -> totalAmount);*/
                Print("Cashier reads the examination sheet of Child Patient [%d] and asks Parent [%d] to pay $ [%d].\n", sizeof("Cashier reads the examination sheet of Child Patient [%d] and asks Parent [%d] to pay $ [%d].\n")/sizeof(char), 3, holdingES->examSheetId*65536+holdingES->examSheetId*256+holdingES->totalAmount);
            }
            SignalCondition(cashCV, cashLock);
            WaitCondition(cashCV, cashLock);
            if(holdingES -> patientType == 0)
            {/* 
              DEBUG('c', "Cashier accepts $[%f] from Adult Patient [%d].\n",
              holdingES-> totalAmount,holdingES -> examSheetId);*/
                Print("Cashier accepts $[%d] from Adult Patient [%d].\n", sizeof("Cashier accepts $[%d] from Adult Patient [%d].\n")/sizeof(char), 2, holdingES->totalAmount*256+holdingES->examSheetId);
            }
            else
            {/* 
              DEBUG('c', "Cashier accepts $[%f] from Parent [%d].\n", 
              holdingES-> totalAmount,holdingES -> examSheetId);*/
                Print("Cashier accepts $[%d] from Parent [%d].\n", sizeof("Cashier accepts $[%d] from Parent [%d].\n")/sizeof(char), 2, holdingES->totalAmount*256+holdingES->examSheetId);
            }
            if(holdingES -> patientType == 0)
            {/*
              DEBUG('c', "Cashier gives a receipt of $[%f] to Adult Patient [%d].\n", holdingES->totalAmount, holdingES -> examSheetId);*/
                Print("Cashier gives a receipt of $[%d] to Adult Patient [%d].\n", sizeof("Cashier gives a receipt of $[%d] to Adult Patient [%d].\n")/sizeof(char), 2, holdingES->totalAmount*256+holdingES->examSheetId);
            }
            else
            {/*
              DEBUG('c', "Cashier gives a receipt of $[%f] to Parent [%d].\n",holdingES -> totalAmount, holdingES -> examSheetId);*/
                Print("Cashier gives a receipt of $[%d] to Parent [%d].\n", sizeof("Cashier gives a receipt of $[%d] to Parent [%d].\n")/sizeof(char), 2, holdingES->totalAmount*256+holdingES->examSheetId);
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
    Exit(0);
}
void GlobalVariablesInit()
{
    int i = 0;
    waitingRoomLock = CreateLock("WaitingRoomLock", sizeof("WaitingRoomLock")/sizeof(char));
    wrnLock = CreateLock("wrnLock", sizeof("wrnLock")/sizeof(char));
    nwrnLock = CreateLock("nwrnLock", sizeof("nwrnLock")/sizeof(char));
    for (; i<MAX_CHILD; i++) {
        childWaitingLock[i] = CreateLock("childWaitingLock", sizeof("childWaitingLock")/sizeof(char));
    }
    childExamSheetLock = CreateLock("childExamSheetLock", sizeof("childExamSheetLock")/sizeof(char));
    ersStatusLock = CreateLock("ersStatusLock", sizeof("ersStatusLock")/sizeof(char));
    broadCastIdLock = CreateLock("broadCastIdLock", sizeof("broadCastIdLock")/sizeof(char));
    for (i=0; i<MAX_EXAMROOMS; i++) {
        examRoomsLock[i] = CreateLock("examRoomsLock", sizeof("examRoomsLock")/sizeof(char));
    }
    for (i=0; i<MAX_DOCTORS; i++) {
        doctWaitingLock[i] = CreateLock("doctWaitingLock", sizeof("doctWaitingLock")/sizeof(char));
    }
    for (i=0; i<MAX_XRAY; i++) {
        xrayRoomsLock[i] = CreateLock("xrayRoomsLock", sizeof("xrayRoomsLock")/sizeof(char));
    }
    xrayWaitingLock = CreateLock("xrayWaitingLock", sizeof("xrayWaitingLock")/sizeof(char));
    cabinetLock = CreateLock("cabinetLock", sizeof("cabinetLock")/sizeof(char));
    cashWaitingLock = CreateLock("cashWaitingLock", sizeof("cashWaitingLock")/sizeof(char));
    cashLock = CreateLock("cashLock", sizeof("cashLock")/sizeof(char));
    xrayRoomsStatusLock = CreateLock("xrayRoomsStatusLock", sizeof("xrayRoomsStatusLock")/sizeof(char));
    
    waitingRoomCV = CreateCondition("waitingRoomCV", sizeof("waitingRoomCV")/sizeof(char));
    wrnCV = CreateCondition("wrnCV", sizeof("wrnCV")/sizeof(char));
    for (i=0; i<MAX_CHILD; i++) {
        childWaitingCV[i] = CreateCondition("childWaitingCV", sizeof("childWaitingCV")/sizeof(char));
    }
    nwrnCV = CreateCondition("nwrnCV", sizeof("nwrnCV")/sizeof(char));
    nwrnWaitingCV = CreateCondition("nwrnWaitingCV", sizeof("nwrnWaitingCV")/sizeof(char));
    broadCastIdCV = CreateCondition("broadCastIdCV", sizeof("broadCastIdCV")/sizeof(char));
    bcIdWaitingCV = CreateCondition("bcIdWaitingCV", sizeof("bcIdWaitingCV")/sizeof(char));
    for (i=0; i<MAX_EXAMROOMS; i++) {
        examRoomsCV[i] = CreateCondition("examRoomsCV", sizeof("examRoomsCV")/sizeof(char));
    }
    for (i=0; i<MAX_DOCTORS; i++) {
        doctWaitingCV[i] = CreateCondition("doctWaitingCV", sizeof("doctWaitingCV")/sizeof(char));
    }
    for (i=0; i<MAX_XRAY; i++) {
        xrayRoomsCV[i] = CreateCondition("xrayRoomsCV", sizeof("xrayRoomsCV")/sizeof(char));
    }
    cashWaitingCV = CreateCondition("cashWaitingCV", sizeof("cashWaitingCV")/sizeof(char));
    cashCV = CreateCondition("cashCV", sizeof("cashCV")/sizeof(char));
    wrWaitingCount = 0;
    nwrnWaitingCount = 0;
    wrnStatus = (WRNSTATUS)WRNFREE;
    wrnTask = (WRNTASK)NONE;
    wrnExamSheet = NULL;
    examRooms = 3;
    for (i=0; i<MAX_EXAMROOMS; i++) {
        examRoomsStatus[i] = (ERSTATUS)ERFREE;
    }
    broadCastId = -1;
    for (i=0; i<MAX_EXAMROOMS; i++) {
        wallPackets[i] = NULL;
    }
    for (i=0; i<MAX_DOCTORS; i++) {
        docStatus[i] = (DOCSTATUS)DOCFREE;
        ndocESNo[i] = -1;
    }
    docCount = 2;
    for (i=0; i<MAX_XRAY; i++) {
        xrayRoomsES[i] = NULL;
        xrayRoomsStatus[i] = (XRRSTATUS)XRRFREE;
    }
    xrayRoomsCount = 2;
    cabinetCount = 0;
    cashExamSheet = NULL;
    cashWaitingCount = 0;
    bcIdStatus = (BROADCASTIDSTATUS)BROADFREE;
    wrnServiceId = -1;
    nwrnId = -1;
    nwrnExamSheet = NULL;
    childExamSheet = NULL;
    for (i=0; i<MAX_CHILD; i++) {
        childWaitingTask[i] = (CHILDTASK)CHILDNONE;
    }
    patients=5;
    nurses=5;
    doctors=2;
    xrays=2;
    wrns=1;
    cashiers=1;
    children=0;
    wrnCompleteESList.front = 0;
    wrnCompleteESList.rear = 0;
    wrnCompleteESList.range = MAX_LISTELEMENT;
    xrayWaitingES.front = 0;
    xrayWaitingES.rear = 0;
    xrayWaitingES.range = MAX_LISTELEMENT;
    glopatid=31;
    glodocid=1;
    glonurid=1;
    gloxrayid=1;
    gloparid=1;
    glochid=1;
    
    
}
void GlobalVariablesClean()
{
    int i = 0;
    DestroyLock(waitingRoomLock);
    DestroyLock(wrnLock);
    DestroyLock(nwrnLock);
    for (; i<MAX_CHILD; i++) {
        DestroyLock(childWaitingLock[i]);
    }
    DestroyLock(childExamSheetLock);
    DestroyLock(ersStatusLock);
    DestroyLock(broadCastIdLock);
    for (i=0; i<MAX_EXAMROOMS; i++) {
        DestroyLock(examRoomsLock[i]);
    }
    for (i=0; i<MAX_DOCTORS; i++) {
        DestroyLock(doctWaitingLock[i]);
    }
    for (i=0; i<MAX_XRAY; i++) {
        DestroyLock(xrayRoomsLock[i]);
    }
    DestroyLock(xrayWaitingLock);
    DestroyLock(cabinetLock);
    DestroyLock(cashWaitingLock);
    DestroyLock(cashLock);
    DestroyLock(xrayRoomsStatusLock);
    
    DestroyCondition(waitingRoomCV);
    DestroyCondition(wrnCV);
    for (i=0; i<MAX_CHILD; i++) {
        DestroyCondition(childWaitingCV[i]);
    }
    DestroyCondition(nwrnCV);
    DestroyCondition(nwrnWaitingCV);
    DestroyCondition(broadCastIdCV);
    DestroyCondition(bcIdWaitingCV);
    for (i=0; i<MAX_EXAMROOMS; i++) {
        DestroyCondition(examRoomsCV[i]);
    }
    for (i=0; i<MAX_DOCTORS; i++) {
        DestroyCondition(doctWaitingCV[i]);
    }
    for (i=0; i<MAX_XRAY; i++) {
        DestroyCondition(xrayRoomsCV[i]);
    }
    DestroyCondition(cashWaitingCV);
    DestroyCondition(cashCV);
}
int HospitalGetInput()
{
    int flag;
    Print("Enter the Number of Doctors: ", sizeof("Enter the Number of Doctors: ")/sizeof(char), 0, 0);
    docCount = GetInput();
    Print("Enter the Number of Nurses: ", sizeof("Enter the Number of Nurses: ")/sizeof(char), 0, 0);
    nurses = GetInput();
    Print("Enter the Number of XRay Technician/Rooms: ", sizeof("Enter the Number of XRay Technician/Rooms: ")/sizeof(char), 0, 0);
    xrayRoomsCount = GetInput();
    Print("Enter the Number of Patients: ", sizeof("Enter the Number of Patients: ")/sizeof(char), 0, 0);
    patients = GetInput();
    Print("Enter the Number of ExamRooms: ", sizeof("Enter the Number of ExamRooms: ")/sizeof(char), 0, 0);
    examRooms = GetInput();
    wrns = 1;
    cashiers = 1;
    doctors = docCount;
    xrays = xrayRoomsCount;
    flag = true;
    if (examRooms > MAX_EXAMROOMS) {
        flag = false;
        Print("Too many ERS you input, the MAX ERS is %d.\n", sizeof("Too many ERS you input, the MAX ERS is %d.\n")/sizeof(char), 1, MAX_EXAMROOMS);
    }
    if (examRooms < MIN_EXAMROOMS) {
        flag = false;
        Print("Too few ERS you input, the MIN ERS is %d.\n", sizeof("Too few ERS you input, the MIN ERS is %d.\n")/sizeof(char), 1, MIN_EXAMROOMS);
    }
    if (xrayRoomsCount > MAX_XRAY) {
        flag = false;
        Print("Too many XRAYS you input, the MAX XRAYS is %d.\n", sizeof("Too many XRAYS you input, the MAX XRAYS is %d.\n")/sizeof(char), 1, MAX_XRAY);
    }
    if (xrayRoomsCount < MIN_XRAY) {
        flag = false;
        Print("Too few XRAYS you input, the MIN XRAYS is %d.\n", sizeof("Too few XRAYS you input, the MIN XRAYS is %d.\n")/sizeof(char), 1, MIN_XRAY);
    }
    if (docCount > MAX_DOCTORS) {
        flag = false;
        Print("Too many DOCTORS you input, the MAX DOCTORS is %d.\n", sizeof("Too many DOCTORS you input, the MAX DOCTORS is %d.\n")/sizeof(char), 1, MAX_DOCTORS);
    }
    
    if (docCount < MIN_DOCTORS) {
        flag = false;
        Print("Too few DOCTORS you input, the MIN DOCTORS is %d.\n", sizeof("Too few DOCTORS you input, the MIN DOCTORS is %d.\n")/sizeof(char), 1, MIN_DOCTORS);
    }
    if (nurses > MAX_NURSES) {
        flag = false;
        Print("Too many NURSES you input, the MAX NURSES is %d.\n", sizeof("Too many NURSES you input, the MAX NURSES is %d.\n")/sizeof(char), 1, MAX_NURSES);
    }
    if (nurses < MIN_NURSES) {
        flag = false;
        Print("Too few NURSES you input, the MIN NURSES is %d.\n", sizeof("Too few NURSES you input, the MIN NURSES is %d.\n")/sizeof(char), 1, MIN_NURSES);
    }
    return flag;
}
void main()
{
    int i;
    GlobalVariablesInit();
    if(!HospitalGetInput()) {
        Exit(0);
    }

    for (i=0; i<wrns; i++) {
        Fork(WaitingRoomNurse);
    }
    for (i=0; i<patients;i++) {
        if (Random(2)) {
            Fork(Parent);
            children++;
        } else {
            Fork(Patient);
        }
    }
    Print("******************************************************\n", sizeof("******************************************************\n")/sizeof(char), 0, 0);
    Print("Starting System Test Case\n", sizeof("Starting System Test Case\n")/sizeof(char), 0, 0);
    Print("******************************************************\n", sizeof("******************************************************\n")/sizeof(char), 0, 0); 
    Print("Number of Doctors(%d to %d) = [%d]\n", sizeof("Number of Doctors(%d to %d) = [%d]\n")/sizeof(char), 3, MIN_DOCTORS*65536+MAX_DOCTORS*256+doctors);
    Print("Number of Nurse(%d to %d) = [%d]\n", sizeof("Number of Nurse(%d to %d) = [%d]\n")/sizeof(char), 3, MIN_NURSES*65536+MAX_NURSES*256+nurses);
    Print("Number of XRay Technicians/Rooms(%d to %d) = [%d]\n", sizeof("Number of XRay Technicians/Rooms(%d to %d) = [%d]\n")/sizeof(char), 3, MIN_XRAY*65536+MAX_XRAY*256+xrays);
    Print("Number of Patients = [%d]\n", sizeof("Number of Patients = [%d]\n")/sizeof(char), 1, patients);
    Print("Number of Parents/Child Patients = [%d].\n", sizeof("Number of Parents/Child Patients = [%d].\n")/sizeof(char), 1, children);
    Print("Number of Examination Rooms(%d to %d) = [%d]\n", sizeof("Number of Examination Rooms(%d to %d) = [%d]\n")/sizeof(char), 3, MIN_EXAMROOMS*65536+MAX_EXAMROOMS*256+examRooms);
    Print("******************************************************\n", sizeof("******************************************************\n")/sizeof(char), 0, 0);
    for (i=0; i<nurses; i++) {
        Fork(Nurse);
    }
    for (i=0; i<doctors; i++) {
        Fork(Doctor);
    }
    for (i=0; i<xrays; i++) {
        Fork(XRayTechnician);
    }
    for (i=0; i<cashiers; i++) {
        Fork(Cashier);
    }
    GlobalVariablesClean();
    Exit(0);
}


