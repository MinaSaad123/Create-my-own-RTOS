/*
 * Stm32f103c6_RCC_Driver.h
 *
 *  Created on: Sep 17, 2023
 *      Author: HP
 */

#ifndef INC_STM32F103C6_RCC_DRIVER_H_
#define INC_STM32F103C6_RCC_DRIVER_H_

//======Include
#include "Stm32f103c6_GPIO_Driver.h"
#include "Stm32f103x8.h"

//===========Clocks
#define HSI_RC_Clk   (uint32_t)8000000
#define HSE_Clk      (uint32_t)16000000

//============Function Prototype
uint32_t MCAL_RCC_GetHCLKFreq (void);
uint32_t MCAL_RCC_GetPCLK1Freq (void);
uint32_t MCAL_RCC_GetPCLK2Freq (void);
uint32_t MCAL_RCC_GetSYS_CLCKFreq (void);



#endif /* INC_STM32F103C6_RCC_DRIVER_H_ */
