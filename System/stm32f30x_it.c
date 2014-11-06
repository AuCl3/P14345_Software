/**
  ******************************************************************************
  * @file    System / stm32f30x_it.c
  * @author  P14345
  * @version V1.1.0
  * @date    11/6/14
  * @brief   Program body
  ******************************************************************************
  */ 

/*----------------------------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/


	#include "main.h"
	#include "math.h"
	
	
/** @addtogroup STM32F3_Discovery_SD_Projects
  * @{
  */

/** @addtogroup System
  * @{
  */

/* Private typedef -----------------------------------------------------------*/

	RCC_ClocksTypeDef RCC_Clocks;


/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


/* Necessary system global variables */

extern	double		threshold;
extern	double 		attack;
extern	double 		release;
extern	double 		ratio;

extern	int 			Compress;
extern	int				Att;
extern	int				Rel;


/* RCC Clock Variables */

__IO uint16_t IC2Value = 0;
__IO uint16_t DutyCycle = 0;
__IO uint32_t Frequency = 0;


/* TIMER values */

uint16_t capture = 0;
extern __IO uint16_t CCR1_TIM3_Val;
extern __IO uint16_t CCR1_TIM2_Val;


/* Timer2 Variables */
				
				uint16_t	attackStep = 1;
				uint16_t	releaseStep = 1;
				int 			attackCounter = 1;
				int				releaseCounter = 1;
				
				
/* Timer3 Variables */

				double		SampledSignal = 0;
				
				uint32_t	SS16q16int = 0;
				int  			base2SSdB = 0;
				double		SSdBbase2 = 0;
				double		SSdB = 0;
				
				double 		OutputSignal = 0;
				double 		GainReduction = 0;
				uint16_t 	OutputData = 0;


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/




/*----------------------------------------------------------------------------*/
/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
/*----------------------------------------------------------------------------*/

void NMI_Handler(void)
{
}






/*----------------------------------------------------------------------------*/
/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
/*----------------------------------------------------------------------------*/

void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}






/*----------------------------------------------------------------------------*/
/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
/*----------------------------------------------------------------------------*/

void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}






/*----------------------------------------------------------------------------*/
/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
/*----------------------------------------------------------------------------*/

void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}






/*----------------------------------------------------------------------------*/
/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
/*----------------------------------------------------------------------------*/

void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}






/*----------------------------------------------------------------------------*/
/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
/*----------------------------------------------------------------------------*/

void SVC_Handler(void)
{
}






/*----------------------------------------------------------------------------*/
/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
/*----------------------------------------------------------------------------*/

void DebugMon_Handler(void)
{
}






/*----------------------------------------------------------------------------*/
/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
/*----------------------------------------------------------------------------*/

void PendSV_Handler(void)
{
}






/*----------------------------------------------------------------------------*/
/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
/*----------------------------------------------------------------------------*/

void SysTick_Handler(void)
{
}






/******************************************************************************/
/*                 STM32F30x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f30x.s).                                            */
/******************************************************************************/






/*----------------------------------------------------------------------------*/
/**
  * @brief  This function handles TIM2 global interrupt request.
  * @param  None
  * @retval None
	*
	*	Current Tasks:
	*
	*		1. 	Read ADC
	*		2.	Convert ADC1 value to dB
	*		3.	Enable Attack/Release 
	*		4.	Compute VCA Voltage
	*		5.	Output VCA Voltage to DAC1
	*
  */
/*----------------------------------------------------------------------------*/

void TIM2_IRQHandler(void)
{
	
  if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)
  {
    TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
    
		
		//LED for Timing Analysis
		STM_EVAL_LEDToggle(LED6);
		
		
		
		/*------------------------------------------------------------------------*/
		/*		INTERRUPT CODE START HERE																						*/
		/*------------------------------------------------------------------------*/
		
		
		
		/*	1. Read ADC ----------------------------------------------------------*/
		
		
		//Read ADC if ADC not busy
		if(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) != RESET)
		{
			SampledSignal = ADC_GetConversionValue(ADC1);
		}
			
		
		
		/*	2. Convert ADC1 Value to dB ------------------------------------------*/
		
		
		//Convert SampledSignal into 16q16 int (16 bit value, 16 bit decimal)
		SS16q16int = SampledSignal * 48;
		
					//Note, 48 = ( 3 * 0x10000 ) / ( 0xFFF + 1 )
		
		
		
		//Convert SS into a dB value
		if( SS16q16int == 0 )
		{
			SSdB = -90;
		}
		else
		{
			
			//Take log2
			base2SSdB = log2( SS16q16int );
		
		
			//Decode SSdBbase2
		
			SSdBbase2 =   base2SSdB;						// First, convert int to double
			SSdB =  SSdBbase2 / 22293082	;			
			
					// Note, 1 / 22293082 = [ ( 20 * log10(2) ) / 2^27 ]
		
		}
		
		
		
		/*	3. Enable Attack or Release ------------------------------------------*/
		
		
		//if signal above threshold, and compression not running,
		// enable Attack Time and Compression
		if( SSdB >= threshold && Compress == 0 )
		{
			TIM_Cmd(TIM3, ENABLE );
			Compress = 1;
			Att = 1;
		}
		
		//if signal below threshold, compression running, and Attack disabled, 
		// enable Release Timer
		else if( SSdB <= threshold && Compress == 1 && Att == 0 )
		{
			TIM_Cmd(TIM3, ENABLE );
			Rel = 1 ;
		}
		
		
		
		/*	4. Compute VCA Voltage -----------------------------------------------*/
		
		
		//If not compressing, output = 0
		if( Compress == 0 )
		{
			//Output 0x000 to DAC
			DAC_SetChannel1Data(DAC_Align_12b_R, 0x000);
		}
		
		
		//if compressing, determine VCA voltage
		if( Compress == 1 )
		{
			
			/* Compute Output Voltage*/
			OutputSignal = threshold + (( SSdB - threshold ) / ratio );
		
			GainReduction = OutputSignal - threshold;
			
			OutputData = 183.183 * GainReduction;
			
					//Note, 183.183 = 0.0061 * 22 * ( 0xFFF / 3 )
			
		
		
			if( Att == 1 )
			{
 				
				OutputData = OutputData * attackCounter / attackStep ;
																	
				//							2V			* 51						/ 150
				
			}
			
			if( Rel == 1 )
			{
				
				OutputData = OutputData * ( releaseStep - releaseCounter ) / releaseStep ;
				
				//					2V					*	2000					/ 6000
				
			}
			
			
			
			/*	5. Output VCA Voltage to DAC1 --------------------------------------*/
			
			
			//Output to DAC
			DAC_SetChannel1Data(DAC_Align_12b_R, OutputData);
			
		}
	
		
		/*------------------------------------------------------------------------*/
		/*		INTERRUPT CODE END HERE																							*/
		/*------------------------------------------------------------------------*/
		
		
		//LED for Timing Analysis
		STM_EVAL_LEDToggle(LED8);
		
    capture = TIM_GetCapture1(TIM2);
    TIM_SetCompare1(TIM2, capture + CCR1_TIM2_Val);
		
  }
} //End Timer2 Interrupt Service Routine






/*----------------------------------------------------------------------------*/
/**
  * @brief  This function handles TIM3 global interrupt request.
  * @param  None
  * @retval None
  *
	*	Current Tasks:
	*
	*		1.	Check/Update TIMER mode (Attack or Release)
	*		2. 	Increment/Decrement CounterValue
	*		3.	Disable TIMER3 if done counting up to Attack/Release Value
	*		4.	Disable TIMER3 if counterValue goes to 0
	*
  */
/*----------------------------------------------------------------------------*/
	
void TIM3_IRQHandler(void)
{
	
  if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET)
  {
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
	
		
		/*------------------------------------------------------------------------*/
		/*		INTERRUPT CODE START HERE																						*/
		/*------------------------------------------------------------------------*/
	
		
		/*	1. Check/Update TIMER mode (Attack or Release)------------------------*/
		
		
		//update Step values, before comparing them with counter values
		
		//attackStep range:		1 	 to 	300
		//releaseStep rage:		1000 to 	12000
	
		attackStep = attack 	* 10;						// 0.1ms * 10 		= 1
		releaseStep = release * 10000;				// 0.1s  * 10000 	= 1000
		
		
		
		if( Att == 1 && SSdB < threshold )
		{
				Att = 0;
				Rel = 1;
			
				releaseCounter = attackCounter * releaseStep;
				releaseCounter = releaseCounter / attackStep;
				attackCounter = 1;
		}
		
		if( Rel == 1 && SSdB > threshold )
		{
				Rel = 0;
				Att = 1;
				
			
				attackCounter = releaseCounter * attackStep;
				attackCounter = attackCounter / releaseStep;
				releaseCounter = 1;
		}
		
		/*	2. Increment/Decrement Counters --------------------------------------*/
		
		
		//Attack mode
		if( Att == 1 )
		{
			attackCounter++;
		}

		
		//Release mode
		if( Rel == 1 )
		{
			releaseCounter++;
		}
		
		
		
		/*	3.	Disable TIMER3 if done counting up to Attack/Release Value -------*/
		
	
		//Attack mode
		if( Att == 1 && attackCounter - 1 >= attackStep )
		{
			TIM_Cmd(TIM3, DISABLE);
			
			Att = 0;
			attackCounter = 1;
			
		}
		
		//Release mode
		if( Rel == 1 && releaseCounter - 1 >= releaseStep )
		{
			TIM_Cmd(TIM3, DISABLE);
			
			Compress = 0;
			Rel = 0;
			releaseCounter = 1;
		}

		
		/*	4.	Disable TIMER3 if counterValue goes to 0	------------------------*/
		
		
		//Attack mode
		if( Att == 1 && attackCounter < 1 && Att == 1 )
		{
			TIM_Cmd(TIM3, DISABLE);
			
			Compress = 0;
			Att = 0;
			attackCounter = 1;
			
		}
		
		//Release mode
		if( Rel == 1 && releaseCounter < 1 && Rel == 1 )
		{
			TIM_Cmd(TIM3, DISABLE);
			
			Rel = 0;
			releaseCounter = 1;
		}

		
		/*------------------------------------------------------------------------*/
		/*		INTERRUPT CODE END HERE																							*/
		/*------------------------------------------------------------------------*/
		
		
		capture = TIM_GetCapture1(TIM3);
		TIM_SetCompare1(TIM3, capture + CCR1_TIM3_Val);

	}
} //End Timer3 Interrupt Service Routine






/*----------------------------------------------------------------------------*/
/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*----------------------------------------------------------------------------*/

/*void PPP_IRQHandler(void)
{
}*/






/**
  * @}
  */ 

/**
  * @}
  */ 


/*******************************    END OF FILE    ****************************/
