/*
 * CortexMx_OS_Porting.c
 *
 *  Created on: Feb 11, 2024
 *      Author: Mina Saad
 */

//--------------------------------<Include>
#include "CortexMx_OS_Porting.h"

//*=*=*=*=*=**=*=*=*=*=**=*=*=*=*=**=*=*=*=*=**=*=*=*=*=**=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//                                Some prototypes
//*=*=*=*=*=**=*=*=*=*=**=*=*=*=*=**=*=*=*=*=**=*=*=*=*=**=*=*=*=*=*=*=*=*=*=*=*=*=*=*
void Update_TaskWaitingTime ();
void Decide_WhatNext ();

//                               *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//-------------------------------*                  Fault ISR Function                   *------------------------------//
//                               *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
extern uint32_t Count;

uint8_t SysTick_LED;


void HardFault_Handler()
{
	while (1);
}

void MemManage_Handler()
{
	while (1);
}

void BusFault_Handler()
{
	while (1);
}

__attribute ((naked)) void PendSV_Handler () //To switch Context
{
	__asm volatile("b OS_PendSV_Services");

}

void UsageFault_Handler()
{
	while (1);
}

__attribute ((naked)) void SVC_Handler()
{
	__asm ("TST LR, #0x04 \n\t"
			"ITE EQ \n\t"
			"MRSEQ R0, MSP \n\t"
			"MRSNE R0, PSP \n\t"
			"B OS_SVC_Services");
}

/*                                                 ============================
 *=================================================*     Function Implement    *===================================================//
 *                                                 ============================
 */


void Trigger_PendSV ()
{
	SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
}

void HW_Init ()
{
	//initialize clock Tree (RCC -> SysTick & CPU) 8MHZ
	  // 8MHZ
	  // 1Count --> 0.125us
	  // X count --> 1ms
	  // x = 800 count

	//Adjust PendSV interrupt priority
	__NVIC_SetPriority(PendSV_IRQn, 15);  /* SystICK priority = 15 */

}

void Start_Ticker ()
{
	//initialize clock Tree (RCC -> SysTick & CPU) 8MHZ
	  // 8MHZ
	  // 1Count --> 0.125us
	  // X count --> 1ms
	  // x = 800 count

	SysTick_Config(8000);


}

void SysTick_Handler ()
{
	SysTick_LED ^= 1;

	Count++;

	Update_TaskWaitingTime(); /* Update TaskTime */

	Decide_WhatNext();  /* Decide what is next */

	Trigger_PendSV();  /* Go to PendSV Handler to switch context\storing */
}


//===================================================================
//(_   _)( )                   ( )       ( )   ( )               ( )
//  | |  | |__    ____   ___   | |/ )     \ \_/ /  ___    _   _  | |
//  | |  |  _  \ / _  )/  _  \ | | <        \ /   /   \  ( ) ( ) | |
//  | |  | | | |( (_| || ( ) | | |\ \       | |  ( (_) ) | (_) | | |
//  (_)  (_) (_) \___ )(_) (_) (_) (_)      (_)   \___/   \___/  (_)
