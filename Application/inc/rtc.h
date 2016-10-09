/*
 * rtc.h
 *
 *  Created on: 2015 nov. 16
 *      Author: kecsger12
 */

#ifndef INC_RTC_H_
#define INC_RTC_H_

#include "stm32f4xx.h"


extern 		RTC_HandleTypeDef RtcHandle;


/* Buffers used for displaying Time and Date */
uint8_t aShowTime[50];
uint8_t aShowDate[50];


void 		RTC_CalendarConfig(uint8_t year, uint8_t rtc_month, uint8_t date, uint8_t hours, uint8_t minutes, uint8_t seconds);
void 		RTC_CalendarShow(uint8_t* showtime, uint8_t* showdate);






#endif /* INC_RTC_H_ */
