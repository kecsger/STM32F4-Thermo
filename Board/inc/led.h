/*
 * led.h
 *
 *  Created on: 2015 okt. 9
 *      Author: kecsger12
 */

#ifndef LED_H_
#define LED_H_

#include "stm32f4xx.h"

void LED_Init();

void RedLED_On();
void RedLED_Off();
void RedLED_Toggle();

void GreenLED_On();
void GreenLED_Off();
void GreenLED_Toggle();

void ErrorHandler();

#endif /* LED_H_ */
