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

/* 	0= Hardware power up
	1=Hardware failure
	2=Radio task terminated by user
	3=Radio idle
	4=Scan in progress
	5=Scan complete
	6=Join in progress
	7=Joined
	8=Access point started
	9=802.11 handshake complete
	10=Ready to transmit data (i.e. “Link Up”)
*/

typedef enum
{
  WIFI_HWPOWERUP       		= 0x00,
  WIFI_HWFALIURE    		= 0x01,
  WIFI_RADIOTASKIDLE 		= 0x02,
  WIFI_RADIOIDLE 			= 0x03,
  WIFI_SCANINPROGRESS 		= 0x04,
  WIFI_SCANCOMPLETE 		= 0x05,
  WIFI_JOINPROGRESS 		= 0x06,
  WIFI_JOINED 				= 0x07,
  WIFI_ACCESSPOINTSTARTED 	= 0x08,
  WIFI_HANDSHAKECOMPLETE 	= 0x09,
  WIFI_LINKUP 				= 0x10,

} Wifi_ConnectionStatus_t;

/*================================[Internal constants]================================================================*/


/*================================[Prototype function declarations]===================================================*/
uint8_t		WiFi_Init();

void		WiFi_Enable();
uint8_t 	WiFi_isComWorking();
void		WiFi_nReset(uint8_t BitVal);
void 		WiFi_InitGPIO();

void		WiFi_InitSettings();
void		WiFi_ResetToDefSettings();
void		WiFi_SaveSettings();

void		WiFi_GetStatus(char* RxBuff);
void		WiFi_GetAllConfigValues();

void 		WiFi_ScanNetworks();
void		WiFi_ConnetToNetwork();
uint8_t 	WiFi_GetConnectionStatus();
void 		WiFi_ConnectionReady();
void		WiFi_GetSSID();
uint8_t 	WiFi_WaitForPowerUp(uint16_t maxWaitTime_ms);

/*---------------------------------------------*/

uint8_t		WiFi_Send_PING();

/*----------------[HTTP GET requests]----------------------*/

void 		WiFi_GET_ThermoStatus(int minutes, uint8_t isOn, uint8_t isHeating, uint8_t isReady, uint8_t mode);
void 		WiFi_GET_TempDesired(uint8_t mode, int minutes);
void 		WiFi_GET_TimeAndDate();

/*----------------[HTTP POST requests]----------------------*/

void 		WiFi_POST_ThermoStatus(uint8_t isOn, uint8_t isHeating, uint8_t isReady, double tempAct1, double tempAct2, uint8_t mode);
uint8_t		WiFi_POST_TempDesired(double tempDesired, uint8_t mode);

/*----------------[MISC]---------------------*/

void		WiFi_HTTP_DELETE();
void		WiFi_HTTP_PUT();

void		WiFi_PrintAnswer();
uint8_t 	SearchSubstring(char *String, const char* subString);





#endif /* INC_SPWF01SA_Wifi_H_ */
