/*
 * led.c
 *
 *  Created on: 2015 okt. 8
 *      Author: kecsger12
 */


#include "led.h"

void LED_Init()
{
	GPIO_InitTypeDef GPIO_InitStruct;

	__HAL_RCC_GPIOB_CLK_ENABLE();

	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;

	//GPIO_InitStruct.Alternate = GPIO_AF2_TIM4;

	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void RedLED_On()
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
}

void RedLED_Off()
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
}

void RedLED_Toggle()
{

}


void GreenLED_On()
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
}


void GreenLED_Off()
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
}


void GreenLED_Toggle()
{
	HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);
}

/*---------------------------------------------------------------------------*/

void ErrorHandler()
{
	while(1)
	{
		RedLED_On();
		HAL_Delay(250);

		RedLED_Off();
		HAL_Delay(250);
	}

}


