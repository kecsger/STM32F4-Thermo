/*
 * spwf01sa_WiFi.h
 *
 *  Created on: 2015 nov. 5
 *      Author: kecsger12
 */

#ifndef INC_SPWF01SA_Wifi_H_
#define INC_SPWF01SA_Wifi_H_

/*================================[Includes] =========================================================================*/

#include "stm32f4xx.h"
#include "uart.h"
#include "ili9341.h"
#include "font_Default.h"

#include "Application.h"
#include "rtc.h"

/*================================[Macros]============================================================================*/


/*================================[Type definitions]==================================================================*/



/*================================[Internal constants]================================================================*/


/*================================[Prototype function declarations]===================================================*/
uint8_t		WiFi_Init();

void		WiFi_Enable();
uint8_t 	WiFi_isComWorking();
void		WiFi_nReset(uint8_t BitVal);
void 		WiFi_InitGPIO();

void		WiFi_InitSettings();
void		WiFi_RestDeftSettings();
void		WiFi_SaveSettings();

void		WiFi_GetStatus(char* RxBuff);
void		WiFi_GetAllConfigValues();

void 		WiFi_ScanNetworks();
void		WiFi_ConnetToNetwork();
void 		WiFi_ConnectionReady();
void		WiFi_GetSSID();

/*---------------------------------------------*/

uint8_t		WiFi_Send_PING();

/*----------------[HTTP GET requests]----------------------*/

void 		WiFi_GET_ThermoStatus(int minutes, uint8_t isOn, uint8_t isHeating, uint8_t isReady, char *mode);
void 		WiFi_GET_TempDesired(char * mode, int minutes);
void 		WiFi_GET_TimeAndDate();

/*----------------[HTTP POST requests]----------------------*/

void 		WiFi_POST_ThermoStatus(uint8_t isOn, uint8_t isHeating, uint8_t isReady, double tempAct1, double tempAct2, char* mode);
uint8_t		WiFi_POST_TempDesired(double tempDesired, char* mode);

/*----------------[MISC]---------------------*/

void		WiFi_HTTP_DELETE();
void		WiFi_HTTP_PUT();

void		WiFi_PrintAnswer();
uint8_t 	SearchSubstring(char *String, const char* subString);





#endif /* INC_SPWF01SA_Wifi_H_ */
