/**
  ******************************************************************************
  * @file    System / main.c
  * @author  P14345
  * @version V1.0.0
  * @date    10/9/14
  * @brief   Program body
  ******************************************************************************
  */ 

/*----------------------------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/


	#include "main.h"




/** @addtogroup STM32F3_Discovery_SD_Projects
  * @{
  */

/** @addtogroup System
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/

	ADC_InitTypeDef						ADC_InitStructure;
	ADC_CommonInitTypeDef 		ADC_CommonInitStructure;
	
	DAC_InitTypeDef   				DAC_InitStructure;
	
	GPIO_InitTypeDef       		GPIO_InitStructure;
	
	TIM_TimeBaseInitTypeDef  	TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  				TIM_OCInitStructure;
	NVIC_InitTypeDef 					NVIC_InitStructure;

	USART_InitTypeDef 				USART1_InitStructure;
	
	
/* Private define ------------------------------------------------------------*/

	//DAC data addresses
	#define DAC_DHR12R2_ADDRESS      0x40007414
	#define DAC_DHR8R1_ADDRESS       0x40007410
	
	
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
	static const double ratioMin = 1;
	static const double ratioMax = 20;
	static const double releaseMin = 0.1;
	static const double releaseMax = 1.2;
	static const double mugMin = -5;
	static const double mugMax = 12;
	
	
/* Global Variables ----------------------------------------------------------*/

			double 			threshold = -10;
			double 			attack = 15;
			double 			ratio = 5;
			double 			release = 0.6;
			double 			mug = 0;
			
			int 				autoEN = 0;
			int 				bypassEN = 0;
	
			int 				Compress = 0;
			int					Att = 0;
			int					Rel = 0;
			
/* Private variables ---------------------------------------------------------*/


/* Timer CC Register values */
__IO 	uint16_t 		CCR1_TIM3_Val = 1;
__IO 	uint16_t 		CCR1_TIM2_Val = 1;


/* ADC Calibration value */
__IO 	uint16_t  	calibration_value = 0;
__IO 	uint16_t  	TimingDelay = 0;

			uint16_t 		Data = 0;
			
	
/* Private function prototypes -----------------------------------------------*/

	void ADC_Config(void);
	void DAC_Config(void);
	
	void INPUT_Config(void);
	
	void TIM3_Config(void);
	void TIM2_Config(void);
	
	void UART_Config(void);
	
	void DisplayLine( int line, char* array );
	void Display(USART_TypeDef* USARTx, uint16_t Data);
	
	void UI_hl( void );
	int Rotary( void );
	
	char* DoubleToChar ( double );
	
	
/* Private functions ---------------------------------------------------------*/

//extern	log2( );

/*----------------------------------------------------------------------------*/






/*----------------------------------------------------------------------------*/
/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
/*----------------------------------------------------------------------------*/	




int main(void)
{
 
	INPUT_Config();
	
	TIM3_Config();
	TIM2_Config();
	
	ADC_Config();
	DAC_Config();
	
	UART_Config();
	
	/* Initialize Leds mounted on STM32F3-Discovery EVAL board */
  STM_EVAL_LEDInit(LED6);
	STM_EVAL_LEDInit(LED8);
	
  /* Turn on LED6 and LED8 */
  STM_EVAL_LEDOn(LED6);
	STM_EVAL_LEDOn(LED8);
	
	
	/* TIM2 enable counter */
  TIM_Cmd(TIM2, ENABLE);
	
	
	while(1)
	{
		UI_hl();
	}
	
} //end main






/*----------------------------------------------------------------------------*/
/**
  * @brief  ADC Configuration.
  * @param  None
  * @retval None
  */
/*----------------------------------------------------------------------------*/

void ADC_Config( void )
{
	
	RCC_ADCCLKConfig(RCC_ADC12PLLCLK_Div2);
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_ADC12, ENABLE);
	

	/* GPIOA Periph clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

  /* Configure ADC Channel7 as analog input */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
	ADC_SelectCalibrationMode(ADC1, ADC_CalibrationMode_Single);
  ADC_StartCalibration(ADC1);
  
  while(ADC_GetCalibrationStatus(ADC1) != RESET );
  calibration_value = ADC_GetCalibrationValue(ADC1);
     
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;                                                                    
  ADC_CommonInitStructure.ADC_Clock = ADC_Clock_AsynClkMode;                    
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;             
  ADC_CommonInitStructure.ADC_DMAMode = ADC_DMAMode_OneShot;                  
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = 0;          
  ADC_CommonInit(ADC1, &ADC_CommonInitStructure);
  
  ADC_InitStructure.ADC_ContinuousConvMode = ADC_ContinuousConvMode_Enable;
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b; 
  ADC_InitStructure.ADC_ExternalTrigConvEvent = ADC_ExternalTrigConvEvent_0;         
  ADC_InitStructure.ADC_ExternalTrigEventEdge = ADC_ExternalTrigEventEdge_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_OverrunMode = ADC_OverrunMode_Disable;   
  ADC_InitStructure.ADC_AutoInjMode = ADC_AutoInjec_Disable;  
  ADC_InitStructure.ADC_NbrOfRegChannel = 1;
  ADC_Init(ADC1, &ADC_InitStructure);
	
	/* ADC1 regular channel1 configuration */ 
  ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_7Cycles5);
   
  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);
  
  /* wait for ADRDY */
  while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_RDY));
  
  /* Start ADC1 Software Conversion */ 
  ADC_StartConversion(ADC1);
	
	
} //end ADC_Config






/*----------------------------------------------------------------------------*/
/**
  * @brief  DAC Configuration.
  * @param  None
  * @retval None
  */
/*----------------------------------------------------------------------------*/

void DAC_Config( void )
{
	
  /* DAC Periph clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

  /* GPIOA clock enable */
  //RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  
  /* Configure PA.04 (DAC_OUT1) as analog */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* DAC channel1 Configuration */
  DAC_DeInit(); 
  DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;
  DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;
  /* DAC Channel1 Init */
  DAC_Init(DAC_Channel_1, &DAC_InitStructure);
        
  /* Enable DAC Channel1 */
  DAC_Cmd(DAC_Channel_1, ENABLE);

} //end DAC_Config






/*----------------------------------------------------------------------------*/
/**
  * @brief  Input Configuration.
  * @param  None
  * @retval None
  */
/*----------------------------------------------------------------------------*/

void INPUT_Config(void)
{
	
	/* GPIOE Periph clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);

  /* Configure PE8, PE9, and PE10 in input pulldown mode */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	
  GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	
} //end IO_config






/*----------------------------------------------------------------------------*/
/**
  * @brief  TIM3 Configuration.
  * @param  None
  * @retval None
  */
/*----------------------------------------------------------------------------*/

void TIM3_Config(void)
{

	/*
	*	TIM3 with 10kHz freq
	*/
	
  /* TIM3 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

  /* Enable the TIM3 and TIM2 gloabal Interrupts */
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);


  
  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 1;
  TIM_TimeBaseStructure.TIM_Prescaler = 3599;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);


  /* Output Compare Timing Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR1_TIM3_Val;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);
	
  TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Disable);
	


	/* TIM Interrupts enable */
	TIM_ITConfig(TIM3, TIM_IT_CC1, ENABLE);
	
	/* TIM3 enable counter */
  //TIM_Cmd(TIM3, ENABLE);

	
} //end TIM3_Config






/*----------------------------------------------------------------------------*/
/**
  * @brief  TIM2 Configuration.
  * @param  None
  * @retval None
  */
/*----------------------------------------------------------------------------*/

void TIM2_Config(void)
{

	/*
	*	TIM2 with 88kHz freq, PreScale = 408 + 1, Period = 1 + 1
	*
	*	TIM2 with 44kHz freq, PreScale = 817 + 1, Period = 1 + 1
	*/
	
	/* TIM2 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	

  /* Enable the TIM3 and TIM2 gloabal Interrupts */
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  
  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 1;
  TIM_TimeBaseStructure.TIM_Prescaler = 1799;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);


  /* Output Compare Timing Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR1_TIM2_Val;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	
  TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Disable);
	


	/* TIM Interrupts enable */
	TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);

	
} //end TIM2_Config






/*----------------------------------------------------------------------------*/
/**
  * @brief  USART Configuration.
  * @param  None
  * @retval None
  */
/*----------------------------------------------------------------------------*/

void UART_Config(void)
{

	/* Enable USART1 */
	
	
	/* GPIOC Periph clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
	
	/* AF Config */
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource4, GPIO_AF_7);
	
	
	/* Enable USART4 Tx Pin */
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
	
	
	/* Initialize Display */
	DisplayLine ( 0, "                    " );
	DisplayLine ( 1, "                    " );
	DisplayLine ( 2, "                    " );
	DisplayLine ( 3, "                    " );
	
	
} //end USART_Config






/*----------------------------------------------------------------------------*/
/**
  * @brief  Delay function.
  * @param  None
  * @retval None
  */
/*----------------------------------------------------------------------------*/

void Delay(__IO uint32_t nTime)
{ 
  TimingDelay = nTime;

  while(TimingDelay != 0);
	
} //end Delay






/*----------------------------------------------------------------------------*/
/**
  * @brief  DisplayLine function.
  * @param  None
  * @retval None
  */
/*----------------------------------------------------------------------------*/

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






/*----------------------------------------------------------------------------*/
/**
  * @brief  Display function.
  * @param  None
  * @retval None
  */
/*----------------------------------------------------------------------------*/

void Display(USART_TypeDef* USARTx, uint16_t Data)
{
	//Check if USART still busy
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
	{
	}
	
	//Send data
	USART_SendData(USARTx, Data);
	
} //end Display function






/*----------------------------------------------------------------------------*/
/**
  * @brief  UI HighLevel function.
  * @param  None
  * @retval None
  */
/*----------------------------------------------------------------------------*/

void UI_hl(void)
{

	int currentLevel = 1;
	int level1CurrentSelection = 0;
	int level2CurrentSelection = 0;

	
	int buttonFlag = 0;
	int rotaryPressFlag = 0;
	int rotaryFlag = 0;
	
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
					switch (level1CurrentSelection)
					{
						case 0:
							level1CurrentSelection = 1;
							DisplayLine ( 0, "      Automode      " );
							break;
						case 1:
							level1CurrentSelection = 2;
							DisplayLine ( 0, "       Bypass       " );
							break;
						case 2:
							level1CurrentSelection = 0;
							DisplayLine ( 0, "       Manual       " );
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
							break;
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
							break;							
					}
					break;
					
					
					
				case 3:
					switch ( level2CurrentSelection )
					{
						case 0:
							if ( threshold == thresholdMax )
								break;
							threshold += thresholdStep;
							DisplayLine ( 2, DoubleToChar( threshold ) );
							break;
						case 1:
							if ( mug == mugMax )
								break;
							mug += mugStep;
							DisplayLine ( 2, DoubleToChar( mug ) );
							break;
						case 2:
							if ( attack == attackMax)
								break;
							attack += attackStep;
							DisplayLine ( 2, DoubleToChar( attack ) );
							break;
						case 3:
							if ( release == releaseMax )
								break;
							release += releaseStep;
							DisplayLine ( 2, DoubleToChar( release ) );
							break;
						case 4:
							if ( ratio == ratioMax )
								break;
							ratio += ratioStep;
							DisplayLine ( 2, DoubleToChar( ratio ) );
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
					switch ( level1CurrentSelection )
					{
						case 0:
							level1CurrentSelection = 2;
							DisplayLine ( 0, "       Bypass       " );
							break;
						case 1:
							level1CurrentSelection = 0;
							DisplayLine ( 0, "       Manual       " );
							break;
						case 2:
							level1CurrentSelection = 1;
							DisplayLine (0, "      Automode      " );
							
							break;
					}
					break;
				case 2:
					switch ( level1CurrentSelection )
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
							break;
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
							break;
					}
					break;
				case 3:
					switch ( level2CurrentSelection )
					{
						case 0:
							if ( threshold == thresholdMin )
								break;
							threshold -= thresholdStep;
							DisplayLine ( 2, DoubleToChar( threshold ) );
							break;
						case 1:
							if ( mug == mugMin)
								break;
							mug -= mugStep;
							DisplayLine ( 2, DoubleToChar( mug ) );
							break;
						case 2:
							if ( attack == attackMin )
								break;
							attack -= attackStep;
							DisplayLine ( 2, DoubleToChar( attack ) );
							break;
						case 3:
							if ( release == releaseMin )
								break;
							release -= releaseStep;
							DisplayLine ( 2, DoubleToChar( release ) );
							break;
						case 4:
							if ( ratio == ratioMin )
								break;
							ratio -= ratioStep;
							DisplayLine ( 2, DoubleToChar( ratio ) );
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
						if ( level1CurrentSelection == 0 )
							currentLevel ++;
						break;
					case 3:
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






/*----------------------------------------------------------------------------*/
/**
  * @brief  RotaryEncoder function.
  * @param  None
  * @retval None
  */
/*----------------------------------------------------------------------------*/

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






/*----------------------------------------------------------------------------*/
/**
  * @brief  Double to Char function.
  * @param  None
  * @retval None
  */
/*----------------------------------------------------------------------------*/

char* DoubleToChar ( double in )
{
	if ( in == -20 )
		return "       -20.0        ";
	else if ( in == -19.5 )
		return "       -19.5        ";
	else if ( in == -19 )
		return "       -19.0        ";
	else if ( in == -18.5 )
		return "       -18.5        ";
	else if ( in == -18 )
		return "       -18.0        ";
	else if ( in == -17.5 )
		return "       -17.5        ";
	else if ( in == -17 )
		return "       -17.0        ";
	else if ( in == -16.5 )
		return "       -16.5        ";
	else if ( in == -16 )
		return "       -16.0        ";
	else if ( in == -15.5 )
		return "       -15.5        ";
	else if ( in == -15 )
		return "       -15.0        ";
	else if ( in == -14.5 )
		return "       -14.5        ";
	else if ( in == -14 )
		return "       -14.0        ";
	else if ( in == -13.5 )
		return "       -13.5        ";
	else if ( in == -13 )
		return "       -13.0        ";
	else if ( in == -12.5 )
		return "       -12.5        ";
	else if ( in == -12 )
		return "       -12.0        ";
	else if ( in == -11.5 )
		return "       -11.5        ";
	else if ( in == -11 )
		return "       -11.0        ";
	else if ( in == -10.5 )
		return "       -10.5        ";
	else if ( in == -10 )
		return "       -10.0        ";
	else if ( in == -9.5 )
		return "        -9.5        ";
	else if ( in == -9 )
		return "        -9.0        ";
	else if ( in == -8.5 )
		return "        -8.5        ";
	else if ( in == -8 )
		return "        -8.0        ";
	else if ( in == -7.5 )
		return "        -7.5        ";
	else if ( in == -7 )
		return "        -7.0        ";
	else if ( in == -6.5 )
		return "        -6.5        ";
	else if ( in == -6 )
		return "        -6.0        ";
	else if ( in == -5.5 )
		return "        -5.5        ";
	else if ( in == -5 )
		return "        -5.0        ";
	else if ( in == -4.5 )
		return "        -4.5        ";
	else if ( in == -4 )
		return "        -4.0        ";
	else if ( in == -3.5 )
		return "        -3.5        ";
	else if ( in == -3 )
		return "        -3.0        ";
	else if ( in == -2.5 )
		return "        -2.5        ";
	else if ( in == -2 )
		return "        -2.0        ";
	else if ( in == -1.5 )
		return "        -1.5        ";
	else if ( in == -1 )
		return "        -1.0        ";
	else if ( in == -0.5 )
		return "        -0.5        ";
	else if ( in == 0 )
		return "         0.0        ";
	else if ( in == 0.5 )
		return "         0.5        ";
	else if ( in == 1 )
		return "         1.0        ";
	else if ( in == 1.5 )
		return "         1.5        ";
	else if ( in == 2 )
		return "         2.0        ";
	else if ( in == 2.5 )
		return "         2.5        ";
	else if ( in == 3 )
		return "         3.0        ";
	else if ( in == 3.5 )
		return "         3.5        ";
	else if ( in == 4 )
		return "         4.0        ";
	else if ( in == 4.5 )
		return "         4.5        ";
	else if ( in == 5 )
		return "         5.0        ";
	else if ( in == 5.5 )
		return "         5.5        ";
	else if ( in == 6 )
		return "         6.0        ";
	else if ( in == 6.5 )
		return "         6.5        ";
	else if ( in == 7 )
		return "         7.0        ";
	else if ( in == 7.5 )
		return "         7.5        ";
	else if ( in == 8 )
		return "         8.0        ";
	else if ( in == 8.5 )
		return "         8.5        ";
	else if ( in == 9 )
		return "         9.0        ";
	else if ( in == 9.5 )
		return "         9.5        ";
	else if ( in == 10 )
		return "        10.0        ";
	else if ( in == 10.5 )
		return "        10.5        ";
	else if ( in == 11 )
		return "        11.0        ";
	else if ( in == 11.5 )
		return "        11.5        ";	
	else if ( in == 12 )
		return "        12.0        ";
	else if ( in == 12.5 )
		return "        12.5        ";
	else if ( in == 13 )
		return "        13.0        ";
	else if ( in == 13.5 )
		return "        13.5        ";
	else if ( in == 14 )
		return "        14.0        ";
	else if ( in == 14.5 )
		return "        14.5        ";
	else if ( in == 15 )
		return "        15.0        ";
	else if ( in == 15.5 )
		return "        15.5        ";
	else if ( in == 16 )
		return "        16.0        ";
	else if ( in == 16.5 )
		return "        16.5        ";
	else if ( in == 17 )
		return "        17.0        ";
	else if ( in == 17.5 )
		return "        17.5        ";
	else if ( in == 18 )
		return "        18.0        ";
	else if ( in == 18.5 )
		return "        18.5        ";
	else if ( in == 19 )
		return "        19.0        ";
	else if ( in == 19.5 )
		return "        19.5        ";
	else if ( in == 20 )
		return "        20.0        ";
	else if ( in == 20.5 )
		return "        20.5        ";
	else if ( in == 21 )
		return "        21.0        ";
	else if ( in == 21.5 )
		return "        21.5        ";
	else if ( in == 22 )
		return "        22.0        ";
	else if ( in == 22.5 )
		return "        22.5        ";
	else if ( in == 23 )
		return "        23.0        ";
	else if ( in == 23.5 )
		return "        23.5        ";
	else if ( in == 24 )
		return "        24.0        ";
	else if ( in == 24.5 )
		return "        24.5        ";
	else if ( in == 25 )
		return "        25.0        ";
	else if ( in == 25.5 )
		return "        25.5        ";
	else if ( in == 26 )
		return "        26.0        ";
	else if ( in == 26.5 )
		return "        26.5        ";
	else if ( in == 27 )
		return "        27.0        ";
	else if ( in == 27.5 )
		return "        27.5        ";
	else if ( in == 28 )
		return "        28.0        ";
	else if ( in == 28.5 )
		return "        28.5        ";
	else if ( in == 29 )
		return "        29.0        ";
	else if ( in == 29.5 )
		return "        29.5        ";
	else if ( in == 30 )
		return "        30.0        ";
	else if ( in == 0.15 )
		return "        0.15        ";
	else if ( in == 0.25 )
		return "        0.25        ";
	else if ( in == 0.35 )
		return "        0.35        ";
	else if ( in == 0.45 )
		return "        0.45        ";
	else if ( in == 0.55 )
		return "        0.55        ";
	else if ( in == 0.65 )
		return "        0.65        ";
	else if ( in == 0.75 )
		return "        0.75        ";
	else if ( in == 0.85 )
		return "        0.85        ";
	else if ( in == 0.95 )
		return "        0.95        ";
	else if ( in == 1.05 )
		return "        1.05        ";
	else if ( in == 1.15 )
		return "        1.15        ";
	return "                     ";
}






/*----------------------------------------------------------------------------*/
/**
  * @brief  assert_failed function.
  * @param  None
  * @retval None
  */
/*----------------------------------------------------------------------------*/

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
