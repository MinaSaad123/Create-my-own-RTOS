/*
 * GPIO.c
 *
 *  Created on: Sep 7, 2023
 *      Author: HP
 */

//----------------------------------------------------------------<Include Lib>
#include "Stm32f103c6_GPIO_Driver.h"


/*                                             ============================
 *============================================*     Function Implement    *============================================
 *                                             ============================
 */
//=======Reset GPIO Particular
void MCAL_GPIO_Reset_Output_Particular(GPIO_t* GPIOX, uint16_t Portx_Val)
{
	GPIOX->ODR &= ~(uint32_t)(Portx_Val);
}

//=======Set GPIO Particular
void MCAL_GPIO_Set_Output_Particular(GPIO_t* GPIOX, uint16_t Portx_Val)
{
	GPIOX->ODR |= (uint32_t)(Portx_Val);
}


uint8_t Get_Pin_Bit_Position(uint16_t PinNumber)
{
	switch (PinNumber)
	{
	case GPIO_PIN_0:
		return 0;
	case GPIO_PIN_1:
		return 4;
	case GPIO_PIN_2:
		return 8;
	case GPIO_PIN_3:
		return 12;
	case GPIO_PIN_4:
		return 16;
	case GPIO_PIN_5:
		return 20;
	case GPIO_PIN_6:
		return 24;
	case GPIO_PIN_7:
		return 28;
	}
	switch (PinNumber)
	{
	case GPIO_PIN_8:
		return 0;
	case GPIO_PIN_9:
		return 4;
	case GPIO_PIN_10:
		return 8;
	case GPIO_PIN_11:
		return 12;
	case GPIO_PIN_12:
		return 16;
	case GPIO_PIN_13:
		return 20;
	case GPIO_PIN_14:
		return 24;
	case GPIO_PIN_15:
		return 28;
	}
	return 0;
}

/* ================================================================
 * @Fun 			-MCAL_GPIO_Init
 * @brief 			-Initialize GPIOx PINy according to Specified Parameter in GPIO_Config
 * @param [in] 		-GPIOX which is any one of GPIO Instant
 * @param [in] 		-pointer to structure that  include pin config
 * @retval 			-None
 * Note             -None
 */
void MCAL_GPIO_INIT(GPIO_t* GPIOX, GPIO_Config_t* PinConfig )
{
	//Port Configuration register low (GPIOx_CRL) Configure PINS From 0 --> 7
	//Port Configuration register High (GPIOx_CRH) Configure PINS From 8 --> 15
	volatile uint32_t* RegisterConfig = (void*)0;
	uint8_t Config = 0;
	//Pin is is in Register CRL or CRH
	RegisterConfig = (PinConfig->GPIO_PinNumber < GPIO_PIN_8)? &GPIOX->CRL : &GPIOX->CRH;

	//Clear CNFX[1:0] MODEX[1:0]
	*RegisterConfig &= ~(0xf << Get_Pin_Bit_Position(PinConfig->GPIO_PinNumber));

	//Is pin output
	if(PinConfig->GPIO_MODE == GPIO_MODE_OUTPUT_AF_OD || PinConfig->GPIO_MODE == GPIO_MODE_OUTPUT_AF_PP ||
	      PinConfig->GPIO_MODE == GPIO_MODE_OUTPUT_OD || PinConfig->GPIO_MODE == GPIO_MODE_OUTPUT_PP)
	{
		//Set CNFX[1:0] MODEX[1:0]
		Config = ( ((PinConfig->GPIO_MODE - 4) << 2) | (PinConfig->GPIO_Output_SPEED) );
	}
	else
	{
		if(PinConfig->GPIO_MODE == GPIO_MODE_Analog || PinConfig->GPIO_MODE == GPIO_MODE_INPUT_Floating )
		{
			//Set CNFX[1:0] MODEX[1:0]
			Config = ( (PinConfig->GPIO_MODE) << 2);
		}
		else if(PinConfig->GPIO_MODE == GPIO_MODE_INPUT_AF_OD)//Consider it as input floating
		{
			//Set CNFX[1:0] MODEX[1:0]
			Config = ( (PinConfig->GPIO_MODE) << 2);
		}
		else //You deal with pull-up & pull-down
		{
			Config = (GPIO_MODE_INPUT_PU << 2);
			if(PinConfig->GPIO_MODE == GPIO_MODE_INPUT_PU)
			{
				//Set ODR Register to make it pull-up
				GPIOX->ODR = PinConfig->GPIO_PinNumber;
			}
			else
			{
				//Set ODR Register to make it pull-down
				GPIOX->ODR &= ~(PinConfig->GPIO_PinNumber);
			}
		}
	}
    *RegisterConfig |= ( (Config) << Get_Pin_Bit_Position(PinConfig->GPIO_PinNumber) );
}

/* ================================================================
 * @Fun 			-MCAL_GPIO_DEINIT
 * @brief 			-Initialize GPIOx PINy according to Specified Parameter in GPIO_Config
 * @param [in] 		-GPIO in which the pin exists
 * @retval 			-None
 * Note             -None
 */
void MCAL_GPIO_DEINIT(GPIO_t* GPIOX)
{
	if(GPIOX == GPIOA)
		{
			RCC->APB2RSTR |= (1<<2);
			RCC->APB2RSTR &= ~(1<<2);
		}
		else if (GPIOX == GPIOB)
		{
			RCC->APB2RSTR |= (1<<3);
			RCC->APB2RSTR &= ~(1<<3);
		}
		else if (GPIOX == GPIOC)
		{
			RCC->APB2RSTR |= (1<<4);
			RCC->APB2RSTR &= ~(1<<4);

		}
		else if (GPIOX == GPIOD)
		{
			RCC->APB2RSTR |= (1<<5);
			RCC->APB2RSTR &= ~(1<<5);
		}
		else
		{
			RCC->APB2RSTR |= (1<<6);
			RCC->APB2RSTR &= ~(1<<6);
		}
}

/* ================================================================
 * @Fun 			-MCAL_GPIO_ReadPort
 * @brief 			-Initialize GPIOx PINy according to Specified Parameter in GPIO_Config
 * @param [in] 		-GPIOX which is any one of GPIO Instant
 * @retval 			-Return ReadPort value
 * Note             -None
 */
uint16_t MCAL_GPIO_ReadPort(GPIO_t* GPIOX)
{
	uint16_t PORT_Value = (uint16_t)(GPIOX->IDR) ;
	return PORT_Value;
}

/* ================================================================
 * @Fun 			-MCAL_GPIO_Init
 * @brief 			-Initialize GPIOx PINy according to Specified Parameter in GPIO_Config
 * @param [in] 		-GPIOX which is any one of GPIO Instant
 * @param [in] 		-Pin number
 * @retval 			-Return (High or Low)
 * Note             -None
 */
uint8_t MCAL_GPIO_ReadPin(GPIO_t* GPIOX, uint16_t PinNumber)
{
	if( ((GPIOX->IDR & PinNumber) != LOW) )
	{
		return HIGH;
	}
	else
	{
		return LOW;
	}
}

/* ================================================================
 * @Fun 			-MCAL_GPIO_WritePort
 * @brief 			-Initialize GPIOx PINy according to Specified Parameter in GPIO_Config
 * @param [in] 		-GPIOX which is any one of GPIO Instant
 * @param [in] 		-PortX value
 * @retval 			-None
 * Note             -None
 */
void MCAL_GPIO_WritePort(GPIO_t* GPIOX, uint16_t Portx_Val)
{
	GPIOX->ODR = (uint32_t)(Portx_Val);
}

/* ================================================================
 * @Fun 			-MCAL_GPIO_WritePin
 * @brief 			-Initialize GPIOx PINy according to Specified Parameter in GPIO_Config
 * @param [in] 		-GPIOX which is any one of GPIO Instant
 * @param [in] 		-PinX value
 * @param [in] 		-Pin number
 * @retval 			-None
 * Note             -None
 */
void  MCAL_GPIO_WritePin(GPIO_t* GPIOX, uint16_t Pinx_Val, uint16_t PinNumber)
{
	if(Pinx_Val != LOW )
	{
		//These bits are write-only and can be accessed in Word mode only.
		//0: No action on the corresponding ODRx bit
		//1: Set the corresponding ODRx bit
		GPIOX->BSRR = (uint32_t)PinNumber;
	}
	else
	{
		//These bits are write-only and can be accessed in Word mode only.
		//0: No action on the corresponding ODRx bit
		//1: Reset the corresponding ODRx bit
		GPIOX->BRR = (uint32_t)PinNumber;
	}
}

/* ================================================================
 * @Fun 			-MCAL_GPIO_TogglePin
 * @brief 			-Initialize GPIOx PINy according to Specified Parameter in GPIO_Config
 * @param [in] 		-GPIOX which is any one of GPIO Instant
 * @param [in] 		-Pin number
 * @retval 			-None
 * Note             -None
*/
void MCAL_GPIO_TogglePin(GPIO_t* GPIOX, uint16_t PinNumber)
{
	GPIOX->ODR ^= PinNumber;
}

/* ================================================================
 * @Fun 			-MCAL_GPIO_lockPin
 * @brief 			-Initialize GPIOx PINy according to Specified Parameter in GPIO_Config
 * @param [in] 		-GPIOX which is any one of GPIO Instant
 * @param [in] 		-Pin number
 * @retval 			-Return from @ref GPIO_LOCK_define
 * Note             -None
*/
uint8_t  MCAL_GPIO_lockPin(GPIO_t* GPIOX, uint16_t PinNumber)
{

	uint32_t Tmp = 1 << 16;
	//Bit 16 LCKK[16]: Lock key
	//This bit can be read any time. It can only be modified using the Lock Key Writing Sequence.
	//0: Port configuration lock key not active
	//1: Port configuration lock key active. GPIOx_LCKR register is locked until the next reset.
	//LOCK key writing sequence:
	//Write 1
	//Write 0
	//Write 1
	//Read 0
	//Read 1 (this read is optional but confirms that the lock is active)
	//Note: During the LOCK Key Writing sequence, the value of LCK[15:0] must not change.
	//Any error in the lock sequence will abort the lock
    GPIOX->LCKR = (uint32_t)PinNumber;
    //Write 1
    GPIOX->LCKR = Tmp;
    //Write 0
    GPIOX->LCKR ^= ~(Tmp);
   	//Write 1
    GPIOX->LCKR = Tmp;
    //Read 0
    Tmp = GPIOX->LCKR;
	//Read 1 (this read is optional but confirms that the lock is active)
     if( (GPIOX->LCKR & (1 << 16)) )
     {
    	 return GPIO_LOCK_Enabled;
     }
     else return GPIO_LOCK_ERROR;

}


