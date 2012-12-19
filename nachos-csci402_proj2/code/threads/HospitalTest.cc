//
//  HospitalTest.cpp
//  Nachos
//
//  Created by Shengzhe Chen on 1/28/12.
//  Copyright 2012 University of Southern California. All rights reserved.
//

#include "GlobalVariables.h"
#include "thread.h"
#include "list.h"
#include "copyright.h"

extern int examRooms;
extern int docCount;
extern int xrayRoomsCount;
extern int patients, nurses, doctors, wrns, cashiers, xrays, children; 

#include "GlobalVariables.h"

void Parent(int pid);
void Child(int pid);
void WaitingRoomNurse(int pid);
void Patient(int pid);
void Nurse(int pid);
void Doctor(int pid);
void XRayTechnician(int pid);
void Cashier(int pid);
void GlobalVariablesInit();
void GlobalVariablesClean();
bool HospitalGetInput();
void test_1();
void test_2();
void test_3();
void test_4();
void test_5();
void test_6();
void test_suite();
void HospitalTest();

List *threadList = new List();

void Parent(int pid)
{
    ExamSheet *myExamSheet = NULL;
    
    int mp_Type = 1;

    //-----------------------------------
    // Parent Interacts with WRN
    //-----------------------------------
    
    DEBUG('p', "Parent  [%d] has entered the Doctor's Office Waiting Room.\n", pid);
    
    childWaitingLock[pid-1] -> Acquire();
    
    childWaitingTask[pid-1] = (CHILDTASK)CHILDWR;
    
    childWaitingCV[pid-1] -> Wait(childWaitingLock[pid-1]);
    
    
    
    //-----------------------------------
    // Parent Wait in the line of WR
    //-----------------------------------
    
    waitingRoomLock -> Acquire();
    
    wrWaitingCount ++;
    
    DEBUG('p', "Parent [%d] gets in line of the Waiting Room Nurse to get registration form.\n", pid);
    
    waitingRoomCV -> Wait(waitingRoomLock);
    
    
    //-----------------------------------
    // Parent gets form from WR
    //-----------------------------------
    
    
    wrnStatus = (WRNSTATUS)WRNBUSY;
    
    waitingRoomLock -> Release();
    
    wrnLock -> Acquire();
    
    wrnTask = GETFORMCP;
    
    wrnServiceId = pid;
    
    wrnCV -> Signal(wrnLock);
    
    wrnCV -> Wait(wrnLock);
    
    DEBUG('p', "Parent of child patient [%d] gets the form from the Waiting Room Nurse.\n", pid);
    
    myExamSheet = wrnExamSheet;
    
    wrnCV -> Signal(wrnLock);
    
    wrnLock -> Release();
    
    
    //-----------------------------------
    // Parent Fill the Form
    //-----------------------------------
    
    myExamSheet -> name = pid;
    
    myExamSheet -> age = rand()%10 + 5;
    
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
    
    currentThread -> Yield();
    
    waitingRoomLock -> Acquire();
    
    wrWaitingCount ++;
    
    DEBUG('p', "Parent of child patient [%d] gets in line of the Waiting Room Nurse to submit the registration form.\n", pid);
    
    waitingRoomCV -> Wait(waitingRoomLock);
    
    
    //-----------------------------------
    // Parent submit the form
    //-----------------------------------
    
    waitingRoomLock -> Release();
    
    wrnLock -> Acquire();
    
    wrnTask = GIVEFORM;
    
    wrnExamSheet = myExamSheet;
    
    wrnCV -> Signal(wrnLock);
    
    DEBUG('p', "Parent of child patient [%d] submits the filled form to the Waiting Room Nurse.\n", pid);
    
    wrnLock -> Release();
    
    //---------------------------------------------
    // Parent Wait for being Called by Nurse
    //--------------------------------------------- 
    
    broadCastIdLock -> Acquire();
    
    while (broadCastId != pid) 
    {
        
        broadCastIdCV -> Wait(broadCastIdLock);
        
    }
    
    bcIdStatus = (BROADCASTIDSTATUS)BROADFREE;
    
    bcIdWaitingCV -> Signal(broadCastIdLock);
    
    broadCastIdLock -> Release();
    
    int erIndex = myExamSheet -> erId;
    
    childWaitingLock[pid-1] -> Acquire();
    
    childWaitingTask[pid-1] = (CHILDTASK)CHILDGOES;
    
    childExamSheetLock -> Acquire();
    
    childExamSheet = myExamSheet;
    
    DEBUG('p', "Parent [%d] asks Child Patient [%d] to follow them.\n", pid, pid);
    
    childWaitingCV[pid-1] -> Wait(childWaitingLock[pid-1]);
    
    childExamSheetLock -> Release();
    
    childWaitingLock[pid-1] -> Release();
    
    examRoomsLock[erIndex] -> Acquire();
    
    examRoomsCV[erIndex] -> Signal(examRoomsLock[erIndex]);
    
    examRoomsCV[erIndex] -> Wait(examRoomsLock[erIndex]);
    
    DEBUG('p', "Parent [%d] is following Nurse [%d] to Examination Room [%d].\n", pid, myExamSheet -> nurseId, erIndex+1);
    
    examRoomsCV[erIndex] -> Signal(examRoomsLock[erIndex]);
    
    DEBUG('p', "Parent [%d] has arrived at Examination Room [%d].\n", pid, erIndex+1);
    
    examRoomsCV[erIndex] -> Wait(examRoomsLock[erIndex]);
    
    
    //-----------------------------------
    // Parent tell the SYMPTOM to Nurse
    //-----------------------------------
    
    myExamSheet -> illness = (SYMPTOM)(rand()%3);
    
    switch (myExamSheet -> illness) 
    {
            
        case (SYMPTOM)SYMPAIN:
            
            DEBUG('p', "Parent [%d] says, \"His symptoms are Pain\".\n", pid);
            
            break;
            
        case (SYMPTOM)SYMNAUSEA:
            
            DEBUG('p', "Parent [%d] says, \"His symptoms are Nausea\".\n", pid);
            
            break;
            
        case (SYMPTOM)SYMHEAR:
            
            DEBUG('p', "Parent [%d] says, \"His symptoms are Hear Alien Voices\".\n", pid);
            
            break;
            
        default:
            break;
    }
    
    examRoomsCV[erIndex] -> Signal(examRoomsLock[erIndex]);
    
    examRoomsCV[erIndex] -> Wait(examRoomsLock[erIndex]);
    
    examRoomsLock[erIndex] -> Release();
    
    while (true) 
    {
        
        examRoomsLock[erIndex] -> Acquire();
        
        
        //-----------------------------------
        // All things are Done.
        //-----------------------------------
        
        if (myExamSheet -> patientTask == (PATIENTTASK)PATDONE) 
        {
            childWaitingLock[pid-1] -> Acquire();
            
            childWaitingTask[pid-1] = (CHILDTASK)CHILDOVER;
            
            childWaitingCV[pid-1] -> Wait(childWaitingLock[pid-1]);
            
            childWaitingLock[pid-1] -> Release();

            break;            
        } 
        
        //-----------------------------------------------------
        // Stay @ ER and Wait for the 2nd Diagnose after X-ray
        //-----------------------------------------------------
        
        else if (myExamSheet -> patientTask == (PATIENTTASK)PATDIAGNOSEXRAY)
        {
            myExamSheet -> patientTask = (PATIENTTASK)PATNOTHING;
            
            childWaitingLock[pid-1] -> Acquire();
            
            childWaitingTask[pid-1] = (CHILDTASK)CLDIAXRAY;
            
            childWaitingCV[pid-1] -> Wait(childWaitingLock[pid-1]);
            
            childWaitingLock[pid-1] -> Release();
            
            
            examRoomsCV[erIndex] -> Signal(examRoomsLock[erIndex]);
        
            examRoomsCV[erIndex] -> Wait(examRoomsLock[erIndex]);
            
        }
        
        
        //------------------------------------------
        // Stay @ ER and Wait for the 1st Diagnose  
        //------------------------------------------
        
        else if (myExamSheet -> patientTask == (PATIENTTASK)PATDIAGNOSE)
        {
            myExamSheet -> patientTask = (PATIENTTASK)PATNOTHING;
            
            childWaitingLock[pid-1] -> Acquire();
            
            childWaitingTask[pid-1] = (CHILDTASK)FDUNDERGO;
            
            childWaitingCV[pid-1] -> Wait(childWaitingLock[pid-1]);
            
            childWaitingLock[pid-1] -> Release();
            
            examRoomsCV[erIndex] -> Signal(examRoomsLock[erIndex]);
            
            examRoomsCV[erIndex] -> Wait(examRoomsLock[erIndex]);
            
            if (myExamSheet -> xrayImages > 0) 
            {
                
                childWaitingLock[pid-1] -> Acquire();
                
                childWaitingTask[pid-1] = (CHILDTASK)INFORMXRAY;
                
                childWaitingCV[pid-1] -> Wait(childWaitingLock[pid-1]);
                
                childWaitingLock[pid-1] -> Release();
                
                
                examRoomsCV[erIndex] -> Signal(examRoomsLock[erIndex]);
                
                examRoomsCV[erIndex] -> Wait(examRoomsLock[erIndex]);
                
            }
            
            if (myExamSheet -> shotStatus == (SHOTSTATUS)SHOTREQUIRED)
            {
                childWaitingLock[pid-1] -> Acquire();
                
                childWaitingTask[pid-1] = (CHILDTASK)INFORMSHOT;
                
                childWaitingCV[pid-1] -> Wait(childWaitingLock[pid-1]);
                
                childWaitingLock[pid-1] -> Release();
                
                
                examRoomsCV[erIndex] -> Signal(examRoomsLock[erIndex]);
                
                examRoomsCV[erIndex] -> Wait(examRoomsLock[erIndex]);
                
            }
            
            childWaitingLock[pid-1] -> Acquire();
            
            childWaitingTask[pid-1] = (CHILDTASK)FDDONE;
            
            childWaitingCV[pid-1] -> Wait(childWaitingLock[pid-1]);
            
            childWaitingLock[pid-1] -> Release();
            
            examRoomsCV[erIndex] -> Signal(examRoomsLock[erIndex]);
            
        }
        
        
        //------------------------------------------
        // Stay @ ER and Wait for escort to XRAY
        //------------------------------------------
        
        else if ((myExamSheet -> patientTask == (PATIENTTASK)PATGOXRAY))
        {
            DEBUG('p', "Parent [%d] waits for a Nurse to escort them to the Xray room.\n", pid);
            
            myExamSheet -> patientTask = (PATIENTTASK)PATNOTHING;
            
            myExamSheet -> xrayStatus = (XRAYSTATUS)XRAYREQUIRED;
            
            examRoomsCV[erIndex] -> Wait(examRoomsLock[erIndex]);
            
            
            childWaitingLock[pid-1] -> Acquire();
            
            childWaitingTask[pid-1] = (CHILDTASK)GETONTABLE;
            
            childWaitingCV[pid-1] -> Wait(childWaitingLock[pid-1]);
            
            childWaitingLock[pid-1] -> Release();
            
            
            
            examRoomsCV[erIndex] -> Signal(examRoomsLock[erIndex]);
            
            examRoomsCV[erIndex] -> Wait(examRoomsLock[erIndex]);
            
            for (int i =0; i< myExamSheet -> xrayImages; i++) 
            {
                childWaitingLock[pid-1] -> Acquire();
                
                childWaitingTask[pid-1] = (CHILDTASK)XRAYMOVE;
                
                childWaitingCV[pid-1] -> Wait(childWaitingLock[pid-1]);
                
                childWaitingLock[pid-1] -> Release();
                
                
                examRoomsCV[erIndex] -> Signal(examRoomsLock[erIndex]);
                
                examRoomsCV[erIndex] -> Wait(examRoomsLock[erIndex]);
                
                
                childWaitingLock[pid-1] -> Acquire();
                
                childWaitingTask[pid-1] = (CHILDTASK)XRAYONE;
                
                childWaitingCV[pid-1] -> Wait(childWaitingLock[pid-1]);
                
                childWaitingLock[pid-1] -> Release();
                
                
                examRoomsCV[erIndex] -> Signal(examRoomsLock[erIndex]);
                
                examRoomsCV[erIndex] -> Wait(examRoomsLock[erIndex]);
                
            }
            
            examRoomsCV[erIndex] -> Signal(examRoomsLock[erIndex]);
            
            examRoomsCV[erIndex] -> Wait(examRoomsLock[erIndex]);
            
            DEBUG('p', "Parent [%d] is following Nurse [%d] to Examination Room [%d].\n", pid, myExamSheet -> nurseId, erIndex+1);
            
            examRoomsCV[erIndex] -> Signal(examRoomsLock[erIndex]);
            
            DEBUG('p', "Parent [%d] has arrived at Examination Room [%d].\n", pid, erIndex+1);
            
        }
        
        //-------------------------------------------------
        // Stay @ ER and Wait for a Nurse to be give a shot
        //-------------------------------------------------
        
        else if (myExamSheet -> patientTask == (PATIENTTASK)PATSHOT)
        {
            myExamSheet -> patientTask = (PATIENTTASK)PATNOTHING;
            
            childWaitingLock[pid-1] -> Acquire();
            
            childWaitingTask[pid-1] = (CHILDTASK)NURSESHOT;
            
            childWaitingCV[pid-1] -> Wait(childWaitingLock[pid-1]);
            
            childWaitingLock[pid-1] -> Release();
            
            DEBUG('p', "Parent [%d] says, \"Yes He is ready for the shot\".\n", pid);
            
            examRoomsCV[erIndex] -> Signal(examRoomsLock[erIndex]);
            
            examRoomsCV[erIndex] -> Wait(examRoomsLock[erIndex]);
            
            
        }
        else 
        {
            
            
        }
        
        examRoomsLock[erIndex] -> Release();
        
        currentThread -> Yield();
        
    }
    
    
    examRoomsCV[erIndex] -> Signal(examRoomsLock[erIndex]);
    
    examRoomsCV[erIndex] -> Wait(examRoomsLock[erIndex]);
    
    examRoomsCV[erIndex] -> Signal(examRoomsLock[erIndex]);
    
    examRoomsLock[erIndex] -> Release();
    
    
    //-------------------------------------------------
    // Stay @ Cashier Waiting Room
    //-------------------------------------------------
    
    cashWaitingLock -> Acquire();
    
    cashWaitingCount ++;
    
    DEBUG('p', "Parent [%d] enters the queue for Cashier.\n", pid);
    
    cashWaitingCV -> Wait(cashWaitingLock);
    
    cashWaitingLock -> Release();
    
    DEBUG('p', "Parent [%d] reaches the Cashier.\n", pid);
    
    cashLock -> Acquire();
    
    cashExamSheet = myExamSheet;
    
    DEBUG('p', "Parent [%d] hands over his examination sheet to the Cashier.\n", pid);
    
    cashCV -> Signal(cashLock);
    
    cashCV -> Wait(cashLock);
    
    DEBUG('p', "Parent [%d] pays the Cashier $%f.\n", pid, myExamSheet -> totalAmount);
    
    cashCV -> Signal(cashLock);
    
    cashCV -> Wait(cashLock);
    
    DEBUG('p', "Parent [%d] receives a receipt for Child Patient[%d] from the Cashier.\n", pid, pid);
    
    cashCV -> Signal(cashLock);
    
    DEBUG('p', "Parent [%d] leaves the doctor's office with Child Patient [%d].\n", pid, pid);
    
    cashLock -> Release();
    
}

void Child(int pid)
{
    
    ExamSheet *myExamSheet = NULL;
    
    while (true) 
    {
        childWaitingLock[pid-1] -> Acquire();
        
        //-------------------------------------------------
        // Child is informed to Enter the WR
        //-------------------------------------------------
        
        if (childWaitingTask[pid-1] == (CHILDTASK)CHILDWR)
        {
            childWaitingTask[pid-1] = (CHILDTASK)CHILDNONE;
            
            DEBUG('h', "Child Patient [%d] has entered the Doctor's Office Waiting Room with Parent [%d].\n", pid, pid);
            
            childWaitingCV[pid-1] -> Signal(childWaitingLock[pid-1]);
        
            childWaitingLock[pid-1] -> Release();
        }
        
        //------------------------------
        // Child is informed to Follow 
        //------------------------------
        
        else if (childWaitingTask[pid-1] == (CHILDTASK)CHILDGOES)
        {
            childWaitingTask[pid-1] = (CHILDTASK)CHILDNONE;
            
            myExamSheet = childExamSheet;
            
            childWaitingCV[pid-1] -> Signal(childWaitingLock[pid-1]);
            
            childWaitingLock[pid-1] -> Release();
            
        }
 
        //-------------------------------------------------------------
        // Child is informed that he/she is undergoing the 1st diagnose
        //-------------------------------------------------------------
        
        
        else if (childWaitingTask[pid-1] == (CHILDTASK)FDUNDERGO)
        {
            childWaitingTask[pid-1]  = (CHILDTASK)CHILDNONE;
            
            childWaitingCV[pid-1] -> Signal(childWaitingLock[pid-1]);
            
            childWaitingLock[pid-1] -> Release();
            
        }
        
        //-------------------------------------------------------------
        // Child is informed that he/she should take X-Ray
        //-------------------------------------------------------------
        
        else if (childWaitingTask[pid-1] == (CHILDTASK)INFORMXRAY)
        {
            childWaitingTask[pid-1] = (CHILDTASK)CHILDNONE;
            
            DEBUG('h', "Child Patient [%d] has been informed by Doctor [%d] that he needs an Xray.\n", pid, myExamSheet -> docId);
            
            childWaitingCV[pid-1] -> Signal(childWaitingLock[pid-1]);
            
            childWaitingLock[pid-1] -> Release();
        }
        
        //-------------------------------------------------------------
        // Child is informed that he/she should take a shot
        //-------------------------------------------------------------
        
        else if (childWaitingTask[pid-1] == (CHILDTASK)INFORMSHOT)
        {
            
            childWaitingTask[pid-1] = (CHILDTASK)CHILDNONE;
            
            DEBUG('h', "Child Patient [%d] has been informed by Doctor [%d] that he will be administered a shot.\n", pid, myExamSheet -> docId);
            
            childWaitingCV[pid-1] -> Signal(childWaitingLock[pid-1]);
            
            childWaitingLock[pid-1] -> Release();
        }
        
        
        //-------------------------------------------------------------
        // Child is informed that 1st diagnose is done
        //-------------------------------------------------------------
        
        
        else if (childWaitingTask[pid-1] == (CHILDTASK)FDDONE)
        {
            
            childWaitingTask[pid-1] = (CHILDTASK)CHILDNONE;
            
            DEBUG('h', "Child Patient [%d] has been diagnosed by Doctor [%d].\n", pid, myExamSheet -> docId);
            
            childWaitingCV[pid-1] -> Signal(childWaitingLock[pid-1]);
            
            childWaitingLock[pid-1] -> Release();
        }
        
        
        //-------------------------------------------------------------
        // Child is informed that he/she should get to the  x-ray table
        //-------------------------------------------------------------
        
        else if (childWaitingTask[pid-1] == (CHILDTASK)GETONTABLE)
        {
            childWaitingTask[pid-1] = (CHILDTASK)CHILDNONE;
            
            DEBUG('h', "Child Patient [%d] gets on the table.\n", pid);
            
            childWaitingCV[pid-1] -> Signal(childWaitingLock[pid-1]);
            
            childWaitingLock[pid-1] -> Release();
        }
        
        
        //-------------------------------------------------------------
        // Child is informed that he/she should move to take x-ray
        //-------------------------------------------------------------
        
        else if (childWaitingTask[pid-1] == (CHILDTASK)XRAYMOVE)
        {
            childWaitingTask[pid-1] = (CHILDTASK)CHILDNONE;
            
            DEBUG('h', "Child Patienht [%d] moves for the next Xray.\n", pid);
            
            childWaitingCV[pid-1] -> Signal(childWaitingLock[pid-1]);
            
            childWaitingLock[pid-1] -> Release();
        } 
        
        //-------------------------------------------------------------
        // Child is informed that he/she has taken a x-ray image
        //-------------------------------------------------------------
        
        else if (childWaitingTask[pid-1] == (CHILDTASK)XRAYONE)
        {
            childWaitingTask[pid-1] = (CHILDTASK)CHILDNONE;
            
            DEBUG('h', "Child Patient [%d] has been asked to take an Xray.\n", pid);
            
            childWaitingCV[pid-1] -> Signal(childWaitingLock[pid-1]);
            
            childWaitingLock[pid-1] -> Release();
        }
        
        
        //-------------------------------------------------------------
        // Child is informed that nures is coming for a shot
        //-------------------------------------------------------------
        
        else if (childWaitingTask[pid-1] == (CHILDTASK)NURSESHOT)
        {
            childWaitingTask[pid-1] = (CHILDTASK)CHILDNONE;
            
            childWaitingCV[pid-1] -> Signal(childWaitingLock[pid-1]);
            
            childWaitingLock[pid-1] -> Release();
            
        }
        
        //-------------------------------------------------------------
        // Child is informed that he/she is diagnosed by doctor 2nd time
        //-------------------------------------------------------------
        
        else if (childWaitingTask[pid-1] == (CHILDTASK)CLDIAXRAY)
        {
            childWaitingTask[pid-1] = (CHILDTASK)CHILDNONE;
            
            DEBUG('h', "Child Patient [%d] has been diagnosed by Doctor [%d].\n", pid, myExamSheet -> docId);
            
            childWaitingCV[pid-1] -> Signal(childWaitingLock[pid-1]);
            
            childWaitingLock[pid-1] -> Release();
            
        }        
        
        //-------------------------------------------------------------
        // Child is informed that he/she should leave the hospital
        //-------------------------------------------------------------
        
        else if (childWaitingTask[pid-1] == (CHILDTASK)CHILDOVER)
        {
            childWaitingTask[pid-1] = (CHILDTASK)CHILDNONE;
            
            childWaitingCV[pid-1] -> Signal(childWaitingLock[pid-1]);
            
            childWaitingLock[pid-1] -> Release();
            
            break;
            
        }        

        else
        {
            childWaitingLock[pid-1] -> Release();
        }
        
        
        currentThread -> Yield();
    }
    
    
}

void WaitingRoomNurse(int pid)
{
    
    while (true) {
        
        //--------------------------------------------------------      
        // WRN Service the Patient
        //--------------------------------------------------------
        
        waitingRoomLock -> Acquire();
        
        wrnStatus = (WRNSTATUS)WRNFREE;
        
        if (wrWaitingCount > 0) 
        {
            
            wrWaitingCount --;
            
            waitingRoomCV -> Signal(waitingRoomLock);
            
            wrnStatus = (WRNSTATUS)WRNBUSY;
            
            wrnLock -> Acquire();
            
            waitingRoomLock -> Release();
            
            wrnCV -> Wait(wrnLock);
            
            
            //------------------------------------------
            // WRN gives the form to the Adult
            //------------------------------------------
            
            if (wrnTask == (WRNTASK)GETFORMAP) 
            {
                wrnExamSheet = new ExamSheet();
                
                wrnCV -> Signal(wrnLock);
                
                wrnTask = (WRNTASK)NONE;
                
                DEBUG('w', "Waiting Room nurse gives a form to Adult patient [%d].\n", wrnServiceId);
                
                wrnServiceId = -1;
                
                wrnCV -> Wait(wrnLock);
                
                wrnLock -> Release();
                
                currentThread -> Yield();
                
            }
            
            //------------------------------------------
            // WRN gives the form to the Parent
            //------------------------------------------
            
            else if (wrnTask == GETFORMCP)
            {
                wrnExamSheet = new ExamSheet();
                
                wrnCV -> Signal(wrnLock);
                
                wrnTask = (WRNTASK)NONE;
                
                DEBUG('w', "Waiting Room nurse gives a form to the Parent of Child patient [%d].\n", wrnServiceId);
                
                wrnServiceId = -1;
            
                wrnLock -> Release();
                
                currentThread -> Yield();
            
            }
            
            
            //------------------------------------------
            // WRN receives the form from patient
            //------------------------------------------
            
            else if (wrnTask == GIVEFORM) 
            {
                
                if (wrnExamSheet -> patientType == (PATIENTTYPE)ADULT)
                {
                    DEBUG('w', "Waiting Room nurse accepts the form from Adult Patient [%d] and age [%d].\n", wrnExamSheet -> name, wrnExamSheet -> age);
                }
                else
                {
                    DEBUG('w', "Waiting Room nurse accepts the form from Parent with name [%d] and age [%d].\n", wrnExamSheet -> name, wrnExamSheet -> age);
                }
                
                wrnCompleteESList -> Append(wrnExamSheet);
                
                if (wrnExamSheet -> patientType == (PATIENTTYPE)ADULT)
                {
                    DEBUG('w', "Waiting Room nurse creates an examination sheet for Adult patient with name [%d] and age [%d].\n", wrnExamSheet -> name, wrnExamSheet -> age);
                    
                    DEBUG('w', "Waiting Room nurse tells the Adult Patient [%d] to wait in the waiting room for a nurse.\n", wrnExamSheet -> examSheetId);
                }
                else
                {
                    DEBUG('w', "Waiting Room nurse creates an examination sheet for Child patient with name [%d] and age [%d].\n", wrnExamSheet -> name, wrnExamSheet -> age);
                    
                    DEBUG('w', "Waiting Room nurse tells Parent [%d] to wait in the waiting room for a nurse.\n", wrnExamSheet -> examSheetId);
                }
                

                
                wrnCV -> Signal(wrnLock);
                
                wrnExamSheet = NULL;
                
                wrnTask = NONE;
                
                wrnLock -> Release();
                
            } 
            else 
            {
                
                printf("Error: Do nothing while patients are waiting in the WR.\n");
                
                wrnLock-> Release();
                
            }
            
        } 
        else 
        {
            
            //----------------------------------------- 
            // WRN Service the Nurse and give the form
            //-----------------------------------------
            
            wrnStatus = (WRNSTATUS)WRNFREE;
            
            waitingRoomLock -> Release();
            
            wrnLock -> Acquire();
            
            if (nwrnWaitingCount > 0) 
            {
                nwrnWaitingCount --;
                
                nwrnWaitingCV -> Signal(wrnLock);
            
                nwrnLock -> Acquire();
                
                wrnLock -> Release();
            
                nwrnCV -> Wait(nwrnLock);
            
                if (!wrnCompleteESList -> IsEmpty()) 
                {
                
                    nwrnExamSheet = (ExamSheet *)wrnCompleteESList -> Remove();
                    
                    nwrnCV -> Signal(nwrnLock);
                    
                    DEBUG('w', "Waiting Room nurse gives examination sheet of patient [%d] to Nurse [%d].\n", nwrnExamSheet->examSheetId, nwrnId);
                    
                    nwrnCV -> Wait(nwrnLock);
                    
                    nwrnId = -1;
                
                    nwrnLock -> Release();
                
                }
                else
                {
                    nwrnExamSheet = NULL;
                    
                    nwrnCV -> Signal(nwrnLock);
                
                    nwrnCV -> Wait(nwrnLock);
                
                    nwrnLock -> Release();
                }
            }
            else
            {
                wrnLock -> Release();
            }
        }
        
        currentThread -> Yield();
    }
    
}

void Patient(int pid)
{
    ExamSheet *myExamSheet = NULL;
    
    int mp_Type = 0;
    
    //-----------------------------------
    // Adult Patient Interacts with WRN
    //-----------------------------------
    
    DEBUG('l', "Adult Patient [%d] has entered the Doctor's Office Waiting Room.\n", pid);
    
    waitingRoomLock -> Acquire();
    
    wrWaitingCount ++;
    
    DEBUG('l', "Adult Patient [%d] gets in line of the Waiting Room Nurse to get registration form.\n", pid);
    
    waitingRoomCV -> Wait(waitingRoomLock);
    
    wrnStatus = (WRNSTATUS)WRNBUSY;
    
    waitingRoomLock -> Release();
    
    wrnLock -> Acquire();
    
    wrnTask = GETFORMAP;
    
    wrnServiceId = pid;
    
    wrnCV -> Signal(wrnLock);
    
    wrnCV -> Wait(wrnLock);
    
    DEBUG('l', "Adult patient [%d] gets the form from the Waiting Room Nurse.\n", pid);
    
    myExamSheet = wrnExamSheet;
    
    wrnCV -> Signal(wrnLock);
    
    wrnLock -> Release();
    
    myExamSheet -> name = pid;
    
    myExamSheet -> age = rand()%60 + 15;
    
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
    
    currentThread -> Yield();
    
    waitingRoomLock -> Acquire();
    
    wrWaitingCount ++;
    
    DEBUG('l', "Adult Patient [%d] gets in line of the Waiting Room Nurse to submit the registration form.\n", pid);
    
    waitingRoomCV -> Wait(waitingRoomLock);
    
    waitingRoomLock -> Release();
    
    wrnLock -> Acquire();
    
    wrnTask = GIVEFORM;
    
    wrnExamSheet = myExamSheet;
    
    wrnCV -> Signal(wrnLock);
    
    DEBUG('l', "Adult patient [%d] submits the filled form to the Waiting Room Nurse.\n", pid);
    
    wrnCV -> Wait(wrnLock);
    
    wrnLock -> Release();
    
    
    
    //-------------------------------------------------
    // Adult Patient Wait for being Called by Nurse
    //-------------------------------------------------    
    
    broadCastIdLock -> Acquire();
    
    while (broadCastId != pid) 
    {
        
        broadCastIdCV -> Wait(broadCastIdLock);
    }
    
    bcIdStatus = (BROADCASTIDSTATUS)BROADFREE;
    
    bcIdWaitingCV -> Signal(broadCastIdLock);
    
    broadCastIdLock -> Release();
    
    //-------------------------------------------------------------------------
    // Paitent Wait for the Nurse to take the temperature
    //-------------------------------------------------------------------------
    
    int erIndex = myExamSheet -> erId;
    
    examRoomsLock[erIndex] -> Acquire();
    
    examRoomsCV[erIndex] -> Signal(examRoomsLock[erIndex]);
    
    examRoomsCV[erIndex] -> Wait(examRoomsLock[erIndex]);
    
    DEBUG('l', "Adult Patient [%d] is following Nurse [%d] to Examination Room [%d].\n", pid, myExamSheet -> nurseId, erIndex+1);
    
    examRoomsCV[erIndex] -> Signal(examRoomsLock[erIndex]);
    
    DEBUG('l', "Adult Patient [%d] has arrived at Examination Room [%d].\n", pid, erIndex+1);
    
    examRoomsCV[erIndex] -> Wait(examRoomsLock[erIndex]);
    
    myExamSheet -> illness = (SYMPTOM)(rand()%3);
    
    switch (myExamSheet -> illness) {
        
        case (SYMPTOM)SYMPAIN:
                
            DEBUG('l', "Adult Patient [%d] says, \"My symptoms are Pain\".\n", pid);
            
            break;
        
        case (SYMPTOM)SYMNAUSEA:
            
            DEBUG('l', "Adult Patient [%d] says, \"My symptoms are Nausea\".\n", pid);
            
            break;
            
        case (SYMPTOM)SYMHEAR:
            
            DEBUG('l', "Adult Patient [%d] says, \"My symptoms are Hear Alien Voices\".\n", pid);
            
            break;
            
        default:
            break;
    }
    
    examRoomsCV[erIndex] -> Signal(examRoomsLock[erIndex]);
    
    examRoomsCV[erIndex] -> Wait(examRoomsLock[erIndex]);
    
    examRoomsLock[erIndex] -> Release();
    
    while (true) 
    {
        
        examRoomsLock[erIndex] -> Acquire();
        
        //-----------------------------------
        // All things are Done.
        //-----------------------------------
        
        if (myExamSheet -> patientTask == (PATIENTTASK)PATDONE) 
        {
            
            break;            
        } 
        
        //-----------------------------------------------------
        // Stay @ ER and Wait for the 2nd Diagnose after X-ray
        //-----------------------------------------------------
        
        else if (myExamSheet -> patientTask == (PATIENTTASK)PATDIAGNOSEXRAY)
        {
            myExamSheet -> patientTask = (PATIENTTASK)PATNOTHING;
            
            examRoomsCV[erIndex] -> Signal(examRoomsLock[erIndex]);
            
            DEBUG('l', "Adult Patient [%d] has been diagnosed by Doctor [%d].\n", pid, myExamSheet -> docId);
            
            examRoomsCV[erIndex] -> Wait(examRoomsLock[erIndex]);
            
        }
        
        
        //------------------------------------------
        // Stay @ ER and Wait for the 1st Diagnose  
        //------------------------------------------
        
        else if (myExamSheet -> patientTask == (PATIENTTASK)PATDIAGNOSE)
        {
            myExamSheet -> patientTask = (PATIENTTASK)PATNOTHING;
            
            examRoomsCV[erIndex] -> Signal(examRoomsLock[erIndex]);
            
            examRoomsCV[erIndex] -> Wait(examRoomsLock[erIndex]);
            
            if (myExamSheet -> xrayImages > 0) 
            {
            
                DEBUG('l', "Adult Patient [%d] has been informed by Doctor [%d] that he needs an Xray.\n", pid, myExamSheet -> docId);
                
                examRoomsCV[erIndex] -> Signal(examRoomsLock[erIndex]);
                
                examRoomsCV[erIndex] -> Wait(examRoomsLock[erIndex]);
            
            }
            
            if (myExamSheet -> shotStatus == (SHOTSTATUS)SHOTREQUIRED)
            {
                DEBUG('l', "Adult Patient [%d] has been informed by Doctor [%d] that he will be administered a shot.\n", pid, myExamSheet -> docId);
                
                examRoomsCV[erIndex] -> Signal(examRoomsLock[erIndex]);
                
                examRoomsCV[erIndex] -> Wait(examRoomsLock[erIndex]);
                
            }
            
            DEBUG('l', "Adult Patient [%d] has been diagnosed by Doctor [%d].\n", pid, myExamSheet -> docId);
            
            examRoomsCV[erIndex] -> Signal(examRoomsLock[erIndex]);
            
            
        }
        
        //------------------------------------------
        // Stay @ ER and Wait for escort to XRAY
        //------------------------------------------
        
        else if ((myExamSheet -> patientTask == (PATIENTTASK)PATGOXRAY))
        {
            DEBUG('l', "Adult Patient [%d] waits for a Nurse to escort them to the Xray room.\n", pid);
            
            myExamSheet -> patientTask = (PATIENTTASK)PATNOTHING;
            
            myExamSheet -> xrayStatus = (XRAYSTATUS)XRAYREQUIRED;
            
            examRoomsCV[erIndex] -> Wait(examRoomsLock[erIndex]);
            
            DEBUG('l', "Adult Patient [%d] gets on the table.\n", pid);
            
            examRoomsCV[erIndex] -> Signal(examRoomsLock[erIndex]);
            
            examRoomsCV[erIndex] -> Wait(examRoomsLock[erIndex]);
            
            for (int i =0; i< myExamSheet -> xrayImages; i++) 
            {
                DEBUG('l', "Adult Patient [%d] has been asked to take an Xray.\n", pid);
                
                examRoomsCV[erIndex] -> Signal(examRoomsLock[erIndex]);
                
                examRoomsCV[erIndex] -> Wait(examRoomsLock[erIndex]);
                
                examRoomsCV[erIndex] -> Signal(examRoomsLock[erIndex]);
                
                examRoomsCV[erIndex] -> Wait(examRoomsLock[erIndex]);
                                               
                                                            
            }
            
            examRoomsCV[erIndex] -> Signal(examRoomsLock[erIndex]);
            
            examRoomsCV[erIndex] -> Wait(examRoomsLock[erIndex]);
            
            DEBUG('l', "Adult Patient [%d] is following Nurse [%d] to Examination Room [%d].\n", pid, myExamSheet -> nurseId, erIndex+1);
            
            examRoomsCV[erIndex] -> Signal(examRoomsLock[erIndex]);
            
            DEBUG('l', "Adult Patient [%d] has arrived at Examination Room [%d].\n", pid, erIndex+1);
            
        }
        
        
        //-------------------------------------------------
        // Stay @ ER and Wait for a Nurse to be give a shot
        //-------------------------------------------------
        
        else if (myExamSheet -> patientTask == (PATIENTTASK)PATSHOT)
        {
            myExamSheet -> patientTask = (PATIENTTASK)PATNOTHING;
            
            DEBUG('l', "Adult Patient [%d] says, \"Yes I am/He is ready for the shot\".\n", pid);
            
            examRoomsCV[erIndex] -> Signal(examRoomsLock[erIndex]);
            
            examRoomsCV[erIndex] -> Wait(examRoomsLock[erIndex]);
            
            
        }
        else 
        {
            
            
        }
        
        examRoomsLock[erIndex] -> Release();
        
        currentThread -> Yield();
        
    }
    
    examRoomsCV[erIndex] -> Signal(examRoomsLock[erIndex]);
    
    examRoomsCV[erIndex] -> Wait(examRoomsLock[erIndex]);
    
    examRoomsCV[erIndex] -> Signal(examRoomsLock[erIndex]);
    
    examRoomsLock[erIndex] -> Release();
    
    //-------------------------------------------------
    // Stay @ Cashier Waiting Room
    //-------------------------------------------------
    
    cashWaitingLock -> Acquire();
    
    cashWaitingCount ++;
    
    DEBUG('l', "Adult Patient [%d] enters the queue for Cashier.\n", pid);
    
    cashWaitingCV -> Wait(cashWaitingLock);
    
    cashWaitingLock -> Release();
    
    DEBUG('l', "Adult Patient [%d] reaches the Cashier.\n", pid);
    
    cashLock -> Acquire();
    
    cashExamSheet = myExamSheet;
    
    DEBUG('l', "Adult Patient [%d] hands over his examination sheet to the Cashier.\n", pid);
    
    cashCV -> Signal(cashLock);
    
    cashCV -> Wait(cashLock);
    
    DEBUG('l', "Adult Patient [%d] pays the Cashier $%f.\n", pid, myExamSheet -> totalAmount);
    
    cashCV -> Signal(cashLock);
    
    cashCV -> Wait(cashLock);
    
    DEBUG('l', "Adult Patient [%d] receives a receipt from the Cashier.\n", pid);
    
    cashCV -> Signal(cashLock);
    
    DEBUG('l', "Adult Patient [%d] leaves the doctor's office.\n", pid);
    
    cashLock -> Release();
    
    
    
    
}


void Nurse(int pid)
{
    
    ExamSheet *holdingES;
    
    int patientId;
    
    while (true) 
    {
        
        //-------------------------------------------------------------------------
        // Check whether there is empty ER so they can go for ask a patient to come
        //-------------------------------------------------------------------------
        
        
        
        for (int erIndex = 0; erIndex < examRooms; erIndex ++) 
        {
            
            ersStatusLock -> Acquire();
            
            if (examRoomsStatus[erIndex] == (ERSTATUS)ERFREE) 
            {
                
                examRoomsStatus[erIndex] = (ERSTATUS)ERBLOCKED;
                
                ersStatusLock -> Release();
                
                wrnLock -> Acquire();
                
                nwrnWaitingCount ++;
                
                nwrnWaitingCV -> Wait(wrnLock);
                
                wrnLock -> Release();
                
                nwrnLock -> Acquire();
                
                nwrnId = pid;
                
                nwrnCV -> Signal(nwrnLock);
                
                nwrnCV -> Wait(nwrnLock);
                
                holdingES = nwrnExamSheet;
                
                if (holdingES == NULL) 
                {
                    nwrnCV -> Signal(nwrnLock);
                    
                    nwrnLock -> Release();
                    
                    ersStatusLock -> Acquire();
                    
                    examRoomsStatus[erIndex] = (ERSTATUS)ERFREE;
                    
                    break;
                }
                //--------------------------------------------------
                //nurse interacts with the wrn to get the examination sheet
                //------------------------------------------------------------
                
                DEBUG('u',"Nurse [%d] tells Waiting Room Nurse to give a new examination sheet.\n", pid);
                
                nwrnCV -> Signal(nwrnLock);
                
                nwrnLock -> Release();
                
                holdingES -> erId = erIndex;
                
                holdingES -> nurseId = pid;
                
                broadCastIdLock -> Acquire();
                
                if (bcIdStatus == (BROADCASTIDSTATUS)BROADFREE)
                {
                    
                    bcIdStatus = (BROADCASTIDSTATUS)BROADBUSY;
                    
                }
                else
                {
                    
                    bcIdWaitingCV -> Wait(broadCastIdLock);
                    
                    bcIdStatus = (BROADCASTIDSTATUS)BROADBUSY;
                }
                
                examRoomsLock[erIndex] -> Acquire(); 
                
                broadCastId = holdingES -> examSheetId;
                
                broadCastIdCV -> Broadcast(broadCastIdLock);
                
                broadCastIdLock -> Release();
                
                examRoomsCV[erIndex] -> Wait(examRoomsLock[erIndex]);
                
                //----------------------------------------------------------
                //nurse escort the patient to enter the examination room.
                //----------------------------------------------------------
                
                if (holdingES -> patientType == (PATIENTTYPE)ADULT)
                {
                    DEBUG('u', "Nurse [%d] escorts Adult Patient [%d] to the examination room [%d].\n", pid, holdingES -> examSheetId, erIndex+1);
                }
                else
                {
                    DEBUG('u', "Nurse [%d] escorts Parent [%d] to the examination room [%d].\n", pid, holdingES -> examSheetId, erIndex+1);
                }
                
                examRoomsCV[erIndex] -> Signal(examRoomsLock[erIndex]);
                
                examRoomsCV[erIndex] -> Wait(examRoomsLock[erIndex]);
                
                //------------------------------------------------------
                //nurse takes the temperature and blood pressure in exmination room
                //-------------------------------------------------------
                
                if (holdingES -> patientType == (PATIENTTYPE)ADULT)
                {
                    DEBUG('u', "Nurse [%d] takes the temperature and blood pressure of Adult Patient [%d].\n", pid, holdingES -> examSheetId);
                }
                else
                {
                    DEBUG('u', "Nurse [%d] takes the temperature and blood pressure of Parent [%d].\n", pid, holdingES -> examSheetId);
                }
                
                holdingES -> temperature = rand()%10 + 32;
                
                holdingES -> bloodPressure = rand()%80 + 70;
                
                if (holdingES -> patientType == (PATIENTTYPE)ADULT)
                {
                    DEBUG('u', "Nurse [%d] ask Adult Patient [%d] \"What Symptoms do you have?\".\n", pid, holdingES -> examSheetId);
                }
                else
                {
                    DEBUG('u', "Nurse [%d] ask Parent [%d] \"What Symptoms do you have?\".\n", pid, holdingES -> examSheetId);
                }
                
                examRoomsCV[erIndex] -> Signal(examRoomsLock[erIndex]);
                
                examRoomsCV[erIndex] -> Wait(examRoomsLock[erIndex]);
                
                //------------------------------------------------------
                //nurse records the information on examination sheet
                //-------------------------------------------------------
                
                if (holdingES -> patientType == (PATIENTTYPE)ADULT)
                {
                    DEBUG('u', "Nurse [%d] writes all the information of Adult Patient [%d] in his examination sheet.\n", pid, holdingES -> examSheetId);
                }
                else
                {
                    DEBUG('u', "Nurse [%d] writes all the information of Parent [%d] in his examination sheet.\n", pid, holdingES -> examSheetId);
                }
                
                //------------------------------------------------------
                //nurse inform the doctor to diagnose
                //-------------------------------------------------------
                
                for (int docIndex = 0; docIndex < docCount ; docIndex ++) 
                {
                    
                    doctWaitingLock[docIndex] -> Acquire();
                    
                    if (docStatus[docIndex] == (DOCSTATUS)DOCFREE) 
                    { 
                        docStatus[docIndex] = (DOCSTATUS)DOCBUSY;
                        
                        ndocESNo[docIndex] = erIndex;
                        
                        doctWaitingCV[docIndex] -> Signal(doctWaitingLock[docIndex]);
                        
                        holdingES -> esProcess = (ESPROCESS)ESINPROCESS;
                        
                        doctWaitingLock[docIndex] -> Release();
                        
                        if (holdingES -> patientType == (PATIENTTYPE)ADULT)
                        {
                            
                            DEBUG('u', "Nurse [%d] informs Doctor [%d] that Adult Patient [%d] is waiting in the examination room [%d].\n", pid, docIndex+1, holdingES -> examSheetId,erIndex+1);
                            
                            DEBUG('u', "Nurse [%d] hands over to the Doctor [%d] the examination sheet of Adult Patient [%d].\n", pid, docIndex+1, erIndex+1);
                        }
                        else
                        {
                            DEBUG('u', "Nurse [%d] informs Doctor [%d] that Child Patient [%d] is waiting in the examination room [%d].\n", pid, docIndex+1, holdingES -> examSheetId,erIndex+1);
                            
                            DEBUG('u', "Nurse [%d] hands over to the Doctor [%d] the examination sheet of Child Patient [%d].\n", pid, docIndex+1, erIndex+1);
                        }
                        
                        break;
                        
                    } 
                    else 
                    {
                        doctWaitingLock[docIndex] -> Release();
                    }
                    
                }
                
                examRoomsCV[erIndex] -> Signal(examRoomsLock[erIndex]);
                
                wallPackets[erIndex] = holdingES;
                
                holdingES = NULL;
                
                examRoomsLock[erIndex] -> Release();
                
                break;
                
            } 
            else 
            {
                
                ersStatusLock -> Release();
            }
        }
        
        currentThread -> Yield();
        
        //-----------------------------------------------------
        // Check whether Patient is Already done in X-Ray Room
        //-----------------------------------------------------
        
        xrayWaitingLock -> Acquire();
        
        if (!xrayWaitingES -> IsEmpty()) 
        {
            
            holdingES = (ExamSheet *)xrayWaitingES -> Remove();
            
            xrayWaitingLock -> Release();
            
            int erIndex = holdingES -> erId;
            
            if (holdingES -> patientType == (PATIENTTYPE)ADULT)
            {
                
                DEBUG('u', "Nurse [%d] gets examination sheet for Adult Patient [%d] in Xray waiting room.\n", pid, holdingES -> examSheetId);
            }
            else
            {
                DEBUG('u', "Nurse [%d] gets examination sheet for Parent [%d] in Xray waiting room.\n", pid, holdingES -> examSheetId);
            }
            
            examRoomsLock[erIndex] -> Acquire();
            
            holdingES -> nurseId = pid;
            
            holdingES -> patientTask = (PATIENTTASK)PATBACKXRAY;
            
            
            //------------------------------------------------------
            //nurse escort the patient to examination room after xray
            //-------------------------------------------------------   
            
            
            if (holdingES -> patientType == (PATIENTTYPE)ADULT)
            {
                
                DEBUG('u', "Nurse [%d] escorts Adult Patient [%d] to the examination room [%d].\n", pid, holdingES -> examSheetId, erIndex+1);
                
            }
            else
            {
                
                DEBUG('u', "Nurse [%d] escorts Parent [%d] to the examination room [%d].\n", pid, holdingES -> examSheetId, erIndex+1);
            }
            
            examRoomsCV[erIndex] -> Signal(examRoomsLock[erIndex]);
            
            examRoomsCV[erIndex] -> Wait(examRoomsLock[erIndex]);
            
            holdingES -> esProcess = (ESPROCESS)ESIDLE;
            
            wallPackets[erIndex] = holdingES;
            
            examRoomsLock[erIndex] -> Release();
            
            holdingES = NULL;
            
        }    
        else 
        {
            xrayWaitingLock -> Release();
        }
        
        currentThread -> Yield();
        
        
        //-------------------------------------
        // Nurse Checks the wallpacket
        //-------------------------------------
        
        for (int erIndex = 0; erIndex<examRooms; erIndex ++) 
        {
            
            
            examRoomsLock[erIndex] -> Acquire();
            
            DEBUG('u', "Nurse [%d] checks the wall box of examination room [%d].\n", pid, erIndex+1);
            
            
            holdingES = wallPackets[erIndex];
            
            if (holdingES == NULL) 
            {
                
                examRoomsLock[erIndex] -> Release();
                
                continue;
            }
            
            
            wallPackets[erIndex] = NULL;
            
            
            //---------------------------------------------
            // Patient needs the doctor to service 2nd
            //---------------------------------------------
            
            if (holdingES -> xrayStatus == (XRAYSTATUS)XRAYHALFDONE &&
                holdingES -> esProcess != (ESPROCESS)ESINPROCESS) 
            {
                
                for (int docIndex = 0; docIndex < docCount; docIndex ++) 
                {
                    doctWaitingLock[docIndex] -> Acquire();
                    
                    if (docStatus[docIndex] == (DOCSTATUS)DOCFREE) 
                    {
                        
                        docStatus[docIndex] = (DOCSTATUS)DOCBUSY;
                        
                        ndocESNo[docIndex] = erIndex;
                        
                        holdingES -> esProcess = (ESPROCESS)ESINPROCESS;
                        
                        doctWaitingCV[docIndex] -> Signal(doctWaitingLock[docIndex]);
                        
                        doctWaitingLock[docIndex] -> Release();
                        
                        DEBUG('u', "Nurse [%d] informs Doctor [%d] that Adult/Child Patient [%d] is waiting in the examination room [%d].\n", pid, docIndex+1, holdingES -> examSheetId,erIndex+1);
                        
                        DEBUG('u', "Nurse [%d] hands over to the Doctor [%d] the examination sheet of Adult/Child Patient [%d].\n", pid, docIndex+1, erIndex+1);
                        
                        break;
                        
                    } 
                    else 
                    {
                        
                        doctWaitingLock[docIndex] -> Release();
                    }
                    
                }
                
                wallPackets[erIndex] = holdingES;
                
                holdingES = NULL;
                
                examRoomsLock[erIndex] -> Release();
                
            } 
            //------------------------------------------------
            // Patients need to be sent to XRAYTechnician
            //------------------------------------------------
            
            else if (holdingES -> xrayStatus == (XRAYSTATUS)XRAYREQUIRED && 
                     holdingES -> esProcess != (ESPROCESS)ESINPROCESS) 
            {
                
                bool flag = false;
                
                int xrIndex;
                
                xrayRoomsStatusLock -> Acquire();
                
                for (xrIndex = 0; xrIndex < xrayRoomsCount; xrIndex++) 
                {
                    if (xrayRoomsStatus[xrIndex] == (XRRSTATUS)XRRFREE) 
                    {
                        
                        xrayRoomsStatus[xrIndex] = (XRRSTATUS)XRRBUSY;
                        
                        flag = true;
                        
                        break;
                    } 
                }
                
                xrayRoomsStatusLock -> Release();
                
                if (flag) 
                {
                    
                    holdingES -> esProcess = (ESPROCESS)ESINPROCESS;
                    
                    xrayRoomsLock[xrIndex] -> Acquire();
                    
                    xrayRoomsES[xrIndex] = holdingES;
                    
                    xrayRoomsCV[xrIndex] -> Signal(xrayRoomsLock[xrIndex]);
                    
                    xrayRoomsLock[xrIndex] -> Release();
                    
                    if (holdingES -> patientType == (PATIENTTYPE)ADULT)
                    {
                        
                        DEBUG('u', "Nurse [%d] escorts Adult Patient [%d] to the X-ray room [%d].\n", pid, holdingES -> examSheetId, xrIndex+1);
                        
                        DEBUG('u', "Nurse [%d] informs X-Ray Technician [%d] about Adult Patient [%d] and hands over the examination sheet.\n", pid, xrIndex+1, holdingES -> examSheetId);
                    }
                    else
                    {
                        DEBUG('u', "Nurse [%d] escorts Parent [%d] to the X-ray room [%d].\n", pid, holdingES -> examSheetId, xrIndex+1);
                        
                        DEBUG('u', "Nurse [%d] informs X-Ray Technician [%d] about Child Patient [%d] and hands over the examination sheet.\n", pid, xrIndex+1, holdingES -> examSheetId);
                        
                    }
                    
                    
                    
                    DEBUG('u', "Nurse [%d] leaves the X-ray Room [%d].\n", pid, xrIndex+1);
                    
                    holdingES = NULL;
                    
                    wallPackets[erIndex] = NULL;
                    
                    examRoomsLock[erIndex] -> Release();
                    
                    
                }
                else 
                {
                    
                    wallPackets[erIndex] = holdingES;
                    
                    holdingES = NULL;
                    
                    examRoomsLock[erIndex] -> Release();
                    
                }
                
                
            } 
            
            else if (holdingES -> shotStatus == (SHOTSTATUS)SHOTREQUIRED && 
                     holdingES -> esProcess != (ESPROCESS)ESINPROCESS &&
                     holdingES -> patientTask != (PATIENTTASK)PATGOXRAY) 
            {
                
                
                //------------------------------
                // Patient needs a shot
                //------------------------------
                
                if (holdingES -> patientType == (PATIENTTYPE)ADULT) 
                {
                    DEBUG('u', "Nurse [%d] goes to supply cabinet to give to take medicine for Adult Patient [%d].\n", pid, holdingES -> examSheetId);
                }
                else
                {
                    DEBUG('u', "Nurse [%d] goes to supply cabinet to give to take medicine for Parent [%d].\n", pid, holdingES -> examSheetId);
                }
                
                holdingES -> esProcess = (ESPROCESS)ESINPROCESS;
                
                cabinetLock -> Acquire();
                
                cabinetCount ++;
                
                cabinetLock -> Release();
                
                holdingES -> patientTask = (PATIENTTASK)PATSHOT;
                
                if (holdingES -> patientType == (PATIENTTYPE)ADULT)
                {
                    DEBUG('u', "Nurse [%d] asks Adult Patient [%d] \"Are you ready for the shot?\".\n", pid, holdingES -> examSheetId);
                }
                else
                {
                    DEBUG('u', "Nurse [%d] asks Parent [%d] \"Are you ready for the shot?\".\n", pid, holdingES -> examSheetId);
                    
                }
                
                
                
                examRoomsCV[erIndex] -> Wait(examRoomsLock[erIndex]);
                
                holdingES -> shotStatus = (SHOTSTATUS)SHOTDONE;
                
                holdingES -> esProcess = (ESPROCESS)ESIDLE;
                
                if (holdingES -> patientType == (PATIENTTYPE)ADULT)
                {
                    DEBUG('u', "Nurse [%d] tells Adult Patient [%d] \"Your shot is over.\".\n", pid, holdingES -> examSheetId);
                }
                else
                {
                    DEBUG('u', "Nurse [%d] tells Parent [%d] \"Your shot is over.\".\n", pid, holdingES -> examSheetId);
                }
                
                
                
                examRoomsCV[erIndex] -> Signal(examRoomsLock[erIndex]);
                
                wallPackets[erIndex] = holdingES;
                
                holdingES = NULL;
                
                examRoomsLock[erIndex] -> Release();
                
            } 
            
            else if (holdingES -> xrayStatus == (XRAYSTATUS)XRAYUNKNOWN && 
                     holdingES -> shotStatus == (SHOTSTATUS)SHOTUNKNOWN &&
                     holdingES -> esProcess != (ESPROCESS)ESINPROCESS) 
            {
                
                
                
                //-----------------------------------------------
                // Patient needs a doctor for 1st diagnose
                //-----------------------------------------------
                
                
                for (int docIndex = 0; docIndex < docCount; docIndex ++) {
                    
                    doctWaitingLock[docIndex] -> Acquire();
                    
                    if (docStatus[docIndex] == (DOCSTATUS)DOCFREE) 
                    {
                        
                        docStatus[docIndex] = (DOCSTATUS)DOCBUSY;
                        
                        ndocESNo[docIndex] = erIndex;
                        
                        doctWaitingCV[docIndex] -> Signal(doctWaitingLock[docIndex]);
                        
                        doctWaitingLock[docIndex] -> Release();
                        
                        if (holdingES -> patientType == (PATIENTTYPE)ADULT)
                        {
                            DEBUG('u', "Nurse [%d] informs Doctor [%d] that Adult Patient [%d] is waiting in the examination room [%d].\n", pid, docIndex+1, holdingES -> examSheetId,erIndex+1);
                            
                            DEBUG('u', "Nurse [%d] hands over to the Doctor [%d] the examination sheet of Adult Patient [%d].\n", pid, docIndex+1, erIndex+1);
                            
                        }
                        else
                        {
                            DEBUG('u', "Nurse [%d] informs Doctor [%d] that Child Patient [%d] is waiting in the examination room [%d].\n", pid, docIndex+1, holdingES -> examSheetId,erIndex+1);
                            
                            DEBUG('u', "Nurse [%d] hands over to the Doctor [%d] the examination sheet of Child Patient [%d].\n", pid, docIndex+1, erIndex+1);
                            
                        }
                        
                        holdingES -> esProcess = (ESPROCESS)ESINPROCESS;
                        
                        break;
                        
                    } 
                    else 
                    {
                        
                        doctWaitingLock[docIndex] -> Release();
                    }
                    
                }
                
                wallPackets[erIndex] = holdingES;
                
                holdingES = NULL;
                
                examRoomsLock[erIndex] -> Release();
                
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
                
                if (holdingES -> esStatus == (ESSTATUS)ESFINISHED && 
                    holdingES -> esProcess != (ESPROCESS)ESINPROCESS) 
                {   
                    holdingES -> esProcess = (ESPROCESS)ESINPROCESS;
                    
                    holdingES -> patientTask = (PATIENTTASK)PATDONE;
                    
                    examRoomsCV[erIndex] -> Wait(examRoomsLock[erIndex]);
                    
                    if (holdingES -> patientType == (PATIENTTYPE)ADULT)
                    {
                        DEBUG('u', "Nurse [%d] escorts Adult Patient [%d] to Cashier.\n", pid, holdingES -> examSheetId);
                    }
                    else
                    {
                        DEBUG('u', "Nurse [%d] escorts Parent [%d] to Cashier.\n", pid, holdingES -> examSheetId);
                    }
                    
                    
                    
                    holdingES -> esProcess = (ESPROCESS)ESIDLE;
                    
                    holdingES = NULL;
                    
                    wallPackets[erIndex] = NULL;
                    
                    examRoomsCV[erIndex] -> Signal(examRoomsLock[erIndex]);
                    
                    examRoomsCV[erIndex] -> Wait(examRoomsLock[erIndex]);
                    
                    examRoomsLock[erIndex] -> Release();
                    
                    ersStatusLock -> Acquire();
                    
                    examRoomsStatus[erIndex] = (ERSTATUS)ERFREE;
                    
                    ersStatusLock -> Release();
                    
                } 
                else 
                {
                    
                    wallPackets[erIndex] = holdingES;
                    
                    holdingES = NULL;
                    
                    examRoomsLock[erIndex] -> Release();
                    
                }
            }
            
            
            currentThread -> Yield();
        }
    }
}

void Doctor(int pid)
{
    int erIndex;
    
    ExamSheet *holdingES;
    
    while (true) 
    {
        
        
        //-------------------------------------------------------------------------
        // Doctor @ Office and Wait for the Nurse to Signal them
        //-------------------------------------------------------------------------
        
        doctWaitingLock[pid-1] -> Acquire();
        
        docStatus[pid-1] = (DOCSTATUS)DOCFREE;
        
        //------------------------------------------------------
        //doctor is waked up by nurse and go to the examination room
        //-------------------------------------------------------
        
        doctWaitingCV[pid-1] -> Wait(doctWaitingLock[pid-1]);
        
        DEBUG('o', "Doctor [%d] is leaving their office.\n", pid);
        
        erIndex = ndocESNo[pid-1];
        
        doctWaitingLock[pid-1] -> Release();
        
        examRoomsLock[erIndex] -> Acquire();
        
        if (wallPackets[erIndex] != NULL) 
        {
            
            holdingES = wallPackets[erIndex];
            
            holdingES -> docId = pid;
            
            wallPackets[erIndex] = NULL;
            
            if (holdingES -> patientType == 0) 
            {
                
                DEBUG('o', "Doctor [%d] is reading the examination sheet of Adult Patient [%d] in Examination room [%d].\n", pid, holdingES -> examSheetId, erIndex+1);
                
            }
            else
            {
                DEBUG('o', "Doctor [%d] is reading the examination sheet of Child Patient [%d] in Examination room [%d].\n", pid, holdingES -> examSheetId, erIndex+1);
            }
            
            //---------------------------------------
            // First Diagnose of Patient
            //---------------------------------------
            
            if (holdingES -> xrayStatus ==(XRAYSTATUS)XRAYUNKNOWN && 
                holdingES -> shotStatus == (SHOTSTATUS)SHOTUNKNOWN ) 
            {
                //------------------------------------------------------
                //determine whether a patient needs xray
                //-------------------------------------------------------
                
                int possibility = rand()%4;
                
                holdingES -> patientTask = (PATIENTTASK)PATDIAGNOSE;
                
                examRoomsCV[erIndex] -> Wait(examRoomsLock[erIndex]);
                
                if (possibility != 0) //nothing need to do
                {
                    
                    holdingES -> xrayStatus = (XRAYSTATUS)XRAYUNREQUIRED;
                    
                    holdingES -> xrayImages = 0;
                }
                else//patient need to take xray
                {
                    
                    holdingES -> xrayImages = rand()%3 + 1;//determine how many xray images patient to take
                    
                    if (holdingES -> patientType == 0) 
                    {
                        
                        DEBUG('o', "Doctor [%d] notes down in the sheet that Xray is needed for Adult Patient [%d] in Examination room [%d].\n", pid, holdingES -> examSheetId, erIndex+1);
                        
                    }
                    else
                    {
                        DEBUG('o', "Doctor [%d] notes down in the sheet that Xray is needed for Child Patient [%d] in Examination room [%d].\n", pid, holdingES -> examSheetId, erIndex+1);
                    }
                    
                    examRoomsCV[erIndex] -> Signal(examRoomsLock[erIndex]);
                    
                    examRoomsCV[erIndex] -> Wait(examRoomsLock[erIndex]);
                    
                    holdingES -> patientTask = (PATIENTTASK)PATGOXRAY;
                    
                }
                
                //------------------------------------------------------
                //determine whether a patient needs a shot
                //-------------------------------------------------------
                
                possibility = rand()%4;
                
                if (possibility != 0)// no shot 
                {
                    
                    holdingES -> shotStatus = (SHOTSTATUS)SHOTUNREQUIRED;
                }
                else//take shot
                {
                    holdingES -> shotStatus = (SHOTSTATUS)SHOTREQUIRED;
                    
                    if (holdingES -> patientType == 0) 
                    {
                        
                        DEBUG('o', "Doctor [%d] notes down in the sheet that Adult Patient [%d] needs to be given a shot in Examination room [%d].\n", pid, holdingES -> examSheetId, erIndex+1);
                        
                    }
                    else
                    {
                        DEBUG('o', "Doctor [%d] notes down in the sheet that Child Patient [%d] needs to be given a shot in Examination room [%d].\n", pid, holdingES -> examSheetId, erIndex+1);
                    }
                    
                    examRoomsCV[erIndex] -> Signal(examRoomsLock[erIndex]);
                    
                    examRoomsCV[erIndex] -> Wait(examRoomsLock[erIndex]);
                    
                }
                
                if (holdingES -> xrayStatus == (XRAYSTATUS)XRAYUNREQUIRED &&
                    holdingES -> shotStatus == (SHOTSTATUS)SHOTUNREQUIRED)//no shot and xray
                {
                    if (holdingES -> patientType == 0) 
                    {
                        
                        DEBUG('o', "Doctor [%d] diagnoses Adult Patient [%d] to be fine and is leaving Examination Room [%d].\n", pid, holdingES -> examSheetId, erIndex+1);
                        
                    }
                    else
                    {
                        DEBUG('o', "Doctor [%d] diagnoses Child Patient [%d] to be fine and is leaving Examination Room [%d].\n", pid, holdingES -> examSheetId, erIndex+1);
                    }
                }
                
                holdingES -> esProcess = (ESPROCESS)ESIDLE;
                
                wallPackets[erIndex] = holdingES;
                
                holdingES = NULL;
                
                //------------------------------------------------------
                //after diagnose,patient is waiting for nurse to escort to go proper place
                //-------------------------------------------------------
                
                examRoomsCV[erIndex] -> Signal(examRoomsLock[erIndex]);
                
                examRoomsCV[erIndex] -> Wait(examRoomsLock[erIndex]);
                
                examRoomsLock[erIndex] -> Release();                
                
                DEBUG('o', "Doctor [%d] has left Examination Room [%d].\n", pid, erIndex+1);
            } 
            else 
            {
                
                //-------------------------------------------
                // 2nd Diagnose of Patient With XRAY Finished
                //-------------------------------------------
                
                if (holdingES -> xrayStatus == (XRAYSTATUS)XRAYHALFDONE)
                {
                    
                    if (holdingES -> patientType == (PATIENTTYPE)ADULT)
                    {
                        
                        DEBUG('o', "Doctor [%d] is examining the Xrays of Adult Patient [%d] in Examination room [%d].\n", pid, holdingES -> examSheetId, erIndex+1);
                        
                    }
                    else
                    {
                        DEBUG('o', "Doctor [%d] is examining the Xrays of Child Patient [%d] in Examination room [%d].\n", pid, holdingES -> examSheetId, erIndex+1);
                    }
                    
                    holdingES -> patientTask = (PATIENTTASK)PATDIAGNOSEXRAY;
                    
                    holdingES -> docId = pid;
                    
                    examRoomsCV[erIndex] -> Wait(examRoomsLock[erIndex]);
                    
                    holdingES -> xrayStatus = (XRAYSTATUS)XRAYDONE;
                    
                    holdingES -> esProcess = (ESPROCESS)ESIDLE;
                    
                    examRoomsCV[erIndex] -> Signal(examRoomsLock[erIndex]);
                    
                    wallPackets[erIndex] = holdingES;
                    
                    holdingES = NULL;
                    
                    examRoomsLock[erIndex] -> Release();
                    
                    DEBUG('o', "Doctor [%d] has left Examination Room [%d].\n", pid, erIndex+1);
                    
                } 
                else 
                {
                    
                    examRoomsLock[erIndex] -> Release();
                    
                    printf("Error!!!!!!!.\n");
                }
            }
        } 
        else 
        {
            
            examRoomsLock[erIndex] -> Release();
            
            printf("Error?!.\n");
        }
        
        //------------------------------------------------------
        //after diagnose, doctor goes back to office
        //-------------------------------------------------------
        
        DEBUG('o', "Doctor [%d] is going to their office.\n", pid);
        
        currentThread -> Yield();
        
    }
}

void XRayTechnician(int pid)
{
    
    int erIndex;
    
    ExamSheet *holdingES;
    
    while (true) 
    {
        //------------------------------------------------------
        //check whether the user input number is valid
        //-------------------------------------------------------
        
        if (pid > xrayRoomsCount) 
        {
            
            printf("ERROR X-Ray Rooms Count.\n");
            
            break;
        } 
        
        //------------------------------------------------------
        //xray technician enters the xray room and waits for nurse signal
        //-------------------------------------------------------
        
        xrayRoomsLock[pid-1] -> Acquire();
        
        xrayRoomsCV[pid-1] -> Wait(xrayRoomsLock[pid-1]);
        
        //------------------------------------------------------
        //one patient comes to xray room and begins to take xray
        //-------------------------------------------------------
        
        holdingES = xrayRoomsES[pid-1];
        
        xrayRoomsES[pid-1] = NULL;
        
        xrayRoomsLock[pid-1] -> Release();
        
        erIndex = holdingES -> erId;
        
        examRoomsLock[erIndex] -> Acquire();
        
        if (holdingES -> patientType == (PATIENTTYPE)ADULT) 
        { 
            
            DEBUG('x', "Xray technician [%d] asks Adult Patient [%d] to get on the table.\n", pid, holdingES -> examSheetId);
        }
        else
        {
            
            DEBUG('x', "Xray technician [%d] asks Child Patient [%d] to get on the table.\n", pid, holdingES -> examSheetId);
            
        }
        
        //------------------------------------------------------
        //wait patient to prepare
        //-------------------------------------------------------
        
        examRoomsCV[erIndex] -> Signal(examRoomsLock[erIndex]);
        
        examRoomsCV[erIndex] -> Wait(examRoomsLock[erIndex]);
        
        //------------------------------------------------------
        //take xray images repetablely
        //-------------------------------------------------------
        
        for (int i = 0; i<holdingES -> xrayImages; i++) 
        {
            if (holdingES -> patientType == (PATIENTTYPE)ADULT)
            {   
                DEBUG('x', "Xray Technician [%d] asks Adult Patient [%d] to move.\n", pid, 
                      holdingES -> examSheetId);
            }
            else
            {
                DEBUG('x', "Xray Technician [%d] asks Child Patient [%d] to move.\n", pid, 
                      holdingES -> examSheetId);
            }
            
            examRoomsCV[erIndex] -> Signal(examRoomsLock[erIndex]);
            
            examRoomsCV[erIndex] -> Wait(examRoomsLock[erIndex]);
            
            if (holdingES -> patientType == (PATIENTTYPE)ADULT)
            {
                DEBUG('x', "Xray Technician [%d] takes an Xray Image of Adult Patient [%d].\n", pid, holdingES -> examSheetId);
            }
            else
            {
                DEBUG('x', "Xray Technician [%d] takes an Xray Image of Child Patient [%d].\n", pid, holdingES -> examSheetId);
            }
            
            examRoomsCV[erIndex] -> Signal(examRoomsLock[erIndex]);
            
            examRoomsCV[erIndex] -> Wait(examRoomsLock[erIndex]);
        }
        
        holdingES -> xrayStatus = (XRAYSTATUS)XRAYHALFDONE;
        
        holdingES -> xrayResult = (XRAYRESULT)(rand()%3);
        
        //------------------------------------------------------
        //xray writes the xray result on exmination sheet
        //-------------------------------------------------------
        
        switch (holdingES -> xrayResult) 
        {
                
            case (XRAYRESULT)NOTHING:
                
                if (holdingES -> patientType == (PATIENTTYPE)ADULT)
                {
                    DEBUG('x', "Xray Technician [%d] records nothing on Adult Patient [%d]'s examination sheet.\n", pid, holdingES -> examSheetId);
                }
                else
                {
                    DEBUG('x', "Xray Technician [%d] records nothing on Child Patient [%d]'s examination sheet.\n", pid, holdingES -> examSheetId);
                }
                break;
                
            case (XRAYRESULT)FRACTURE:
                
                if (holdingES -> patientType == (PATIENTTYPE)ADULT)
                {
                    DEBUG('x', "Xray Technician [%d] records compound fracture on Adult Patient [%d]'s examination sheet.\n", pid, holdingES -> examSheetId);
                }
                else
                {
                    DEBUG('x', "Xray Technician [%d] records compound fracture on Child Patient [%d]'s examination sheet.\n", pid, holdingES -> examSheetId);
                    
                }
                
                break;
                
            case (XRAYRESULT)BREAK:
                
                if (holdingES -> patientType == (PATIENTTYPE)ADULT)
                {
                    DEBUG('x', "Xray Technician [%d] records break on Adult Patient [%d]'s examination sheet.\n", pid, holdingES -> examSheetId);
                }
                else
                {
                    DEBUG('x', "Xray Technician [%d] records break on Child Patient [%d]'s examination sheet.\n", pid, holdingES -> examSheetId);
                }
                
                
                break;
                
            default:
                break;
        }
        
        //------------------------------------------------------
        //tells the patient to wait nurse in xray waiting room 
        //-------------------------------------------------------
        
        if (holdingES -> patientType == (PATIENTTYPE)ADULT)
        {
            
            DEBUG('x', "X-ray Technician [%d] tells Adult Patient [%d] to wait in Xray waiting room.\n", pid, holdingES -> examSheetId);
        }
        else
        {
            
            DEBUG('x', "X-ray Technician [%d] tells Parent [%d] to wait in Xray waiting room.\n", pid, holdingES -> examSheetId);
        }
        
        examRoomsCV[erIndex] -> Signal(examRoomsLock[erIndex]);
        
        examRoomsCV[erIndex] -> Wait(examRoomsLock[erIndex]);
        
        xrayWaitingLock -> Acquire();
        
        xrayWaitingES -> Append(holdingES);
        
        xrayWaitingLock -> Release();
        
        //------------------------------------------------------
        //xray puts the patient's examination sheet into wall pocket for nurse to take away
        //-------------------------------------------------------
        
        if (holdingES -> patientType == (PATIENTTYPE)ADULT) 
        {
            DEBUG('x', "X-ray Technician [%d] puts Adult Patient [%d] in Xray waiting room wall pocket.\n", pid, holdingES -> examSheetId);
        }
        else
        {
            DEBUG('x', "X-ray Technician [%d] puts Parent [%d] in Xray waiting room wall pocket.\n", pid, holdingES -> examSheetId);
        }
        
        
        holdingES = NULL;
        
        
        examRoomsLock[erIndex] -> Release();
        
        xrayRoomsStatusLock -> Acquire();
        
        xrayRoomsStatus[pid-1] = (XRRSTATUS)XRRFREE;
        
        xrayRoomsStatusLock -> Release();
        
    }
}
void Cashier(int pid)
{
    ExamSheet *holdingES;
    
    while (true) {
        
        //-----------------------------------------------------
        // Cashier wait for Nurse bringing patient to his desk
        //-----------------------------------------------------
        
        cashWaitingLock -> Acquire();
        
        if (cashWaitingCount > 0) 
        {
            
            cashWaitingCount --;
            
            cashWaitingCV -> Signal(cashWaitingLock);
            
            cashLock -> Acquire();
            
            cashWaitingLock -> Release();
            
            cashCV -> Wait(cashLock);
            
            
            //------------------------------------------
            // Cashier gets the ES and calculate the fee
            //------------------------------------------
            
            holdingES = cashExamSheet;
            
            if(holdingES -> patientType == 0)
            {
                
                DEBUG('c', "Cashier receives the examination sheet from Adult Patient [%d].\n",holdingES -> examSheetId);
            }
            else
            {
                
                DEBUG('c', "Cashier receives the examination sheet from Child Patient [%d] from Parent [%d].\n",holdingES -> examSheetId,holdingES -> examSheetId);
            }
            
            float totalAmount = 50.00 + 150.00;
            
            if (holdingES -> xrayStatus == (XRAYSTATUS)XRAYDONE) {
                
                totalAmount += holdingES -> xrayImages * 100.00 + 50.00;
                
                totalAmount += 150.00;
                
            }
            
            if (holdingES -> shotStatus == (SHOTSTATUS)SHOTDONE) {
                
                totalAmount += 40.00;
            }
            
            holdingES -> totalAmount = totalAmount;
            
            if(holdingES -> patientType == 0)
            {
                
                DEBUG('c', "Cashier reads the examination sheet of Adult Patient [%d] and asks him to pay $ [%f].\n", holdingES -> examSheetId,holdingES -> totalAmount);
            }
            else 
            {
                
                DEBUG('c', "Cashier reads the examination sheet of Child Patient [%d] and asks Parent [%d] to pay $ [%f].\n", holdingES -> examSheetId,holdingES -> examSheetId,holdingES -> totalAmount);
            }
            
            cashCV -> Signal(cashLock);
            
            cashCV -> Wait(cashLock);
            
            if(holdingES -> patientType == 0)
            {
                
                DEBUG('c', "Cashier accepts $[%f] from Adult Patient [%d].\n",
                       holdingES-> totalAmount,holdingES -> examSheetId);
            }
            else
            {
                
                DEBUG('c', "Cashier accepts $[%f] from Parent [%d].\n", 
                       holdingES-> totalAmount,holdingES -> examSheetId);
            }
            
            if(holdingES -> patientType == 0)
            {
                
                DEBUG('c', "Cashier gives a receipt of $[%f] to Adult Patient [%d].\n", holdingES->totalAmount, holdingES -> examSheetId);
            }
            else
            {
                
                DEBUG('c', "Cashier gives a receipt of $[%f] to Parent [%d].\n",holdingES -> totalAmount, holdingES -> examSheetId);
            }
            
            cashCV -> Signal(cashLock);
            
            cashCV -> Wait(cashLock);
       
            cashLock -> Release();
            
            delete holdingES;
            
        }
        else
        {
            cashWaitingLock -> Release();
        }
        
        
        currentThread -> Yield();
        
    }
    
}


// Global variables initiate, including Lock, Condition and other shared data
void GlobalVariablesInit()
{
    // Lock Init
    
    waitingRoomLock = new Lock("waitingRoomLock");
    
    wrnLock = new Lock("wrnLock");
    
    nwrnLock = new Lock("nwrnLock");
    
    for (int i=0; i<MAX_CHILD; i++) 
    {
        
        childWaitingLock[i] = new Lock("childWaitingLock");
    }
        
    childExamSheetLock = new Lock("childExamSheetLock");
    
    ersStatusLock = new Lock("ersStatusLock");
    
    broadCastIdLock = new Lock("broadCastIdLock");
    
    for (int i=0; i<MAX_EXAMROOMS; i++) 
    {
        examRoomsLock[i] = new Lock("examRoomsLock");
    }
    
    for (int i=0; i<MAX_DOCTORS; i++) 
    {
        
        doctWaitingLock[i] = new Lock("doctWaitingLock");
        
    }
    
    for (int i=0; i<MAX_XRAY; i++) 
    {
        
        xrayRoomsLock[i] = new Lock("xrayRoomsLock");
    
    }
    
    xrayWaitingLock = new Lock("xrayWaitingLock");
    
    cabinetLock = new Lock("cabinetLock");
    
    cashWaitingLock = new Lock("cashWaitingLock");
    
    cashLock = new Lock("cashLock");
    
    xrayRoomsStatusLock = new Lock("xrayRoomsStatusLock");
    
   
    // Condition Init
    
    waitingRoomCV = new Condition("waitingRoomCV");
    
    wrnCV = new Condition("wrnCV");
    
    nwrnCV = new Condition("nwrnCV");
    
    for (int i=0; i<MAX_CHILD; i++) 
    {
        
        childWaitingCV[i] = new Condition("childWaitingCV");
    }
    
    nwrnWaitingCV = new Condition("nwrnWaitingCV");
    
    broadCastIdCV = new Condition("broadCastIdCV");
    
    bcIdWaitingCV = new Condition("bcIdWaitingCV");
    
    for (int i=0; i<MAX_EXAMROOMS; i++) 
    {
        
        examRoomsCV[i] = new Condition("examRoomsCV");
    }
    
    for (int i=0; i<MAX_DOCTORS; i++) 
    {
        
        doctWaitingCV[i] = new Condition("doctWaitingCV");
    }
    
    for (int i=0; i<MAX_XRAY; i++) 
    {
        
        xrayRoomsCV[i] = new Condition("xrayRoomsCV");

    }
    
    cashWaitingCV = new Condition("cashWaitingCV");
    
    cashCV = new Condition("cashCV");
    
    // Monitor Variables Init
    
    wrWaitingCount = 0;
    
    wrnStatus = (WRNSTATUS)WRNFREE;
    
    wrnTask = (WRNTASK)NONE;
    
    wrnExamSheet = NULL;
    
    wrnCompleteESList = new List();
    
    for (int i = 0; i<examRooms; i++) 
    {
        
        examRoomsStatus[i] = (ERSTATUS)ERFREE;
    }
    
    for (int i=0; i<MAX_EXAMROOMS; i++) 
    {
        
        wallPackets[i] = NULL;
    }
    
    for (int i=0; i<MAX_DOCTORS; i++) 
    {
        
        ndocESNo[i] = -1;
        
        docStatus[i] = (DOCSTATUS)DOCFREE;
    }
    
    for (int i=0; i<MAX_XRAY; i++) 
    {
        
        xrayRoomsES[i] = NULL;
        
        xrayRoomsStatus[i] = (XRRSTATUS)XRRFREE;
    }
    
    xrayWaitingES= new List();
    
    cashExamSheet = NULL;
    
    cashWaitingCount = 0;
    
    bcIdStatus = (BROADCASTIDSTATUS)BROADFREE;

    nwrnExamSheet = NULL;

    for (int i=0; i<MAX_CHILD; i++) 
    {
        childWaitingTask[i] = (CHILDTASK)CHILDNONE;
    }
}

// Global variables clean up
void GlobalVariablesClean()
{
    delete waitingRoomLock;
    
    delete wrnLock;
    
    delete nwrnLock;
    
    for (int i=0; i<MAX_CHILD; i++) 
    {
        
        delete childWaitingLock[i];
    }
    
    delete childExamSheetLock;
    
    delete ersStatusLock;
    
    delete broadCastIdLock;
    
    for (int i=0; i<MAX_EXAMROOMS; i++) 
    {
        delete examRoomsLock[i];
    }
    
    for (int i=0; i<MAX_DOCTORS; i++) 
    {
        
        delete doctWaitingLock[i];
        
    }
    
    for (int i=0; i<MAX_XRAY; i++) 
    {
        
        delete xrayRoomsLock[i];
        
    }
    
        
    
    delete xrayWaitingLock;
    
    delete cabinetLock;
    
    delete cashWaitingLock;
    
    
    delete cashLock;
    
    delete xrayRoomsStatusLock;
    
    delete waitingRoomCV;
    
    delete wrnCV;
    
    delete nwrnCV;
    
    for (int i=0; i<MAX_CHILD; i++) 
    {
        
        delete childWaitingCV[i];
    }
    
    delete nwrnWaitingCV;
    
    delete broadCastIdCV;
    
    delete bcIdWaitingCV;
    
    for (int i=0; i<MAX_EXAMROOMS; i++) 
    {
        
        delete examRoomsCV[i];
    }
    
    for (int i=0; i<MAX_DOCTORS; i++) 
    {
        
        delete doctWaitingCV[i];
    }
    
    for (int i=0; i<MAX_XRAY; i++) 
    {
        
        delete xrayRoomsCV[i];
        
    }
    
    delete cashWaitingCV;
    
    delete cashCV;
    
    // Monitor Variables Init
    
    delete wrnCompleteESList;
    
    delete xrayWaitingES;
    
}

// Get input form console
bool HospitalGetInput()
{
    
    printf("Enter the Number of Doctors: ");
    
    if(scanf("%d", &docCount) == 0)
    {
        printf("Error Input Format.\n");
        
        return false;
    };
    
    printf("Enter the Number of Nurses: ");
    
    
    if(scanf("%d", &nurses) == 0)
    {
        printf("Error Input Format.\n");
        
        return false;
    };
    
    printf("Enter the Number of XRay Technician/Rooms: ");
    
    if(scanf("%d", &xrayRoomsCount) == 0)
    {
        printf("Error Input Format.\n");
        
        return false;
    };
    
    printf("Enter the Number of Patients: ");
    
    if(scanf("%d", &patients) == 0)
    {
        printf("Error Input Format.\n");
        
        return false;
    };
    
    printf("Enter the Number of ExamRooms: ");
    
    if(scanf("%d", &examRooms) == 0)
    {
        printf("Error Input Format.\n");
        
        return false;
    };
    
    
    wrns = 1;
    
    cashiers = 1;
    
    doctors = docCount;
    
    xrays = xrayRoomsCount;
    
    bool flag = true;
    
    if (examRooms > MAX_EXAMROOMS) 
    {
        flag = false;
        
        printf("Too many ERS you input, the MAX ERS is %d.\n", MAX_EXAMROOMS);
    }
    
    if (examRooms < MIN_EXAMROOMS) {
        
        flag = false;
        
        printf("Too few ERS you input, the MIN ERS is %d.\n", MIN_EXAMROOMS);
    }
    
    if (xrayRoomsCount > MAX_XRAY) 
    {
        flag = false;
        
        printf("Too many XRAYS you input, the MAX XRAYS is %d.\n", MAX_XRAY);
    }
    
    if (xrayRoomsCount < MIN_XRAY) {
        
        flag = false;
        
        printf("Too few XRAYS you input, the MIN XRAYS is %d.\n", MIN_XRAY);
    }
    
    if (docCount > MAX_DOCTORS) 
    {
        flag = false;
        
        printf("Too many DOCTORS you input, the MAX DOCTORS is %d.\n", MAX_DOCTORS);
    }
    
    if (docCount < MIN_DOCTORS) {
        
        flag = false;
        
        printf("Too few DOCTORS you input, the MIN DOCTORS is %d.\n", MIN_DOCTORS);
    }
    
    if (nurses > MAX_NURSES) {
        
        flag = false;
        
        printf("Too many NURSES you input, the MAX NURSES is %d.\n", MAX_NURSES);
    }
    if (nurses < MIN_NURSES) {
        
        flag = false;
        
        printf("Too few NURSES you input, the MIN NURSES is %d.\n", MIN_NURSES);
    }
    
    return flag;
}



/*test case 1, testing that Child Patients are 
 never abandoned by their Parent, nor go anywhere without their Parent. */
void test_1()
{
    Thread *t;
    
    GlobalVariablesInit();
    
    DebugInit("ph");
    
    printf("******************************************************\n");
    
    printf("Starting Test 1\n");
    
    printf("******************************************************\n");
    
    printf("Number of Doctors = [%d]\n", 1);
    
    printf("Number of Nurse = [%d]\n", 2);
    
    printf("Number of XRay Technicians/Rooms = [%d]\n", 1);
    
    printf("Number of Patients = [%d]\n", 1);
    
    printf("Number of Parents/Child Patients = [%d]", 1);
    
    printf("Number of Examination Rooms = [%d]\n", 2);
    
    printf("******************************************************\n");
    
    
    t = new Thread("WaitingRoomNurse");
    
    t -> Fork(WaitingRoomNurse, 1);
    
    threadList -> Append(t);
    
    
    t = new Thread("Parent");
    
    t -> Fork(Parent, 1);
    
    threadList -> Append(t);
    
    
    t = new Thread("Child");
    
    t -> Fork(Child, 1);
    
    threadList -> Append(t);

    
    t = new Thread("Nurse");
    
    t -> Fork(Nurse, 1);
    
    threadList -> Append(t);
    
    
    t = new Thread("Doctor");
    
    t -> Fork(Doctor, 1);
    
    threadList -> Append(t);
    
    
    t = new Thread("XRayTechnician");
    
    t -> Fork(XRayTechnician, 1);
    
    threadList -> Append(t);
    
    
    t = new Thread("Cashier");
    
    t -> Fork(Cashier, 1);
    
    threadList -> Append(t);
    
    
    for (int i=0; i<1000; i++) 
    {
        
        currentThread -> Yield();
    }
    
    while (!threadList -> IsEmpty()) 
    {
        
        t = (Thread *)threadList -> Remove();
        
        delete t;
    }
    
    scheduler -> Clear();
    
    GlobalVariablesClean();
    
    printf("\n\n");
    
    printf("Test 1 Complete\n");
    
    printf("******************************************************\n");
}

/*test case 2, testing that Waiting Room Nurses only talk to one Patient/Parent at a time. */
void test_2()
{
    Thread *t;
    
    GlobalVariablesInit();
    
    DebugInit("wl");
    
    printf("******************************************************\n");
    
    printf("Starting Test 2\n");
    
    printf("******************************************************\n");
    
    printf("Number of Doctors = [%d]\n", 0);
    
    printf("Number of Nurse = [%d]\n", 0);
    
    printf("Number of XRay Technicians/Rooms = [%d]\n", 0);
    
    printf("Number of Patients = [%d]\n", 2);
    
    printf("Number of Parents/Child Patients = [%d]", 1);
    
    printf("Number of Examination Rooms = [%d]\n", 2);
    
    printf("******************************************************\n");
        
    

    
    t = new Thread("WaitingRoomNurse");
    
    t -> Fork(WaitingRoomNurse, 1);
    
    threadList -> Append(t);
    
    
    t = new Thread("Patient");
    
    t -> Fork(Patient, 1);
    
    threadList -> Append(t);
    
    
    t = new Thread("Patient");
    
    t -> Fork(Patient, 2);
    
    threadList -> Append(t);
    
    
    for (int i=0; i<1000; i++) 
    {
        
        currentThread -> Yield();
    }
    
    while (!threadList -> IsEmpty()) 
    {
        
        t = (Thread *)threadList -> Remove();
        
        delete t;
    }
    
    scheduler -> Clear();
    
    GlobalVariablesClean();
    
    printf("\n\n");
    
    printf("Test 2 Complete\n");
    
    printf("******************************************************\n");

}

/*test case 3, testing that Cashiers only talk to one Patient/Parent at a time.*/
void test_3()
{
    Thread *t;
    
    GlobalVariablesInit();
    
    DebugInit("cl");
    
    printf("******************************************************\n");
    
    printf("Starting Test 3\n");
    
    printf("******************************************************\n");
    
    printf("Number of Doctors = [%d]\n", 1);
    
    printf("Number of Nurse = [%d]\n", 1);
    
    printf("Number of XRay Technicians/Rooms = [%d]\n", 1);
    
    printf("Number of Patients = [%d]\n", 2);
    
    printf("Number of Parents/Child Patients = [%d]", 0);
    
    printf("Number of Examination Rooms = [%d]\n", 2);
    
    printf("******************************************************\n");
    
    
    t = new Thread("WaitingRoomNurse");
    
    t -> Fork(WaitingRoomNurse, 1);
    
    threadList -> Append(t);
    
    
    t = new Thread("Patient");
    
    t -> Fork(Patient, 1);
    
    threadList -> Append(t);
    
    
    t = new Thread("Patient");
    
    t -> Fork(Patient, 2);
    
    threadList -> Append(t);
    
    
    t = new Thread("Nurse");
    
    t -> Fork(Nurse, 1);
    
    threadList -> Append(t);
    
    
    t = new Thread("Doctor");
    
    t -> Fork(Doctor, 1);
    
    threadList -> Append(t);
    
    
    t = new Thread("XRayTechnician");
    
    t -> Fork(XRayTechnician, 1);
    
    threadList -> Append(t);
    
    
    t = new Thread("Cashier");
    
    t -> Fork(WaitingRoomNurse, 1);
    
    threadList -> Append(t);
    
    for (int i=0; i<1000; i++) 
    {
        
        currentThread -> Yield();
    }
    
    while (!threadList -> IsEmpty()) 
    {
        
        t = (Thread *)threadList -> Remove();
        
        delete t;
    }
    
    scheduler -> Clear();

    GlobalVariablesClean();
    
    printf("\n\n");
    
    printf("Test 3 Complete\n");

    printf("******************************************************\n");
    
}

/*test case 4, testing that Patients/Parents never go anywhere without being escorted by a Nurse.*/
void test_4()
{
    Thread *t;
    
    GlobalVariablesInit();
    
    DebugInit("lu");
    
    printf("******************************************************\n");
    
    printf("Starting Test 4\n");
    
    printf("******************************************************\n");
    
    printf("Number of Doctors = [%d]\n", 1);
    
    printf("Number of Nurse = [%d]\n", 1);
    
    printf("Number of XRay Technicians/Rooms = [%d]\n", 1);
    
    printf("Number of Patients = [%d]\n", 1);
    
    printf("Number of Parents/Child Patients = [%d]", 1);
    
    printf("Number of Examination Rooms = [%d]\n", 2);
    
    printf("******************************************************\n");
    
    t = new Thread("WaitingRoomNurse");
    
    t -> Fork(WaitingRoomNurse, 1);
    
    threadList -> Append(t);
    
    
    t = new Thread("Patient");
    
    t -> Fork(Patient, 1);
    
    threadList -> Append(t);
    
    
    t = new Thread("Nurse");
    
    t -> Fork(Nurse, 1);
    
    threadList -> Append(t);
    
    
    t = new Thread("Doctor");
    
    t -> Fork(Doctor, 1);
    
    threadList -> Append(t);
    
    
    t = new Thread("XRayTechnician");
    
    t -> Fork(XRayTechnician, 1);
    
    threadList -> Append(t);
    
    
    t = new Thread("Cashier");
    
    t -> Fork(WaitingRoomNurse, 1);
    
    threadList -> Append(t);
    
    for (int i=0; i<1000; i++) 
    {
        
        currentThread -> Yield();
    }
    
    while (!threadList -> IsEmpty()) 
    {
        
        t = (Thread *)threadList -> Remove();
        
        delete t;
    }
    
    scheduler -> Clear();

    GlobalVariablesClean();
    
    printf("\n\n");
    
    printf("Test 4 Complete\n");
    
    printf("******************************************************\n");
}

/*test case 5, testing that All Patients leave the Doctor's Office. No one stays in the Waiting Room, Examination Room, or Xray Room, forever.*/
void test_5()
{
    Thread *t;
    
    GlobalVariablesInit();
    
    DebugInit("l");
    
    printf("******************************************************\n");
    
    printf("Starting Test 5\n");
    
    printf("******************************************************\n");  
    
    printf("Number of Doctors = [%d]\n", 1);
    
    printf("Number of Nurse = [%d]\n", 3);
    
    printf("Number of XRay Technicians/Rooms = [%d]\n", 1);
    
    printf("Number of Patients = [%d]\n", 5);
    
    printf("Number of Parents/Child Patients = [%d]", 0);
    
    printf("Number of Examination Rooms = [%d]\n", 2);
    
    printf("******************************************************\n");
    
    t = new Thread("WaitingRoomNurse");
    
    t -> Fork(WaitingRoomNurse, 1);
    
    threadList -> Append(t);
    
    
    for (int i =1; i<=5; i++) {
        
        t = new Thread("Patient");
        
        t -> Fork(Patient, i);
        
        threadList -> Append(t);
        
    }
    
    for (int i = 1; i<=3; i++) {
        
        
        t = new Thread("Nurse");
        
        t -> Fork(Nurse, i);
        
        threadList -> Append(t);
    }
    
    
    t = new Thread("Doctor");
    
    t -> Fork(Doctor, 1);
    
    threadList -> Append(t);
    
    
    t = new Thread("XRayTechnician");
    
    t -> Fork(XRayTechnician, 1);
    
    threadList -> Append(t);
    
    
    t = new Thread("Cashier");
    
    t -> Fork(WaitingRoomNurse, 1);
    
    threadList -> Append(t);
    
    for (int i=0; i<1000; i++) 
    {
        
        currentThread -> Yield();
    }
    
    while (!threadList -> IsEmpty()) 
    {
        
        t = (Thread *)threadList -> Remove();
        
        delete t;
    }
    
    scheduler -> Clear();
    
    GlobalVariablesClean();
    
    printf("\n\n");
    
    printf("Test 5 Complete\n");
    
    printf("******************************************************\n");
    

}

/*test case 6, testing thatTwo Doctors never examine the same Patient at the same time.*/
void test_6()
{
    Thread *t;
    
    GlobalVariablesInit();
    
    DebugInit("o");
    
    printf("******************************************************\n");
    
    printf("Starting Test 6\n");
    
    printf("******************************************************\n");
    
    printf("Number of Doctors = [%d]\n", 2);
    
    printf("Number of Nurse = [%d]\n", 1);
    
    printf("Number of XRay Technicians/Rooms = [%d]\n", 1);
    
    printf("Number of Patients = [%d]\n", 1);
    
    printf("Number of Parents/Child Patients = [%d]", 1);
    
    printf("Number of Examination Rooms = [%d]\n", 2);
    
    printf("******************************************************\n");
    
    
    

    
    t = new Thread("WaitingRoomNurse");
    
    t -> Fork(WaitingRoomNurse, 1);
    
    threadList -> Append(t);
    
    
    t = new Thread("Patient");
    
    t -> Fork(Patient, 1);
    
    threadList -> Append(t);
    
    
    t = new Thread("Nurse");
    
    t -> Fork(Nurse, 1);
    
    threadList -> Append(t);
    
    
    t = new Thread("Doctor");
    
    t -> Fork(Doctor, 1);
    
    threadList -> Append(t);
    
    t = new Thread("Doctor");
    
    t -> Fork(Doctor, 2);
    
    threadList -> Append(t);
    
    
    t = new Thread("XRayTechnician");
    
    t -> Fork(XRayTechnician, 1);
    
    threadList -> Append(t);
    
    
    for (int i=0; i<1000; i++) 
    {
        
        currentThread -> Yield();
    }
    
    while (!threadList -> IsEmpty()) 
    {
        
        t = (Thread *)threadList -> Remove();
        
        delete t;
    }
    
    scheduler -> Clear();
    
    GlobalVariablesClean();
    
    printf("\n\n");
    
    printf("Test 6 Complete\n");
    
    printf("******************************************************\n");
}

void test_suite()
{
    
    char op;
    
    bool flag = false;
    
    while(true)
    {
        
        printf("**************************\n");
        
        printf("1. Test Case 1.\n");
        
        printf("2. Test Case 2.\n");
        
        printf("3. Test Case 3.\n");
        
        printf("4. Test Case 4.\n");
        
        printf("5. Test Case 5.\n");
        
        printf("6. Test Case 6.\n");
        
        printf("7. System Case.\n");
        
        printf("8. Quit.\n");
        
        printf("**************************\n");
        
        printf("Your Choice?");
        
        fflush(stdin);
        
        op = getchar();
        
        getchar();
        
        switch(op)
        {
            case '1':
                
                test_1();
                
                break;
            
            case '2':
            
                test_2();
                
                break;
                
            case '3':
                
                test_3();
                
                break;
                
            case '4':
                
                test_4();
                
                break;
                
            case '5':
                
                test_5();
                
                break;
                
            case '6':
                
                test_6();
                
                break;
                
            case '7':
                
                HospitalTest();
                
                break;
                
            case '8':
                
                flag = true;
                
                break;
                
            default: 
                break;
        }
        
        if(flag)
        {
            break;
    
        }
    }
}

/*system case test, test many entities at a time */
void HospitalTest()
{
   
    GlobalVariablesInit();
    
    DebugInit("ulwocphx");
    
    if(!HospitalGetInput())
        
        return; 
    
    Thread *t;
    
    children = 0;
    
    for (int pid = 1; pid<= xrays; pid ++) 
    {
        
        t = new Thread("XRayTechnician_" + (pid));
        
        t -> Fork(XRayTechnician, pid);
        
        threadList -> Append(t);
        
    }
    
    for (int pid = 1; pid <= cashiers; pid++) 
    {
        
        t = new Thread("Cashier_" + (pid));
        
        t -> Fork(Cashier, pid);
        
        threadList -> Append(t);
    
    }
    
    for (int pid = 1; pid <= doctors; pid ++) 
    {
        
        t = new Thread("Doctor_" + (pid));
        
        t -> Fork(Doctor, pid);
        
        threadList -> Append(t);
    }
    
    for (int pid = 1; pid <= wrns; pid ++) 
    {
        
        t = new Thread("WaitingRoomNurse_" + (pid));
        
        t -> Fork(WaitingRoomNurse, pid);
        
        threadList -> Append(t);
    }
    
    
    for (int pid = 1; pid <= nurses; pid ++) 
    {
        
        t = new Thread("Nurse_" + (pid));
        
        t -> Fork(Nurse, pid);
        
        threadList -> Append(t);
        
    }
    
    for (int pid = 1; pid<=patients; pid++) 
    {
        if (rand()%2 == 0) {
        
            t = new Thread("Patient_"+(pid));
            
            t -> Fork(Patient, pid);
            
            threadList -> Append(t);
            
        } 
        else
        {
            t = new Thread("Patient_" + (pid));
            
            t -> Fork(Parent, pid);
            
            threadList -> Append(t);
            
            t = new Thread("Child_" + (pid));
            
            t -> Fork(Child, pid);
            
            threadList -> Append(t);
            
            children++;
            
        }
        
    }
    
    printf("******************************************************\n");
    
    printf("Starting System Test Case\n");
    
    printf("******************************************************\n"); 
    
    printf("Number of Doctors(%d to %d) = [%d]\n", MIN_DOCTORS, MAX_DOCTORS, doctors);
    
    printf("Number of Nurse(%d to %d) = [%d]\n", MIN_NURSES, MAX_NURSES, nurses);
    
    printf("Number of XRay Technicians/Rooms(%d to %d) = [%d]\n", MIN_XRAY, MAX_XRAY, xrays);
    
    printf("Number of Patients = [%d]\n", patients);
    
    printf("Number of Parents/Child Patients = [%d].\n", children);
    
    printf("Number of Examination Rooms(%d to %d) = [%d]\n", MIN_EXAMROOMS, MAX_EXAMROOMS, examRooms);
    
    printf("******************************************************\n");
    
    for (long int i=0; i<patients * 1000; i++) {
        
        currentThread -> Yield();
    }
    
    while (!threadList -> IsEmpty()) 
    {
        
        t = (Thread *)threadList -> Remove();
        
        delete t;
    }
    
    scheduler -> Clear();
    
    GlobalVariablesClean();
    
    printf("\n\n");
    
    printf("System Test Case Complete\n");
    
    printf("******************************************************\n");
    

    
}













