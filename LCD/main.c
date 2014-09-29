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



	GPIO_InitTypeDef        GPIO_InitStructure;
	USART_InitTypeDef 			USART1_InitStructure;
	
/* Private function prototypes -----------------------------------------------*/

	int Rotary( void );
	void DisplayLine( int , char* );
	
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
 
	
	
	
	/* Enable Rotary Encoder Inputs */
	
	/* GPIOE Periph clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);

  /* Configure PE8, PE9, and PE10 in input pushpull mode */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	
	
	
	/* Enable USART1 */
	
	/* GPIOC Periph clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
	
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	/* USART1 Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	/* Configure USART1 for Rx with Baud Rate of 9600 */
	USART1_InitStructure.USART_BaudRate = 9600;
  USART1_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART1_InitStructure.USART_StopBits = USART_StopBits_1;
  USART1_InitStructure.USART_Parity = USART_Parity_No;
  USART1_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART1_InitStructure.USART_Mode = USART_Mode_Rx;
  USART_Init(USART1, &USART1_InitStructure);
	
	/* Turn on USART1 */
	USART_Cmd(USART1,ENABLE);
	
	
	
	
	while(1)
	{
		
		Rotary();
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
	} //end while

} //end main




int Rotary()
{
		uint8_t CW;
		uint8_t CCW;
		uint8_t OUT = 0;
	
		CW = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_7);
		
		CCW = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_8);
		
		
		//Want to only accept data if one is zero and other goes from low to high
		//Change occurs when value != old value
		//Old value should be equal to 0, current value should be high ( greater than 0 )
	
		if( CW > 0 )
		{
			
			while( CW > 0 )
			{
				CW = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_7);
				CCW = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_8);
	
				if( CCW > 0 )
				{
					OUT = 1;
				}
			}
		}
		
		if( CCW > 0 )
		{
			
			while( CCW > 0 )
			{
				CW = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_7);
				CCW = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_8);
	
				if( CW > 0 )
				{
					OUT = 2;
				}
					
				
			}
		}
} //end Rotary Function




void DisplayLine( int line, char* array )
{
	
	uint8_t arraysize = 20;
	uint8_t point = 0;
	
	
	//First, set the line to 1, 2, 3, or 4
	
	 if( line == 1 )
	 {
		 
			while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
			{
			}
			USART_SendData(USART1, 0xFE); //Command
			
			while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
			{
			}
			USART_SendData(USART1, 0x80); //Move to position
		 
			while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
			{
			}
			USART_SendData(USART1, 0x00); //Position of Line 1
	 }
	 
	 if( line == 2 )
	 {
		 
			while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
			{
			}
			USART_SendData(USART1, 0xFE); //Command
			
			while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
			{
			}
			USART_SendData(USART1, 0x80); //Move to position
		 
			while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
			{
			}
			USART_SendData(USART1, 0x40); //Position of Line 2
	 }
	 
	 if( line == 3 )
	 {
		 
			while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
			{
			}
			USART_SendData(USART1, 0xFE); //Command
			
			while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
			{
			}
			USART_SendData(USART1, 0x80); //Move to position
		 
			while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
			{
			}
			USART_SendData(USART1, 0x14); //Position of Line 3
	 }
	 
	 
	 
	 
	 //Set array to the line
	 
	 while( point < arraysize )
	 {
		 

			while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
			{
			}
			USART_SendData(USART1, array[point] ); //Position of Line 3
	
			
			point++;
		 
	 } //end while
	
} //end DisplayLine Function




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
