/*
 * WatchDog.c
 *
 *  Created on: 2016 jan. 20
 *      Author: kecsger12
 */


#include "WatchDog.h"



void WatchDog_Init()
{
	__HAL_RCC_WWDG_CLK_ENABLE();

	/* APB1(42MHz)/4096/256 = 85,8Hz = 23.3ms */
	WDG_HandleDef.Init.Prescaler = IWDG_PRESCALER_256;

	/* 0x800 = 2048 => 23.3ms * 2048 = 40 sec
	 *
	 * 0xFFF = 4095 => 23.3ms * 4095 = 80 sec
	 *
	 * ha ennyi idõ után sem hivódik meg a refresh függvény,
	 * azaz nem töltõdik újra a WWDG számláló,
	 * akkor reseteli az eszközt
	 *
	 * */
	WDG_HandleDef.Init.Reload = 0x0FFF; //0x800;
	WDG_HandleDef.Instance = IWDG;

	if (HAL_IWDG_Init(&WDG_HandleDef) != HAL_OK)
	{
		ErrorHandler();
	}

	if(HAL_IWDG_Start(&WDG_HandleDef) != HAL_OK)
	{
		ErrorHandler();
	}
}


void WatchDog_Refresh()
{

	if(HAL_IWDG_Refresh(&WDG_HandleDef) != HAL_OK)
	{
		ErrorHandler();
	}
}
