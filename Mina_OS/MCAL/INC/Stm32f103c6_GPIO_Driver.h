/*
 * Stm32f103c6_GPIO.h
 *
 *  Created on: Sep 5, 2023
 *      Author: HP
 */

#ifndef STM32F103C6_GPIO_H_
#define STM32F103C6_GPIO_H_

/******Include******/
#include "Stm32f103x8.h"
//===================================

/******Config structure*****/
typedef struct
{
	uint16_t GPIO_PinNumber;   //Specifies the GPIO Pins to configured
	                           //This parameter must be a value of @ref GPIO_PIN_define

	uint8_t GPIO_MODE;         //Specifies the GPIO Mode to configured
	                           //This parameter must be a value of @ref GPIO_MODE_define

	uint8_t GPIO_Output_SPEED; //Specifies the GPIO Speed to configured
	                           //This parameter must be a value of @ref GPIO_SPEED_define
}GPIO_Config_t;

//*************************************
//     Macros Configuration References
//*************************************

//===========@ref GPIO_PIN_define
#define GPIO_PIN_0		((uint16_t)0x0001)
#define GPIO_PIN_1		((uint16_t)0x0002)
#define GPIO_PIN_2		((uint16_t)0x0004)
#define GPIO_PIN_3		((uint16_t)0x0008)
#define GPIO_PIN_4		((uint16_t)0x0010)
#define GPIO_PIN_5		((uint16_t)0x0020)
#define GPIO_PIN_6		((uint16_t)0x0040)
#define GPIO_PIN_7		((uint16_t)0x0080)
#define GPIO_PIN_8		((uint16_t)0x0100)
#define GPIO_PIN_9		((uint16_t)0x0200)
#define GPIO_PIN_10 	((uint16_t)0x0400)
#define GPIO_PIN_11		((uint16_t)0x0800)
#define GPIO_PIN_12		((uint16_t)0x1000)
#define GPIO_PIN_13		((uint16_t)0x2000)
#define GPIO_PIN_14		((uint16_t)0x4000)
#define GPIO_PIN_15		((uint16_t)0x8000)

//===========@ref GPIO_MODE_define
//0: Analog mode
//1: Floating input (reset state)
//2: Input with pull-up
//3: Input with pull-down
//4: General purpose output push-pull
//5: General purpose output Open-drain
//6: Alternate function output Push-pull
//7: Alternate function output Open-drain
//8: Alternate function input Open-drain
#define GPIO_MODE_Analog         ((uint8_t)0x00)
#define GPIO_MODE_INPUT_Floating ((uint8_t)0x01)
#define GPIO_MODE_INPUT_PU       ((uint8_t)0x02)
#define GPIO_MODE_INPUT_PD       ((uint8_t)0x03)
#define GPIO_MODE_OUTPUT_PP      ((uint8_t)0x04)
#define GPIO_MODE_OUTPUT_OD      ((uint8_t)0x05)
#define GPIO_MODE_OUTPUT_AF_PP   ((uint8_t)0x06)
#define GPIO_MODE_OUTPUT_AF_OD   ((uint8_t)0x07)
#define GPIO_MODE_INPUT_AF_OD    ((uint8_t)0x08)

//===========@ref GPIO_SPEED_define
//1: Output mode, max speed 10 MHz.
//2: Output mode, max speed 2 MHz.
//3: Output mode, max speed 50 MHz
#define GPIO_OUTPUT_SPEED_10M    ((uint8_t)0x01)
#define GPIO_OUTPUT_SPEED_2M     ((uint8_t)0x02)
#define GPIO_OUTPUT_SPEED_50M    ((uint8_t)0x03)

//===========@ref GPIO_LOCK_define
#define GPIO_LOCK_Enabled        1
#define GPIO_LOCK_ERROR          0

//===========GPIO states
#define HIGH                     (uint16_t)1
#define LOW                      (uint16_t)0

//*******************************************
//     APIS Supported by "MCAL GPIO DRIVER"
//*******************************************

//=======GPIO Init APIs
void     MCAL_GPIO_INIT(GPIO_t* GPIOX, GPIO_Config_t* PinConfig );
void     MCAL_GPIO_DEINIT(GPIO_t* GPIOX);

//=======GPIO Read APIs
uint16_t MCAL_GPIO_ReadPort(GPIO_t* GPIOX);
uint8_t  MCAL_GPIO_ReadPin(GPIO_t* GPIOX, uint16_t PinNumber);

//=======GPIO Write APIs
void     MCAL_GPIO_WritePort(GPIO_t* GPIOX, uint16_t Portx_Val);
void     MCAL_GPIO_WritePin(GPIO_t* GPIOX, uint16_t Pinx_Val, uint16_t PinNumber);
void     MCAL_GPIO_Reset_Output_Particular(GPIO_t* GPIOX, uint16_t Portx_Val);
void     MCAL_GPIO_Set_Output_Particular(GPIO_t* GPIOX, uint16_t Portx_Val);

//=======GPIO Pin features APIs
void     MCAL_GPIO_TogglePin(GPIO_t* GPIOX, uint16_t PinNumber);
uint8_t  MCAL_GPIO_lockPin(GPIO_t* GPIOX, uint16_t PinNumber);

//======Delay
void    _delay_ms(uint32_t Time);

#endif /* STM32F103C6_GPIO_H_ */
