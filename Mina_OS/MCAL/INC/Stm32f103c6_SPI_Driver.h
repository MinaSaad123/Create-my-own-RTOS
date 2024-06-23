/*
 * Stm32f103c6_SPI_Driver.h
 *
 *  Created on: Sep 23, 2023
 *      Author: HP
 */

#ifndef INC_STM32F103C6_SPI_DRIVER_H_
#define INC_STM32F103C6_SPI_DRIVER_H_

//=============================<Includes>
#include "Stm32f103c6_GPIO_Driver.h"
#include "Stm32f103x8.h"

//================Structure to make Interrupt Hander Know who cause the interrupt
struct S_SPI_IRQ
{
	uint8_t RXE : 1;
	uint8_t TXE : 1;
	uint8_t ERR : 1;
	uint8_t Reserved : 5;
};

/*                                                 ==========================
 *=================================================* Typedef For SPI Config *===================================================
 *                                                 ==========================
 */
typedef struct
{

	uint16_t NSS;   		        //Specifies if Slave select pin is in (hardware or software)
	  						        //This Parameter must be set base on @ref SPI_NSS_Define

	uint16_t Device_Mode;           //Specifies MCU is Master of Slave
    					    	    //This Parameter must be set base on @ref SPI_Device_Mode_Define

	uint16_t Communication_DIR;     //Specifies Communication Direction
									//This Parameter must be set base on @ref SPI_Communication_DIR_Define

	uint16_t Data_Format;           //Specifies Data Length is 8-Bit or 16-Bit
									//This Parameter must be set base on @ref SPI_Data_Format_Define

	uint16_t Frame_Format;          //Specifies the Sending data is MSB or LSB
									//This Parameter must be set base on @ref SPI_Frame_Format_Define

	uint16_t BaudRate_Prescaler;    //Specifies the Clock Prescaler
									//This Parameter must be set base on @ref SPI_BaudRate_Prescaler_Define

	uint16_t Clock_Polarity;        //Specifies Clock Idle State
    								//This Parameter must be set base on @ref SPI_Clock_Polarity_Define

	uint16_t Clock_Phase;           //Specifies The action From First-edge or Second-edge
									//This Parameter must be set base on @ref SPI_Clock_Phase_Define

	uint16_t SPI_IRQ_Enable;        //Specifies IRQ is Enable or Disable
									//This Parameter must be set base on @ref SPI_IRQ_Define

	void (*CallBack)(struct S_SPI_IRQ IRQ);
	                                //Specidies Function CallBack Symbol

}SPI_Config;

//*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//                                          SPI config @ref
//*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*

//==========@ref SPI_NSS_Define
//Software
#define SPI_NSS_Internal_SOFT_Set        		    (1 << 9 | 1 << 8)
#define SPI_NSS_Internal_SOFT_Reset        			(1 << 9)
//Hardware
#define SPI_NSS_Hard_Slave                          (0x0U)
#define SPI_NSS_Internal_Hard_Output_ENR  			(1 << 2)
#define SPI_NSS_Internal_Hard_Output_Dis  			(0x0U)

//=========@ref SPI_Device_Mode_Define
#define SPI_Device_Mode_Slave             			(0x00U)
#define SPI_Device_Mode_Master        			    (1 << 2)

//=========@ref SPI_Communication_DIR_Define
#define SPI_Communication_DIR_2Lines 		     	(0x00U)
#define SPI_Communication_DIR_2Direction_Rx_Only  	(1 << 10)
#define SPI_Communication_DIR_1Direction_Rx_Only    (1 << 15)
#define SPI_Communication_DIR_1Direction_Tx_Only    (1 << 14 | 1 << 15)

//=========@ref SPI_Data_Format_Define
#define SPI_Data_Format_8bit			     		(0x00U)
#define SPI_Data_Format_16bit				    	(0x1 << 11)

//=========@ref  SPI_Frame_Format_Define
#define SPI_Frame_Format_MSB						(0x0U)
#define SPI_Frame_Format_LSB						(0x1 << 7)

//=========@ref SPI_Clock_Polarity_Define
#define SPI_Clock_Polarity_Low_Idle					(0x0U)
#define SPI_Clock_Polarity_High_Idle				(0x1 << 1)

//=========@ref SPI_Clock_Phase_Define
#define SPI_1Edge_Transimation_First_Data_Capture	(0x0U)
#define SPI_2Edge_Transimation_First_Data_Capture	(0x1)

//=========@ref SPI_BaudRate_Prescaler_Define
#define SPI_BaudRate_Prescaler_By2					(0x0U)
#define SPI_BaudRate_Prescaler_By4					(0b001 << 3)
#define SPI_BaudRate_Prescaler_By8				    (0b010 << 3)
#define SPI_BaudRate_Prescaler_By16					(0b011 << 3)
#define SPI_BaudRate_Prescaler_By32					(0b100 << 3)
#define SPI_BaudRate_Prescaler_By64					(0b101 << 3)
#define SPI_BaudRate_Prescaler_By12				    (0b110 << 3)
#define SPI_BaudRate_Prescaler_By26				    (0b111 << 3)

//=========@ref SPI_IRQ_Define
#define SPI_IRQ_Enable_None							 (uint8_t)(0)
#define SPI_IRQ_Enable_Tx_Only						 (uint8_t)(1 << 7)
#define SPI_IRQ_Enable_Rx_Only						 (uint8_t)(1 << 6)
#define SPI_IRQ_Enable_TX_Rx                         ( ( (uint8_t)(1 << 7) ) | ( (uint8_t)(1 << 6) ) )
#define SPI_IRQ_Enable_Err							 (uint8_t)(1 << 5)
//==========================================================================================================


//*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//                                               Support APIs by "SPI"
//*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*

//---------------------------------------<Function Related to Init>
void MCAL_SPI_Init (SPI_t* SPIx, SPI_Config* Pin_Config);
void MCAL_SPI_Deinit (SPI_t* SPIx);

//------------------<Function Related to GPIO>
void MCAL_SPI_Set_Pin (SPI_t* SPIx);

//-----------------------------------------------<Function Related to Send and Receive Data>
void MCAL_SPI_Send (SPI_t* SPIx, uint16_t* Tx_Buffer, enum Polling_Mechansim Polling);
void MCAL_SPI_Receive (SPI_t* SPIx, uint16_t* Rx_Buffer, enum Polling_Mechansim Polling);
void MCAL_SPI_TX_RX (SPI_t* SPIx, uint16_t* Tx_Buffer, uint16_t* Rx_Buffer, enum Polling_Mechansim Polling);
//===============================================================================================================

#endif /* INC_STM32F103C6_SPI_DRIVER_H_ */
