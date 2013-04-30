#include "interrupt.h"

static volatile uint32_t TimingDelay;
extern int mode;
extern int index,start,flag_uart,tmp_start;
extern int data_x[150], data_y[150], data_z[150];
extern int fsync_cnt;

void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	//벡터테이블 플래쉬 롬으로 셋팅
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
	//인터럽트 우선순위 그룹 설정
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	
	//EXTI 설정
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//EXTI 설정
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

//=================== NVIC 관련 함수들================================
//벡터테이블 설정하는 함수
void NVIC_SetVectorTable(uint32_t NVIC_VectTab, uint32_t Offset)
{
	SCB->VTOR = NVIC_VectTab | (Offset & (uint32_t)0x1FFFFF80);
}
//SBC->AIRCR의 그룹 우선순위를 설정해줌
void NVIC_PriorityGroupConfig(uint32_t NVIC_PriorityGroup)
{
	SCB->AIRCR = AIRCR_VECTKEY_MASK | NVIC_PriorityGroup;
}

//인터럽트(EXTI) 우선순위 설정 group priority, sub priority
void NVIC_Init(NVIC_InitTypeDef* NVIC_InitStruct)
{
	uint32_t tmppriority = 0x00, tmppre = 0x00, tmpsub = 0x0F;
	
	if (NVIC_InitStruct->NVIC_IRQChannelCmd != DISABLE)
	{
		tmppriority = (0x700 - ((SCB->AIRCR) & (uint32_t)0x700)) >> 0x08;
		tmppre = (0x4 - tmppriority);
		tmpsub = tmpsub >> tmppriority;
		
		tmppriority = (uint32_t)NVIC_InitStruct->NVIC_IRQChannelPreemptionPriority << tmppre;
		tmppriority |= NVIC_InitStruct->NVIC_IRQChannelSubPriority & tmpsub;
		tmppriority = tmppriority << 0x04;
		
		NVIC->IP[NVIC_InitStruct->NVIC_IRQChannel] = tmppriority;
		
		NVIC->ISER[NVIC_InitStruct->NVIC_IRQChannel >> 0x05] = (uint32_t)0x01 << (NVIC_InitStruct->NVIC_IRQChannel & (uint8_t)0x1F);
	}
	else
	{
		NVIC->ICER[NVIC_InitStruct->NVIC_IRQChannel >> 0x05] = (uint32_t)0x01 << (NVIC_InitStruct->NVIC_IRQChannel & (uint8_t)0x1F);
	}
	
}

//System Timer 설정
uint32_t SysTick_Config(uint32_t ticks)
{
	if (ticks > SYSTICK_MAXCOUNT) return (1);
	
	SysTick->LOAD = (ticks & SYSTICK_MAXCOUNT) - 1;
	NVIC_SetPriority (SysTick_IRQn, (1 << __NVIC_PRIO_BITS)-1);
	SysTick->VAL = (0x00);
	SysTick->CTRL = (1 << SYSTICK_CLKSOURCE) | (1<<SYSTICK_ENABLE) | (1<<SYSTICK_TICKINT);
	return (0);	
}

//System Timer 인터럽트 우선순위 정할때 쓴 함수
void NVIC_SetPriority(IRQn_Type IRQn, uint32_t priority)
{
	if(IRQn < 0)
	{
		SCB->SHP[((uint32_t)(IRQn) & 0xF) -4] = ((priority << (8 - __NVIC_PRIO_BITS)) & 0xff);
	}
	else
	{
		NVIC->IP[(uint32_t)(IRQn)] = ((priority << (8 - __NVIC_PRIO_BITS)) & 0xff);
	}
}

//System Timer 인터럽트
void SysTick_Handler(void)
{
	TimingDelay_Decrement();
}

void Delay(volatile uint32_t nTime)
{
	TimingDelay = nTime;
	
	while(TimingDelay !=0);
}

void TimingDelay_Decrement(void)
{
	if(TimingDelay != 0x00)
	{
		TimingDelay--;
	}
}

void delay_1_second(void)
{
	Delay(1000000);
}

void delay_100_milli_second(void)
{
	Delay(100000);
}


//EXTI 인터럽트
//PA0 -> Mode select
void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(GPIO_EXTI_Line_KEY1)!=RESET)
	{
		EXTI_ClearITPendingBit(GPIO_EXTI_Line_KEY1);
//		printf("Left-WKUP Button Press\n");
		if(mode==3)
			mode=0;
		else
			mode=mode+1;
	}	
}

//PA1 -> F_SYNC : 100Hz
void EXTI1_IRQHandler(void)
{
	if(EXTI_GetITStatus(GPIO_EXTI_Line_KEY2)!=RESET)
	{
		EXTI_ClearITPendingBit(GPIO_EXTI_Line_KEY2);
		if(index>98)		
		{
			index=0;
			if(start==99)
				start=0;
			else
				start=start+1;
		}
		
	}	
}

//PA2 -> F_SCLK : 10KHz
void EXTI2_IRQHandler(void)
{
	if(EXTI_GetITStatus(GPIO_EXTI_Line_KEY3)!=RESET)
	{
		EXTI_ClearITPendingBit(GPIO_EXTI_Line_KEY3);
		if(index<14) {	
			data_x[start] = ((data_x[start]<<1) & 0x3FFF);
			data_y[start] = ((data_y[start]<<1) & 0x3FFF);
			data_z[start] = ((data_z[start]<<1) & 0x3FFF);

			// read x, y, z data
			if(GPIOB->IDR & 0x00000020)	// PB5, X data
				data_x[start]|=1;
			if(GPIOB->IDR & 0x00000001)	// PB0, Y data
				data_y[start]|=1;
			if(GPIOB->IDR & 0x00000002)	// PB1, Z data
				data_z[start]|=1;
			flag_uart=0;
		}
		else
			flag_uart=1;
		/*
		if(index<14)
		{
			if(GPIOA->IDR & 0x00000008)
				SerialPutChar('1');
			else
				SerialPutChar('0');
		}
		else if(index==14)
		{
			SerialPutChar(10);
		}
		else if(index==15)
		{	
			SerialPutChar(13);
		}	
		*/
		index++;
	}	
}

void EXTI_Configuration(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	
	GPIO_EXTILineConfig(GPIO_PORTSOURCE_KEY, GPIO_PINSOURCE_KEY1);
	
	EXTI_InitStructure.EXTI_Line    = GPIO_EXTI_Line_KEY1;
  EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;    
	EXTI_InitStructure.EXTI_LineCmd	= ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PORTSOURCE_KEY, GPIO_PINSOURCE_KEY2);
	
	EXTI_InitStructure.EXTI_Line    = GPIO_EXTI_Line_KEY2;
  EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd	= ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PORTSOURCE_KEY, GPIO_PINSOURCE_KEY3);
	
	EXTI_InitStructure.EXTI_Line    = GPIO_EXTI_Line_KEY3;
  EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd	= ENABLE;
	EXTI_Init(&EXTI_InitStructure);
}

//AFIO를 EXTI 사용하도록 설정
void GPIO_EXTILineConfig(uint8_t GPIO_PortSource, uint8_t GPIO_PinSource)
{
	uint32_t tmp = 0x00;
	
	tmp = ((uint32_t)0x0F) << (0x04 * (GPIO_PinSource & (uint8_t)0x03));
	AFIO->EXTICR[GPIO_PinSource >> 0x02] &= ~tmp;
	AFIO->EXTICR[GPIO_PinSource >> 0x02] |= (((uint32_t)GPIO_PortSource) << (0x04 * (GPIO_PinSource & (uint8_t)0x03)));
}


//EXTI 인터럽트, 폴링 라이징 설정
void EXTI_Init(EXTI_InitTypeDef* EXTI_InitStruct)
{
  uint32_t tmp = 0;

  tmp = (uint32_t)EXTI_BASE;
     
  if (EXTI_InitStruct->EXTI_LineCmd != DISABLE)
  {
    /* Clear EXTI line configuration */
    EXTI->IMR &= ~EXTI_InitStruct->EXTI_Line;
    EXTI->EMR &= ~EXTI_InitStruct->EXTI_Line;
    
    tmp += EXTI_InitStruct->EXTI_Mode;

    *(volatile uint32_t *) tmp |= EXTI_InitStruct->EXTI_Line;

    /* Clear Rising Falling edge configuration */
    EXTI->RTSR &= ~EXTI_InitStruct->EXTI_Line;
    EXTI->FTSR &= ~EXTI_InitStruct->EXTI_Line;
    
    /* Select the trigger for the selected external interrupts */
    if (EXTI_InitStruct->EXTI_Trigger == EXTI_Trigger_Rising_Falling)
    {
      /* Rising Falling edge */
      EXTI->RTSR |= EXTI_InitStruct->EXTI_Line;
      EXTI->FTSR |= EXTI_InitStruct->EXTI_Line;
    }
    else
    {
      tmp = (uint32_t)EXTI_BASE;
      tmp += EXTI_InitStruct->EXTI_Trigger;

      *(volatile uint32_t *) tmp |= EXTI_InitStruct->EXTI_Line;
    }
  }
  else
  {
    tmp += EXTI_InitStruct->EXTI_Mode;

    /* Disable the selected external lines */
    *(volatile uint32_t *) tmp &= ~EXTI_InitStruct->EXTI_Line;
  }
}

FlagStatus EXTI_GetITStatus(uint32_t EXTI_Line)
{
	FlagStatus bitstatus = RESET;
	uint32_t enablestatus = 0;
	
	enablestatus = EXTI->IMR & EXTI_Line;
	if(((EXTI->PR & EXTI_Line)!=(uint32_t)RESET) && (enablestatus!=(uint32_t)RESET))
	{
		bitstatus = SET;
	}
	else
	{
		bitstatus = RESET;
	}
	
	return bitstatus;
}

void EXTI_ClearITPendingBit(uint32_t EXTI_Line)
{
	EXTI->PR = EXTI_Line;	
}