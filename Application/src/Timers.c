/*
 * task.c
 *
 *  Created on: 2016 jan. 5
 *      Author: kecsger12
 */

/*================================[Includes] ======================================================================================*/

#include "Timers.h"


/*================================[Internal constants]================================================================*/

uint8_t TIM_ElapsedCnt = 0;

uint8_t TIM_TaskElapsedCnt = 0;

uint8_t is1Minute = RESET;
uint8_t is5Minute = RESET;
uint8_t is10Minute = RESET;

/*================================[Macros]==============================================================*/


/*================================[Internal functions]==============================================================*/



/*================================[Function definitions]==============================================================*/
void TIM_Task1min_Init()
{
	__HAL_RCC_TIM2_CLK_ENABLE();

	uint32_t TIM2_countClock = 1000;  // 1kHz counter -> 1sec
	uint32_t Task1min_period = 60; // 60sec

	 /*##-1- Configure the TIM peripheral #######################################*/
	TIM_HandleDef_Task1min.Instance = TIM2;

	TIM_HandleDef_Task1min.Init.Prescaler = (uint32_t)(SystemCoreClock / TIM2_countClock) - 1;
	TIM_HandleDef_Task1min.Init.Period = (uint32_t)(TIM2_countClock * Task1min_period * 2) - 1;

	TIM_HandleDef_Task1min.Init.CounterMode = TIM_COUNTERMODE_UP;

	TIM_HandleDef_Task1min.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	TIM_HandleDef_Task1min.Init.RepetitionCounter = 0;

	/* Setting IRQ for timer */
	HAL_NVIC_SetPriority(TIM2_IRQn, 2, 1);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);

	if(HAL_TIM_Base_Init(&TIM_HandleDef_Task1min) != HAL_OK)
	{
		ErrorHandler();
	}

	if(HAL_TIM_Base_Start_IT(&TIM_HandleDef_Task1min) != HAL_OK)
	{
		ErrorHandler();
	}
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

	if(htim == &TIM_HandleDef_LCDBcklight)
	{
		TIM_ElapsedCnt++;

		if(TIM_ElapsedCnt == 2)
		{
			HAL_TIM_Base_Stop_IT(&TIM_HandleDef_LCDBcklight);
			HAL_TIM_Base_DeInit(&TIM_HandleDef_LCDBcklight);
			HAL_NVIC_DisableIRQ(TIM4_IRQn);
			LCD_FadeOutTimerStart();

			TIM_ElapsedCnt = 0;
		}

		//GreenLED_Toggle();
	}

	if(htim == &TIM_HandleDef_LCDFadeOut)
	{
		LCD_BacklightFadeOut();
		//LCD_BacklightFadeOut2(); // Xmas edition
	}

	if(htim == &TIM_HandleDef_Task1min)
	{
		//TaskManager();

		if((TIM_TaskElapsedCnt % 1) == 0)
		{
			is1Minute = SET;
		}

		if((TIM_TaskElapsedCnt % 5) == 0)
		{
			is5Minute = SET;
		}

		if((TIM_TaskElapsedCnt % 10) == 0)
		{
			TIM_TaskElapsedCnt = 0;
			is10Minute = SET;
		}

		TIM_TaskElapsedCnt++;

	}
}
