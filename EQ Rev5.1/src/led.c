#include "platform_config.h"
#include "interrupt.h"

void LED_On_Red (void)
{
	GPIO_LED->BRR |= GPIO_LED1_PIN;	
}

void LED_On_Yellow (void)
{
	GPIO_LED->BRR |= GPIO_LED2_PIN;	
}

void LED_On_Blue (void)
{
	GPIO_LED->BRR |= GPIO_LED3_PIN;	
}

void LED_Off_Red (void)
{
	GPIO_LED->BSRR |= GPIO_LED1_PIN;	
}

void LED_Off_Yellow (void)
{
	GPIO_LED->BSRR |= GPIO_LED2_PIN;	
}

void LED_Off_Blue (void)
{
	GPIO_LED->BSRR |= GPIO_LED3_PIN;	
}

void LED_On_All (void)
{
	LED_On_Red();
	LED_On_Yellow();
	LED_On_Blue();
}

void LED_Off_All (void)
{
	LED_Off_Red();
	LED_Off_Yellow();
	LED_Off_Blue();	
}

void LED_OnOffAll_Mult(uint32_t count)
{
	for(; count > 0; count --)
	{
		LED_Off_Red();
		LED_On_Yellow();
		LED_On_Blue();
		delay_1_second();
		
		LED_On_Red();
		LED_Off_Yellow();
		LED_On_Blue();
		delay_1_second();
		
		LED_On_Red();
		LED_On_Yellow();
		LED_Off_Blue();
		delay_1_second();
	}
}

void LED_Test (void)
{
		LED_On_All();
		delay_1_second();
		LED_Off_All();
		delay_1_second();
		LED_On_All();
    delay_1_second();
		LED_OnOffAll_Mult(5);
	
}
