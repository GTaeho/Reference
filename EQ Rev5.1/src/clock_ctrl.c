#include "platform_config.h"

void RCC_Configuration(void) // 72MHz 클록 RCC 설정
{
	uint32_t HSEStatus;
	uint16_t StartUpCounter;
	
	/* Enable HSE */
	RCC->CR |= ((uint32_t)RCC_CR_HSEON);
	
	/* Wait HSE is ready */
	do{
		HSEStatus = RCC->CR & RCC_CR_HSERDY;
		StartUpCounter++;
	}while((HSEStatus == 0)&&(StartUpCounter != HSEStartUp_TimeOut));
	
	/* Flash 2 wait state */	
	FLASH->ACR |= (uint32_t)FLASH_ACR_LATENCY_2;
	
	/* 프리스케일 분주 설정 */
	RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;
	RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;
	RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV2;
	
	/* PLLMUL 배수 설정 + PLL 소스 설정 */
	RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC_HSE | RCC_CFGR_PLLMULL6);
	
	/* PLL ON */
	RCC->CR |= RCC_CR_PLLON;
	
	/* Wait PLL is ready */
	while((RCC->CR & RCC_CR_PLLRDY) == 0){;}
	
	/* 시스템 클락 소스 설정 */
	RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;
	while((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)0x08){;}
}