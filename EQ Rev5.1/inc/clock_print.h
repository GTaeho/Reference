#ifndef __CLOCK
#define __CLOCK

#include "platform_config.h"


typedef struct
{
	uint32_t SYSCLK_Frequency;
	uint32_t HCLK_Frequency;
	uint32_t PCLK1_Frequency;
	uint32_t PCLK2_Frequency;
	uint32_t ADCCLK_Frequency;
}RCC_ClocksTypeDef;

#define CFGR_SWS_Mask							((uint32_t)0x0000000C)
#define CFGR_PLLMull_Mask					((uint32_t)0x003C0000)
#define CFGR_PLLSRC_Mask					((uint32_t)0x00010000)
#define CFGR_PLLXTPRE_Mask				((uint32_t)0x00020000)
#define CFGR_HPRE_Set_Mask				((uint32_t)0x000000F0)
#define CFGR_PPRE1_Set_Mask				((uint32_t)0x00000700)
#define CFGR_PPRE2_Set_Mask				((uint32_t)0x00003800)
#define CFGR_ADCPRE_Set_Mask			((uint32_t)0x0000C000)

void RCC_GetClocksFreq(RCC_ClocksTypeDef* RCC_Clocks);

#endif