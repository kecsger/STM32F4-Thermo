/*
 * Application.h
 *
 *  Created on: 2015 nov. 16
 *      Author: kecsger12
 */

#ifndef INC_APPLICATION_H_
#define INC_APPLICATION_H_

/*================================[Includes] =========================================================================*/

#include "stm32f4xx.h"
#include "stdio.h"

#include "uart.h"
#include "spi.h"

#include "lcd.h"
#include "ili9341.h"
#include "font_Arial.h"
#include "font_Default.h"

#include "ad7792_temp.h"
#include "ad7843_touch.h"

#include "spwf01sa_wifi.h"
#include "rfm73.h"

#include "led.h"
#include "pwm.h"

#include "relay.h"

#include "rtc.h"
#include "Timers.h"

#include "Task.h"
#include "MainTask.h"

#include "WatchDog.h"


/*================================[Macros]============================================================================*/



/*================================[Type definitions]==================================================================*/

typedef enum{
	Online = 1,
	TimeOut,
	NoConnection,
	Error
}InternetState_t;

typedef enum {
	Manual = 0,
	Auto
}ThermostatMode_t;

/*================================[Internal constants]================================================================*/

/* Thermo status variables */
uint8_t 			isOn;				/* Thermostat ON/Off */
uint8_t 			isHeating;			/* Thermostat is HEATING */
uint8_t 			isReady;			/* Previous heating is done */
float 				tempAct_mainModule;	/* Temperature where the main module is */
float 				tempAct_smallModule;/* Temperature where the small module is */
float 				tempDesired;		/* Desired temperature */
float 				tempDesired_Current;/* Desired temperature */
uint8_t			 	mode;				/* Manual or Automatic mode */

/* DIAGNOSTIC variables */
extern uint16_t task_idle_runtime_ms;
extern uint16_t task_1sec_runtime_ms;
extern uint16_t task_10sec_runtime_ms;
extern uint16_t task_1min_runtime_ms;
extern uint16_t tmpRuntime_var;
extern int32_t upTimeCounter_1sec;

extern uint8_t internetStatus;

extern float oneDayTemp1[288];
extern float oneDayTemp2[288];

extern uint8_t Rx_buf[MAX_PACKET_LEN];

/*================================[Prototype function declarations]===================================================*/

void 		Application_RUN();

void 		LCD_ScreenTouched();
void		LCD_UpdateStatus();

void		InitBoard();
void		InitVariables();


void 		Task_10minute();

void 		Button_init();
void 		ButtonWasPressed();



#endif /* INC_APPLICATION_H_ */
