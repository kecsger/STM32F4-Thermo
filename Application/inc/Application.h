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

	OK = 1,
	Success,
	Online,
	TimeOut,
	NoConnection,
	Error
}InternetState_t;

/*================================[Internal constants]================================================================*/

/* Thermo status variables */
uint8_t isOn;				/* Thermostat ON/Off */
uint8_t isHeating;			/* Thermostat is HEATING */
uint8_t isReady;			/* Previous heating is done */
double tempAct1;			/* Temperature where the main module is */
double tempAct2;			/* Temperature where the small module is */
double tempDesired;			/* Desired temperature */
double tempDesired_Current;	/* Desired temperature */
char *mode;					/* Manual or Automatic mode */

extern uint8_t internetStatus;

extern double oneDayTemp1[288];
extern double oneDayTemp2[288];

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
