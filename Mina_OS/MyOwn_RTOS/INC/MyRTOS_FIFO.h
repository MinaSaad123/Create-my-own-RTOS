/*
 * MyRTOS_FIFO.h
 *
 *  Created on: Mar 31, 2024
 *      Author: Mina Saad
 */

#ifndef INC_MYRTOS_FIFO_H_
#define INC_MYRTOS_FIFO_H_

//---------------------<Include>
#include "Scheduler.h"

//                          *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//--------------------------*                     Generic Macros                    *------------------------------
//                          *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
#define element_type Task_REF*


//=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//                                            User Define
//=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*

//==============<FIFO_buf_t structure>
typedef struct
{
	unsigned int length;
	unsigned int count;
	element_type* base;
	element_type* head;
	element_type* tail;

}FIFO_buf_t;

//==============<FIFO_buf_status enumeration>
typedef enum
{
	FIFO_No_Error,
	FIFO_Null,
	FIFO_Empty,
	FIFO_Full

}FIFO_buf_status;

//                                           *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//===========================================*            Generic APIs           *========================================//
//                                           *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
void FIFO_print_buffer (FIFO_buf_t* FIFO_buf);
FIFO_buf_status FIFO_buf_is_full (FIFO_buf_t* FIFO_buf);
FIFO_buf_status FIFO_Clean_Queue (FIFO_buf_t* FIFO_buf );
FIFO_buf_status FIFO_buf_enqueue (FIFO_buf_t* FIFO_buf, element_type item);
FIFO_buf_status FIFO_buf_dequeue (FIFO_buf_t* FIFO_buf, element_type* item);
FIFO_buf_status FIFO_buf_init (FIFO_buf_t* FIFO_buf, element_type* buf, uint32_t Length);

#endif /* INC_MYRTOS_FIFO_H_ */
