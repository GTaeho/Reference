#include "platform_config.h"

#ifndef __INTERRUPT
#define __INTERRUPT

typedef enum
{
	DISABLE = 0,
	ENABLE = 1
}FunctionalState;

typedef enum
{
	EXTI_Mode_Interrupt = 0x00,
	EXTI_Mode_Event = 0x04
}EXTIMode_TypeDef;

typedef enum
{
	EXTI_Trigger_Rising = 0x08,
	EXTI_Trigger_Falling = 0x0C,
	EXTI_Trigger_Rising_Falling = 0x10
}EXTITrigger_TypeDef;


typedef enum IRQn
{
	
	/* Cortex-M3 Processor Exceptions Numbers */
	NonMaskableInt_IRQn = -14,
	MemoryManagement_IRQn = -12,
	BusFault_IRQn = -11,
	UsageFault_IRQn = -10,
	SVCall_IRQn = -5,
	DebugMonitor_IRQn = -4,
	PendSV_IRQn = -2,
	SysTick_IRQn = -1,
	
	/* STM32 specific Interrupt Numbers */
	WWDG_IRQn = 0,
	PVD_IRQn = 1,
	TAMPER_IRQn = 2,
	RTC_IRQn = 3,
	FLASH_IRQn = 4,
	RCC_IRQn = 5,
	EXTI0_IRQn = 6, 
	EXTI1_IRQn = 7,
	EXTI2_IRQn = 8,
	EXTI3_IRQn = 9,
	EXTI4_IRQn = 10,
	DMA1_Channel1_IRQn = 11,
	DMA1_Channel2_IRQn = 12,
	DMA1_Channel3_IRQn = 13,
	DMA1_Channel4_IRQn = 14,
	DMA1_Channel5_IRQn = 15,
	DMA1_Channel6_IRQn = 16,
	DMA1_Channel7_IRQn = 17,
	ACD1_2_IRQn = 18,
	USB_HP_CAN1_TX_IRQn = 19,
	USB_LP_CAN1_RX0_IRQn = 20,
	CAN1_RX1_IRQn = 21,
	CAN1_SCE_IRQn = 22,
	EXTI9_5_IRQn = 23,
	TIM1_BRK_IRQn = 24,
	TIM1_UP_IRQn = 25,
	TIM1_TRG_IRQn = 26,
	TIM1_CC_IRQn = 27,
	TIM2_IRQn = 28,
	TIM3_IRQn = 29,
	TIM4_IRQn = 30,
	I2C1_EV_IRQn = 31,
	I2C1_ER_IRQn = 32,
	I2C2_EV_IRQn = 33,
	I2C2_ER_IRQn = 34,
	SPI1_IRQn = 35,
	SPI2_IRQn = 36,
	USART1_IRQn = 37,
	USART2_IRQn = 38,
	USART3_IRQn = 39,
	EXTI15_10_IRQn = 40,
	RTCAlarm_IRQn = 41,
	USBWakeUp_IRQn = 42,	
}IRQn_Type;

typedef struct
{
	volatile uint32_t CTRL; 
	volatile uint32_t LOAD;
	volatile uint32_t VAL;
	__I uint32_t CALIB;
}SysTick_Type;


typedef struct
{
	__I uint32_t CPUID;
	volatile uint32_t ICSR;
	volatile uint32_t VTOR;
	volatile uint32_t AIRCR;
	volatile uint32_t SCR;
	volatile uint32_t CCR;
	volatile uint8_t SHP[12];
	volatile uint32_t SHCSR;
	volatile uint32_t CFSR;
	volatile uint32_t HFSR;
	volatile uint32_t DFSR;
	volatile uint32_t MMFAR;
	volatile uint32_t BFAR;
	volatile uint32_t AFSR;
	__I uint32_t PFR[2];
	__I uint32_t DFR;
	__I uint32_t ADR;
	__I uint32_t MMFR[4];
	__I uint32_t ISAR[5];
}SCB_Type;

typedef struct
{
	volatile uint32_t ISER[8];
	uint32_t RESERVED0[24];
	volatile uint32_t ICER[8];
	uint32_t RESERVED1[24];
	volatile uint32_t ISPR[8];
	uint32_t RESERVED2[24];
	volatile uint32_t ICPR[8];
	uint32_t RESERVED3[24];
	volatile uint32_t IABR[8];
	uint32_t RESERVED4[56];
	volatile uint32_t IP[240];
	uint32_t RESERVED5[644];
	uint32_t STIR;
}NVIC_Type;

typedef struct
{
	uint8_t NVIC_IRQChannel;
	uint8_t NVIC_IRQChannelPreemptionPriority;
	uint8_t NVIC_IRQChannelSubPriority;
	FunctionalState NVIC_IRQChannelCmd;
}NVIC_InitTypeDef;

typedef struct
{
	volatile uint32_t EVCR;
	volatile uint32_t MAPR;
	volatile uint32_t EXTICR[4];
}AFIO_TypeDef;

typedef struct
{
	volatile uint32_t IMR;
	volatile uint32_t EMR;
	volatile uint32_t RTSR;
	volatile uint32_t FTSR;
	volatile uint32_t SWIER;
	volatile uint32_t PR;	
}EXTI_TypeDef;

typedef struct
{
	uint32_t EXTI_Line;
	EXTIMode_TypeDef EXTI_Mode;
	EXTITrigger_TypeDef EXTI_Trigger;
	FunctionalState EXTI_LineCmd;
}EXTI_InitTypeDef;


#define SCS_BASE										(0xE000E000)

#define SysTick_BASE								(SCS_BASE + 0x0010)
#define NVIC_BASE										(SCS_BASE + 0x0100)
#define SCB_BASE										(SCS_BASE + 0x0D00)
#define AFIO_BASE										(0x40010000)
#define EXTI_BASE										(APB2PERIPH_BASE + 0x0400)

#define SysTick											((SysTick_Type *) SysTick_BASE)
#define NVIC												((NVIC_Type *) NVIC_BASE)
#define SCB													((SCB_Type *) SCB_BASE)
#define AFIO												((AFIO_TypeDef *) AFIO_BASE)
#define EXTI												((EXTI_TypeDef *) EXTI_BASE)


#define NVIC_VectTab_RAM						((uint32_t)0x20000000)
#define NVIC_VectTab_FLASH					((uint32_t)0x08000000)

#define SYSTICK_ENABLE			0
#define SYSTICK_TICKINT			1
#define SYSTICK_CLKSOURCE		2

#define __NVIC_PRIO_BITS		4

#define SYSTICK_MAXCOUNT						((1<<24) -1)

#define NVIC_PriorityGroup_0				((uint32_t)0x700)
#define AIRCR_VECTKEY_MASK   				((uint32_t)0x05FA0000)

#define GPIO_PortSourceGPIOA					((uint8_t)0x00)
#define GPIO_PortSourceGPIOB					((uint8_t)0x01)
#define GPIO_PortSourceGPIOC					((uint8_t)0x02)
#define GPIO_PortSourceGPIOD					((uint8_t)0x03)
#define GPIO_PortSourceGPIOE					((uint8_t)0x04)
#define GPIO_PortSourceGPIOF					((uint8_t)0x05)
#define GPIO_PortSourceGPIOG					((uint8_t)0x06)

#define GPIO_PinSource0								((uint8_t)0x00)
#define GPIO_PinSource1								((uint8_t)0x01)
#define GPIO_PinSource2								((uint8_t)0x02)
#define GPIO_PinSource3								((uint8_t)0x03)
#define GPIO_PinSource4								((uint8_t)0x04)
#define GPIO_PinSource5								((uint8_t)0x05)
#define GPIO_PinSource6								((uint8_t)0x06)
#define GPIO_PinSource7								((uint8_t)0x07)
#define GPIO_PinSource8								((uint8_t)0x08)
#define GPIO_PinSource9								((uint8_t)0x09)
#define GPIO_PinSource10							((uint8_t)0x0A)
#define GPIO_PinSource11							((uint8_t)0x0B)
#define GPIO_PinSource12							((uint8_t)0x0C)
#define GPIO_PinSource13							((uint8_t)0x0D)
#define GPIO_PinSource14							((uint8_t)0x0E)
#define GPIO_PinSource15							((uint8_t)0x0F)

#define GPIO_PORTSOURCE_KEY						GPIO_PortSourceGPIOA
#define GPIO_PINSOURCE_KEY1						GPIO_PinSource0
#define GPIO_PINSOURCE_KEY2						GPIO_PinSource1
#define GPIO_PINSOURCE_KEY3						GPIO_PinSource2



#define EXTI_Line0			((uint32_t)0x00001)
#define EXTI_Line1      ((uint32_t)0x00002)
#define EXTI_Line2      ((uint32_t)0x00004)
#define EXTI_Line3      ((uint32_t)0x00008)
#define EXTI_Line4      ((uint32_t)0x00010)
#define EXTI_Line5      ((uint32_t)0x00020)
#define EXTI_Line6      ((uint32_t)0x00040)
#define EXTI_Line7      ((uint32_t)0x00080)
#define EXTI_Line8      ((uint32_t)0x00100)
#define EXTI_Line9      ((uint32_t)0x00200)
#define EXTI_Line10     ((uint32_t)0x00400)
#define EXTI_Line11     ((uint32_t)0x00800)
#define EXTI_Line12     ((uint32_t)0x01000)
#define EXTI_Line13     ((uint32_t)0x02000)
#define EXTI_Line14     ((uint32_t)0x04000)
#define EXTI_Line15     ((uint32_t)0x08000)

#define GPIO_EXTI_Line_KEY1		EXTI_Line0
#define GPIO_EXTI_Line_KEY2		EXTI_Line1
#define GPIO_EXTI_Line_KEY3		EXTI_Line2

void NVIC_SetVectorTable(uint32_t NVIC_VectTab, uint32_t Offset);
void NVIC_Configuration(void);
uint32_t SysTick_Config(uint32_t ticks);
void NVIC_SetPriority(IRQn_Type IRQn, uint32_t priority);
void SysTick_Handler(void);
void Delay(volatile uint32_t nTime);
void TimingDelay_Decrement(void);
void delay_1_second(void);
void delay_100_milli_second(void);
void NVIC_Init(NVIC_InitTypeDef* NVIC_InitStruct);
void NVIC_PriorityGroupConfig(uint32_t NVIC_PriorityGroup);
void EXTI0_IRQHandler(void);
void EXTI1_IRQHandler(void);
void GPIO_EXTILineConfig(uint8_t GPIO_PortSource, uint8_t GPIO_PinSource);
void EXTI_Init(EXTI_InitTypeDef* EXTI_InitStruct);
FlagStatus EXTI_GetITStatus(uint32_t EXTI_Line);
void EXTI_ClearITPendingBit(uint32_t EXTI_Line);
void EXTI_Configuration(void);


#endif