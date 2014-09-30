/**
  ******************************************************************************
  * @file    blank.c
  * @author  name
  * @version V1.0
  * @date    0/0/00
  * @brief   Main program body
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/

#include "main.h"

/** @addtogroup STM32F3_Discovery_SD_Projects
  * @{
  */

/** @addtogroup [program name]
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
	GPIO_InitTypeDef        GPIOC_InitStructure;
	USART_InitTypeDef 			USART1_InitStructure;
	
/* Private function prototypes -----------------------------------------------*/
	void GPIOC_Init(void);
	void USART1_Init(void);
	
	int x = 0;
/* Private functions ---------------------------------------------------------*/


/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
	
  USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
   
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
   
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource4, GPIO_AF_7);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource5, GPIO_AF_7);
   
  /* Configure USART1 pins:  Rx and Tx ----------------------------*/
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
   
  USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART1, &USART_InitStructure);
   
  USART_Cmd(USART1,ENABLE);
	 

	 
	 while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
	 {
	 }
		USART_SendData(USART1, 0xFE); //Command
	 
	 	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
	 {
	 }
		USART_SendData(USART1, 0x01); //Clear Display
	 
	 while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
	 {
	 }
		USART_SendData(USART1, 0xFE); //Command
	 
	 	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
	 {
	 }
		USART_SendData(USART1, 0x0D); //Blinking Cursor ON
	 
	 
	while(1)
	{
		 
			while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
			{
			}
			USART_SendData(USART1, 0xFE); //Command
			
			
			while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
			{
			}
			USART_SendData(USART1, 0x80); //Move to position 0 ( 0x80 + 0x00 )
		 
			
			while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
			{
			}
			USART_SendData(USART1, 'A');
	 
		 
			
			while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
			{
			}
			USART_SendData(USART1, 0xFE); //Command
			
			
			while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
			{
			}
			USART_SendData(USART1, 0xC0); //Move to position 64 ( 0x80 + 0x40 = 0xC0 ) 
			
			
			while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
			{
			}
			USART_SendData(USART1, 'B');
	 
		 
			
			while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
			{
			}
			USART_SendData(USART1, 0xFE); //Command
			
			while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
			{
			}
			USART_SendData(USART1, 0x94); //Move to position 20 ( 0x80 + 0x14 = 0x94 ) 
		
			
			while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
			{
			}
			USART_SendData(USART1, 'C');
	 
	 
	}
 
}


	





#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
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

/**
  * @}
  */ 

/**
  * @}
  */ 

/*******************************    END OF FILE    ****************************/
