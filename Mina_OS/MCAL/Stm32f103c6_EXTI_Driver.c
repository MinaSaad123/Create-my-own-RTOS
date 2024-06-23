/*
 * Stm32f103c6_EXTI_Driver.c
 *
 *  Created on: ٠٩‏/٠٩‏/٢٠٢٣
 *      Author: HP
 */

//=====Include
#include "Stm32f103c6_EXTI_Driver.h"

//*=*=*=*=*=*=*=*=**=*=*=*=*=*=*=*=**=*=*=*=*=*=*=*=*
//             Generic Macros
//*=*=*=*=*=*=*=*=**=*=*=*=*=*=*=*=**=*=*=*=*=*=*=*=*
#define AFIO_GPIO_EXTI_Mapping(x) \
	                              ( (x == GPIOA)?0:\
	                            	(x == GPIOB)?1:\
	                            	(x == GPIOC)?2:\
	                            	(x == GPIOD)?3:0)

//*=*=*=*=*=*=*=*=**=*=*=*=*=*=*=*=**=*=*=*=*=*=*=*=*
//             Generic Variable
//*=*=*=*=*=*=*=*=**=*=*=*=*=*=*=*=**=*=*=*=*=*=*=*=*

void (* GP_IRQ_CallBack[15])(void);

//*=*=*=*=*=*=*=*=**=*=*=*=*=*=*=*=**=*=*=*=*=*=*=*=*
//             Generic Function
//*=*=*=*=*=*=*=*=**=*=*=*=*=*=*=*=**=*=*=*=*=*=*=*=*

void Enable_Nvic(uint16_t IRQ_Num)
{
	switch (IRQ_Num)
	{
	case 0:
		NVIC_IRQ6_EXTI0_Enable;
		break;
	case 1:
		NVIC_IRQ7_EXTI1_Enable;
		break;
	case 2:
		NVIC_IRQ8_EXTI2_Enable;
		break;
	case 3:
		NVIC_IRQ9_EXTI3_Enable;
		break;
	case 4:
		NVIC_IRQ10_EXTI4_Enable;
		break;
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
		NVIC_IRQ23_EXTI5_9_Enable;
		break;
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
		NVIC_IRQ40_EXTI10_15_Enable;
		break;

	}
}

void Disable_Nvic(uint16_t IRQ_Num)
{
	switch (IRQ_Num) {
	case 0:
		NVIC_IRQ6_EXTI0_Disable;
		break;
	case 1:
		NVIC_IRQ7_EXTI1_Disable;
		break;
	case 2:
		NVIC_IRQ8_EXTI2_Disable;
		break;
	case 3:
		NVIC_IRQ9_EXTI3_Disable;
		break;
	case 4:
		NVIC_IRQ10_EXTI4_Disable;
		break;
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
		NVIC_IRQ23_EXTI5_9_Disable;
		break;
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
		NVIC_IRQ40_EXTI10_15_Disable;
		break;
	}
}

void EXTI_Uptade (EXTI_PinConfig* EXTI_Config)
{
    AFIO_t AFIO_CNF;

	//1- Make GPIO Floating point
	GPIO_Config_t Pinconfig;
	Pinconfig.GPIO_PinNumber = EXTI_Config->EXTI_Pin.GPIO_Pin;
	Pinconfig.GPIO_MODE = GPIO_MODE_INPUT_Floating;
	MCAL_GPIO_INIT(EXTI_Config->EXTI_Pin.GPIO_Port, &Pinconfig);
	//============================================================

	//2- Update AFIO to route with EXTI Line
	uint32_t AFIO_EXTICR_Index = EXTI_Config->EXTI_Pin.EXTI_Iput_Line / 4;
	uint32_t AFIO_EXTICR_Postion = (EXTI_Config->EXTI_Pin.EXTI_Iput_Line % 4) * 4;

	//Clear 4-Bits
	AFIO_CNF.EXTICR[AFIO_EXTICR_Index] &= ~(0x0f << AFIO_EXTICR_Postion);
	AFIO_CNF.EXTICR[AFIO_EXTICR_Index] |= (AFIO_GPIO_EXTI_Mapping(EXTI_Config->EXTI_Pin.GPIO_Port) << AFIO_EXTICR_Postion);
    //============================================================

	//3- Update Rising and Falling Trigger
	EXTI->RTSR &= ~(1 << EXTI_Config->EXTI_Pin.EXTI_Iput_Line);
	EXTI->FTSR &= ~(1 << EXTI_Config->EXTI_Pin.EXTI_Iput_Line);

    /********************Set (Rising - Falling - both Together)*********************/
	if (EXTI_Config->Trigger == EXTI_Trigger_RISING)	//Case it is Rising trigger

	{
		EXTI->RTSR |= (1 << EXTI_Config->EXTI_Pin.EXTI_Iput_Line);

	} else if (EXTI_Config->Trigger == EXTI_Trigger_FALLING)  //Case it is Falling trigger
	{
		EXTI->FTSR |= (1 << EXTI_Config->EXTI_Pin.EXTI_Iput_Line);

	}else
	{
		EXTI->FTSR |= (1 << EXTI_Config->EXTI_Pin.EXTI_Iput_Line);
		EXTI->RTSR |= (1 << EXTI_Config->EXTI_Pin.EXTI_Iput_Line);
	}
	//============================================================

	//4- Update IRQ Handling Callback
	GP_IRQ_CallBack[AFIO_EXTICR_Postion] = EXTI_Config->P_IRQ_CallBack;
	//============================================================

	//5- Update  Enable or disable IRQ
	if (EXTI_Config->IRQ_EN == EXTI_IRQ_Enable)
	{
		//Enable Interrupt Mask Register
		EXTI->IMR |= (1 << EXTI_Config->EXTI_Pin.EXTI_Iput_Line);

		//Enable Corresponding  IRQ for Line
		Enable_Nvic(EXTI_Config->EXTI_Pin.EXTI_Iput_Line);

	} else if (EXTI_Config->IRQ_EN == EXTI_IRQ_Disable)
	{
		//Disable Interrupt Mask Register
		EXTI->IMR &= ~(1 << EXTI_Config->EXTI_Pin.EXTI_Iput_Line);

		//Disable Corresponding  IRQ for Line
		Disable_Nvic(EXTI_Config->EXTI_Pin.EXTI_Iput_Line);

	}

}
/* ================================================================
 * @Fun 			-MCAL_EXTI_GPIO_DeInit
 * @brief 			-Reset EXTI Register and NVIC Corresponding IRQ
 * @retval 			-None
 * Note             -None
 */
void  MCAL_EXTI_GPIO_DeInit (void)
{
	//Reset EXTI Registers
	EXTI->PR    = 0xFFFFFFFF;
	EXTI->EMR   = 0x00;
	EXTI->FTSR  = 0x00;
	EXTI->RTSR  = 0x00;
	EXTI->SWIER = 0x00;
	EXTI->IMR   = 0x00;

	//Clear NVIC
	NVIC_IRQ6_EXTI0_Disable;
	NVIC_IRQ7_EXTI1_Disable;
	NVIC_IRQ8_EXTI2_Disable;
	NVIC_IRQ9_EXTI3_Disable;
	NVIC_IRQ10_EXTI4_Disable;
	NVIC_IRQ23_EXTI5_9_Disable;
	NVIC_IRQ40_EXTI10_15_Disable;

}

/* ================================================================
 * @Fun 			-MCAL_EXTI_GPIO_Init
 * @brief 			-Initialize GPIOx PINy according to Specified Parameter in GPIO_Config
 * @param [in] 		-GPIOX which is any one of GPIO Instant
 * @param [in] 		-pointer to structure that  include pin config
 * @retval 			-None
 * Note             -None
 */
void  MCAL_EXTI_GPIO_Init (EXTI_PinConfig* EXTI_Config)
{
	EXTI_Uptade (EXTI_Config);

}

/* ================================================================
 * @Fun 			-MCAL_EXTI__GPIO_Uptade
 * @brief 			-Initialize GPIOx PINy according to Specified Parameter in GPIO_Config
 * @param [in] 		-pointer to structure that  include EXTI configure
 * @retval 			-None
 * Note             -None
 */
void MCAL_EXTI__GPIO_Uptade (EXTI_PinConfig* EXTI_Config)
{
	EXTI_Uptade (EXTI_Config);
}


//*=*=*=*=*=*=*=*=**=*=*=*=*=*=*=*=**=*=*=*=*=*=*=*=*
//             Interrupt Handler
//*=*=*=*=*=*=*=*=**=*=*=*=*=*=*=*=**=*=*=*=*=*=*=*=*

void EXTI0_IRQHandler(void)
{
	//Clearing PR register
	EXTI->PR |= (1<<0);
	//Calling function
	GP_IRQ_CallBack[0]();

}
void EXTI1_IRQHandler(void)
{
	//Clearing PR register
	EXTI->PR |= (1<<1);
	//Calling function
	GP_IRQ_CallBack[1]();
}
void EXTI2_IRQHandler(void)
{
	//Clearing PR register
	EXTI->PR |= (1<<2);
	//Calling function
	GP_IRQ_CallBack[2]();
}
void EXTI3_IRQHandler(void)
{
	//Clearing PR register
	EXTI->PR |= (1<<3);
	//Calling function
	GP_IRQ_CallBack[3]();

}
void EXTI4_IRQHandler(void)
{

	//Clearing PR register
	EXTI->PR |= (1<<4);
	//Calling function
	GP_IRQ_CallBack[4]();
}
void EXTI9_5_IRQHandler(void)
{
	if(EXTI->PR & 1<<5) {EXTI->PR |= (1<<5) ; GP_IRQ_CallBack[5]();}
	if(EXTI->PR & 1<<6) {EXTI->PR |= (1<<6) ; GP_IRQ_CallBack[6]();}
	if(EXTI->PR & 1<<7) {EXTI->PR |= (1<<7) ; GP_IRQ_CallBack[7]();}
	if(EXTI->PR & 1<<8) {EXTI->PR |= (1<<8) ; GP_IRQ_CallBack[8]();}
	if(EXTI->PR & 1<<9) {EXTI->PR |= (1<<9) ; GP_IRQ_CallBack[9]();}
}
void EXTI15_10_IRQHandler(void)
{
	if(EXTI->PR & 1<<10) {EXTI->PR |= (1<<10) ; GP_IRQ_CallBack[10]();}
	if(EXTI->PR & 1<<11) {EXTI->PR |= (1<<11) ; GP_IRQ_CallBack[11]();}
	if(EXTI->PR & 1<<12) {EXTI->PR |= (1<<12) ; GP_IRQ_CallBack[12]();}
	if(EXTI->PR & 1<<13) {EXTI->PR |= (1<<13) ; GP_IRQ_CallBack[13]();}
	if(EXTI->PR & 1<<14) {EXTI->PR |= (1<<14) ; GP_IRQ_CallBack[14]();}
	if(EXTI->PR & 1<<15) {EXTI->PR |= (1<<15) ; GP_IRQ_CallBack[15]();}

}


