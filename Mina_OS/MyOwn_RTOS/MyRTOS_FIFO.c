/*
 * MyRTOS_FIFO.c
 *
 *  Created on: Feb 11, 2024
 *      Author: Mina Saad
 */

//========================<Include>
#include"MyRTOS_FIFO.h"

/*                                                |-------------------------|
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*|   Function Implement    |-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//
 *                                                |-------------------------|
 */

//==========================================================================<FIFO_buf_init Function>
FIFO_buf_status FIFO_buf_init(FIFO_buf_t* FIFO_buf, element_type* buf, uint32_t Length)
{
	//checking if buffer is exist
	if (buf == NULL)     // if (!buf) --> Better in optimiztion
	{
		return FIFO_Null;
	}

	//Passing details for buffer
	FIFO_buf->base = buf;
	FIFO_buf->head = FIFO_buf->base;
	FIFO_buf->tail = FIFO_buf->base;
	FIFO_buf->count = 0;
	FIFO_buf->length = Length;

	return FIFO_No_Error;
}

//==========================================================================<FIFO_buf_enqueue Function>
FIFO_buf_status FIFO_buf_enqueue(FIFO_buf_t *FIFO_buf, element_type item)
{
	if (FIFO_buf->base == NULL || FIFO_buf->head == NULL)
	{
		return FIFO_Null;

	} else if ((FIFO_buf->count == FIFO_buf->length))
	{
		return FIFO_Full;

	} else if (FIFO_buf->head == (FIFO_buf->base + (FIFO_buf->length - 1) )) //Circular Queue
	{
		*(FIFO_buf->head) = item;
		FIFO_buf->head = FIFO_buf->base;

	} else
	{
		*(FIFO_buf->head) = item;
		FIFO_buf->head++;

	}

	FIFO_buf->count++;

	return FIFO_No_Error;
}

//==========================================================================<FIFO_buf_dequeue Function>
FIFO_buf_status FIFO_buf_dequeue(FIFO_buf_t *FIFO_buf, element_type* item)
{

	if (FIFO_buf->tail == ( FIFO_buf->base + ( FIFO_buf->length - 1) ) )
	{
		*item = *(FIFO_buf->tail);
		FIFO_buf->tail = FIFO_buf->base;
		FIFO_buf->count--;


	} else if (FIFO_buf->count == 0)
	{
		return FIFO_Empty;

	} else
	{
		*item = *(FIFO_buf->tail);
		FIFO_buf->tail++;
		FIFO_buf->count--;
	}

	return FIFO_No_Error;
}

//==========================================================================<FIFO_buf_is_full Function>
FIFO_buf_status FIFO_buf_is_full(FIFO_buf_t* FIFO_buf)
{
	if (FIFO_buf->count == FIFO_buf->length)
	{
		return FIFO_Full;
	}

	return FIFO_No_Error;
}

//==========================================================================<FIFO_buf_is_full Function>
FIFO_buf_status FIFO_Clean_Queue (FIFO_buf_t* FIFO_buf )
{
	FIFO_buf->head = FIFO_buf->base; //Return base to his start point ||OR FIFO_buf->head -= (FIFO_buf->count + 1)

	FIFO_buf->count = 0;  	/* Counts equal Zero */
	FIFO_buf->tail = FIFO_buf->base;

   return FIFO_No_Error;
}

//===================================================================
//(_   _)( )                   ( )       ( )   ( )               ( )
//  | |  | |__    ____   ___   | |/ )     \ \_/ /  ___    _   _  | |
//  | |  |  _  \ / _  )/  _  \ | | <        \ /   /   \  ( ) ( ) | |
//  | |  | | | |( (_| || ( ) | | |\ \       | |  ( (_) ) | (_) | | |
//  (_)  (_) (_) \___ )(_) (_) (_) (_)      (_)   \___/   \___/  (_)

