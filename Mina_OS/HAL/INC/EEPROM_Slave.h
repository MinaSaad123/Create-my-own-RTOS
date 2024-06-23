/*
 * EEPROM_Slave.h
 *
 *  Created on: ٢١‏/١١‏/٢٠٢٣
 *      Author: HP
 */

#ifndef INC_EEPROM_SLAVE_H_
#define INC_EEPROM_SLAVE_H_

//===========================<Include>
#include "Stm32f103c6_I2C_Driver.h"

//===========================<Macros>
#define EEPROM_Slave_address 0x2A

//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//                                Function Prototype
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
void EEPROM_Init (void);
void EEPROM_Write_NBytes (uint16_t Memory_Address, uint8_t* Bytes , uint32_t Data_Length);
void EEPROM_Read_Nbyte (uint16_t Memory_Address, uint8_t *Dataout, uint32_t Datalen);



#endif /* INC_EEPROM_SLAVE_H_ */
