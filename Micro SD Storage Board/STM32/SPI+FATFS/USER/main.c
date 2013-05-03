/********************************************************************************************************
*
* File                : main.c
* Hardware Environment: 
* Build Environment   : RealView MDK-ARM  Version: 4.14
* Version             : V1.0
* By                  : xiao xian hui
*
*                                  (c) Copyright 2005-2011, WaveShare
*                                       http://www.waveShare.net
*                                          All Rights Reserved
*
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "ff.h"
#include "stm32f10x.h"
#include "SPI_MSD_Driver.h"
#include <stdio.h>
#include <string.h>

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/* Private variables ---------------------------------------------------------*/
FATFS fs;         /* Work area (file system object) for logical drive */
FIL fsrc;         /* file objects */   
FRESULT res;
UINT br;

char path[512]="0:";
uint8_t textFileBuffer[] = "Thank you for using Open107V �� \r\n";   

/* Private function prototypes -----------------------------------------------*/
int SD_TotalSize(void);
void USART_Configuration(void);
FRESULT scan_files (char* path);


/*******************************************************************************
* Function Name  : Delay
* Description    : Delay Time
* Input          : - nCount: Delay Time
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void  Delay (uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}

/*******************************************************************************
* Function Name  : main
* Description    : Main program
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
int main(void)
{
	USART_Configuration();
	MSD_SPI_Configuration();

	if( _card_insert() == 0 )
    {
	  printf("-- SD card detected OK \r\n");
    }
    else
    {
      printf("-- Please connect a SD card \r\n");
      while( _card_insert() != 0 );
      printf("-- SD card connection detected \r\n");
	  Delay(0xffffff);
    }

	f_mount(0,&fs);	

	res = f_open( &fsrc , "0:/Demo.TXT" , FA_CREATE_NEW | FA_WRITE);		

    if ( res == FR_OK )
    { 
      /* Write buffer to file */
      res = f_write(&fsrc, textFileBuffer, sizeof(textFileBuffer), &br);     
 
	  printf("Demo.TXT successfully created        \r\n");
    
      /*close file */
      f_close(&fsrc);      
    }
    else if ( res == FR_EXIST )
    {
	  printf("Demo.TXT created in the disk      \r\n");
    }

	scan_files(path);
	SD_TotalSize();

    /* Infinite loop */
    while (1){

    }
}

/*******************************************************************************
* Function Name  : scan_files
* Description    : �����ļ�Ŀ¼�������ļ�
* Input          : - path: ��Ŀ¼
* Output         : None
* Return         : FRESULT
* Attention		 : ��֧�ֳ��ļ���
*******************************************************************************/
FRESULT scan_files (char* path)
{
    FILINFO fno;
    DIR dir;
    int i;
    char *fn;
#if _USE_LFN
    static char lfn[_MAX_LFN * (_DF1S ? 2 : 1) + 1];
    fno.lfname = lfn;
    fno.lfsize = sizeof(lfn);
#endif

    res = f_opendir(&dir, path);
    if (res == FR_OK) {
        i = strlen(path);
        for (;;) {
            res = f_readdir(&dir, &fno);
            if (res != FR_OK || fno.fname[0] == 0) break;
            if (fno.fname[0] == '.') continue;
#if _USE_LFN
            fn = *fno.lfname ? fno.lfname : fno.fname;
#else
            fn = fno.fname;
#endif
            if (fno.fattrib & AM_DIR) {
                sprintf(&path[i], "/%s", fn);
                res = scan_files(path);
                if (res != FR_OK) break;
                path[i] = 0;
            } else {
                printf("%s/%s \r\n", path, fn);
            }
        }
    }

    return res;
}

/*******************************************************************************
* Function Name  : USART_Configuration
* Description    : Configure USART1 
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void USART_Configuration(void)
{ 
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure; 

  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB | RCC_APB2Periph_USART1 | RCC_APB2Periph_AFIO ,ENABLE);

  GPIO_PinRemapConfig(GPIO_Remap_USART1,ENABLE); 

  /*
  *  USART1_TX -> PB6 , USART1_RX -> PB7
  */				
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;	         
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOB, &GPIO_InitStructure);		   

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;	        
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  USART_Init(USART1, &USART_InitStructure); 
  USART_ClearFlag(USART1,USART_FLAG_TC);
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
  USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
  USART_Cmd(USART1, ENABLE);
}

/*******************************************************************************
* Function Name  : SD_TotalSize
* Description    : �ļ��ռ�ռ�����
* Input          : None
* Output         : None
* Return         : ����1�ɹ� ����0ʧ��
* Attention		 : None
*******************************************************************************/
int SD_TotalSize(void)
{
    FATFS *fs;
    DWORD fre_clust;        

    res = f_getfree("0:", &fre_clust, &fs);  /* �����Ǹ�Ŀ¼��ѡ�����0 */
    if ( res==FR_OK ) 
    {
	  /* Print free space in unit of MB (assuming 512 bytes/sector) */
      printf("\r\n%d MB total drive space.\r\n"
           "%d MB available.\r\n",
           ( (fs->n_fatent - 2) * fs->csize ) / 2 /1024 , (fre_clust * fs->csize) / 2 /1024 );
		
	  return ENABLE;
	}
	else 
	  return DISABLE;   
}	 

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(USART1, (uint8_t) ch);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
  {}

  return ch;
}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
