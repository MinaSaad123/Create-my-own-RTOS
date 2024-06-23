/*
 * Stm32f103c6_I2C_Driver.h
 *
 *  Created on: ١١‏/١٠‏/٢٠٢٣
 *      Author: Mina Saad
 */

#ifndef INC_STM32F103C6_I2C_DRIVER_H_
#define INC_STM32F103C6_I2C_DRIVER_H_

//=======================<Include>
#include "BitField_I2C.h"
#include "Stm32f103x8.h"

//=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//                                            User Define
//=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*

//=======================<Config Slave Address>
typedef struct
{
	uint16_t Slave_Address_Mode;
	uint16_t Slave_DualAddress_EN;
	uint16_t Primary_Address;
	uint16_t Secondary_Address;

}I2C_Slave_Address_t;

//=======================<Config Slave State>
typedef enum
{
	I2C_EV_Stop,
	I2C_Error_AF,
	I2C_Ev_Address_Matched,
	I2C_Ev_Data_Req,		//this app should send data (Slave_Send_Data)
	I2C_Ev_Data_RCV 		//this app should Read Data (Slave_Read_Data)

}Slave_State_t;

/*                                                 ==========================
 *=================================================* Typedef For I2C Config *===================================================
 *                                                 ==========================
 */

typedef struct
{
	uint32_t I2C_Mode;      				//Specifies I2C Mode (Slave - Master - SMBus)
                          	  	  			//This Parameter must be set base on @ref I2C_Mode_Define

	//                          =======<I2C_CR1>=======

	uint32_t I2C_Acknowledge;               //Specifies Acknowledge (Enable - Disabled)
	                                        //This Parameter must be set base on @ref I2C_Stretch_Define

    uint32_t I2C_NOStretch;          		    //Specifies Stretch (Enable - Disabled)
    										//This Parameter must be set base on @ref I2C_Stretch_Define

    uint32_t I2C_GeneralCall;       		//Specifies General Call (Enable - Disabled)
    										//This Parameter must be set base on @ref I2C_GenCall_Define

	//                          =======<I2C_CR2>=======

    uint32_t I2C_Interrupt;       			//Specifies Interrupt (Enable - Disabled)
    									    //This Parameter must be set base on @ref I2C_Interrupt_Define

    void (*P_Slave_Event_CallBack)(Slave_State_t Slave_State);

	//                    =======<Slave Address Config>=======

    I2C_Slave_Address_t Slave_Address_CFG;  //Specifies Slave Address Config
    										//This Parameter must be set base on @ref I2C_Ack_Control_Define

	//                          =======<I2C_CCR>=======

    uint32_t I2C_Master_ClockSpeed;  		   //Specifies Clock Speed in Master Mode
    									   //This Parameter must be set base on @ref I2C_Master_ClockSpeed_Define


}I2C_CFG_t;
//==========================================================================================================

//=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//                                                  I2C @ref
//=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*

//-------------------------<@ref I2C_Mode_Define>
#define I2C_Mode_Slave            0x0UL
#define I2C_Mode_Master           0x01UL
#define I2C_Mode_SMBus            I2C_CR1_SMBUS
//===============================================

//----------------<@ref I2C_Ack_Control_Define>

//                  <Bit 10 ACK: Acknowledge enable>
//This bit is set and cleared by software and cleared by hardware when PE=0.
//0: No acknowledge returned
//1: Acknowledge returned after a byte is received (matched address or data)
#define I2C_Ack_Control_EN        I2C_CR1_ACK
#define I2C_Ack_Control_DIS       0x0UL
//===============================================

//---------------<@ref I2C_Stretch_Define>

//          <Bit 7 NOSTRETCH: Clock stretching disable (Slave mode)>
//This bit is used to disable clock stretching in slave mode when ADDR or BTF flag is set, until
//it is reset by software.
//0: Clock stretching enabled
//1: Clock stretching disabled
#define I2C_NOStretch_EN        0x0UL
#define I2C_NOStretch_DIS       I2C_CR1_NOSTRETCH
//===============================================

//---------------<@ref I2C_GenCall_Define>

//                  <Bit 6 ENGC: General call enable>
//0: General call disabled. Address 00h is NACKed.
//1: General call enabled. Address 00h is ACKed.
#define I2C_GeneralCall_EN      I2C_CR1_ENGC
#define I2C_GeneralCall_DIS     0x0UL
//===============================================

//---------------<@ref I2C_Interrupt_Define>
#define I2C_Interrupt_EN         0x1UL
#define I2C_Interrupt_DIS        0x0UL
//===============================================

//---------------<@ref I2C_Master_ClockSpeed_Define>

//           <Bit 15 F/S: I2C master mode selection>
//0: Sm mode I2C
//1: Fm mode I2C
#define I2C_Master_ClockSpeed_SM_50K  50000UL
#define I2C_Master_ClockSpeed_SM_100K 100000UL
#define I2C_Master_ClockSpeed_FM_200K 500000UL //TODO
#define I2C_Master_ClockSpeed_FM_400K 500000UL //TODO
//===============================================

//---------------<@ref I2C_Interrupt_Define>
#define I2C_Interrupt_EN             0x1UL
#define I2C_Interrupt_DIS            0x0UL
//==========================================================================================================

/*                                            *=*=*=*=*=*=*=*=*=*=*
 *          |=--------------------------------*  Some User Define *----------------------------------=|
 *                                            *=*=*=*=*=*=*=*=*=*=*
 */

//==============<Stop state enumeration>
typedef enum
{
	WithoutStop,
	WithStop
}StopState_t;

//==============<Repeated Start state enumeration>
typedef enum
{
	Start,
	RepeatedStart

}StartState_t;

//==============<Functional state enumeration>
typedef enum
{
	DISABLE,
	ENABLE
}Functional_State_t;

//==============<Flag Status  enumeration>
typedef enum
{
	Reset,
	Set
}FlagStatus_t;

//==============<I2C Direction  enumeration>
typedef enum
{
	I2C_Direction_Transmitter,
	I2C_Direction_Receiver
}I2C_Direction_t;

//==============<Status enumeration>
typedef enum
{
	I2C_Flag_Busy,
	EV5,           //SB=1, cleared by reading SR1 register followed by writing DR register with Address
	EV6,           //EV6: ADDR=1, cleared by reading SR1 register followed by reading SR2
	EV7,           //EV7: RxNE=1 cleared by reading DR register.
	EV8,           //EV8: TxE=1, shift register not empty, data register empty, cleared by writing DR register .
	EV8_2,         //TxE=1, BTF = 1, Program Stop request. TxE and BTF are cleared by hardware by the Stop condition
	EV8_1,         //EV8_1: TxE=1, shift register empty, data register empty, write Data1 in DR.
	Master_Byte_Transmitting =(uint32_t)(0x00070080)   /*TRA MSL TXE*/
}Status_t;
//==========================================================================================================

//=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//                                            Support APIs by: I2C
//=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*

//-----------------------------------<I2C Init Function>
void MCAL_I2C_Init (I2C_t* I2Cx, I2C_CFG_t* I2C_CFG);
void MCAL_I2C_DeInit (I2C_t* I2Cx);
void MCAL_I2C_GPIO_Init (I2C_t* I2Cx);

//--------------------------------------------------------<I2C Master Send/Receive Function With Polling Mechanism>
void MCAL_I2C_Master_SendData (I2C_t* I2Cx, uint16_t Slave_Address, uint8_t* Dataout, uint32_t DataLen, StopState_t StopState);
void MCAL_I2C_Master_ReceiveData (I2C_t* I2Cx, uint16_t Slave_Address, uint8_t* Dataout, uint32_t DataLen, StartState_t StartState, StopState_t StopState);

//--------------------------------------------------------<I2C Salve Send/Receive Function With Interrupt>
void MCAL_I2C_Slave_SendData (I2C_t* I2Cx, uint8_t Data);
uint8_t MCAL_I2C_Slave_ReceiveData (I2C_t* I2Cx);

//                                   *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//  =================================*            Generic APIs           *==================================
//                                   *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*

FlagStatus_t I2C_Get_FlagState (I2C_t* I2Cx, Status_t Flag);
void I2C_Generate_Start (I2C_t* I2Cx, Functional_State_t Fun_State, StartState_t StartState);
void I2C_Generate_Stop (I2C_t* I2Cx, Functional_State_t Fun_State);
void I2C_Send_Slave_Address (I2C_t* I2Cx, uint8_t Slave_Address, I2C_Direction_t I2C_Direction);
void I2C_ACK_Control (I2C_t* I2Cx, Functional_State_t State );


#endif /* INC_STM32F103C6_I2C_DRIVER_H_ */
