/*
 * Scheduler.c
 *
 *  Created on: Feb 11, 2024
 *      Author: Mina Saad
 */

//-------------------------<Include>
#include "Scheduler.h"
#include "MyRTOS_FIFO.h"

//*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*//
//*                                        Generic variables                                       //
//*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*//

//The FIFO
FIFO_buf_t Ready_QUEUE;

//The created Buffer
Task_REF* Tasks_Buffer[10];
//Define Idle Task
Task_REF MyRTOS_IdleTask;

uint32_t IdleTask_LED = 1;

/*                                                *=*=*=*=*=*=*=*=*=*=*=*
 *-------------------|============================*  Global User Define *===========================|----------------------//
 *                                                *=*=*=*=*=*=*=*=*=*=*=*
 */

 struct
{
	Task_REF* OS_Tasks[30]; //Scheduler Table
	uint32_t* _S_MSP_Task;
	uint32_t* _E_MSP_Task;
	uint32_t* Task_Locator;
	uint32_t  NoOFActiveTasks;
	Task_REF* CurrentTask;
	Task_REF* NextTask;
	enum
	{
		OSsuspend, OSrunning

	}OSmodeID;

} OS_Control;

 typedef enum
{
	SVC_ActivateTask, SVC_TerminateTask,
	SVC_WaitingTask

}SVC_ID;

//                               *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//-------------------------------*             Services funtion to PendS                 *------------------------------//
//                               *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*

__attribute ((naked)) void OS_PendSV_Services () //To switch Context/Store
{
	//=============================================================================
	//Save the Context of the Current Task
	//=============================================================================
	OS_Get_PSP(OS_Control.CurrentTask->Current_PSP);   /* Get the Current PSP for the current tast */

	//Use this Current_PSP to store (r4 to r11)
	OS_Control.CurrentTask->Current_PSP--;
	__asm volatile("mov %0, r4" : "=r" (*(OS_Control.CurrentTask->Current_PSP)) );
	OS_Control.CurrentTask->Current_PSP--;
	__asm volatile("mov %0, r5" : "=r" (*(OS_Control.CurrentTask->Current_PSP)) );
	OS_Control.CurrentTask->Current_PSP--;
	__asm volatile("mov %0, r6" : "=r" (*(OS_Control.CurrentTask->Current_PSP)) );
	OS_Control.CurrentTask->Current_PSP--;
	__asm volatile("mov %0, r7" : "=r" (*(OS_Control.CurrentTask->Current_PSP)) );
	OS_Control.CurrentTask->Current_PSP--;
	__asm volatile("mov %0, r8" : "=r" (*(OS_Control.CurrentTask->Current_PSP)) );
	OS_Control.CurrentTask->Current_PSP--;
	__asm volatile("mov %0, r9" : "=r" (*(OS_Control.CurrentTask->Current_PSP)) );
	OS_Control.CurrentTask->Current_PSP--;
	__asm volatile("mov %0, r10" : "=r" (*(OS_Control.CurrentTask->Current_PSP)) );
	OS_Control.CurrentTask->Current_PSP--;
		__asm volatile("mov %0, r11" : "=r" (*(OS_Control.CurrentTask->Current_PSP)) );
	//==================================================================================================================

	//=============================================================================
	//Restore the Context of the Current Task
	//=============================================================================
	//Switch to next task
	if (OS_Control.NextTask != NULL)
	{
		OS_Control.CurrentTask = OS_Control.NextTask;
		OS_Control.NextTask = NULL;
	}

	//Use this Current_PSP to restore (r4 to r11)
	__asm volatile("mov r11, %0" : : "r" (*(OS_Control.CurrentTask->Current_PSP)) );
	OS_Control.CurrentTask->Current_PSP++;
	__asm volatile("mov r10, %0" : : "r" (*(OS_Control.CurrentTask->Current_PSP)) );
	OS_Control.CurrentTask->Current_PSP++;
	__asm volatile("mov r9, %0 " : : "r" (*(OS_Control.CurrentTask->Current_PSP)) );
	OS_Control.CurrentTask->Current_PSP++;
	__asm volatile("mov r8, %0 " : : "r" (*(OS_Control.CurrentTask->Current_PSP)) );
	OS_Control.CurrentTask->Current_PSP++;
	__asm volatile("mov r7, %0 " : : "r" (*(OS_Control.CurrentTask->Current_PSP)) );
	OS_Control.CurrentTask->Current_PSP++;
	__asm volatile("mov r6, %0 " : : "r" (*(OS_Control.CurrentTask->Current_PSP)) );
	OS_Control.CurrentTask->Current_PSP++;
	__asm volatile("mov r5, %0 " : : "r" (*(OS_Control.CurrentTask->Current_PSP)) );
	OS_Control.CurrentTask->Current_PSP++;
	__asm volatile("mov r4, %0 " : : "r" (*(OS_Control.CurrentTask->Current_PSP)) );
	OS_Control.CurrentTask->Current_PSP++;

	OS_Set_PSP(OS_Control.CurrentTask->Current_PSP); /* Get the Current PSP for the next tast */
	__asm volatile("bx LR"); //Retrun to task in ROM

}

//========================================================================================================================

//                             *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//-----------------------------*                  Services Function                    *---------------------------------//
//                             *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*

void BubbleSort ()
{
	int i = 0, j = 0, n = OS_Control.NoOFActiveTasks;
    Task_REF* Temp = NULL;

    for (i = 0; i < n - 1; i++) //Number of Elements - 1"Last element will not be compared"
    {
    	for (j = 0; j < n - i - 1; j++) //Number of compares
    	{
    		if (OS_Control.OS_Tasks[j]->Priority >  OS_Control.OS_Tasks[j + 1]->Priority)
    		{
    			//Swipping with Temp Variable
    			Temp = OS_Control.OS_Tasks[j];
    			OS_Control.OS_Tasks[j] = OS_Control.OS_Tasks[j + 1];
    			OS_Control.OS_Tasks[j + 1] = Temp;
    		}

    	}

	}

}

void Decide_WhatNext ()
{
	//If ready queue is empty && OS_Control->CurrentTask != Suspend
	if (Ready_QUEUE.count == 0 && OS_Control.CurrentTask->TaskState != Suspend)
	{
		OS_Control.CurrentTask->TaskState = Running;

		//Add the current task to Queue
		FIFO_buf_enqueue(&Ready_QUEUE, OS_Control.CurrentTask);

		OS_Control.NextTask = OS_Control.CurrentTask;

	} else
	{
		FIFO_buf_dequeue(&Ready_QUEUE, &OS_Control.NextTask);
		OS_Control.NextTask->TaskState = Running;

		//Update ready queue
		if (OS_Control.CurrentTask->Priority == OS_Control.NextTask->Priority && OS_Control.CurrentTask->TaskState != Suspend)
		{
			FIFO_buf_enqueue(&Ready_QUEUE, OS_Control.CurrentTask);
			OS_Control.CurrentTask->TaskState = Ready;
		}

	}
}


void MyRTOS_Update_SchedulerTable () //Handler Mode
{
	int i = 0;
	Task_REF* PTask = NULL , *PNextTask = NULL;

	//1- Bubble Sort scheduler TAble OS_Contro--> OSTasks[100] priority high than low
	BubbleSort();
    //=============================================================================================================

	//2- Clean Ready Queue
	while (FIFO_Clean_Queue(&Ready_QUEUE));
    //=============================================================================================================
	//3- Update Ready Queue
	while (i < (OS_Control.NoOFActiveTasks))
    {
    	PTask = OS_Control.OS_Tasks[i];
    	PNextTask = OS_Control.OS_Tasks[i + 1];

    	if (PTask->TaskState != Suspend)
    	{   //Not to test the PNextTask is the end always Because: In Task_REF* OS_Tasks[30] there are mix of suspend and waiting Tasks

    		if (PNextTask->TaskState == Suspend)
    		{   //ASSUMPTION: in case the second Task is suspend the third element is not the Same priority as the First task

    			FIFO_buf_enqueue(&Ready_QUEUE, PTask); /* Put the task in Queue */
    			PTask->TaskState = Ready;
    			break;

    		} else if (PTask->Priority < PNextTask->Priority )
    		{
    			FIFO_buf_enqueue(&Ready_QUEUE, PTask); /* Put the task in Queue */
    			PTask->TaskState = Ready;
    			break;

    		} else if (PNextTask->Priority == PTask->Priority)
    		{
    			FIFO_buf_enqueue(&Ready_QUEUE, PTask); /* Put the task in Queue */
    			PTask->TaskState = Ready;
    		}
    	}
    	i++;
    }
    //=============================================================================================================
    //=============================================================================================================

}

void OS_SVC_Set (SVC_ID ID) //========================To Set specific services
{
	switch (ID)
	{
	case SVC_ActivateTask: //Activate task
		OS_Generate_Exception_To_Activate();
		break;

	case SVC_TerminateTask: //terminate task
		OS_Generate_Exception_To_Terminate();
		break;

	case SVC_WaitingTask: //terminate task
		OS_Generate_Exception_To_Waiting();
		break;
	}
}

//                                                   |Handler Mode|
//-------------------------|==================To execute specfic services===============|----------------------------//
void OS_SVC_Services (int *Frame_Pointer)
{
	//in stack XPSR, PC, LR, r12, r3, r2, r1, r0 -->We want to get Passed argument in SVC instruction
	SVC_ID ID =  (SVC_ID)( *( ( (unsigned char*)(Frame_Pointer[6]) ) - 2 ) );

	switch (ID)
	{
	case SVC_ActivateTask:	//Activate task
	case SVC_TerminateTask:	//Terminate task
		MyRTOS_Update_SchedulerTable(); //--------->/* Update Scheduler Table */

	    //OS is in Running State
		if (OS_Control.OSmodeID == OSrunning)
		{
			if (OS_Control.CurrentTask != &MyRTOS_IdleTask)
			{
				//Decide what is next
				Decide_WhatNext();

			    //Trigger OS_PendSV (Switch context/Restore)
				Trigger_PendSV();
			}
		}
		break;

	case SVC_WaitingTask:
		MyRTOS_Update_SchedulerTable(); //--------->/* Update Scheduler Table */
		break;
	}
}

void MyRTOS_Create_MainStack ()
{
	//Create main stack
	OS_Control._S_MSP_Task = &_estack; /*_S_MSP_Task = Stack_Top*/

	OS_Control._E_MSP_Task = OS_Control._S_MSP_Task - (MainStack / 4); /* _E_MSP_Task is uint32_t Pointer */
    //=============================================================================================================

	//Aligned 8 Byte to Task_Locator
	OS_Control.Task_Locator = ( OS_Control._E_MSP_Task - 2 );
}

void MyRTOS_CreateTaskStack (Task_REF* Tref)
{
	int i = 0;
	/* Task Frame
	 * =========
	 * XPSR
	 * PC (next instruction)
	 * LR
	 * r12
	 * r3
	 * r2
	 * r1
	 * r0
	 * r4, r5, r6, r7, r8, r9, r10, r11
	 */
	Tref->Current_PSP = Tref->_S_PSP_Task;

	Tref->Current_PSP--;
	*(Tref->Current_PSP) = 0x01000000;  //DUMMY_XPSR should T= 1 to avoid Bus fault

	Tref->Current_PSP--;
	*(Tref->Current_PSP) = (uint32_t)Tref->P_TaskEntry; //PC

	Tref->Current_PSP--;
	*(Tref->Current_PSP) = (uint32_t)0xFFFFFFFD; //LR = 0xFFFFFFFD

    for (i = 0; i < 13; i++)
    {
    	Tref->Current_PSP--;
    	*(Tref->Current_PSP) = 0;
    }
}

void MyRTOS_IdleTask_IMPLT ()
{
	while (1)
	{
		IdleTask_LED ^= 1;   /* ASSUMPTION: The IdleTask is not the only task which is running first  */
	}

}


void Update_TaskWaitingTime ()
{
	int i =0;

	for (i = 0; i < OS_Control.NoOFActiveTasks; i++)
	{
		if (OS_Control.OS_Tasks[i]->TaskState == Suspend)
		{
			if (OS_Control.OS_Tasks[i]->TimeWaiting.Blocking == BlockingEnable) //It is blocking until meet the time line
			{
				if (OS_Control.OS_Tasks[i]->TimeWaiting.Ticks_Count == 0)
				{
					OS_Control.OS_Tasks[i]->TimeWaiting.Blocking = BlockingDisable;
					OS_Control.OS_Tasks[i]->TaskState = Waiting;
					OS_SVC_Set(SVC_WaitingTask);
				}
				OS_Control.OS_Tasks[i]->TimeWaiting.Ticks_Count--;
			}
		}
	}
}

//========================================================================================================================

/*                                            |-------------------------|
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*|   Function Implement    |-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//
 *                                            |-------------------------|
 */


MY_RTOS_ErrorID MY_RTOS_Init ()
{
	MY_RTOS_ErrorID Error = NoError;

	//Update OS Mode (Srunning)
    OS_Control.OSmodeID = OSsuspend;

   // OS_Control.NoOFActiveTasks = 0;     |------>No Need to this part because struct is global and all elements is = 0

	//Specify the Main stack for OS
    MyRTOS_Create_MainStack ();

	//Create OS Ready Queue
    if (FIFO_buf_init(&Ready_QUEUE, Tasks_Buffer /* Array name --> Address for First element */, 10) != FIFO_No_Error)
    {
    	Error = ReadyQueue_Init_Error;
    }

	//Configure IDLE task
    strcpy(MyRTOS_IdleTask.TaskName, "IdleTask");

    MyRTOS_IdleTask.Priority = 255;
    MyRTOS_IdleTask.P_TaskEntry = MyRTOS_IdleTask_IMPLT;
    MyRTOS_IdleTask.Stack_Size = 300;

    //Create Task
    MyRTOS_CreateTask(&MyRTOS_IdleTask);

    return Error;
}

void MyRTOS_StartOS ()
{
	OS_Control.OSmodeID = OSrunning;  /* Change OS state */

	OS_Control.CurrentTask = &MyRTOS_IdleTask;

	OS_Control.NextTask = &MyRTOS_IdleTask;

	MyRTOS_ActivateTask(&MyRTOS_IdleTask);  /* Activate task */

	Start_Ticker();  /* Start ticker */

	OS_Set_PSP(OS_Control.CurrentTask->Current_PSP);

	Switch_MSP_to_PSP();  /* Switch to PSP */

	Switch_Privileged_to_unPrivileged();  /* Switch to unprivileged */

	OS_Control.CurrentTask->P_TaskEntry();

}


MY_RTOS_ErrorID MyRTOS_CreateTask (Task_REF* Tref)
{
	MY_RTOS_ErrorID Error = NoError;

	//Create PSP Stack for task
    Tref->_S_PSP_Task = OS_Control.Task_Locator;
    Tref->_E_PSP_Task = Tref->_S_PSP_Task - (Tref->Stack_Size / 4); /* _E_MSP_Task is uint32_t Pointer  */

	//Check task stack size exceeded the HeapStack
    if (Tref->_E_PSP_Task < ( (uint32_t*)(&_E_Heap) ) )
    {
    	return task_Exceeded_StackSize;
    }

    //Create TaskStack
    MyRTOS_CreateTaskStack(Tref);

	//Aligned 8 Byte to Task_Locator
	OS_Control.Task_Locator = ( Tref->_E_PSP_Task - 2 );


	//Passing Task to OS
	OS_Control.OS_Tasks[OS_Control.NoOFActiveTasks] = Tref;
	OS_Control.NoOFActiveTasks++;

	//Task State update--> Suspend
    Tref->TaskState = Suspend;

	return Error;
}

void MyRTOS_TaskWaitingTime (uint32_t NoTicks, Task_REF* Tref)
{
	Tref->TimeWaiting.Blocking = BlockingEnable;  //Enable blocking

	Tref->TimeWaiting.Ticks_Count = NoTicks;  //Pass The Ticks numbers

	Tref->TaskState = Suspend;  //update Task state

	OS_SVC_Set(SVC_TerminateTask);  //Terminate task

}

void MyRTOS_ActivateTask (Task_REF* Tref)
{
	//Update TaskState
	if (Tref->TaskState == Suspend)
	{
		Tref->TaskState = Waiting;

		OS_SVC_Set(SVC_ActivateTask);   /* Generate Exception to Active throughout SCV */
	}
}

void MyRTOS_TerminateTask (Task_REF* Tref)
{
	//Update TaskState
	Tref->TaskState = Suspend;

	OS_SVC_Set(SVC_TerminateTask);  /* Generate Exception to Terminate throughout SVC */
}

MY_RTOS_ErrorID MyRTOS_AcquireMutex (Task_REF* Tref, Mutex_ref* Mref)
{
	if (Mref->CurrentUser == NULL)
	{
		Mref->CurrentUser = Tref;

	} else if (Mref->NextUser == NULL)
	{
		Mref->NextUser = Tref;

		Tref->TaskState = Suspend;  /* Make task state suspend until release mutex */

		OS_SVC_Set(SVC_TerminateTask); /* Terminate task */

	} else
	{
		return MutexIsReachedToMaxNumberOfUsers;
	}

	return NoError;
}

void MyRTOS_ReleaseMutex (Mutex_ref* Mref)
{
	if (Mref->CurrentUser != NULL)
	{
		Mref->CurrentUser = Mref->NextUser;
		Mref->NextUser = NULL;

		Mref->CurrentUser->TaskState = Waiting; /* Make task state suspend */

		OS_SVC_Set(SVC_ActivateTask);  /* Activate task */
	}
}

//===================================================================
//(_   _)( )                   ( )       ( )   ( )               ( )
//  | |  | |__    ____   ___   | |/ )     \ \_/ /  ___    _   _  | |
//  | |  |  _  \ / _  )/  _  \ | | <        \ /   /   \  ( ) ( ) | |
//  | |  | | | |( (_| || ( ) | | |\ \       | |  ( (_) ) | (_) | | |
//  (_)  (_) (_) \___ )(_) (_) (_) (_)      (_)   \___/   \___/  (_)
