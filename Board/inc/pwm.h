/*
 * pwm.h
 *
 *  Created on: 2015 okt. 11
 *      Author: kecsger12
 */

#ifndef INC_PWM_H_
#define INC_PWM_H_

/*================================[Includes] =========================================================================*/

#include "stm32f4xx.h"
#include "led.h"

/*================================[Macros]============================================================================*/


#define  PRESCALER_VALUE	(uint32_t)(SystemCoreClock / 16000000) - 1
#define  PERIOD_VALUE       (uint32_t)(42000000 / 10000) - 1


/*================================[Type definitions]==================================================================*/


/*================================[Internal constants]================================================================*/


TIM_HandleTypeDef TIM_HandleDef_LCDBcklPWM;
TIM_HandleTypeDef TIM_HandleDef_LCDBcklight;
TIM_HandleTypeDef TIM_HandleDef_LCDFadeOut;

TIM_OC_InitTypeDef TIM_OutputChannelInit;

/*================================[Prototype function declarations]===================================================*/


void 		LCD_BacklightPWMInit();
void 		LCD_BacklightGPIOInit();

void 		LCD_BacklightWithPWM(uint8_t percent);

void 		LCD_BacklightTimerInit(uint8_t updateRate);

void 		LCD_BacklightTimerStart(uint8_t updateRate);
void 		LCD_FadeOutTimerStart();

void 		LCD_BacklightFadeOut();
void 		LCD_BacklightFadeOut2();

#endif /* INC_PWM_H_ */
