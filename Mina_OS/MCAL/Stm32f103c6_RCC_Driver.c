/*
 * Stm32f103c6_RCC_Driver.c
 *
 *  Created on: Sep 17, 2023
 *      Author: HP
 */

//------------------------------------------------------<Include>
#include "Stm32f103c6_RCC_Driver.h"

//                          *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//--------------------------*                 Generic variables                     *-------------------------------
//                          *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*

//Bits 10:8 PPRE1: APB low-speed prescaler (APB1)
//Set and cleared by software to control the division factor of the APB low-speed clock (PCLK1).
//Warning: the software has to set correctly these bits to not exceed 36 MHz on this domain.

                   //0xx: HCLK not divided
//100: HCLK divided by 2          //101: HCLK divided by 4
//110: HCLK divided by 8          //111: HCLK divided by 16
const uint8_t APBPrescTable[8U] = {0, 0, 0, 0, 1, 2, 3, 4};

//Bits 7:4 HPRE: AHB prescaler
//Set and cleared by software to control the division factor of the AHB clock.

                  //0xxx: SYSCLK not divided
//1000: SYSCLK divided by 2       //1001: SYSCLK divided by 4
//1010: SYSCLK divided by 8       //1011: SYSCLK divided by 16
//1100: SYSCLK divided by 64      //1101: SYSCLK divided by 128
//1110: SYSCLK divided by 256     //1111: SYSCLK divided by 512
const uint8_t AHBPrescTable[16U] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3 ,4 ,6 ,7 ,8 ,9};

/*                                             ============================
 *============================================*     Function Implement    *============================================
 *                                             ============================
 */

//===============================<MCAL_RCC_GetSYS_CLCKFreq Function>
uint32_t MCAL_RCC_GetSYS_CLCKFreq (void)
{
	//Bits 1:0 SW: System clock switch
	//Set and cleared by software to select SYSCLK source.
	//Set by hardware to force HSI selection when leaving Stop and Standby mode or in case of
	//failure of the HSE oscillator used directly or indirectly as system clock (if the Clock SecuritySystem is enabled).

	//00: HSI selected as system clock      //01: HSE selected as system clock
	//10: PLL selected as system clock      //11: not allowed
    switch( (RCC->CFGR >> 2) & 0b11 )
    {
    case 0:
    	return HSI_RC_Clk;
    	break;

    case 1: //HSE User should Specify it
    	return HSE_Clk;
    	break;

    case 2: //Todo need to calculate it PLLCLK and PLLMUL & PLL source Mux
    	return 16000000;
    	break;
    }
return 0;
}

//===============================<MCAL_RCC_GetHCLKFreq Function>
uint32_t MCAL_RCC_GetHCLKFreq (void)
{
	return (MCAL_RCC_GetSYS_CLCKFreq() >> APBPrescTable[(RCC->CFGR >> 4) & 0xf]);

}

//===============================<MCAL_RCC_GetPCLK1Freq Function>
uint32_t MCAL_RCC_GetPCLK1Freq (void)
{
	//Bits 10:8 PPRE1: APB low-speed prescaler (APB1)
	return (MCAL_RCC_GetHCLKFreq() >> AHBPrescTable[(RCC->CFGR >> 8) & 0b111]);

}

//===============================<MCAL_RCC_GetPCLK2Freq Function>
uint32_t MCAL_RCC_GetPCLK2Freq (void)
{
	//Bits 13:11 PPRE2: APB high-speed prescaler (APB2)
	return (MCAL_RCC_GetHCLKFreq() >> APBPrescTable[(RCC->CFGR >> 11) & 0b111]);
}
