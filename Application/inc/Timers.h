/*
 * task.h
 *
 *  Created on: 2016 jan. 5
 *      Author: kecsger12
 */

#ifndef INC_TIMERS_H_
#define INC_TIMERS_H_

/*================================[Includes] =========================================================================*/

#include "stm32f4xx.h"
#include "led.h"

#include "pwm.h"

#include "Task.h"

/* TIM2 = TIM_1minute
 *
 * TIM4 = LCD_BacklightTIM //when to start
 *
 * TIM5 = LCD_BacklightPWM // LCD_GPIO
 *
 * TIM7 = LCD_FadeOutTIM
 *
 */

/*================================[Macros]============================================================================*/



/*================================[Type definitions]==================================================================*/



/*================================[External constants]================================================================*/

extern uint8_t TIM_TaskElapsedCnt;

extern uint8_t is1Minute;
extern uint8_t is5Minute;
extern uint8_t is10Minute;



/*================================[Internal constants]================================================================*/

TIM_HandleTypeDef TIM_HandleDef_Task1min;

/*================================[Prototype function declarations]===================================================*/

void TIM_Task1min_Init();



#endif /* INC_TIMERS_H_ */
