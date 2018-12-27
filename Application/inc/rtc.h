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

typedef struct
{
  uint8_t year;
  uint8_t month;
  uint8_t day;
}RTC_DateStruct_t;

typedef struct
{
  uint8_t hour;
  uint8_t min;
  uint8_t sec;
}RTC_TimeStruct_t;


void 		RTC_CalendarConfig(uint8_t year, uint8_t rtc_month, uint8_t date, uint8_t hours, uint8_t minutes, uint8_t seconds);
void 		RTC_CalendarShow(uint8_t* showtime, uint8_t* showdate);

void 		RTC_CalendarShow2(RTC_TimeStruct_t showtime, RTC_DateStruct_t showdate);





#endif /* INC_RTC_H_ */
