/**
  ******************************************************************************
  * @file    SystemInit / main.c
  * @author  Jeffrey Auclair
  * @version V1.0.0
  * @date    10/8/14
  * @brief   Main program body
  ******************************************************************************
  */ 



/*----------------------------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/

#include "main.h"

/** @addtogroup STM32F3_Discovery_SD_Projects
  * @{
  */

/** @addtogroup SystemInit
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
	
	GPIO_InitTypeDef       		GPIO_InitStructure;
	
	ADC_InitTypeDef						ADC_InitStructure;
	ADC_CommonInitTypeDef 		ADC_CommonInitStructure;
	DAC_InitTypeDef   				DAC_InitStructure;
	
	TIM_TimeBaseInitTypeDef  	TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  				TIM_OCInitStructure;
	NVIC_InitTypeDef 					NVIC_InitStructure;

	USART_InitTypeDef 				UART4_InitStructure;
	
	
/* Private define ------------------------------------------------------------*/

	//DAC data addresses
	#define DAC_DHR12R2_ADDRESS      0x40007414
	#define DAC_DHR8R1_ADDRESS       0x40007410
	
	
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

__IO 	uint16_t 		CCR1_TIM2_Val = 1;
__IO 	uint16_t 		CCR1_TIM3_Val = 1;
			
__IO 	uint16_t  	ADC1ConvertedValue = 0;
__IO 	uint16_t  	ADC1ConvertedVoltage = 0;
__IO 	uint16_t  	calibration_value = 0;
__IO 	uint32_t 		TimingDelay = 0;

			uint16_t 		Data = 0;
			
			
/* Private function prototypes -----------------------------------------------*/

	void ADC_Config(void);
	void DAC_Config(void);
	
	void INPUT_Config(void);
	
	void TIM2_Config(void);
	void TIM3_Config(void);
	
	void UART_Config(void);
	
	void DisplayLine( int line, char* array );
	void Display(USART_TypeDef* USARTx, uint16_t Data);
	
	
/* Private functions ---------------------------------------------------------*/

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
	
	/* Configure all Peripherals */
	
	INPUT_Config();
	
	TIM2_Config();
	TIM3_Config();
	
	ADC_Config();
	DAC_Config();
	
	UART_Config();
	
	
} //end main






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
  * @brief  TIM2 Configuration.
  * @param  None
  * @retval None
  */
/*----------------------------------------------------------------------------*/

void TIM2_Config(void)
{

  /* TIM2 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);


  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 0;
  TIM_TimeBaseStructure.TIM_Prescaler = 359;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	

  /* Output Compare Timing Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR1_TIM2_Val;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

  TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Disable);
	
	
	/* Enable the TIM2 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	/* TIM Interrupts enable */
 TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);
	
	
} //end TIM2_Config






/*----------------------------------------------------------------------------*/
/**
  * @brief  TIM3 Configuration.
  * @param  None
  * @retval None
  */
/*----------------------------------------------------------------------------*/

void TIM3_Config(void)
{

  /* TIM3 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

  
  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 0;
  TIM_TimeBaseStructure.TIM_Prescaler = 7199;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);


  /* Output Compare Timing Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR1_TIM3_Val;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	
  TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Disable);
	
	
	/* Enable the TIM3 gloabal Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

	/* TIM Interrupts enable */
	TIM_ITConfig(TIM3, TIM_IT_CC1, ENABLE);
	
	
} //end TIM3_Config






/*----------------------------------------------------------------------------*/
/**
  * @brief  UART Configuration.
  * @param  None
  * @retval None
  */
/*----------------------------------------------------------------------------*/

void UART_Config(void)
{

	/* Enable UART4 */
	
	
	/* GPIOC Periph clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
	
	
	/* AF Config */
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_5);
	
	
	/* Enable UART4 Tx Pin */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	
	/* UART4 Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);
	
	
	/* Configure UART4 for Tx with Baud Rate of 9600 */
	UART4_InitStructure.USART_BaudRate = 9600;
  UART4_InitStructure.USART_WordLength = USART_WordLength_8b;
  UART4_InitStructure.USART_StopBits = USART_StopBits_1;
  UART4_InitStructure.USART_Parity = USART_Parity_No;
  UART4_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  UART4_InitStructure.USART_Mode = USART_Mode_Tx;
  USART_Init(UART4, &UART4_InitStructure);
	
	
	/* Turn on UART4 */
	USART_Cmd(UART4,ENABLE);
	
	
	/* Initialize Display */
	DisplayLine ( 0, "                    " );
	DisplayLine ( 1, "                    " );
	DisplayLine ( 2, "                    " );
	DisplayLine ( 3, "                    " );
	
	
} //end UART_Config






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
	
	
	/* Setup SysTick Timer for 1 µsec interrupts  */
  if (SysTick_Config(SystemCoreClock / 1000000))
  { 
    /* Capture error */ 
    while (1)
    {}
  }
	
	
	 /* GPIOC Periph clock enable */
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
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  
  /* Configure PA.04 (DAC_OUT1) as analog */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* DAC channel1 Configuration */
  DAC_DeInit(); 
  DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;
  DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
  /* DAC Channel1 Init */
  DAC_Init(DAC_Channel_1, &DAC_InitStructure);
        
  /* Enable DAC Channel1 */
  DAC_Cmd(DAC_Channel_1, ENABLE);
	
	
} //end DAC_Config






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
