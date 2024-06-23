/*
 * CortexMx_OS_Porting.h
 *
 *  Created on: Feb 11, 2024
 *      Author: Mina Saad
 */

#ifndef INCLUDE_CORTEXMX_OS_PORTING_H_
#define INCLUDE_CORTEXMX_OS_PORTING_H_

//-------------------------<Include>
#include "ARMCM3.h"

//                          *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//--------------------------*                 Generic variables                     *------------------------------
//                          *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
extern uint32_t _estack;
extern uint32_t _E_Heap;

//*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=//
//                                                        Macros                                                        //
//*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=//

//=============Macros for OS
//Manipulate with SP
#define OS_Set_PSP(Add)     __asm volatile("mov r0, %[C_VAL] \n\t msr PSP, r0" : : [C_VAL] "r" (Add) )
#define OS_Get_PSP(Get)     __asm volatile("mrs r0, PSP \n\t mov %[C_VAL], r0"   : [C_VAL] "=r" (Get) )
#define Switch_MSP_to_PSP() __asm volatile("mrs r0, CONTROL \n\t  mov r1, #0x02 \n\t orr r0, r0, r1 \n\t msr CONTROL, r0")
#define Switch_PSP_to_MSP() __asm volatile("mrs r0, CONTROL \n\t  mov r1, #0x05 \n\t and r0, r0, r1 \n\t msr CONTROL, r0")

//Manipulate with mode
#define Switch_Privileged_to_unPrivileged() __asm volatile("mrs r3, CONTROL \n\t orr r3, r3, #0x1 \n\t msr CONTROL, r3")
#define Switch_unPrivileged_to_Privileged() __asm volatile("mrs r3, CONTROL \n\t lsr r3, #0x1 \n\t lsl r3, #0x1 \n\t msr CONTROL, r3")

//Generate exception
#define OS_Generate_Exception_To_Activate()             __asm volatile("SVC 0x00")
#define OS_Generate_Exception_To_Terminate()            __asm volatile("SVC 0x01")
#define OS_Generate_Exception_To_Waiting()              __asm volatile("SVC 0x02")

//Main Stack Size
#define MainStack 4096  /* 4KB */

//Define NULL
#define NULL (void *)0

//======================================================================================================================================

//                                           *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//===========================================*            Generic APIs           *========================================//
//                                           *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
void HW_Init ();
void Trigger_PendSV ();
void Start_Ticker ();


#endif /* INCLUDE_CORTEXMX_OS_PORTING_H_ */
