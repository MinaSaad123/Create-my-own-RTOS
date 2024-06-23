/*
 * Stm32f103c6_I2C_Driver.c
 *
 *  Created on: ١١‏/١٠‏/٢٠٢٣
 *      Author: HP
 */

//-----------------------<Include>
#include "Stm32f103c6_I2C_Driver.h"
#include "Stm32f103c6_GPIO_Driver.h"
#include "Stm32f103c6_RCC_Driver.h"

//                          *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//--------------------------*                 Generic variables                     *------------------------------
//                          *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*

I2C_CFG_t G_S_I2C_CFG[2] = { {0}, {0} };

//                          *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//--------------------------*                     Generic Macros                    *------------------------------
//                          *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
#define I2C1_Index     0
#define I2C2_Index     1

/*=======================================================
 * @Fn				-MCAL_I2C_Init
 * @brief			-I2C Init
 * @param [in]		-I2C_Instance
 * @param [in]		-Pointer to I2C Config
 * @retval			-None
 * Note
 */
void MCAL_I2C_Init (I2C_t* I2Cx, I2C_CFG_t* I2C_CFG)
{
	           /*============Assumption: MCAL_I2C_Init Function initialize one time==============*/

	uint16_t TempReg = 0, FreqRange = 0, Result;

	uint32_t Pclk = 8000000;

	if (I2Cx == I2C1)
	{
		//Passing I2C1 Address to global variable
		G_S_I2C_CFG[I2C1_Index] = *I2C_CFG;

		//Enable Clock I2C1
		RCC_I2C1_CLK_EN();

	} else if (I2Cx == I2C2)
	{
		//Passing I2C2 Address to global variable
		G_S_I2C_CFG[I2C2_Index] = *I2C_CFG;

		//Enable Clock I2C2
		RCC_I2C2_CLK_EN();

	}
	//===============================================================================================================================================

	/*Checking I2C Mode (Master - SMBus)*/
	if (I2C_CFG->I2C_Mode == I2C_Mode_Master)
	{
		//1- Bits 5:0 FREQ[5:0]: Peripheral clock frequency

		TempReg = I2Cx->I2C_CR2;

		TempReg &= ~(0x1F); /* Clear 5 bits */

		Pclk = MCAL_RCC_GetPCLK1Freq();  /*Get Pclk Value*/

		FreqRange = ( Pclk / 1000000 ); //Get Clock VAL with Mega unit

        TempReg = ( FreqRange);

        I2Cx->I2C_CR2 = TempReg;  /*Passing Frequence Value*/

        TempReg = 0;

        //Disable I2C enable peripheral to configure time "Just to make sure"
        I2Cx->I2C_CR1 &= ~(I2C_CR1_PE);

    	//===============================================================================================================================================

		//2-Configure the clock control registers
        if (I2C_CFG->I2C_Master_ClockSpeed == I2C_Master_ClockSpeed_SM_50K || I2C_CFG->I2C_Master_ClockSpeed == I2C_Master_ClockSpeed_SM_100K)
        {
        	//<Bits 11:0 CCR[11:0]: Clock control register in Fm/Sm mode (Master mode)>
        	//Controls the SCL clock in master mode.        //Sm mode or SMBus:
        	//high = CCR * Tpclk1
        	//low = CCR * Tpclk1
        	Result = (uint16_t)( Pclk / (2 * I2C_CFG->I2C_Master_ClockSpeed) ); /*Calculate CCR Value*/

        	I2Cx->I2C_CCR |= Result;                          /*Passing Value for CCR*/

        	//Bits 5:0 TRISE[5:0]: Maximum rise time in Fm/Sm mode (Master mode)
        	I2Cx->I2C_TRISE |= (FreqRange + 1);

        } else
        {
        	//XXX-------TODO------XXX SUPPORT FM Mode
        }
    	//===============================================================================================================================================

        //3- Config CR1 Register
        TempReg = I2Cx->I2C_CR1;

        TempReg |= (I2C_CFG->I2C_Acknowledge);  /*Select Acknowledge State*/
        TempReg |= (I2C_CFG->I2C_GeneralCall);  /*Select GeneralCall State*/
        TempReg |= (I2C_CFG->I2C_NOStretch);    /*Select Stretch State*/

        I2Cx->I2C_CR1|= TempReg;             /*Passing Value to CR1 Register*/
        TempReg = 0;

        if (I2C_CFG->Slave_Address_CFG.Slave_DualAddress_EN == 1)  //Support Dual Address
        {
        	I2Cx->I2C_OAR2 |= I2C_OAR2_ENDUAL;
        	I2Cx->I2C_OAR2 |= ( I2C_CFG->Slave_Address_CFG.Secondary_Address << I2C_OAR2_ADD2_Pos );
        }
        //Set Primary Address
        I2Cx->I2C_OAR1 |= (I2C_CFG->Slave_Address_CFG.Primary_Address << 1);
        I2Cx->I2C_OAR1 |= (I2C_CFG->Slave_Address_CFG.Slave_Address_Mode);

    	if (I2C_CFG->I2C_Mode == I2C_Mode_Slave)
    	{
    		I2Cx->I2C_CR2 |= I2C_CR2_ITBUFEN;
    		I2Cx->I2C_CR2 |= I2C_CR2_ITERREN;
    		I2Cx->I2C_CR2 |= I2C_CR2_ITEVTEN;
    	}

    	//4- Enable Interrupt
        if (I2Cx == I2C1)
        {
        	NVIC_IRQ31_I2C1_Enable;

        } else
        {
        	NVIC_IRQ33_I2C2_Enable;
        }

	} else
	{

		//TODO XXX not supported yet SMS
	}


	//===============================================================================================================================================

    //5- Init  I2C GPIO Pins
    MCAL_I2C_GPIO_Init(I2Cx);

	//6- Enable I2C Peripheral
    I2Cx->I2C_CR1 = I2C_CR1_PE;

	//===============================================================================================================================================
	//===============================================================================================================================================
}

/*=======================================================
 * @Fn				-MCAL_I2C_DeInit
 * @brief			-DeInit I2C
 * @param [in]		-I2C_Instance
 * @retval			-None
 * Note
 */
void MCAL_I2C_DeInit (I2C_t* I2Cx)
{
	if (I2Cx == I2C1)
	{
		NVIC_IRQ31_I2C1_Disable;

		RCC_I2C1_CLK_DIS();

	} else
	{
		NVIC_IRQ33_I2C2_Disable;

		RCC_I2C2_CLK_DIS();
	}
}

/*=======================================================
 * @Fn				-MCAL_I2C_GPIO_Init
 * @brief			-Init GPIO
 * @param [in]		-I2C_Instance
 * @retval			-None
 * Note
 */
void MCAL_I2C_GPIO_Init (I2C_t* I2Cx)
{
	GPIO_Config_t Pin_CFG;

	if (I2Cx == I2C1)
	{
		/*==========================*
		 *        PB6  |  PB7       *
		 *==========================*/

		//PB6: -> SCL
        Pin_CFG.GPIO_Output_SPEED= GPIO_OUTPUT_SPEED_10M;
        Pin_CFG.GPIO_MODE = GPIO_MODE_OUTPUT_AF_OD;
		Pin_CFG.GPIO_PinNumber = GPIO_PIN_6;
		MCAL_GPIO_INIT(GPIOB, &Pin_CFG);

		//PB7: -> SDA
        Pin_CFG.GPIO_Output_SPEED = GPIO_OUTPUT_SPEED_10M;
        Pin_CFG.GPIO_MODE = GPIO_MODE_OUTPUT_AF_OD;
		Pin_CFG.GPIO_PinNumber = GPIO_PIN_7;
		MCAL_GPIO_INIT(GPIOB, &Pin_CFG);

	} else
	{
		/*==========================*
		 *       PB10  |  PB11      *
		 *==========================*/

		//PB6: -> SCL
        Pin_CFG.GPIO_Output_SPEED = GPIO_OUTPUT_SPEED_10M;
        Pin_CFG.GPIO_MODE = GPIO_MODE_OUTPUT_AF_OD;
		Pin_CFG.GPIO_PinNumber = GPIO_PIN_10;
		MCAL_GPIO_INIT(GPIOB, &Pin_CFG);

		//PB7: -> SDA
        Pin_CFG.GPIO_Output_SPEED = GPIO_OUTPUT_SPEED_10M;
        Pin_CFG.GPIO_MODE = GPIO_MODE_OUTPUT_AF_OD;
		Pin_CFG.GPIO_PinNumber = GPIO_PIN_11;
		MCAL_GPIO_INIT(GPIOB, &Pin_CFG);

	}

}

/*=======================================================
 * @Fn				-MCAL_I2C_Master_SendData
 * @brief			-I2C I2C Send Data
 * @param [in]		-I2C_Instance
 * @param [in]		-Slave_Address
 * @param [in]		-DataOut
 * @param [in]		-DataLen
 * @param [in]		-StartState
 * @param [in]		-StopState
 * @retval			-None
 * Note
 */
void MCAL_I2C_Master_SendData (I2C_t* I2Cx, uint16_t Slave_Address, uint8_t* Dataout, uint32_t DataLen, StopState_t StopState)
{
	int i;

	//1- Generate Start bit
	I2C_Generate_Start(I2Cx, ENABLE, Start);
	//===============================================================================================================================================

	//2- Wait until generate start bit
	while ( !(I2C_Get_FlagState(I2Cx, EV5) ) );
	//===============================================================================================================================================

	//3- Send Slave Address
	I2C_Send_Slave_Address(I2Cx, Slave_Address, I2C_Direction_Transmitter);
	//================================================================================================================================================

	//4- Wait until Send Address
	while ( !( I2C_Get_FlagState(I2Cx, EV6) ) );
    //================================================================================================================================================

	//5- Wait TRA, Busy, MSL, TXE flags
	while ( !( I2C_Get_FlagState(I2Cx, Master_Byte_Transmitting) ) );
	//================================================================================================================================================

	//6- Send Data to Slave
	for (i = 0;i < DataLen;++i)
	{
		I2Cx->I2C_DR = Dataout[i];

        //Wait until EV8  occur
		while ( !( I2C_Get_FlagState(I2Cx, EV8) ) );

	}
    //================================================================================================================================================

	//7- Send Stop to Slave
    if (StopState == WithStop)
    {
    	I2C_Generate_Stop(I2Cx, ENABLE);
    }
    //================================================================================================================================================
    //================================================================================================================================================
}

/*=======================================================
 * @Fn				-MCAL_I2C_Master_ReceiveData
 * @brief			-I2C Master Receive Data
 * @param [in]		-I2C_Instance
 * @param [in]		-Slave_Address
 * @param [in]		-DataOut
 * @param [in]		-DataLen
 * @param [in]		-StartState
 * @retval			-None
 * Note
 */
void MCAL_I2C_Master_ReceiveData (I2C_t* I2Cx, uint16_t Slave_Address, uint8_t* Dataout, uint32_t DataLen, StartState_t StartState, StopState_t StopState)
{
	int i;

	//1- Generate Start bit
	I2C_Generate_Start(I2Cx, ENABLE, StartState);
	//===============================================================================================================================================

	//2- Wait until generate start bit
	while ( !(I2C_Get_FlagState(I2Cx, EV5) ) );
	//===============================================================================================================================================

	//3- Send Slave Address
	I2C_Send_Slave_Address(I2Cx, Slave_Address, I2C_Direction_Receiver );
	//================================================================================================================================================

	//4- Wait until Send Address
	while ( !( I2C_Get_FlagState(I2Cx, EV6) ) );
	//================================================================================================================================================

	//5- Enable Acknowledge
	I2C_ACK_Control(I2Cx, Enable);
	//================================================================================================================================================

	//6- Receive data from Slave
	for (i = 0;i < (DataLen - 1);++i)
	{
		//Wait for EV7
		while ( !( I2C_Get_FlagState(I2Cx, EV7) ) );
		Dataout[0] = (uint8_t)I2Cx->I2C_DR;
		Dataout++;

	}
	//================================================================================================================================================

	//7- Disable Acknowledge
	I2C_ACK_Control(I2Cx, Disable);
	//================================================================================================================================================

	//8- Receive the last element of data
	while ( !( I2C_Get_FlagState(I2Cx, EV7) ) );
	Dataout[0] = (uint8_t)I2Cx->I2C_DR;
	//================================================================================================================================================

	//9- Send Stop to Slave
    if (StopState == WithStop)
    {
    	I2C_Generate_Stop(I2Cx, ENABLE);
    }
    //================================================================================================================================================
    //================================================================================================================================================

}

/*=======================================================
 * @Fn				-MCAL_I2C_Slave_SendData
 * @brief			-I2C Slave Send Data
 * @param [in]		-I2C_Instance
 * @param [in]		-Send Data
 * @retval			-None
 * Note
 */
void MCAL_I2C_Slave_SendData (I2C_t* I2Cx, uint8_t Data)
{
	I2Cx->I2C_DR = Data;
}

/*=======================================================
 * @Fn				-MCAL_I2C_Slave_ReceiveData
 * @brief			-I2C Slave Receive Data
 * @param [in]		-I2C_Instance
 * @retval			-None
 * Note
 */
uint8_t MCAL_I2C_Slave_ReceiveData (I2C_t* I2Cx)
{
	return (uint8_t)I2Cx->I2C_DR;
}

void I2C_Send_Slave_Address (I2C_t* I2Cx, uint8_t Slave_Address, I2C_Direction_t I2C_Direction)
{
	//Displace Address with one bit to put I2C Direction
	Slave_Address = Slave_Address << 1;

	if (I2C_Direction == I2C_Direction_Transmitter)
	{
		Slave_Address &= ~(1 << 0);

	} else
	{
		Slave_Address |= (1 << 0);
	}

	I2Cx->I2C_DR= Slave_Address;   /*Send Address*/
}


void I2C_Generate_Start (I2C_t* I2Cx, Functional_State_t Fun_State, StartState_t StartState)
{
	if (StartState != RepeatedStart)
	{
		while ( I2C_Get_FlagState(I2Cx, I2C_Flag_Busy) );
	}

	if (Fun_State == ENABLE)
	{
        //		Bit 8 START: Start generation
		//		This bit is set and cleared by software and cleared by hardware when start is sent or PE=0.
		//		In Master Mode:
		//		0: No Start generation
		//		1: Repeated start generation
		//		In Slave mode:
		//		0: No Start generation
		//		1: Start generation when the bus is free

		I2Cx->I2C_CR1 |= I2C_CR1_START;

	} else
	{
		I2Cx->I2C_CR1 &= ~( I2C_CR1_START );
	}
}

FlagStatus_t I2C_Get_FlagState (I2C_t* I2Cx, Status_t Flag)
{
	uint32_t DummyFlag = 0;

	switch (Flag)
	{
	case I2C_Flag_Busy:
	{
		//		Bit 1 BUSY: Bus busy
		//		0: No communication on the bus
		//		1: Communication ongoing on the bus

		if ( (I2Cx->I2C_SR2) & (I2C_SR2_BUSY) )
		{
			return Set;

		} else
		{
			return Reset;
		}
		break;
	}
	case EV5:
	{
		//		Bit 0 SB: Start bit (Master mode)
		//		0: No Start condition
		//		1: Start condition generated

		if ( (I2Cx->I2C_SR1) & (I2C_SR1_SB) )
		{
			return Set;

		} else
		{
			return Reset;
		}
		break;
	}
	case EV6:
	{
		//		Bit 1 ADDR: Address sent (master mode)/matched (slave mode)
		//		This bit is cleared by software reading SR1 register followed reading SR2, or by hardware
		//		when PE=0.
		//		Address matched (Slave)
		//		0: Address mismatched or not received.
		//		1: Received address matched.

		if ( (I2Cx->I2C_SR1) & (I2C_SR1_ADDR) )
		{
			return Set;

		} else
		{
			return Reset;
		}
		break;
	}
	case EV7:
	{
		if ( (I2Cx->I2C_SR1) & (I2C_SR1_RXNE) )
		{
			return Set;

		} else
		{
			return Reset;
		}
		break;
	}
	case EV8:
	case EV8_1:
	{
		//		Bit 7 TxE: Data register empty (transmitters)
		//		0: Data register not empty
		//		1: Data register empty

		if ( (I2Cx->I2C_SR1) & (I2C_SR1_TXE) )
		{
			return Set;

		} else
		{
			return Reset;
		}
		break;
	}
	case Master_Byte_Transmitting:
	{
		DummyFlag = (I2Cx->I2C_SR2 << 16) | I2Cx->I2C_SR1;

		if (DummyFlag == Flag)
		{
			return Set;

		} else
		{
			return Reset;
		}
	}
	break;

	}

	return 0;
}

void I2C_ACK_Control (I2C_t* I2Cx, Functional_State_t State )
{
	if (State == (Functional_State_t)Enable)
	{
		/*Generate the Aknowladge*/
		I2Cx->I2C_CR1 |= I2C_Ack_Control_EN;


	} else
	{
		/*Disable the Aknowladge*/
		I2Cx->I2C_CR1 &= ~(I2C_CR1_ACK);

	}
}

void I2C_Generate_Stop (I2C_t* I2Cx, Functional_State_t Fun_State)
{
	if (Fun_State == ENABLE)
	{
		//		Bit 9 STOP: Stop generation
		//		The bit is set and cleared by software, cleared by hardware when a Stop condition is
		//		detected, set by hardware when a timeout error is detected.
		//		In Master Mode:
		//		0: No Stop generation.
		//		1: Stop generation after the current byte transfer or after the current Start condition is sent.
		//		In Slave mode:
		//		0: No Stop generation.
		//		1: Release the SCL and SDA lines after the current byte transfer
		I2Cx->I2C_CR1 |= I2C_CR1_STOP;

	} else
	{
		I2Cx->I2C_CR1 &= ~(I2C_CR1_STOP);
	}

}

//===================================================================
//(_   _)( )                   ( )       ( )   ( )               ( )
//  | |  | |__    ____   ___   | |/ )     \ \_/ /  ___    _   _  | |
//  | |  |  _  \ / _  )/  _  \ | | (        \ /   /   \  ( ) ( ) | |
//  | |  | | | |( (_| || ( ) | | |\ \       | |  ( (_) ) | (_) | | |
//  (_)  (_) (_) \___ )(_) (_) (_) (_)      (_)   \___/   \___/  (_)

