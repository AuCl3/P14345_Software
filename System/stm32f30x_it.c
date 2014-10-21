/**
  ******************************************************************************
  * @file    System / stm32f30x_it.c
  * @author  P14345
  * @version V1.0.0
  * @date    10/9/14
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


/* RCC Clock Variables */

__IO uint16_t IC2Value = 0;
__IO uint16_t DutyCycle = 0;
__IO uint32_t Frequency = 0;


/* TIMER values */

uint16_t capture = 0;
extern __IO uint16_t CCR1_TIM2_Val;
extern __IO uint16_t CCR1_TIM3_Val;


/* Necessary system global variables */

extern	double		threshold;
extern	double 		attack;
extern	double 		release;
extern	double 		ratio;

extern	int 			Compress;
extern	int				AttRel;


/* Timer2 Variables */
				
				uint16_t	attackStep = 0;
				uint16_t	releaseStep = 0;
				int 			attackCounter = 0;
				int				releaseCounter = 0;
				
				
/* Timer3 Variables */

__IO 	uint16_t  	ADC1ConvertedValue = 0;

				double 		SampledSignal;
				double 		SampledSignalVoltage;
				double 		SSdB;
				double 		OutputSignal;
				double 		GainReduction;
				double 		OutputVoltage;
				uint16_t 	OutputData;
				

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
  */
	/*----------------------------------------------------------------------------*/
	
void TIM2_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)
  {
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
	
		
		//update Step values
	
		attackStep = attack * 10;
		releaseStep = release * 10000;
	
		
		//Stop and clear Timer and counters if done
		if( attackCounter >= attackStep || releaseCounter >= releaseStep )
		{
			TIM_Cmd(TIM2, DISABLE);
		
			attackCounter = 0;
			releaseCounter = 0;
		
			
			//If attack Timer done, Timer is now Release Timer
			if( attackCounter >= attackStep )
			{
				AttRel = 1;
			}
			
			//disable compression if Release Timer done, Timer now Attack Timer
			if( releaseCounter >= releaseStep )
			{
				Compress = 0;
				AttRel = 0;
			}
			
		}
		
		
		//If counters go to 0, disable Timer
		if( attackCounter <= 0 || releaseCounter <= 0 )
		{
			TIM_Cmd(TIM2, DISABLE);
		}
		
			
		//If timer enabled while not in Compress, Timer in Attack mode
		if( AttRel == 0 )
		{
			if( SSdB >= threshold )
			{
				attackCounter++;
			}
			if( SSdB < threshold && attackCounter > 0 )
			{
				attackCounter--;
			}
		}

		
		//else the Timer is in Release mode
		else
		{
			if( SSdB <= threshold )
			{
				releaseCounter++;
			}
			if( SSdB > threshold && releaseCounter > 0 )
			{
				releaseCounter--;
			}
		}

		
		capture = TIM_GetCapture1(TIM2);
		TIM_SetCompare1(TIM2, capture + CCR1_TIM2_Val);

	}
} //End Timer2 Interrupt






/*----------------------------------------------------------------------------*/
/**
  * @brief  This function handles TIM3 global interrupt request.
  * @param  None
  * @retval None
  */
/*----------------------------------------------------------------------------*/

void TIM3_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET)
  {
    TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
    

		
		
	
			
		/* Read ADC Every Time */
		if(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) != RESET)
		{
			/* Get ADC1 converted data */
			ADC1ConvertedValue = ADC_GetConversionValue(ADC1);
		}
			
			/* Compute the voltage */
			SampledSignalVoltage = (ADC1ConvertedValue *3300)/0xFFF;
			
			SampledSignalVoltage = SampledSignalVoltage / 1000;
			
			
			/* Convert to dB */
			SSdB = 20*log10( SampledSignalVoltage );
			
			/* if signal above threshold, and compression not running, enable compression */
			if( SSdB >= threshold && Compress != 1 )
			{
				TIM_Cmd(TIM2, ENABLE );
				Compress = 1;
			}
		
		STM_EVAL_LEDToggle(LED8);
		
		/* Output 0 if not Compressing */
		if( Compress == 0 )
		{
			DAC_SetChannel1Data(DAC_Align_12b_R, 0x000);
		}
		
		
		/* Determine output if Compressing */
		if( Compress > 0 )
		{
			/* Compute Output Voltage*/
			OutputSignal = threshold + (( SSdB - threshold ) / ratio );
		
			GainReduction = OutputSignal - threshold;
			
			OutputVoltage = (6.1*22*GainReduction ) / 1000;
			
			/* 
						ADD CODE FOR LINEAR TIMER STEP HERE
			*/
		
			/*Convert and output to DAC */
			
			OutputData = OutputVoltage*1000;
			
			
			OutputData = (OutputData*0xFFF)/3300;
			
			DAC_SetChannel1Data(DAC_Align_12b_R, OutputData);
		}
	
		
		STM_EVAL_LEDToggle(LED6);
		
    capture = TIM_GetCapture1(TIM3);
    TIM_SetCompare1(TIM3, capture + CCR1_TIM3_Val);
		
  }
} //End Timer3 Interrupt






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
