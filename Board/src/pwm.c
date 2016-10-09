/*
 * pwm.c
 *
 *  Created on: 2015 okt. 11
 *      Author: kecsger12
 */


/*================================[Includes] ======================================================================================*/

#include "pwm.h"


/*================================[Internal constants]================================================================*/


uint8_t bklCounter = 0;

/*================================[Macros]==============================================================*/


/*================================[Internal functions]==============================================================*/



/*================================[Function definitions]==============================================================*/

void LCD_BacklightWithPWM(uint8_t percent)
{
	HAL_TIM_PWM_Stop(&TIM_HandleDef_LCDBcklPWM, TIM_CHANNEL_2);

	if(percent > 1)
		TIM_OutputChannelInit.Pulse = (uint32_t)(((PERIOD_VALUE+1) * percent)/100) -1 ;  //100%
	else
		TIM_OutputChannelInit.Pulse = (uint32_t)(((PERIOD_VALUE+1) * percent)/200) -1 ;  //100%

	if(HAL_TIM_PWM_ConfigChannel(&TIM_HandleDef_LCDBcklPWM, &TIM_OutputChannelInit, TIM_CHANNEL_2) != HAL_OK)
	{
		ErrorHandler();
	}

	 /*##-3- Start PWM signals generation #######################################*/

	if(HAL_TIM_PWM_Start(&TIM_HandleDef_LCDBcklPWM, TIM_CHANNEL_2) != HAL_OK)
	{
		ErrorHandler();
	}

}

void LCD_BacklightPWMInit()
{
	LCD_BacklightGPIOInit();

	__HAL_RCC_TIM5_CLK_ENABLE();

	 /*##-1- Configure the TIM peripheral #######################################*/
	TIM_HandleDef_LCDBcklPWM.Instance = TIM5;

	TIM_HandleDef_LCDBcklPWM.Init.Prescaler = 0;
	TIM_HandleDef_LCDBcklPWM.Init.Period = PERIOD_VALUE;

	TIM_HandleDef_LCDBcklPWM.Init.CounterMode = TIM_COUNTERMODE_UP;

	TIM_HandleDef_LCDBcklPWM.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	TIM_HandleDef_LCDBcklPWM.Init.RepetitionCounter = 0;


	if(HAL_TIM_PWM_Init(&TIM_HandleDef_LCDBcklPWM) != HAL_OK)
	{
		ErrorHandler();
	}

	/*##-2- Configure the PWM channels #########################################*/
	TIM_OutputChannelInit.OCMode = TIM_OCMODE_PWM1;
	TIM_OutputChannelInit.OCPolarity = TIM_OCPOLARITY_HIGH;
	TIM_OutputChannelInit.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	TIM_OutputChannelInit.OCFastMode = /*TIM_OCFAST_ENABLE*/TIM_OCFAST_DISABLE;
	TIM_OutputChannelInit.OCIdleState = /*TIM_OCIDLESTATE_SET; */TIM_OCIDLESTATE_RESET;
	TIM_OutputChannelInit.OCNIdleState = TIM_OCNIDLESTATE_RESET;

	TIM_OutputChannelInit.Pulse = (uint32_t)(((PERIOD_VALUE+1) * 100)/100) -1 ;  //100%
	if(HAL_TIM_PWM_ConfigChannel(&TIM_HandleDef_LCDBcklPWM, &TIM_OutputChannelInit, TIM_CHANNEL_2) != HAL_OK)
	{
		ErrorHandler();
	}

	 /*##-3- Start PWM signals generation #######################################*/

	if(HAL_TIM_PWM_Start(&TIM_HandleDef_LCDBcklPWM, TIM_CHANNEL_2) != HAL_OK)
	{
		ErrorHandler();
	}
}


void LCD_BacklightGPIOInit()
{
	__HAL_RCC_GPIOA_CLK_ENABLE();

	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pin = GPIO_PIN_1;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	GPIO_InitStruct.Alternate = GPIO_AF2_TIM5;

	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void LCD_BacklightFadeOut()
{
	bklCounter++;

	if(bklCounter < 25)
		LCD_BacklightWithPWM(100 - (bklCounter * 4));
	if(bklCounter == 25)
	{
		bklCounter = 0;
		LCD_BacklightWithPWM(1);

		/* Stop timer */
		HAL_TIM_Base_Stop_IT(&TIM_HandleDef_LCDFadeOut);
		HAL_TIM_Base_DeInit(&TIM_HandleDef_LCDFadeOut);
		HAL_NVIC_DisableIRQ(TIM7_IRQn);
	}
}

/* Fel-le villog */
void LCD_BacklightFadeOut2()
{
	if(bklCounter < 15)
		LCD_BacklightWithPWM(100 - (bklCounter * 7));
	if(bklCounter >= 15 && bklCounter < 30)
		LCD_BacklightWithPWM((bklCounter-14) * 7);
	if(bklCounter == 30)
		bklCounter = 0;

	bklCounter++;
}

void LCD_BacklightTimerStart(uint8_t updateRate)
{
	uint32_t TIM4_countClock = 1000;  // 10kHz counter

	__HAL_RCC_TIM4_CLK_ENABLE();

	/* Set TIMx instance */
	TIM_HandleDef_LCDBcklight.Instance = TIM4;

	/* Stop FadeOut effect, if still active */
		HAL_TIM_Base_Stop_IT(&TIM_HandleDef_LCDFadeOut);
		HAL_TIM_Base_DeInit(&TIM_HandleDef_LCDFadeOut);
		HAL_NVIC_DisableIRQ(TIM7_IRQn);

	/* IF BckLight counter is still active
	 * when screen is touched, Stop it */
		HAL_TIM_Base_Stop_IT(&TIM_HandleDef_LCDBcklight);
		HAL_TIM_Base_DeInit(&TIM_HandleDef_LCDBcklight);
		HAL_NVIC_DisableIRQ(TIM4_IRQn);
		bklCounter = 0;

	TIM_HandleDef_LCDBcklight.Init.CounterMode = TIM_COUNTERMODE_UP;

	/* Compute the prescaler value to have TIM3 counter clock equal to 10 KHz
	 *
	 * Update rate = TIM3 counter clock / (Period + 1),
	 *
	 */
	TIM_HandleDef_LCDBcklight.Init.Prescaler = (uint32_t)(SystemCoreClock / TIM4_countClock) - 1;
	TIM_HandleDef_LCDBcklight.Init.Period = (uint32_t)(TIM4_countClock * updateRate*2 ) - 1;

	TIM_HandleDef_LCDBcklight.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	TIM_HandleDef_LCDBcklight.Init.RepetitionCounter = 0;

	/* Setting IRQ for timer */
	HAL_NVIC_SetPriority(TIM4_IRQn, 2, 3);
	HAL_NVIC_EnableIRQ(TIM4_IRQn);

	if(HAL_TIM_Base_Init(&TIM_HandleDef_LCDBcklight) != HAL_OK)
	{
		ErrorHandler();
	}

	if(HAL_TIM_Base_Start_IT(&TIM_HandleDef_LCDBcklight) != HAL_OK)
	{
		ErrorHandler();
	}
}

void LCD_FadeOutTimerStart()
{
	uint32_t TIM7_countClock = 1000;  // 10kHz counter

	__HAL_RCC_TIM7_CLK_ENABLE();

	/* Set TIMx instance */
	TIM_HandleDef_LCDFadeOut.Instance = TIM7;

	TIM_HandleDef_LCDFadeOut.Init.CounterMode = TIM_COUNTERMODE_UP;

	/* Compute the prescaler value to have TIM3 counter clock equal to 10 KHz
	 *
	 * Update rate = TIM3 counter clock / (Period + 1),
	 *
	 */
	TIM_HandleDef_LCDFadeOut.Init.Prescaler = (uint32_t)(SystemCoreClock / TIM7_countClock) - 1;
	TIM_HandleDef_LCDFadeOut.Init.Period = 300;//(uint32_t)(TIM2_countClock * updateRate * 2) - 1;

	TIM_HandleDef_LCDFadeOut.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	TIM_HandleDef_LCDFadeOut.Init.RepetitionCounter = 0;

	/* Setting IRQ for timer */
	HAL_NVIC_SetPriority(TIM7_IRQn, 2, 2);
	HAL_NVIC_EnableIRQ(TIM7_IRQn);

	if(HAL_TIM_Base_Init(&TIM_HandleDef_LCDFadeOut) != HAL_OK)
	{
		ErrorHandler();
	}

	if(HAL_TIM_Base_Start_IT(&TIM_HandleDef_LCDFadeOut) != HAL_OK)
	{
		ErrorHandler();
	}
}








