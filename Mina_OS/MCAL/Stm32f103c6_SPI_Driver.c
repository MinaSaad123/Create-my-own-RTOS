/*
 * Stm32f103c6_SPI_Driver.c
 *
 *  Created on: Sep 23, 2023
 *      Author: HP
 */

//===========================<Include>
#include "Stm32f103c6_SPI_Driver.h"

//=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//                            Generic Variable
//=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
SPI_Config* G_SPI_CFG[2];

//=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//                            Generic Macros
//=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
#define SPI1_Index  0
#define SPI2_Index  1

/* ================================================================
 * @Fun 			-MCAL_SPI_Init
 * @brief 			-Init SPI
 * @param [in] 		-SPIx which is any one of SPI Instant
 * @param [in] 		-pointer to structure that  include pin config
 * Note             -None
 */
void MCAL_SPI_Init (SPI_t* SPIx, SPI_Config* Pin_Config)
{
	uint16_t Temp_CR1 = 0, Temp_CR2 = 0;

	//1- Any SPI you want to config
	if (SPIx == SPI1)
	{
		G_SPI_CFG[SPI1_Index] = Pin_Config;
		E_SPI1_CLK_EN();

	} else
	{
		G_SPI_CFG[SPI2_Index] = Pin_Config;
		E_SPI2_CLK_EN();

	}
	//=================================

	//2- You want Master or Salve
	Temp_CR1 |= Pin_Config->Device_Mode; ///Look
	//=================================

	//3- Set Communication direction
    Temp_CR1 |= Pin_Config->Communication_DIR;
    //=================================

    //4-Set Data Format
    Temp_CR1 |= Pin_Config->Data_Format;
    //=================================

    //5-Set Frame Format
    Temp_CR1 |= Pin_Config->Frame_Format;
    //=================================

    //6- Set Clock Polarity
    Temp_CR1 |= Pin_Config->Clock_Polarity;
    //=================================

    //7- Set Clock Phase
    Temp_CR1 |= Pin_Config->Clock_Phase;
    //=================================

    //8- Set NSS "Select Salve"
    if (Pin_Config->NSS == SPI_NSS_Internal_Hard_Output_ENR)
    {
    	Temp_CR2 |= Pin_Config->NSS;

    } else
    {
    	Temp_CR1 |= Pin_Config->NSS;
    }
    //=================================

    //9- Set Clock Prescaler
    Temp_CR1 |= Pin_Config->BaudRate_Prescaler;
    //=================================

    //10- Set SPI Enable
    Temp_CR1 |= 1 << 6;
    //=================================

    //11- Init Interrupt Handler
    if(Pin_Config->SPI_IRQ_Enable != SPI_IRQ_Enable_None) //------------------>Enable NVIC in Cortex M3
    {
		if (SPIx == SPI1)
		{
			NVIC_IRQ37_SPI1_Enable;

		} else
		{
			NVIC_IRQ37_SPI2_Enable;

		}
		//                         **************************

	    if (Pin_Config->SPI_IRQ_Enable == SPI_IRQ_Enable_TX_Rx) //---->Enable TX and RX Together
	    {
	    	Temp_CR2 |= (SPI_IRQ_Enable_TX_Rx);

	    } else if (Pin_Config->SPI_IRQ_Enable == SPI_IRQ_Enable_Rx_Only) //---->Enable RX Only
	    {
	    	Temp_CR2 |= (SPI_IRQ_Enable_Rx_Only);

	    } else  //------>Enable TX Only
	    {
	    	Temp_CR2 |= (SPI_IRQ_Enable_Tx_Only);
	    }

	    //=&=&=&=&=&=&Cancel ERR Interupt=&=&=&=&=&=&
	}

    SPIx->CR2 = Temp_CR2;
    SPIx->CR1 = Temp_CR1;
}

/* ================================================================
 * @Fun 			-MCAL_SPI_Deinit
 * @brief 			-DeInit SPI
 * @param [in] 		-SPIx which is any one of SPI Instant
 * Note             -None
 */
void MCAL_SPI_Deinit (SPI_t* SPIx)
{
	if (SPIx == SPI1)
	{
		RCC->APB2RSTR |= 1 << 12;

	} else
	{
		RCC->APB1RSTR |= 1 << 14;
	}
}

/* ================================================================
 * @Fun 			-MCAL_SPI_Set_Pin
 * @brief 			-Set SPI Pins
 * @param [in] 		-SPIx which is any one of SPI Instant
 * Note             -None
 */
void MCAL_SPI_Set_Pin (SPI_t* SPIx)
{
    GPIO_Config_t Pin;

	if ( (SPIx->CR1 >> 2) & 1 ) //------------>Case it is Master
	{
		if (SPIx == SPI1)
		{
			/****************************
			 *       PA7  |  PA6        *
			 *       PA5  |  PA4        *
			 ****************************/

			//====MOSI
			Pin.GPIO_PinNumber = GPIO_PIN_7;
			Pin.GPIO_MODE = GPIO_MODE_OUTPUT_AF_PP;
			Pin.GPIO_Output_SPEED = GPIO_OUTPUT_SPEED_10M;
			MCAL_GPIO_INIT(GPIOA, &Pin);

			//=====MISO
			Pin.GPIO_PinNumber = GPIO_PIN_6;
			Pin.GPIO_MODE = GPIO_MODE_INPUT_Floating;;
			MCAL_GPIO_INIT(GPIOA, &Pin);

			//=====SCK
			Pin.GPIO_PinNumber = GPIO_PIN_5;
			Pin.GPIO_MODE = GPIO_MODE_OUTPUT_AF_PP;
			Pin.GPIO_Output_SPEED = GPIO_OUTPUT_SPEED_10M;
			MCAL_GPIO_INIT(GPIOA, &Pin);

			//=====NSS
			if (G_SPI_CFG[SPI1_Index]->NSS == SPI_NSS_Internal_Hard_Output_Dis) //------>Input Mode
			{
				Pin.GPIO_PinNumber = GPIO_PIN_4;
				Pin.GPIO_MODE = GPIO_MODE_INPUT_Floating;
				MCAL_GPIO_INIT(GPIOA, &Pin);

			} else if (G_SPI_CFG[SPI1_Index]->NSS == SPI_NSS_Internal_Hard_Output_ENR)
			{
				Pin.GPIO_PinNumber = GPIO_PIN_4;
				Pin.GPIO_MODE = GPIO_MODE_OUTPUT_AF_PP;
				Pin.GPIO_Output_SPEED = GPIO_OUTPUT_SPEED_10M;
				MCAL_GPIO_INIT(GPIOA, &Pin);
			}

		} else
		{
			/*****************************
			 *       PB15  |  PB14       *
			 *       PB13  |  PB12       *
			 *****************************/

			//====MOSI
			Pin.GPIO_PinNumber = GPIO_PIN_15;
			Pin.GPIO_MODE = GPIO_MODE_OUTPUT_AF_PP;
			Pin.GPIO_Output_SPEED = GPIO_OUTPUT_SPEED_10M;
			MCAL_GPIO_INIT(GPIOB, &Pin);

			//====MISO
			Pin.GPIO_PinNumber = GPIO_PIN_14;
			Pin.GPIO_MODE = GPIO_MODE_INPUT_Floating;
			MCAL_GPIO_INIT(GPIOB, &Pin);

			//====SCK
			Pin.GPIO_PinNumber = GPIO_PIN_13;
			Pin.GPIO_MODE = GPIO_MODE_OUTPUT_AF_PP;
			Pin.GPIO_Output_SPEED = GPIO_OUTPUT_SPEED_10M;
			MCAL_GPIO_INIT(GPIOB, &Pin);

			//====NSS
			if (G_SPI_CFG[SPI2_Index]->NSS == SPI_NSS_Internal_Hard_Output_Dis) //------>Input Mode
			{
				Pin.GPIO_PinNumber = GPIO_PIN_12;
				Pin.GPIO_MODE = GPIO_MODE_INPUT_Floating;
				MCAL_GPIO_INIT(GPIOB, &Pin);

			} else
			{
				Pin.GPIO_PinNumber = GPIO_PIN_12;
				Pin.GPIO_MODE = GPIO_MODE_OUTPUT_AF_PP;
				Pin.GPIO_Output_SPEED = GPIO_OUTPUT_SPEED_10M;
				MCAL_GPIO_INIT(GPIOB, &Pin);
			}
		}
	} else //---------->Case it is Slave
	{
		if (SPIx == SPI1)
		{
			/****************************
			 *       PA7  |  PA6        *
			 *       PA5  |  PA4        *
			 ****************************/

			//====MOSI
			Pin.GPIO_PinNumber = GPIO_PIN_7;
			Pin.GPIO_MODE = GPIO_MODE_INPUT_Floating;
			MCAL_GPIO_INIT(GPIOA, &Pin);

			//====MISO
			Pin.GPIO_PinNumber = GPIO_PIN_6;
			Pin.GPIO_MODE = GPIO_MODE_OUTPUT_AF_PP;
			Pin.GPIO_Output_SPEED = GPIO_OUTPUT_SPEED_10M;
			MCAL_GPIO_INIT(GPIOA, &Pin);

			//====SCK
			Pin.GPIO_PinNumber = GPIO_PIN_5;
			Pin.GPIO_MODE = GPIO_MODE_INPUT_Floating;
			MCAL_GPIO_INIT(GPIOA, &Pin);

			//====NSS
			if (G_SPI_CFG[SPI1_Index]->NSS == SPI_NSS_Hard_Slave)
			{
				Pin.GPIO_PinNumber = GPIO_PIN_4;
				Pin.GPIO_MODE = GPIO_MODE_INPUT_Floating;
				MCAL_GPIO_INIT(GPIOA, &Pin);
			}

		} else
		{
			/*****************************
			 *       PB15  |  PB14       *
			 *       PB13  |  PB12       *
			 *****************************/

			//====MOSI
			Pin.GPIO_PinNumber = GPIO_PIN_15;
			Pin.GPIO_MODE = GPIO_MODE_INPUT_Floating;
			MCAL_GPIO_INIT(GPIOB, &Pin);

			//====MISO
			Pin.GPIO_PinNumber = GPIO_PIN_14;
			Pin.GPIO_MODE = GPIO_MODE_OUTPUT_AF_PP;
			Pin.GPIO_Output_SPEED = GPIO_OUTPUT_SPEED_10M;
			MCAL_GPIO_INIT(GPIOB, &Pin);

			//====SCK
			Pin.GPIO_PinNumber = GPIO_PIN_13;
			Pin.GPIO_MODE = GPIO_MODE_INPUT_Floating;
			MCAL_GPIO_INIT(GPIOB, &Pin);

			//====NSS
			if (G_SPI_CFG[SPI1_Index]->NSS == SPI_NSS_Hard_Slave)
			{
				Pin.GPIO_PinNumber = GPIO_PIN_12;
				Pin.GPIO_MODE = GPIO_MODE_INPUT_Floating;
				MCAL_GPIO_INIT(GPIOB, &Pin);
		    }
       	}
	}
}

/*=======================================================
 * @Fn				-MCAL_SPI_Send
 * @brief			-Send Data from UART
 * @param [in]		-UART_Instance
 * @param [in]		-Pointer to UART Buffer
 * @param [in]		-Enum for (Polling - Interrupt)
 * @retval			-None
 * Note
 */
void MCAL_SPI_Send (SPI_t* SPIx, uint16_t* Tx_Buffer, enum Polling_Mechansim Polling)
{
	if (Polling == Enable)
	{
		//Loop Until Buffer get Empty
		while ( !( (SPIx->SR >> 1) & 1) );
	}

	//Write in Buffer
	SPIx->DR = *Tx_Buffer;
}

/*=======================================================
 * @Fn				-MCAL_SPI_Receive
 * @brief			-Send Data from UART
 * @param [in]		-UART_Instance
 * @param [in]		-Pointer to UART Buffer
 * @param [in]		-Enum for (Polling - Interrupt)
 * @retval			-None
 * Note
 */
void MCAL_SPI_Receive (SPI_t* SPIx, uint16_t* Rx_Buffer, enum Polling_Mechansim Polling)
{
	if (Polling == Enable)
	{
		//Loop until Receive Buffer
		while ( !( (SPIx->SR) & 1 ) );
	}

	//Receive Data
	*Rx_Buffer = SPIx->DR;

}

/*=======================================================
 * @Fn				-MCAL_SPI_TX_RX
 * @brief			-Send Data from UART
 * @param [in]		-UART_Instance
 * @param [in]		-Pointer to UART Buffer
 * @param [in]		-Enum for (Polling - Interrupt)
 * @retval			-None
 * Note
 */
void MCAL_SPI_TX_RX (SPI_t* SPIx, uint16_t* Tx_Buffer, uint16_t* Rx_Buffer, enum Polling_Mechansim Polling)
{
	//==============Transmitter Action
	if (Polling == Enable)
	{
		//Loop Until Buffer get Empty
		while ( !( (SPIx->SR >> 1) & 1) );
	}

	//Write in Buffer
	SPIx->DR = *Tx_Buffer;

	//=============Receive Action
	if (Polling == Enable)
	{
		//Loop until Receive Buffer
		while ( !( (SPIx->SR) & 1 ) );
	}

	//Receive Data
	*Rx_Buffer = SPIx->DR;

}

/*                                                 ==========================
 *=================================================*    Interrupt Handler   *===================================================
 *                                                 ==========================
 */

//===============================<SPI1_IRQHandler Function>
void SPI1_IRQHandler ()
{
	struct S_SPI_IRQ SPI_IRQ;

	SPI_IRQ.RXE = ( (SPI1->SR) & 1);
	SPI_IRQ.TXE = ( (SPI1->SR >> 1) & 1 );

	//Todo ERR Interrupt
	G_SPI_CFG[SPI1_Index]->CallBack(SPI_IRQ);

}

//===============================<SPI2_IRQHandler Function>
void SPI2_IRQHandler ()
{
	struct S_SPI_IRQ SPI_IRQ;

	SPI_IRQ.RXE = ( (SPI2->SR) & 1);
	SPI_IRQ.TXE = ( (SPI2->SR >> 1) & 1 );

	//Todo ERR Interrupt
	G_SPI_CFG[SPI2_Index]->CallBack(SPI_IRQ);

}

