/*
 * Scheduler.h
 *
 *  Created on: Mar 31, 2024
 *      Author: Mina Saad
 */

#ifndef INC_SCHEDULER_H_
#define INC_SCHEDULER_H_

//=======================<Include>
#include "CortexMx_OS_Porting.h"
#include "string.h"

//=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//                                            User Define
//=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*

//==============<MYRTOS_errorID enumeration>
typedef enum
{
	NoError,
	ReadyQueue_Init_Error,
	task_Exceeded_StackSize,
	MutexIsReachedToMaxNumberOfUsers

}MY_RTOS_ErrorID;

//==============<Task_ref structure>
typedef struct
{
	uint32_t Stack_Size;
	unsigned char Priority;
	void (*P_TaskEntry)(void);
	unsigned char AutoStart;
	uint32_t* _S_PSP_Task;  //Not Entered by the user
	uint32_t* _E_PSP_Task;  //Not Entered by the user
	uint32_t* Current_PSP; //Not Entered by the user
	char TaskName[30] ;
	enum
	{
		Suspend,
		Running,
		Waiting,
		Ready

	}TaskState;//Not Entered by the user

	struct
	{
		uint32_t Ticks_Count;
		enum
		{
			BlockingDisable,
			BlockingEnable

		}Blocking;

	}TimeWaiting;

}Task_REF;

//==============<Mutex_ref structure>
typedef struct
{
	uint8_t*   Ppayload;
	uint32_t   PayloadSize;
	Task_REF*  CurrentUser;
	Task_REF*  NextUser;
	uint8_t    MutexName[30];

}Mutex_ref;

//                                           *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//===========================================*            Generic APIs           *========================================//
//                                           *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
void MyRTOS_StartOS ();
MY_RTOS_ErrorID MY_RTOS_Init ();
void MyRTOS_ActivateTask (Task_REF* Tref);
void MyRTOS_TerminateTask (Task_REF* Tref);
MY_RTOS_ErrorID MyRTOS_CreateTask (Task_REF* Tref);
void MyRTOS_TaskWaitingTime (uint32_t NoTicks, Task_REF* Tref);
MY_RTOS_ErrorID MyRTOS_AcquireMutex (Task_REF* Tref, Mutex_ref* Mref);

#endif /* INC_SCHEDULER_H_ */
