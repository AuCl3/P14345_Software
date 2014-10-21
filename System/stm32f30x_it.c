/**
  ******************************************************************************
  * @file    TIM_PWM_Input/stm32f30x_it.c 
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    20-September-2012
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "math.h"

/** @addtogroup STM32F3_Discovery_Peripheral_Examples
  * @{
  */

/** @addtogroup TIM_PWM_Input
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint16_t IC2Value = 0;
__IO uint16_t DutyCycle = 0;
__IO uint32_t Frequency = 0;
RCC_ClocksTypeDef RCC_Clocks;

uint16_t capture = 0;
extern __IO uint16_t CCR1_TIM2_Val;
extern __IO uint16_t CCR1_TIM3_Val;



extern	double		threshold;
extern	double 		attack;
extern	double 		release;
extern	double 		ratio;

extern	int 			Compress;
extern  uint16_t	attackStep;
extern  uint16_t	releaseStep;


				int 			Counter = 0;
				
				
				double 		SampledSignalVoltage;
				double 		SSdB;
				
				double 		SampledSignal;
				double 		OutputSignal;
				double 		GainReduction;
				double 		OutputVoltage;
				uint16_t 	OutputData;
				
				

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F30x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f30x.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles TIM2 global interrupt request.
  * @param  None
  * @retval None
  */
void TIM2_IRQHandler(void)
{
  while(TIM_GetITStatus(TIM2, TIM_IT_CC1) == RESET)
  
    TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
	
	//Stop timer when done
	if( attackStep == attack || releaseStep == release )
	{
		TIM_Cmd(TIM2, DISABLE);
	}
	
 
	//If timer enabled while not in Compress, Timer in Attack mode
	if( Compress != 1 )
	{
		if( attackStep < attack && SSdB >= threshold )
		{
			Counter++;
		}
		if( attackStep > 0 && SSdB < threshold )
		{
			Counter--;
		}
	}
	
	//else the Timer is in Release mode
	else
	{
		if( releaseStep < release && SSdB <= threshold )
		{
			Counter++;
		}
		if( releaseStep > 0 && SSdB > threshold )
		{
			Counter--;
		}
	}
		
		
    capture = TIM_GetCapture1(TIM2);
    TIM_SetCompare1(TIM2, capture + CCR1_TIM2_Val);

}

/**
  * @brief  This function handles TIM3 global interrupt request.
  * @param  None
  * @retval None
  */
void TIM3_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET)
  {
    TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
    
		 STM_EVAL_LEDToggle(LED6);
		/* Test EOC flag */
    if(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) != RESET)
    {
			/* Get ADC1 converted data */
			SampledSignal = ADC_GetConversionValue(ADC1);
			
			/* Compute the voltage */
			SampledSignalVoltage = (SampledSignal *3300)/0xFFF;
			
			/* Convert to dB */
			SSdB = 20*log( SampledSignalVoltage );
			
			/* Compute Output Voltage*/
			OutputSignal = threshold + (( SSdB - threshold ) / ratio );
		
			GainReduction = OutputSignal - threshold;
			
			OutputVoltage = 6.1*22*GainReduction;
			
			/* 
					ADD CODE FOR LINEAR TIMER STEP HERE
			*/
		
			/*Convert and output to DAC */
			OutputData = (OutputVoltage*0xFFF)/3300;
			
			DAC_SetChannel1Data(DAC_Align_12b_R, OutputData);
		}
		
		
		
		 STM_EVAL_LEDToggle(LED8);
 
    capture = TIM_GetCapture1(TIM3);
    TIM_SetCompare1(TIM3, capture + CCR1_TIM3_Val);
		
  }
}



/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 

/**
  * @}
  */ 


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
