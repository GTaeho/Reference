#ifndef __PALTFORM
#define __PALTFORM

#include <stdint.h>

/* ========================열거형 구조체 선언========================== */

typedef enum
{
	GPIO_Speed_10MHz = 1,
	GPIO_Speed_2MHz,
	GPIO_Speed_50MHz
}GPIOSpeed_TypeDef;

typedef enum
{
	GPIO_Mode_AIN = 0x0,
	GPIO_Mode_IN_FLOATING = 0x04,
	GPIO_Mode_IPD = 0x28,
	GPIO_Mode_IPU = 0x48,
	GPIO_Mode_Out_OD = 0x14,
	GPIO_Mode_Out_PP = 0x10,
	GPIO_Mode_AF_OD = 0x1C,
	GPIO_Mode_AF_PP = 0x18
}GPIOMode_TypeDef;

typedef enum
{
	Bit_RESET = 0,
	Bit_SET
}BitAction;

typedef enum
{
	RESET = 0,
	SET
}FlagStatus;
/* ==================================================================== */

/* ======================구조체 선언============================= */
typedef struct
{
	volatile uint32_t CR;
	volatile uint32_t CFGR;
	volatile uint32_t CIR;
	volatile uint32_t APB2RSTR;
	volatile uint32_t APB1RSTR;
	volatile uint32_t AHBENR;
	volatile uint32_t APB2ENR;
	volatile uint32_t APB1ENR;
	volatile uint32_t BDCR;
	volatile uint32_t CSR;
}RCC_TypeDef;

typedef struct
{
	volatile uint32_t CRL;
	volatile uint32_t CRH;
	volatile uint32_t IDR;
	volatile uint32_t ODR;
	volatile uint32_t BSRR;
	volatile uint32_t BRR;
	volatile uint32_t LCKR;
}GPIO_TypeDef;

typedef struct
{
  uint32_t USART_BaudRate;            /*!< This member configures the USART communication baud rate.
                                           The baud rate is computed using the following formula:
                                            - IntegerDivider = ((PCLKx) / (16 * (USART_InitStruct->USART_BaudRate)))
                                            - FractionalDivider = ((IntegerDivider - ((u32) IntegerDivider)) * 16) + 0.5 */

  uint16_t USART_WordLength;          /*!< Specifies the number of data bits transmitted or received in a frame.
                                           This parameter can be a value of @ref USART_Word_Length */

  uint16_t USART_StopBits;            /*!< Specifies the number of stop bits transmitted.
                                           This parameter can be a value of @ref USART_Stop_Bits */

  uint16_t USART_Parity;              /*!< Specifies the parity mode.
                                           This parameter can be a value of @ref USART_Parity
                                           @note When parity is enabled, the computed parity is inserted
                                                 at the MSB position of the transmitted data (9th bit when
                                                 the word length is set to 9 data bits; 8th bit when the
                                                 word length is set to 8 data bits). */
 
  uint16_t USART_Mode;                /*!< Specifies wether the Receive or Transmit mode is enabled or disabled.
                                           This parameter can be a value of @ref USART_Mode */

  uint16_t USART_HardwareFlowControl; /*!< Specifies wether the hardware flow control mode is enabled
                                           or disabled.
                                           This parameter can be a value of @ref USART_Hardware_Flow_Control */
} USART_InitTypeDef;

typedef struct
{
	uint16_t GPIO_Pin;
	GPIOSpeed_TypeDef GPIO_Speed;
	GPIOMode_TypeDef GPIO_Mode;
}GPIO_InitTypeDef;

typedef struct
{
	volatile uint16_t SR;
	uint16_t RESERVED0;
	volatile uint16_t DR;
	uint16_t RESERVED1;
	volatile uint16_t BRR;
	uint16_t RESERVED2;
	volatile uint16_t CR1;
	uint16_t RESERVED3;
	volatile uint16_t CR2;
	uint16_t RESERVED4;
	volatile uint16_t CR3;
	uint16_t RESERVED5;
	volatile uint16_t GTPR;	
	uint16_t RESERVED6;
}USART_TypeDef;

typedef struct
{
	uint32_t ACR;
	uint32_t KEYR;
	uint32_t OPTKEYR;
	uint32_t SR;
	uint32_t CR;
	uint32_t AR;
	uint32_t RESERVED;
	uint32_t OBR;
	uint32_t WRPR;
}FLASH_TypeDef;


/* ==================================================================== */


/* ======================주소값 변수 define============================ */

#ifdef __cplusplus
#define __I volatile
#else
#define __I volatile const
#endif

#define PERIPH_BASE		((uint32_t)0x40000000)	/*	SRAM base address in the bit-band region	*/

#define APB1PERIPH_BASE		PERIPH_BASE
#define APB2PERIPH_BASE		(PERIPH_BASE + 0x10000)
#define AHBPERIPH_BASE		(PERIPH_BASE + 0x20000)

#define RCC_BASE					(AHBPERIPH_BASE + 0x1000)

#define RCC								((RCC_TypeDef *)RCC_BASE)

#define GPIOA_BASE				(APB2PERIPH_BASE + 0x0800)
#define GPIOB_BASE				(APB2PERIPH_BASE + 0x0C00)
#define GPIOC_BASE				(APB2PERIPH_BASE + 0x1000)
#define GPIOD_BASE				(APB2PERIPH_BASE + 0x1400)
#define GPIOE_BASE				(APB2PERIPH_BASE + 0x1800)
#define GPIOF_BASE				(APB2PERIPH_BASE + 0x1C00)
#define GPIOG_BASE				(APB2PERIPH_BASE + 0x2000)

#define USART1_BASE						(APB2PERIPH_BASE + 0x3800)
#define USART2_BASE						(APB1PERIPH_BASE + 0x4400)

#define FLASH_BASE				(AHBPERIPH_BASE + 0x2000)		
/* ==================================================================== */


/* ==============================주소 포인터=========================== */

#define GPIOA							((GPIO_TypeDef *)GPIOA_BASE)
#define GPIOB							((GPIO_TypeDef *)GPIOB_BASE)
#define GPIOC							((GPIO_TypeDef *)GPIOC_BASE)
#define GPIOD							((GPIO_TypeDef *)GPIOD_BASE)
#define GPIOE							((GPIO_TypeDef *)GPIOE_BASE)
#define GPIOF							((GPIO_TypeDef *)GPIOF_BASE)
#define GPIOG							((GPIO_TypeDef *)GPIOG_BASE)

#define GPIO_LED					GPIOB

#define GPIO_KEY				GPIOA

#define USART1								((USART_TypeDef *) USART1_BASE)	//USART1의 각종 환경 설정
#define USART2								((USART_TypeDef *) USART2_BASE)
#define GPIO_USART						GPIOA  //USART1의 Rx,Tx핀 설정을 위함

#define FLASH							((FLASH_TypeDef *)FLASH_BASE)
/* ==================================================================== */



/* ==================================핀 설정=========================== */

#define GPIO_Pin_0				((uint16_t)0x0001	)	/* Pin 0 selected */
#define GPIO_Pin_1				((uint16_t)0x0002	)	/* Pin 1 selected */
#define GPIO_Pin_2				((uint16_t)0x0004	)	/* Pin 2 selected */
#define GPIO_Pin_3				((uint16_t)0x0008	)	/* Pin 3 selected */
#define GPIO_Pin_4				((uint16_t)0x0010	)	/* Pin 4 selected */
#define GPIO_Pin_5				((uint16_t)0x0020	)	/* Pin 5 selected */
#define GPIO_Pin_6				((uint16_t)0x0040	)	/* Pin 6 selected */
#define GPIO_Pin_7				((uint16_t)0x0080	)	/* Pin 7 selected */
#define GPIO_Pin_8				((uint16_t)0x0100	)	/* Pin 8 selected */
#define GPIO_Pin_9				((uint16_t)0x0200	)	/* Pin 9 selected */
#define GPIO_Pin_10				((uint16_t)0x0400	)	/* Pin 10 selected */
#define GPIO_Pin_11				((uint16_t)0x0800	)	/* Pin 11 selected */
#define GPIO_Pin_12				((uint16_t)0x1000	)	/* Pin 12 selected */
#define GPIO_Pin_13				((uint16_t)0x2000	)	/* Pin 13 selected */
#define GPIO_Pin_14				((uint16_t)0x4000	)	/* Pin 14 selected */
#define GPIO_Pin_15				((uint16_t)0x8000	)	/* Pin 15 selected */
#define GPIO_Pin_All			((uint16_t)0xFFFF	)	/* All pins selected */


#define GPIO_LED1_PIN			GPIO_Pin_9 /* RED */
#define GPIO_LED2_PIN			GPIO_Pin_5 /* YELLOW */
#define GPIO_LED3_PIN			GPIO_Pin_8 /* BLUE */

#define GPIO_KEY1_PIN		GPIO_Pin_0
#define GPIO_KEY2_PIN		GPIO_Pin_1

#define RS							GPIO_Pin_0
#define RW							GPIO_Pin_1
#define EN							GPIO_Pin_2
#define CS1							GPIO_Pin_3
#define CS2							GPIO_Pin_4

//RCC_APB2ENR 레지스터의 핀 설정
#define RCC_APB2Periph_AFIO       ((uint32_t)0x00000001)
#define RCC_APB2Periph_GPIOA      ((uint32_t)0x00000004)
#define RCC_APB2Periph_GPIOB      ((uint32_t)0x00000008)
#define RCC_APB2Periph_GPIOC      ((uint32_t)0x00000010)
#define RCC_APB2Periph_GPIOD      ((uint32_t)0x00000020)
#define RCC_APB2Periph_GPIOE      ((uint32_t)0x00000040)
#define RCC_APB2Periph_GPIOF      ((uint32_t)0x00000080)
#define RCC_APB2Periph_GPIOG      ((uint32_t)0x00000100)
#define RCC_APB2Periph_ADC1       ((uint32_t)0x00000200)
#define RCC_APB2Periph_ACD2       ((uint32_t)0x00000400)
#define RCC_APB2Periph_TIM1       ((uint32_t)0x00000800)
#define RCC_APB2Periph_SPI1       ((uint32_t)0x00001000)
#define RCC_APB2Periph_TIM8       ((uint32_t)0x00002000)
#define RCC_APB2Periph_USART1     ((uint32_t)0x00004000)
#define RCC_APB2Periph_ADC3				((uint32_t)0x00008000)

#define RCC_APB1Periph_USART2			((uint32_t)0x00020000)

#define GPIO_USART_Rx_Pin			GPIO_Pin_10
#define GPIO_USART_Tx_Pin			GPIO_Pin_9

#define USART_FLAG_TXE                       ((uint16_t)0x0080)
#define USART_FLAG_RXNE											((uint16_t)0x0020)

#define USART_WordLength_8b                  ((uint16_t)0x0000)
#define USART_WordLength_9b                  ((uint16_t)0x1000)

#define USART_StopBits_1                     ((uint16_t)0x0000)
#define USART_StopBits_0_5                   ((uint16_t)0x1000)
#define USART_StopBits_2                     ((uint16_t)0x2000)
#define USART_StopBits_1_5                   ((uint16_t)0x3000)

#define USART_Parity_No                      ((uint16_t)0x0000)
#define USART_Parity_Even                    ((uint16_t)0x0400)
#define USART_Parity_Odd                     ((uint16_t)0x0600) 

#define USART_HardwareFlowControl_None       ((uint16_t)0x0000)
#define USART_HardwareFlowControl_RTS        ((uint16_t)0x0100)
#define USART_HardwareFlowControl_CTS        ((uint16_t)0x0200)
#define USART_HardwareFlowControl_RTS_CTS    ((uint16_t)0x0300)

#define USART_Mode_Rx                        ((uint16_t)0x0004)
#define USART_Mode_Tx                        ((uint16_t)0x0008)

#define CR1_UE_Set                ((uint16_t)0x2000)  /*!< USART Enable Mask */


#define RCC_CR_HSEON												((uint32_t)0x00010000)
#define RCC_CR_HSERDY												((uint32_t)0x00020000)
#define FLASH_ACR_LATENCY_2									((uint8_t)0x02)
#define RCC_CFGR_HPRE_DIV1									((uint32_t)0x00000000)
#define RCC_CFGR_PPRE2_DIV1									((uint32_t)0x00000000)
#define RCC_CFGR_PPRE1_DIV2									((uint32_t)0x00000400)
#define RCC_CFGR_PLLSRC_HSE									((uint32_t)0x00010000)
#define RCC_CFGR_PLLMULL6										((uint32_t)0x00100000)
#define RCC_CFGR_PLLMULL9										((uint32_t)0x001C0000)
#define RCC_CR_PLLON												((uint32_t)0x01000000)
#define RCC_CR_PLLRDY												((uint32_t)0x02000000)
#define RCC_CFGR_SW_PLL											((uint32_t)0x00000002)
#define RCC_CFGR_SWS												((uint32_t)0x0000000C)


/* ==================================================================== */

/* ==========================그냥 define=============================== */

#define HSI_Value                 ((uint32_t)8000000)
#define HSE_Value									((uint32_t)12000000)
#define HSEStartUp_TimeOut				((uint16_t)0x0500)

#define CR1_CLEAR_Mask            ((uint16_t)0xE9F3)  /*!< USART CR1 Mask */
#define CR2_STOP_CLEAR_Mask       ((uint16_t)0xCFFF)  /*!< USART CR2 STOP Bits Mask */
#define CR3_CLEAR_Mask            ((uint16_t)0xFCFF)  /*!< USART CR3 Mask */


/* ==================================================================== */

/* ===============================함수 선언============================ */

/*
void delay_1_second (void);
void delay_100_milli_second(void);
*/

void KEY_Test(void);
void LED_Test (void);


//LED
void LED_On_Red (void);
void LED_On_Yellow (void);
void LED_On_Blue (void);
void LED_Off_Red (void);
void LED_Off_Yellow (void);
void LED_Off_Blue (void);
void LED_On_All (void);
void LED_Off_All (void);


//USART
void USART_SendData(USART_TypeDef* USARTx, uint16_t Data);
uint16_t USART_ReceiveData(USART_TypeDef* USARTx);
FlagStatus USART_GetFlagStatus(USART_TypeDef* USARTx, uint16_t USART_FLAG);
void USART_Init(USART_TypeDef* USARTx, USART_InitTypeDef* USART_InitStruct);
void USART1_Init(void);
void SerialPutChar(uint8_t c);
void Serial_PutString(uint8_t *s);
uint8_t USART_GetCharacter(USART_TypeDef* usart_p);

void USART2_Init(void);
void SerialPutChar2(uint8_t c);


//GPIO
void GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_InitStruct);
void GPIO_Configuration(void);

void RCC_Configuration(void);

/* ==================================================================== */

//GLCD
void GLCD_command(uint16_t signal, uint16_t command);
void GLCD_data(uint16_t signal, uint16_t character);
void GLCD_clear(void);



#endif



