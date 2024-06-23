/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Mina Saad Nazir
 * @brief          : Main program body
 ******************************************************************************
 */

//-----------------------------<Include>
#include "Scheduler.h"

//------------------------------------------<GLOBAL Variables>
Task_REF Task1, Task2, Task3, Task4;

Mutex_ref Mutex1, Mutex2;

unsigned char Payload[3] = {1, 2, 3};

uint32_t Task1_LED = 1, Task2_LED = 1, Task3_LED = 1, Task4_LED = 1, SysTick_LED = 1;

uint32_t Count, TaskCounter;
//=====================================================================================================

//------------------------------------------<Init Task Function>
void Task1_IMPLT ()
{
	while (1)
	{
		Task1_LED ^= 1;

		if (Count == 1000)
		{
			Count = 0;
			MyRTOS_AcquireMutex(&Task1, &Mutex1);
			MyRTOS_ActivateTask(&Task2);

		} else if (Count == 3000)
		{
			MyRTOS_AcquireMutex(&Task1, &Mutex2);
		}
	}
}

void Task2_IMPLT ()
{
	while (1)
	{
		Task2_LED ^= 1;

		if (Count == 2000)
		{
			MyRTOS_AcquireMutex(&Task2, &Mutex2);

			MyRTOS_AcquireMutex(&Task2, &Mutex1);
		}
	}
}

void Task3_IMPLT ()
{
	while (1)
	{
		Task3_LED ^= 1;

		if (Count == 3)
		{
			Count = 0;
			MyRTOS_TaskWaitingTime(9, &Task3);

		}
	}
}

void Task4_IMPLT ()
{
	while (1)
	{
		Task4_LED ^= 1;

		if (Count == 3)
		{
			Count = 0;
			TaskCounter = 1;
			MyRTOS_TaskWaitingTime(15, &Task4);
		}
	}
}


int main(void)
{
	MY_RTOS_ErrorID Error = NoError;

	HW_Init(); /* To init Hardware */

	while ( MY_RTOS_Init() ); /* To check MyRTOS are initialized correctly */

	//----------------------<Init Mutex1>
	Mutex1.PayloadSize = 3;
	Mutex1.Ppayload = Payload;
	strcpy(Mutex1.MutexName, "Mutex1");
	//=====================================================================

	//----------------------<Init Mutex2>
	Mutex2.PayloadSize = 3;
	Mutex2.Ppayload = Payload;
	strcpy(Mutex2.MutexName, "Mutex2");
	//=====================================================================

	//----------------------<Init Task1>
	Task1.Stack_Size = 1024;
	Task1.Priority = 4;
	Task1.P_TaskEntry = Task1_IMPLT;
	strcpy(Task1.TaskName, "Task1");
	//=====================================================================

	//----------------------<Init Task2>
	Task2.Stack_Size = 1024;
	Task2.Priority = 3;
	Task2.P_TaskEntry = Task2_IMPLT;
	strcpy(Task2.TaskName, "Task2");
	//=====================================================================

	//----------------------<Init Task3>
	Task3.Stack_Size = 1024;
	Task3.Priority = 2;
	Task3.P_TaskEntry = Task3_IMPLT;
	strcpy(Task3.TaskName, "Task3");
	//=====================================================================

	//----------------------<Init Task4>
	Task4.Stack_Size = 1024;
	Task4.Priority = 1;
	Task4.P_TaskEntry = Task4_IMPLT;
	strcpy(Task4.TaskName, "Task4");
	//=====================================================================

	Error += MyRTOS_CreateTask(&Task1);
	Error += MyRTOS_CreateTask(&Task2);
	Error += MyRTOS_CreateTask(&Task3);
	Error += MyRTOS_CreateTask(&Task4);

	MyRTOS_ActivateTask(&Task1);

	//Start OS
	MyRTOS_StartOS();

	//Infinite loop
	while (1)
	{
		;
	}

}
