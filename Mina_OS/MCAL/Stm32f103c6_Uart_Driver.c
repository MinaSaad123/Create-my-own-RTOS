/*
 * Stm32f103c6_Uart_Driver.c
 *
 *  Created on: Sep 17, 2023
 *      Author: HP
 */

//=====Include
#include "Stm32f103c6_Uart_Driver.h"
#include "Stm32f103c6_RCC_Driver.h"
#include "Stm32f103c6_RCC_Driver.h"

//=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//                      Generic Variables
//=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
UART_Config* G_UART_CFG = (void*)0;


//=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//                      Generic Function
//=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*

/*========================================
 * @Fn				-MCAL_UART_Init
 * @brief			-Initializes UART
 * @param [in]		-UART Instance
 * @param [in]		-UART_CFG
 * @retval			-None
 * Note
 */
void MCAL_UART_Init (UART_t* UARTx, UART_Config* UART_CFG)
{
	G_UART_CFG = UART_CFG;
    uint32_t PCLK, BRR;
	//1- Enable Clock for UART
	if (UARTx == UART1)
	{
		RCC_UART1_CLK_EN();

	} if (UARTx == UART2)
	{
		RCC_UART2_CLK_EN();
	} if (UARTx == UART3)
	{
		RCC_UART3_CLK_EN();
	}
	//==================================

	//2- Enable TX or RX or both according to  uart Config
	UARTx->CR1 |= UART_CFG->UART_Mode;
	//==================================

	//3- Set Parity
	UARTx->CR1 |= UART_CFG->Parity;
	//==================================

	//4- Set Payload
	UARTx->CR1 |= UART_CFG->Pay_Load_Length;
	//===================================

	//5- Set number of stop Bits
	UARTx->CR2 |= UART_CFG->Stop_Bits;

	//6- Set HwFlowCTRL
	UARTx->CR3 |= UART_CFG->HwFlowCTRL;
	//===================================

	//7- Enable Clock
	if (UARTx == UART1)
	{
		PCLK = MCAL_RCC_GetPCLK2Freq();
	} else
	{
		PCLK = MCAL_RCC_GetPCLK1Freq();
	}
	//==================================

	//8- Set BaudRate
	BRR = UART_BRR_Register(PCLK, UART_CFG->UART_BaudRate);
	UARTx->BRR |=  BRR;
    //==================================

	//9- UART Enable
	UARTx->CR1 |= 1 << 13;
	//==================================


    //10- Set IRQ and NVIC
	if (UART_CFG->IRQ_Enable != UART_IRQ_Enable_NONE)
	{
		UARTx->CR1 |= UART_CFG->IRQ_Enable;

		if (UARTx == UART1)
		{
			NVIC_IRQ37_UART1_15_Enable;

		} else if (UARTx == UART2)
		{
			NVIC_IRQ38_UART2_15_Enable;
		} else
		{
			NVIC_IRQ39_UART3_15_Enable;

		}
	}
	//==================================

}

/*=======================================================
 * @Fn				-MCAL_UART_DeInit
 * @brief			-DeInitializes UART
 * @param [in]		-UART_Instance
 * @retval			-None
 * Note
 */
void MCAL_UART_DeInit(UART_t* UARTx)
{
	if (UARTx == UART1)
	{
		RCC_UART1_Reset();
		NVIC_IRQ37_UART1_15_Disable;

	} else if (UARTx == UART2)
	{
		RCC_UART2_Reset();
		NVIC_IRQ38_UART2_15_Disable;

	} else
	{
		RCC_UART3_Reset();
		NVIC_IRQ39_UART3_15_Disable;
	}

}

/*=======================================================
 * @Fn				-MCAL_UART_GPIO_Set_Pins
 * @brief			-Set GPIO Pins
 * @param [in]		-UART_Instance
 * @retval			-None
 * Note
 */
void MCAL_UART_GPIO_Set_Pins(UART_t* UARTx)
{
	GPIO_Config_t Pin;
	//1- Check Uart(1- 2- 3)
	if (UARTx == UART1)
	{
//		PA9 TX
//		PA10 RX
//		PA11 CTS
//		PA12 RTS

		//PA9 TX
		Pin.GPIO_PinNumber = GPIO_PIN_9;
		Pin.GPIO_Output_SPEED = GPIO_OUTPUT_SPEED_10M;
		Pin.GPIO_MODE = GPIO_MODE_OUTPUT_AF_PP;
		MCAL_GPIO_INIT(GPIOA, &Pin);
		//=============================================

		//PA10 RX
		Pin.GPIO_PinNumber = GPIO_PIN_10;
		Pin.GPIO_MODE = GPIO_MODE_INPUT_Floating;
		MCAL_GPIO_INIT(GPIOA, &Pin);
        //============================================

		if (G_UART_CFG->HwFlowCTRL == UART_HwFlowCtl_CTS || G_UART_CFG->HwFlowCTRL == UART_HwFlowCtl_RTS_Cts )
		{
			//PA1 CTS
			Pin.GPIO_PinNumber = GPIO_PIN_11;
			Pin.GPIO_MODE = GPIO_MODE_INPUT_Floating;
			MCAL_GPIO_INIT(GPIOA, &Pin);
		}

		if (G_UART_CFG->HwFlowCTRL == UART_HwFlowCtl_RTS || G_UART_CFG->HwFlowCTRL == UART_HwFlowCtl_RTS_Cts )
		{
			//PA1 CTS
			Pin.GPIO_PinNumber = GPIO_PIN_12;
			Pin.GPIO_Output_SPEED = GPIO_OUTPUT_SPEED_10M;
			Pin.GPIO_MODE = GPIO_MODE_OUTPUT_AF_PP;
			MCAL_GPIO_INIT(GPIOA, &Pin);
		}

	} else if (UARTx == UART2)
	{
//		PA2 TX
//		PA3 RX
//		PA0 CTS
//		PA1 RTS

		//PA9 TX
		Pin.GPIO_PinNumber = GPIO_PIN_2;
		Pin.GPIO_Output_SPEED = GPIO_OUTPUT_SPEED_10M;
		Pin.GPIO_MODE = GPIO_MODE_OUTPUT_AF_PP;
		MCAL_GPIO_INIT(GPIOA, &Pin);
		//=============================================

		//PA10 RX
		Pin.GPIO_PinNumber = GPIO_PIN_3;
		Pin.GPIO_MODE = GPIO_MODE_INPUT_Floating;
		MCAL_GPIO_INIT(GPIOA, &Pin);
        //============================================

		if (G_UART_CFG->HwFlowCTRL == UART_HwFlowCtl_CTS || G_UART_CFG->HwFlowCTRL == UART_HwFlowCtl_RTS_Cts )
		{
			//PA1 CTS
			Pin.GPIO_PinNumber = GPIO_PIN_0;
			Pin.GPIO_MODE = GPIO_MODE_INPUT_Floating;
			MCAL_GPIO_INIT(GPIOA, &Pin);
		}

		if (G_UART_CFG->HwFlowCTRL == UART_HwFlowCtl_RTS || G_UART_CFG->HwFlowCTRL == UART_HwFlowCtl_RTS_Cts )
		{
			//PA1 CTS
			Pin.GPIO_PinNumber = GPIO_PIN_1;
			Pin.GPIO_Output_SPEED = GPIO_OUTPUT_SPEED_10M;
			Pin.GPIO_MODE = GPIO_MODE_OUTPUT_AF_PP;
			MCAL_GPIO_INIT(GPIOA, &Pin);
		}

	} else
	{
//		PB10 TX
//		PB11 RX
//		PB13 CTS
//		PB14 RTS

		//PB10 TX
		Pin.GPIO_PinNumber = GPIO_PIN_10;
		Pin.GPIO_Output_SPEED = GPIO_OUTPUT_SPEED_10M;
		Pin.GPIO_MODE = GPIO_MODE_OUTPUT_AF_PP;
		MCAL_GPIO_INIT(GPIOB, &Pin);
		//=============================================

		//PB11 RX
		Pin.GPIO_PinNumber = GPIO_PIN_11;
		Pin.GPIO_MODE = GPIO_MODE_INPUT_Floating;
		MCAL_GPIO_INIT(GPIOB, &Pin);
        //============================================

		if (G_UART_CFG->HwFlowCTRL == UART_HwFlowCtl_CTS || G_UART_CFG->HwFlowCTRL == UART_HwFlowCtl_RTS_Cts )
		{
			//PB13 CTS
			Pin.GPIO_PinNumber = GPIO_PIN_13;
			Pin.GPIO_MODE = GPIO_MODE_INPUT_Floating;
			MCAL_GPIO_INIT(GPIOB, &Pin);
		}

		if (G_UART_CFG->HwFlowCTRL == UART_HwFlowCtl_RTS || G_UART_CFG->HwFlowCTRL == UART_HwFlowCtl_RTS_Cts )
		{
			//PB14 CTS
			Pin.GPIO_PinNumber = GPIO_PIN_14;
			Pin.GPIO_Output_SPEED = GPIO_OUTPUT_SPEED_10M;
			Pin.GPIO_MODE = GPIO_MODE_OUTPUT_AF_PP;
			MCAL_GPIO_INIT(GPIOB, &Pin);
		}

	}

}

/*=======================================================
 * @Fn				-MCAL_UART_SendData
 * @brief			-Send Data from UART
 * @param [in]		-UART_Instance
 * @param [in]		-Pointer to UART Buffer
 * @param [in]		-Enum for (Polling - Interrupt)
 * @retval			-None
 * Note
 */
void MCAL_UART_SendData(UART_t* UARTx, uint16_t* P_Buffer, enum Polling_Mechansim PollingEN )
{
	//Wait Until TXE flag is Set
	if (PollingEN == Enable)
	{
		while (! (UARTx->SR & 1 << 7) );
	}
	//Check the uart Word_Length is 8-bit or 9-bit

//	When transmitting with the parity enabled (PCE bit set to 1 in the USART_CR1 register),
//	the value written in the MSB (bit 7 or bit 8 depending on the data length) has no effect
//	because it is replaced by the parity.
//	When receiving with the parity enabled, the value read in the MSB bit is the received parity bit.

	if ( G_UART_CFG->Pay_Load_Length == UART_Pay_Load_Length_9bit)
	{
		UARTx->DR = (*P_Buffer & (uint16_t)0x1ff);
	} else
	{
		// This 8-bit Data Transfer
		UARTx->DR = (*P_Buffer & (uint16_t)0xff);
	}
}

/*=======================================================
 * @Fn				-MCAL_UART_ReceiveData
 * @brief			-Receive data
 * @param [in]		-UART_Instance
 * @param [in]		-Pointer to UART Buffer
 * @param [in]		-Enum for (Polling - Interrupt)
 * @retval			-None
 * Note
 */
void MCAL_UART_ReceiveData(UART_t* UARTx, uint16_t* P_Buffer, enum Polling_Mechansim PollingEN)
 {
		//Wait until RXEN flag is set in the SR
		if (PollingEN == Enable)
		{
			while (!(UARTx->SR & 1 << 5));

		}

		//Check the uart Word_Length is 8-bit or 9-bit
		if (G_UART_CFG->Pay_Load_Length == UART_Pay_Load_Length_9bit)
		{
			if (G_UART_CFG->Parity == UART_Parity_NONE)
			{
				*((uint16_t*) P_Buffer) = UARTx->DR;

			} else
			{
				*((uint16_t*) P_Buffer) = (UARTx->DR & (uint8_t) 0xff);
			}
		} else
		{
			// This 8-bit Data Transfer
			if (G_UART_CFG->Parity == UART_Parity_NONE) {
				/*Test*/*((uint16_t*) P_Buffer) = (UARTx->DR & (uint8_t) 0xff);

			} else
			{
				*((uint16_t*) P_Buffer) = (UARTx->DR & (uint8_t) 0x7f);
			}
		}
}

//=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#
//                               IRQ Handler
//=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#

void USART1_IRQHandler ()
{
	G_UART_CFG->P_IRQ_CallBack ();
}

void USART2_IRQHandler ()
{
	G_UART_CFG->P_IRQ_CallBack ();
}

void USART3_IRQHandler ()
{
	G_UART_CFG->P_IRQ_CallBack ();
}

