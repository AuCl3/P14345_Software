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
/* Private constants ---------------------------------------------------------*/

	static const double thresholdStep = 0.5;
	static const double attackStep = 0.5;
	static const double ratioStep = 1;
	static const double releaseStep = 0.05;
	static const double mugStep = 0.5;
	
	static const double thresholdMax = 15;
	static const double thresholdMin = -20;
	static const double attackMin = 0.1;
	static const double attackMax = 30;
	static const double ratioMin = 0.1;
	static const double ratioMax = 20;
	static const double releaseMin = 0.1;
	static const double releaseMax = 1.2;
	static const double mugMin = -5;
	static const double mugMax = 12;
	
	
/* Global Variables ----------------------------------------------------------*/

	double threshold = 0;
	double attack = 15;
	double ratio = 10;
	double release = 0.6;
	double mug = 0;
	int autoEN = 0;
	int bypassEN = 0;
	
	
/* Private variables ---------------------------------------------------------*/



	GPIO_InitTypeDef        GPIO_InitStructure;
	USART_InitTypeDef 			USART1_InitStructure;
	
/* Private function prototypes -----------------------------------------------*/

	int Rotary( void );
	void Display(USART_TypeDef*, uint16_t );
	void DisplayLine( int , char* );
	void UI_hl( void );
	
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
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource4, GPIO_AF_7);
	
	
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
  USART1_InitStructure.USART_Mode = USART_Mode_Tx;
  USART_Init(USART1, &USART1_InitStructure);
	
	/* Turn on USART1 */
	USART_Cmd(USART1,ENABLE);
	
	
	
	
	//Enter UI forever
	
	UI_hl();
		
	
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
		
	return OUT;
		
} //end Rotary Function








void Display(USART_TypeDef* USARTx, uint16_t Data)
{
	//Check if USART still busy
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
	{
	}
	
	//Send data
	USART_SendData(USARTx, Data);
	
} //end Display function







void DisplayLine( int line, char* array )
{
	
	uint8_t arraysize = 20;
	uint8_t point = 0;
	
	
	//First, set the line to 1, 2, 3, or 4
	
	 if( line == 0 )
	 {
			Display(USART1, 0xFE); //Command
			Display(USART1, 0x80); //Move to position 0 ( 0x80 + 0x00 )
	 }
	 
	 if( line == 1 )
	 {
			Display(USART1, 0xFE); //Command
			Display(USART1, 0xC0); //Move to position 64 ( 0x80 + 0x40 = 0xC0 )
	 }
	 
	 if( line == 2 )
	 {
			Display(USART1, 0xFE); //Command
			Display(USART1, 0x94); //Move to position 20 ( 0x80 + 0x14 = 0x94 )
	 }
	 
	 
	 
	 //Set array to the line
	 
	 while( point < arraysize )
	 {
		
			Display(USART1, array[point] ); 
	
			point++;
		 
	 } //end while
	
} //end DisplayLine Function




void UI_hl(void)
{

	int currentLevel = 1;
	int level1CurrentSelection = 0;
	int level2CurrentSelection = 0;

	
	int buttonFlag = 0;
	int rotaryPressFlag = 0;
	int rotaryFlag = 0;
	
	char* strLine;
	
	DisplayLine ( 0, "       Manual       " );
	
	while ( 1 )
	{
		switch ( Rotary () )
		{
		case 0:			// No change
			rotaryFlag = 0;
			break;
		case 1:			// Clockwise
			if ( rotaryFlag == 1 )
				break;
			rotaryFlag = 1;
			switch ( currentLevel )
			{
				case 1:
					level1CurrentSelection ++;
					switch (level1CurrentSelection)
					{
						case 0:
							DisplayLine ( 0, "       Manual       " );
							break;
						case 1:
							DisplayLine ( 0, "      Automode      " );
							break;
						case 2:
							DisplayLine ( 0, "       Bypass       " );
							break;
					}
					break;
				case 2:
					switch (level1CurrentSelection)
					{
						case 0:
							if ( level2CurrentSelection == 4 )
								level2CurrentSelection = 0;
							else
								level2CurrentSelection ++;
							switch ( level2CurrentSelection )
							{
								case 0:
									DisplayLine ( 1, "      Threshold     " );
									break;
								case 1:
									DisplayLine ( 1, "     MakeUp Gain    " );
									break;
								case 2:
									DisplayLine ( 1, "       Attack       " );
									break;
								case 3:
									DisplayLine ( 1, "      Release       " );
									break;
								case 4:
									DisplayLine ( 1, "       Ratio        " );
									break;
							}
							break;
						case 1:
							if ( autoEN == 1 )
							{
								autoEN = 0;
								DisplayLine ( 1, "      Disabled      " );
							}
							else
							{
								autoEN = 1;
								DisplayLine ( 1, "      Enabled       " );
							}	
						case 2:
							if ( bypassEN == 1 )
							{
								bypassEN = 0;
								DisplayLine ( 1, "      Disabled      " );
							}
							else
							{
								bypassEN = 1;
								DisplayLine ( 1, "      Enabled       " );
							}	
					}
					break;
				case 3:
					switch ( level2CurrentSelection )
					{
						case 0:
							if ( threshold == thresholdMax )
								break;
							threshold += thresholdStep;
							sprintf ( strLine, "%g", threshold );
							DisplayLine ( 2, strLine);
							break;
						case 1:
							if ( mug == mugMax )
								break;
							mug += mugStep;
							sprintf ( strLine, "%g", mug );
							DisplayLine ( 2, strLine);
							break;
						case 2:
							if ( attack == attackMax)
								break;
							attack += attackStep;
							sprintf ( strLine, "%g", attack );
							DisplayLine ( 2, strLine);
							break;
						case 3:
							if ( release == releaseMax )
								break;
							release += releaseStep;
							sprintf ( strLine, "%g", release );
							DisplayLine ( 2, strLine );
							break;
						case 4:
							if ( ratio == ratioMax )
								break;
							ratio += ratioStep;
							sprintf ( strLine, "%g", ratio );
							DisplayLine ( 2, strLine );
							break;
						default:
							break;
					}
				case 4:
					// No level 4, yet
					break;
				default:
					break;
			}
		case 2:			// Counter-Clockwise
			if ( rotaryFlag == 1 )
				break;
			rotaryFlag = 1;
			switch ( currentLevel )
			{
				case 1:
					level1CurrentSelection --;
					switch ( level1CurrentSelection )
					{
						case 0:
							DisplayLine ( 0, "       Manual       " );
							break;
						case 1:
							DisplayLine (0, "      Automode      " );
							break;
						case 2:
							DisplayLine ( 0, "       Bypass       " );
							break;
					}
					break;
				case 2:
					switch ( level2CurrentSelection )
					{
						case 0:
							if ( level2CurrentSelection == 0 )
								level2CurrentSelection = 4;
							else
								level2CurrentSelection --;
							switch ( level2CurrentSelection )
							{
								case 0:
									DisplayLine ( 1, "      Threshold     " );
									break;
								case 1:
									DisplayLine ( 1, "     MakeUp Gain    " );
									break;
								case 2:
									DisplayLine ( 1, "       Attack       " );
									break;
								case 3:
									DisplayLine ( 1, "      Release       " );
									break;
								case 4:
									DisplayLine ( 1, "       Ratio        " );
									break;
							}
						case 1:
							if ( autoEN == 1 )
							{
								autoEN = 0;
								DisplayLine ( 1, "      Disabled      " );
							}
							else
							{
								autoEN = 1;
								DisplayLine ( 1, "      Enabled       " );
							}	
						case 2:
							if ( bypassEN == 1 )
							{
								bypassEN = 0;
								DisplayLine ( 1, "      Disabled      " );
							}
							else
							{
								bypassEN = 1;
								DisplayLine ( 1, "      Enabled       " );
							}	
					}
					break;
				case 3:
					switch ( level2CurrentSelection )
					{
						case 0:
							if ( threshold == thresholdMin )
								break;
							threshold -= thresholdStep;
							sprintf ( strLine, "%g", threshold );
							DisplayLine ( 2, strLine );
							break;
						case 1:
							if ( mug == mugMin)
								break;
							mug -= mugStep;
							sprintf ( strLine, "%g", mug );
							DisplayLine ( 2, strLine );
							break;
						case 2:
							if ( attack == attackMin )
								break;
							attack -= attackStep;
							sprintf ( strLine, "%g", attack );
							DisplayLine ( 2, strLine );
							break;
						case 3:
							if ( release == releaseMin )
								break;
							release -= releaseStep;
							sprintf ( strLine, "%g", release );
							DisplayLine ( 2, strLine );
							break;
						case 4:
							if ( ratio == ratioMin )
								break;
							ratio -= ratioStep;
							sprintf ( strLine, "%g", ratio );
							break;
						default:
							break;
					}
				case 4:
					// No level 4, yet
					break;
				default:
					break;
			}
		default:
			break;
		}
		
		
		/* Rotary Button code */
		
		//If flag is low and INPUT = HIGH, flag = HIGH
		if ( rotaryPressFlag == 0 )
		{
			if( GPIO_ReadInputDataBit( GPIOE, GPIO_Pin_9 ) > 0 )
			{
				rotaryPressFlag = 1;
				switch ( currentLevel )
				{
					case 1:
						currentLevel ++;
						switch ( level1CurrentSelection )
						{
							case 0:
								DisplayLine ( 1, "     Threshold      " );
								level2CurrentSelection = 0;
								break;
							case 1:
								DisplayLine ( 1, "      Disabled      " );
								break;
							case 2:
								DisplayLine ( 1, "      Disabled      " );
								break;
						}
						break;
					case 2:
						currentLevel ++;
						break;
					case 3:
						currentLevel ++;
						break;
					case 4:
						// Do Nothing
						break;
				}
			}
		}
	
		//Otherwise, if flag = HIGH, wait until INPUT = LOW to reset flag
		else
		{
			if( GPIO_ReadInputDataBit( GPIOE, GPIO_Pin_9 ) == 0 )
			{
				rotaryPressFlag = 0;
			}
		}
		
		
		/* Back Button code */
		
		//If flag is low and INPUT = HIGH, flag = HIGH
		if ( buttonFlag == 0 )
		{
			if( GPIO_ReadInputDataBit( GPIOE, GPIO_Pin_10 ) > 0 )
			{
				buttonFlag = 1;
				switch ( currentLevel )
				{
					case 1:
						// Do Nothing
						break;
					case 2:
						currentLevel-- ;
						DisplayLine ( 1, "                    " );
						if ( autoEN == 1 )
							autoEN = 0;
						if ( bypassEN == 1 )
							bypassEN = 0;
						break;
					case 3:
						currentLevel-- ;
						DisplayLine ( 2, "                    " );
						break;
					case 4:
						currentLevel-- ;
						break;
				}	
			}
		}
			
		//Otherwise, if flag = HIGH, wait until INPUT = LOW to reset flag
		else
		{
			if( GPIO_ReadInputDataBit( GPIOE, GPIO_Pin_10 ) == 0 )
			{
				buttonFlag = 0;
			}
		}
		
		
		
		
	}  //end while
} //end UI_hl



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
