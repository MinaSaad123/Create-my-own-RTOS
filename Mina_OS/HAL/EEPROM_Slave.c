/*
 * EEPROM_Slave.c
 *
 *  Created on: ٢١‏/١١‏/٢٠٢٣
 *      Author: HP
 */

//--------------------------<Include>
#include "EEPROM_Slave.h"


//                          *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//--------------------------*                     Generic Macros                    *------------------------------
//                          *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*

#define NULL (void*)0

//=======================================================================================================================
//=======================================================================================================================

//======================<EEPROM_Init Function>
void EEPROM_Init (void)
{
	I2C_CFG_t I2C1_CFG;

	/*Passing Values for struct I2C_CFG*/
	I2C1_CFG.I2C_Mode = I2C_Mode_Master;
	I2C1_CFG.I2C_Master_ClockSpeed = I2C_Master_ClockSpeed_SM_100K;
	I2C1_CFG.I2C_NOStretch = I2C_NOStretch_EN;
	I2C1_CFG.P_Slave_Event_CallBack = NULL;
    I2C1_CFG.I2C_GeneralCall = I2C_GeneralCall_EN;

    MCAL_I2C_Init(I2C1, &I2C1_CFG);      /*I2C Init*/
}

//======================<EEPROM_Write_NBytes Function>
void EEPROM_Write_NBytes (uint16_t Memory_Address, uint8_t* Bytes , uint32_t Data_Length)
{
	int i;
	uint8_t Buffer[256];
	Buffer[0] = (uint8_t)(Memory_Address >> 8); /*Set Upper Slave Address*/
	Buffer[1] = (uint8_t)(Memory_Address);      /*Set Lower Slave Address*/

	for ( i = 0; i < Data_Length;i++)
	{
		Buffer[i + 2] = Bytes[i];
	}

	MCAL_I2C_Master_SendData (I2C1, EEPROM_Slave_address, Buffer, Data_Length + 2, WithStop);
	//================================================================================================================================================
}

//======================<EEPROM_Read_Nbyte Function>
void EEPROM_Read_Nbyte (uint16_t Memory_Address, uint8_t *Dataout, uint32_t Datalen)
{
	uint8_t Buffer[2];

	Buffer[0] = (uint8_t)(Memory_Address >> 8); /*Set Upper Slave Address*/
	Buffer[1] = (uint8_t)(Memory_Address);      /*Set Lower Slave Address*/

	MCAL_I2C_Master_SendData (I2C1, EEPROM_Slave_address, Buffer, 2, WithoutStop);

	MCAL_I2C_Master_ReceiveData (I2C1, EEPROM_Slave_address, Dataout, Datalen, RepeatedStart, WithStop);
	//================================================================================================================================================

}

