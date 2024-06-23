/*
 * Stm32f103c6_Uart_Driver.h
 *
 *  Created on: Sep 17, 2023
 *      Author: HP
 */

#ifndef INC_STM32F103C6_UART_DRIVER_H_
#define INC_STM32F103C6_UART_DRIVER_H_

//<<<<<<<<Include>>>>>>>>>
#include "Stm32f103x8.h"

//=======Typedef
typedef struct
{
	uint8_t     UART_Mode;         //Specifies TX/RX Enable/Disable
	                               //This parameter must be set based on @Ref UART_Mode_Define

uint32_t 		UART_BaudRate;    //Specifies UART_BaudRate
	                               //This parameter must be set based on @ref UART_BaudRate_Define

uint8_t 		Pay_Load_Length;   //Specifies Pay_Load_Length of UART
	                               //This parameter must be set based on @ref UART_Pay_Load_Length_Define

uint8_t			Parity;		       //Specifies Parity
                                   //This parameter must be set based on @ref UART_Parity_Define

uint8_t 		Stop_Bits;	       //Specifies Number of stop bits
	                               //This parameter must be set based on @ref UART_Stop_Bits_Define

uint8_t 		HwFlowCTRL;		   //Specifies if flow control is enabled or not
	                               //This parameter must be set based on @ref UART_HwFlowCTRL_Define

uint8_t			IRQ_Enable;		   //Specifies if RX or Tx interrupt is Enable
	                               //This parameter must be set based on @ref UART_IRQ_Enable

void(*P_IRQ_CallBack)(void);	   //Call_Back_Function
}UART_Config;

//=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#
//             Reference Macros
//=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#

//======@Ref UART_Mode_Define
#define UART_Mode_Rx					(uint32_t)(1<<2)           //Bit 3 TE: Transmitter Enable
#define UART_Mode_TX					(uint32_t)(1<<3)           //Bit 2 RE: Receiver Enable
#define UART_Mode_RX_TX					((uint32_t)(1<<2 | 1<<3))  //Transmit & Receiver Enable

//======@ref UART_BaudRate_Define
#define UART_BaudRate_2400				2400
#define UART_BaudRate_9600				9600
#define UART_BaudRate_19200				19200
#define UART_BaudRate_57600				57600
#define UART_BaudRate_115200			115200
#define UART_BaudRate_230400			230400
#define UART_BaudRate_460800			460800
#define UART_BaudRate_921600			921600
#define UART_BaudRate_2250000			2250000
#define UART_BaudRate_4500000			4500000

//======@ref UART_Pay_Load_Length_Define
#define UART_Pay_Load_Length_8bit		(uint32_t)(0)
#define UART_Pay_Load_Length_9bit		(uint32_t)(1<<12)

//======@ref UART_Parity_Define
#define UART_Parity_NONE				(uint32_t)(0)
#define UART_Parity_EVEN				(uint32_t)(1<<10)
#define UART_Parity_ODD					(uint32_t)(1<<10 | (1<<9))

//=====@ref UART_Stop_Bits_Define
#define UART_Stop_Bit1           1

//======@ref UART_HwFlowCTRL_Define
#define UART_HwFlowCtl_None      0
#define UART_HwFlowCtl_RTS       1
#define UART_HwFlowCtl_CTS       2
#define UART_HwFlowCtl_RTS_Cts   3

//======@ref UART_IRQ_Enable_Define
#define UART_IRQ_Enable_NONE			(uint32_t)(0)
#define UART_IRQ_Enable_TXE				(uint32_t)(1<<7)
#define UART_IRQ_Enable_TC				(uint32_t)(1<<6)
#define UART_IRQ_Enable_RXNEIE			(uint32_t)(1<<5)
#define UART_IRQ_Enable_PE				(uint32_t)(1<<8)


//BaudRate Calculation
//UARTDIV = fclk/(16*Baudrate)
//UARTDIV_Mul100 =
//uint32(100*fclk)/(16*Baudrate)==(25*fclk)/(4*Baudrate))
//DIV_Mantissa_MUL100 = Integer Part(UARTDIV)*100
//DIV_Mantissa = Integer Part (UARTDIV)
//DIV_Fraction = ((UARTDIV_MUL100- DIV_Mantissa_Mull100) * 16)/100

#define UARTDIV(_PCLK_,_BAUD_)					(uint32_t)(_PCLK_ / (16 * _BAUD_))
#define UARTDIV_MUL100(_PCLK_,_BAUD_)			(uint32_t)((25*_PCLK_)/(4 * _BAUD_))
#define Mantissa_MUL100(_PCLK_,_BAUD_)			(uint32_t)(UARTDIV(_PCLK_,_BAUD_)*100)
#define Mantissa(_PCLK_,_BAUD_)					(uint32_t)(UARTDIV(_PCLK_,_BAUD_))
#define DIV_Fraction(_PCLK_,_BAUD_)				(uint32_t)(((UARTDIV_MUL100(_PCLK_,_BAUD_))- Mantissa_MUL100(_PCLK_,_BAUD_))*16)/100
#define UART_BRR_Register(_PCLK_,_BAUD_)		((Mantissa(_PCLK_,_BAUD_) ) <<4)|((DIV_Fraction(_PCLK_,_BAUD_)) & 0xF)

//-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#
// APIs supported by "MCAL UART DRIVER"
//-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#

void MCAL_UART_Init (UART_t* UARTx, UART_Config* UART_CFG);
void MCAL_UART_GPIO_Set_Pins(UART_t* UARTx);
void MCAL_UART_DeInit(UART_t* UARTx);
//===================================

void MCAL_UART_SendData(UART_t* UARTx, uint16_t* P_Buffer, enum Polling_Mechansim PollingEN );
void MCAL_UART_ReceiveData(UART_t* UARTx, uint16_t* P_Buffer, enum Polling_Mechansim PollingEN);
//===================================

void MCAL_UART_Wait_TC(UART_t* UARTx);

#endif /* INC_STM32F103C6_UART_DRIVER_H_ */
