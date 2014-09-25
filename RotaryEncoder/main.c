/**
  ******************************************************************************
  * @file    RotaryEncoder/main.c
  * @author  Jeffrey Auclair
  * @version V1.0.0
  * @date    9/11/14
  * @brief   Main program body
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/

#include "main.h"

/** @addtogroup STM32F3_Discovery_SD_Projects
  * @{
  */

/** @addtogroup RotaryEncoder
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

	
		uint8_t RotaryPosition = 1;
		uint8_t CW;
		uint8_t CCW;
		GPIO_InitTypeDef        GPIO_InitStructure;
		
		

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
	
	
	/* GPIOE Periph clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);
  
  /* Configure PE8, PE9, and PE10 in input pushpull mode */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	/*
	 * GPIO_Pin_8  = CW
	 * GPIO_Pin_9  = CCW
	 * GPIO_Pin_10 = Button
	 */
	 
	
	/* Configure PE7 in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	/*
	 * GPIO_Pin_7  = VDD
	 */
	
	
	//Set Pin E7 to VDD
		GPIOE->BSRR = 0x0080;



  /* Initialize Leds mounted on STM32F3-Discovery EVAL board */
  STM_EVAL_LEDInit(LED6);
	STM_EVAL_LEDInit(LED7);
  /* Turn on LED3, LED4, LED5 and LED6 */
  STM_EVAL_LEDOff(LED6);
	STM_EVAL_LEDOff(LED7);
	
	
	
	
	while (1)
  {
		
		
		CW = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_8);
		
		CCW = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_9);
		
		
		//Want to only accept data if one is zero and other goes from low to high
		//Change occurs when value != old value
		//Old value should be equal to 0, current value should be high ( greater than 0 )
		if( CW > 0 )
		{
			
			while( CW > 0 )
			{
				CW = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_8);
				CCW = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_9);
	
				if( CCW > 0 )
				{
					if( RotaryPosition < 10 )
					{
						RotaryPosition++;
					}
					
					STM_EVAL_LEDToggle(LED7);
				}
			}
		}
		
		if( CCW > 0 )
		{
			
			while( CCW > 0 )
			{
				CW = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_8);
				CCW = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_9);
	
				if( CW > 0 )
				{
					if( RotaryPosition > 0 )
					{
						RotaryPosition--;
					}
					
					STM_EVAL_LEDToggle(LED6);
				}
			}
		}
		
		
		/*
		if( CW > 0 )
		{
			if( RotaryPosition < 10 )
			{
				RotaryPosition++;
			}
			
			STM_EVAL_LEDToggle(LED6);
			
			while( CW > 0 || CCW > 0 )
			{
				CW = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_8);
		
				CCW = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_9);
			}
			
		}
		
		if( CCW > 0 )
		{
			if( RotaryPosition > 0 )
			{
				RotaryPosition--;
			}
			STM_EVAL_LEDToggle(LED7);
			
			while( CW > 0 || CCW > 0 )
			{
				CW = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_8);
		
				CCW = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_9);
			}
			
		}
		*/
  } //end while
	
} //end main







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
