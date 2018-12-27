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

uint16_t TIM_TaskElapsedCnt = 0;

uint8_t is1Second = RESET;
uint8_t is10Second = RESET;
uint8_t is1Minute = RESET;
uint8_t is5Minute = RESET;
uint8_t is10Minute = RESET;

uint32_t sysTick_couter;

/*================================[Macros]==============================================================*/


/*================================[Internal functions]==============================================================*/



/*================================[Function definitions]==============================================================*/

// TODO: kiszamolni pontosan a frekvenciat

void TIM_Task1Sec_Init()
{
	__HAL_RCC_TIM2_CLK_ENABLE();


	uint32_t Task1sec_period = 10000;
	uint32_t TIM2_countClock = 18000; //HAL_RCC_GetSysClockFreq() / Task1sec_period;

	/*
	 *
	 * UpdateFreq = ( SystemCoreClock / [(Prescaler+1) * (Period+1)] )
	 *
	 */

	 /*##-1- Configure the TIM peripheral #######################################*/
	TIM_HandleDef_Task1sec.Instance = TIM2;

	TIM_HandleDef_Task1sec.Init.Prescaler = (uint32_t) (TIM2_countClock);
	TIM_HandleDef_Task1sec.Init.Period = (uint32_t) ((Task1sec_period / 2));

	TIM_HandleDef_Task1sec.Init.CounterMode = TIM_COUNTERMODE_UP;

	TIM_HandleDef_Task1sec.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	TIM_HandleDef_Task1sec.Init.RepetitionCounter = 0;

	/* Setting IRQ for timer */
	HAL_NVIC_SetPriority(TIM2_IRQn, 10, 2);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);

	if(HAL_TIM_Base_Init(&TIM_HandleDef_Task1sec) != HAL_OK)
	{
		ErrorHandler();
	}

	if(HAL_TIM_Base_Start_IT(&TIM_HandleDef_Task1sec) != HAL_OK)
	{
		ErrorHandler();
	}
}


void TIM_Task1Min_Init()
{
	__HAL_RCC_TIM2_CLK_ENABLE();

	/*
	 * 				SysClock
	 * Freq = ----------------------------
	 * 			Prescaler x Period
	 *
	 * 			Prescaler -> uint16
	 * 			Period    -> uint32
	 *
	 * TimeBase = 1 / Freq
	 *
	 */


	uint32_t Task1min_period = 60 * 10000; // 60sec
	uint16_t TIM2_countClock = HAL_RCC_GetSysClockFreq() / Task1min_period;

	 /*##-1- Configure the TIM peripheral #######################################*/
	TIM_HandleDef_Task1min.Instance = TIM2;

	TIM_HandleDef_Task1min.Init.Prescaler = (uint16_t)(TIM2_countClock - 1);
	TIM_HandleDef_Task1min.Init.Period = (uint32_t)(Task1min_period - 1);

	TIM_HandleDef_Task1min.Init.CounterMode = TIM_COUNTERMODE_UP;

	TIM_HandleDef_Task1min.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	TIM_HandleDef_Task1min.Init.RepetitionCounter = 0;

	/* Setting IRQ for timer */
	HAL_NVIC_SetPriority(TIM2_IRQn, 20, 5);
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
	static uint32_t tmpSysTick;

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

	if(htim == &TIM_HandleDef_Task1sec)
	{
		if((TIM_TaskElapsedCnt % 1) == 0)
		{
			is1Second = SET;
		}

		if((TIM_TaskElapsedCnt % 10) == 0)
		{
			is10Second = SET;
		}

		if((TIM_TaskElapsedCnt % 60) == 0)
		{
			is1Minute = SET;
		}

		if((TIM_TaskElapsedCnt % 300) == 0)
		{
			is5Minute = SET;
		}

		if((TIM_TaskElapsedCnt % 600) == 0)
		{
			is10Minute = SET;
		}

		if (TIM_TaskElapsedCnt == UINT16_MAX)
			TIM_TaskElapsedCnt = 0;
		else
			TIM_TaskElapsedCnt++;

	}
}
