/*
 * WatchDog.h
 *
 *  Created on: 2016 jan. 20
 *      Author: kecsger12
 */

#ifndef INC_WATCHDOG_H_
#define INC_WATCHDOG_H_

#include "stm32f4xx.h"
#include "led.h"

IWDG_HandleTypeDef WDG_HandleDef;


void	WatchDog_Init();

void	WatchDog_Refresh();


#endif /* INC_WATCHDOG_H_ */
