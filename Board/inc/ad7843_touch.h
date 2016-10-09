/*
 * ad7843_touch.h
 *
 *  Created on: 2015 okt. 26
 *      Author: kecsger12
 */

#ifndef INC_AD7843_TOUCH_H_
#define INC_AD7843_TOUCH_H_

/*================================[Includes] =========================================================================*/

#include "stm32f4xx.h"
#include "spi.h"

/*================================[Macros]============================================================================*/

/* AD7843 CONTROL Register definitions
 *
 *   -------------------------------------------------------
 * 	|	  7		|	 6:4	|	  3		|	  2		|	1:0	|
 * 	 -------------------------------------------------------
 * 	|	Start	|  CH Sel	|	Mode	|	SFR/DFR	|	Pwr	|
 * 	 -------------------------------------------------------
 */
//===================================================================
#define		AD7843_START				(1<<7)

#define		AD7843_CH_X					(0b001<<4)
#define		AD7843_CH_Y					(0b101<<4)

#define		AD7843_MODE_12BIT			(0<<3)
#define		AD7843_MODE_8BIT			(1<<3)

#define		AD7843_SINGLE_ENDED			(1<<2)
#define		AD7843_DIFF_REF_SELECT		(0<<2)

#define		AD7843_PWRDOWN_PENIRQ_EN	(0b00)
#define		AD7843_PWRUP_PENIRQ_EN		(0b10)
#define		AD7843_PWRDWN_PENIRQ_DIS	(0b01)
#define		AD7843_PWRUP_PENIRQ_DIS		(0b11)


/*--------------------------------------------*/
#define 	AD7843_SPI_PORT		GPIOE
#define 	AD7843_GPIO_PORT	GPIOC

#define 	AD7843_SPI			SPI4

#define 	AD7843_nSS			GPIO_PIN_4
#define 	AD7843_SCK			GPIO_PIN_2
#define 	AD7843_MISO			GPIO_PIN_5
#define 	AD7843_MOSI			GPIO_PIN_6

#define 	AD7843_BUSY			GPIO_PIN_8
#define 	AD7843_PENIRQ		GPIO_PIN_9

/*================================[Type definitions]==================================================================*/

//extern SPI_HandleTypeDef SPI_HandleDef_AD7843;

/*================================[Internal constants]================================================================*/


/*================================[Prototype function declarations]===================================================*/

void		AD7843_Init();

void 		AD7843_SPI_GPIOInit();
void 		AD7843_SPI_ConfigInit();
void		AD7843_GPIO_Init();

uint16_t	AD7843_GetTouch_X();
uint16_t	AD7843_GetTouch_Y();

uint8_t		AD7843_isBusy();

void 		AD7843_Write(uint8_t value);
uint16_t 	AD7843_Read();
void 		AD7843_SPI_nSS(uint8_t BitVal);

#endif /* INC_AD7843_TOUCH_H_ */
