/*
 * Stm32f103c6_EXTI_Driver.h
 *
 *  Created on: ٠٩‏/٠٩‏/٢٠٢٣
 *      Author: HP
 */

#ifndef INC_STM32F103C6_EXTI_DRIVER_H_
#define INC_STM32F103C6_EXTI_DRIVER_H_

//=====Include
#include "Stm32f103c6_GPIO_Driver.h"
#include "Stm32f103x8.h"

typedef struct
{
	uint16_t EXTI_Iput_Line;
	GPIO_t*  GPIO_Port;
	uint16_t GPIO_Pin;
	uint8_t  IVIT_IRG_Number;

}EXTI_GPIO_Mapping_t;

typedef struct
{
	EXTI_GPIO_Mapping_t EXTI_Pin;   //Specifies the External interrupt mapping
	                                //This parameter must be based on @ref EXTI_Define

    uint8_t             Trigger;    //Specifies Rising or Falling or both
                                    //This parameter must be set based on @Ref EXTI_Trigger_Define

    uint8_t             IRQ_EN;     //Enable or Disabled the EXTI IRQ (will Enable the IRQ Mask in EXTI and also on the NVIC)
                                    //This parameter must be set based on @Ref EXTI_IRQ_Define

    void (* P_IRQ_CallBack)(void);  //Set the C Function which will be called once the IRQ happen

}EXTI_PinConfig;

/*********EXTI***********/
#define  EXTI0   0
#define  EXTI1   1
#define  EXTI2   2
#define  EXTI3   3
#define  EXTI4   4
#define  EXTI5   5
#define  EXTI6   6
#define  EXTI7   7
#define  EXTI8   8
#define  EXTI9   9
#define  EXTI10  10
#define  EXTI11  11
#define  EXTI12  12
#define  EXTI13  13
#define  EXTI14  14
#define  EXTI15  15

/*********************************Macros Reference****************************************/

//=======@Ref EXTI_Define
//EXTI0
#define EXTI0pA0         (EXTI_GPIO_Mapping_t){EXTI0, GPIOA, GPIO_PIN_0, EXTI0_IRQ}
#define EXTI0pB0         (EXTI_GPIO_Mapping_t){EXTI0, GPIOB, GPIO_PIN_0, EXTI0_IRQ}
#define EXTI0pC0         (EXTI_GPIO_Mapping_t){EXTI0, GPIOC, GPIO_PIN_0, EXTI0_IRQ}
#define EXTI0pD0         (EXTI_GPIO_Mapping_t){EXTI0, GPIOD, GPIO_PIN_0, EXTI0_IRQ}

//EXTI1
#define EXTI1pA1         (EXTI_GPIO_Mapping_t){EXTI1, GPIOA, GPIO_PIN_1, EXTI1_IRQ}
#define EXTI1pB1         (EXTI_GPIO_Mapping_t){EXTI1, GPIOB, GPIO_PIN_1, EXTI1_IRQ}
#define EXTI1pC1         (EXTI_GPIO_Mapping_t){EXTI1, GPIOC, GPIO_PIN_1, EXTI1_IRQ}
#define EXTI1pD1         (EXTI_GPIO_Mapping_t){EXTI1, GPIOD, GPIO_PIN_1, EXTI1_IRQ}

//EXTI2
#define EXTI2pA2         (EXTI_GPIO_Mapping_t){EXTI2, GPIOA, GPIO_PIN_2, EXTI2_IRQ}
#define EXTI2pB2         (EXTI_GPIO_Mapping_t){EXTI2, GPIOB, GPIO_PIN_2, EXTI2_IRQ}
#define EXTI2pC2         (EXTI_GPIO_Mapping_t){EXTI2, GPIOC, GPIO_PIN_2, EXTI2_IRQ}
#define EXTI2pD2         (EXTI_GPIO_Mapping_t){EXTI2, GPIOD, GPIO_PIN_2, EXTI2_IRQ}

//EXTI3
#define EXTI3pA3         (EXTI_GPIO_Mapping_t){EXTI3, GPIOA, GPIO_PIN_3, EXTI3_IRQ}
#define EXTI3pB3         (EXTI_GPIO_Mapping_t){EXTI3, GPIOB, GPIO_PIN_3, EXTI3_IRQ}
#define EXTI3pC3         (EXTI_GPIO_Mapping_t){EXTI3, GPIOC, GPIO_PIN_3, EXTI3_IRQ}
#define EXTI3pD3         (EXTI_GPIO_Mapping_t){EXTI3, GPIOD, GPIO_PIN_3, EXTI3_IRQ}

//EXTI4
#define EXTI4pA4         (EXTI_GPIO_Mapping_t){EXTI4, GPIOA, GPIO_PIN_4, EXTI4_IRQ}
#define EXTI4pB4         (EXTI_GPIO_Mapping_t){EXTI4, GPIOB, GPIO_PIN_4, EXTI4_IRQ}
#define EXTI4pC4         (EXTI_GPIO_Mapping_t){EXTI4, GPIOC, GPIO_PIN_4, EXTI4_IRQ}
#define EXTI4pD4         (EXTI_GPIO_Mapping_t){EXTI4, GPIOD, GPIO_PIN_4, EXTI4_IRQ}

//EXTI5
#define EXTI5pA5         (EXTI_GPIO_Mapping_t){EXTI5, GPIOA, GPIO_PIN_5, EXTI5_IRQ}
#define EXTI5pB5         (EXTI_GPIO_Mapping_t){EXTI5, GPIOB, GPIO_PIN_5, EXTI5_IRQ}
#define EXTI5pC5         (EXTI_GPIO_Mapping_t){EXTI5, GPIOC, GPIO_PIN_5, EXTI5_IRQ}
#define EXTI5pD5         (EXTI_GPIO_Mapping_t){EXTI5, GPIOD, GPIO_PIN_5, EXTI5_IRQ}

//EXTI6
#define EXTI6pA6         (EXTI_GPIO_Mapping_t){EXTI6, GPIOA, GPIO_PIN_6, EXTI6_IRQ}
#define EXTI6pB6         (EXTI_GPIO_Mapping_t){EXTI6, GPIOB, GPIO_PIN_6, EXTI6_IRQ}
#define EXTI6pC6         (EXTI_GPIO_Mapping_t){EXTI6, GPIOC, GPIO_PIN_6, EXTI6_IRQ}
#define EXTI6pD6         (EXTI_GPIO_Mapping_t){EXTI6, GPIOD, GPIO_PIN_6, EXTI6_IRQ}

//EXTI7
#define EXTI7pA7         (EXTI_GPIO_Mapping_t){EXTI7, GPIOA, GPIO_PIN_7, EXTI7_IRQ}
#define EXTI7pB7         (EXTI_GPIO_Mapping_t){EXTI7, GPIOB, GPIO_PIN_7, EXTI7_IRQ}
#define EXTI7pC7         (EXTI_GPIO_Mapping_t){EXTI7, GPIOC, GPIO_PIN_7, EXTI7_IRQ}
#define EXTI7pD7         (EXTI_GPIO_Mapping_t){EXTI7, GPIOD, GPIO_PIN_7, EXTI7_IRQ}

//EXTI8
#define EXTI8pA8         (EXTI_GPIO_Mapping_t){EXTI8, GPIOA, GPIO_PIN_8, EXTI8_IRQ}
#define EXTI8pB8         (EXTI_GPIO_Mapping_t){EXTI8, GPIOB, GPIO_PIN_8, EXTI8_IRQ}
#define EXTI8pC8         (EXTI_GPIO_Mapping_t){EXTI8, GPIOC, GPIO_PIN_8, EXTI8_IRQ}
#define EXTI8pD8         (EXTI_GPIO_Mapping_t){EXTI8, GPIOD, GPIO_PIN_8, EXTI8_IRQ}

//EXTI9
#define EXTI9pA9         (EXTI_GPIO_Mapping_t){EXTI9, GPIOA, GPIO_PIN_9, EXTI9_IRQ}
#define EXTI9pB9         (EXTI_GPIO_Mapping_t){EXTI9, GPIOB, GPIO_PIN_9, EXTI9_IRQ}
#define EXTI9pC9         (EXTI_GPIO_Mapping_t){EXTI9, GPIOC, GPIO_PIN_9, EXTI9_IRQ}
#define EXTI9pD9         (EXTI_GPIO_Mapping_t){EXTI9, GPIOD, GPIO_PIN_9, EXTI9_IRQ}

//EXTI10
#define EXTI10pA10       (EXTI_GPIO_Mapping_t){EXTI10, GPIOA, GPIO_PIN_10, EXTI10_IRQ}
#define EXTI10pB10       (EXTI_GPIO_Mapping_t){EXTI10, GPIOB, GPIO_PIN_10, EXTI10_IRQ}
#define EXTI10pC10       (EXTI_GPIO_Mapping_t){EXTI10, GPIOC, GPIO_PIN_10, EXTI10_IRQ}
#define EXTI10pD10       (EXTI_GPIO_Mapping_t){EXTI10, GPIOD, GPIO_PIN_10, EXTI10_IRQ}

//EXTI11
#define EXTI11pA11       (EXTI_GPIO_Mapping_t){EXTI11, GPIOA, GPIO_PIN_11, EXTI11_IRQ}
#define EXTI11pB11       (EXTI_GPIO_Mapping_t){EXTI11, GPIOB, GPIO_PIN_11, EXTI11_IRQ}
#define EXTI11pC11       (EXTI_GPIO_Mapping_t){EXTI11, GPIOC, GPIO_PIN_11, EXTI11_IRQ}
#define EXTI11pD11       (EXTI_GPIO_Mapping_t){EXTI11, GPIOD, GPIO_PIN_11, EXTI11_IRQ}

//EXTI12
#define EXTI12pA12       (EXTI_GPIO_Mapping_t){EXTI12, GPIOA, GPIO_PIN_12, EXTI12_IRQ}
#define EXTI12pB12       (EXTI_GPIO_Mapping_t){EXTI12, GPIOB, GPIO_PIN_12, EXTI12_IRQ}
#define EXTI12pC12       (EXTI_GPIO_Mapping_t){EXTI12, GPIOC, GPIO_PIN_12, EXTI12_IRQ}
#define EXTI12pD12       (EXTI_GPIO_Mapping_t){EXTI12, GPIOD, GPIO_PIN_12, EXTI12_IRQ}

//EXTI13
#define EXTI13pA13       (EXTI_GPIO_Mapping_t){EXTI13, GPIOA, GPIO_PIN_14, EXTI13_IRQ}
#define EXTI13pB13       (EXTI_GPIO_Mapping_t){EXTI13, GPIOB, GPIO_PIN_14, EXTI13_IRQ}
#define EXTI13pC13       (EXTI_GPIO_Mapping_t){EXTI13, GPIOC, GPIO_PIN_14, EXTI13_IRQ}
#define EXTI13pD13       (EXTI_GPIO_Mapping_t){EXTI13, GPIOD, GPIO_PIN_14, EXTI13_IRQ}

//EXTI14
#define EXTI14pA14       (EXTI_GPIO_Mapping_t){EXTI14, GPIOA, GPIO_PIN_14, EXTI14_IRQ}
#define EXTI14pB14       (EXTI_GPIO_Mapping_t){EXTI14, GPIOB, GPIO_PIN_14, EXTI14_IRQ}
#define EXTI14pC14       (EXTI_GPIO_Mapping_t){EXTI14, GPIOC, GPIO_PIN_14, EXTI14_IRQ}
#define EXTI14pD14       (EXTI_GPIO_Mapping_t){EXTI14, GPIOD, GPIO_PIN_14, EXTI14_IRQ}

//EXTI15
#define EXTI15pA15       (EXTI_GPIO_Mapping_t){EXTI15, GPIOA, GPIO_PIN_15, EXTI15_IRQ}
#define EXTI15pB15       (EXTI_GPIO_Mapping_t){EXTI15, GPIOB, GPIO_PIN_15, EXTI15_IRQ}
#define EXTI15pC15       (EXTI_GPIO_Mapping_t){EXTI15, GPIOC, GPIO_PIN_15, EXTI15_IRQ}
#define EXTI15pD15       (EXTI_GPIO_Mapping_t){EXTI15, GPIOD, GPIO_PIN_15, EXTI15_IRQ}
//=====================================================================================

//=====@Ref EXTI_Trigger_define
#define EXTI_Trigger_RISING              0
#define EXTI_Trigger_FALLING             1
#define EXTI_Trigger_RISING_And_FALLING  2
//=====================================================================================

//=====@Ref EXTI_IRQ_Define
#define EXTI_IRQ_Enable                   1
#define EXTI_IRQ_Disable                  0
//=====================================================================================

/*
 * =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
 *              APIs Supported by "MCAL GPIO DRIVE"
 * =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
 */
void  MCAL_EXTI_GPIO_DeInit();
void  MCAL_EXTI_GPIO_Init(EXTI_PinConfig* EXTI_Config);

void MCAL_EXTI__GPIO_Uptade(EXTI_PinConfig* EXTI_Config);

#endif /* INC_STM32F103C6_EXTI_DRIVER_H_ */
