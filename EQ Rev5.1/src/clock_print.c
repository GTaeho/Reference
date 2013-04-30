#include "clock_print.h"

static __I uint8_t APBAHBPrescTable[16] = {0,0,0,0,1,2,3,4,1,2,3,4,6,7,8,9};
static __I uint8_t ADCPrescTable[4] = {2,4,6,8};


void RCC_GetClocksFreq(RCC_ClocksTypeDef* RCC_Clocks)
{
	uint32_t tmp = 0, pllmull = 0, pllsource = 0, presc = 0;
	
	/* SYSCLK_Frequency 구하기 */
	tmp = RCC->CFGR & CFGR_SWS_Mask;
	
	switch(tmp)
	{
		case 0x00 :
			RCC_Clocks->SYSCLK_Frequency = HSI_Value;
			break;
		
		case 0x04 :
			RCC_Clocks->SYSCLK_Frequency = HSE_Value;
			break;
			
		case 0x08 :
			pllmull = RCC->CFGR & CFGR_PLLMull_Mask;
			pllsource = RCC->CFGR & CFGR_PLLSRC_Mask;
			
			pllmull = (pllmull >> 18) + 2;
			if(pllsource == 0x00)
			{
				RCC_Clocks->SYSCLK_Frequency = (HSI_Value >> 1) * pllmull;
			}
			else
			{
				if((RCC->CFGR & CFGR_PLLXTPRE_Mask) != (uint32_t)RESET)
				{
					RCC_Clocks->SYSCLK_Frequency = (HSE_Value >> 1) * pllmull;
				}
				else
				{
					RCC_Clocks->SYSCLK_Frequency = HSE_Value * pllmull;
				}
			}
			break;	
	}

	/* HCLK_Frequency 구하기 */
	tmp = RCC->CFGR & CFGR_HPRE_Set_Mask;
	tmp = tmp >> 4;
	presc = APBAHBPrescTable[tmp];
	RCC_Clocks->HCLK_Frequency = RCC_Clocks->SYSCLK_Frequency >> presc;
	
	/* PCLK1_Frequency 구하기 */
	tmp = RCC->CFGR & CFGR_PPRE1_Set_Mask;
	tmp = tmp >> 8;
	presc = APBAHBPrescTable[tmp];
	RCC_Clocks->PCLK1_Frequency = RCC_Clocks->HCLK_Frequency >> presc;
	
	/* PCLK2_Frequency 구하기 */
	tmp = RCC->CFGR & CFGR_PPRE2_Set_Mask;
	tmp = tmp >> 11;
	presc = APBAHBPrescTable[tmp];
	RCC_Clocks->PCLK2_Frequency = RCC_Clocks->HCLK_Frequency >> presc;
	
	/* ADCCLK_Frequency 구하기 */
	tmp = RCC->CFGR & CFGR_ADCPRE_Set_Mask;
	tmp = tmp >> 14;
	presc = ADCPrescTable[tmp];
	RCC_Clocks->ADCCLK_Frequency = RCC_Clocks->PCLK2_Frequency / presc;
	

}
