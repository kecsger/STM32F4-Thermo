/*
 * rtc.c
 *
 *  Created on: 2015 nov. 16
 *      Author: kecsger12
 */


#include "rtc.h"


void RTC_CalendarConfig(uint8_t year, uint8_t rtc_month, uint8_t date, uint8_t hours, uint8_t minutes, uint8_t seconds)
{
	RTC_DateTypeDef RTC_DateStruct;
	RTC_TimeTypeDef RTC_TimeStruct;

	/* Configure the Date */
	RTC_DateStruct.Year = year;
	RTC_DateStruct.Month = rtc_month;
	RTC_DateStruct.Date = date;
	//RTC_DateStruct.WeekDay = rtc_weekday;


	if(HAL_RTC_SetDate(&RtcHandle, &RTC_DateStruct, RTC_FORMAT_BCD) != HAL_OK)
	{
		ErrorHandler();
	}


	/* Configure the Time */
	RTC_TimeStruct.TimeFormat = RTC_HOURFORMAT12_AM;
	RTC_TimeStruct.Hours = hours;
	RTC_TimeStruct.Minutes = minutes;
	RTC_TimeStruct.Seconds = seconds;
	RTC_TimeStruct.SubSeconds = (uint32_t)(RtcHandle.Instance->SSR);
	RTC_TimeStruct.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	RTC_TimeStruct.StoreOperation = RTC_STOREOPERATION_RESET;

	if(HAL_RTC_SetTime(&RtcHandle, &RTC_TimeStruct, RTC_FORMAT_BCD) != HAL_OK)
	{
		ErrorHandler();
	}

	/* Writes a data in a RTC Backup data Register0 */
	HAL_RTCEx_BKUPWrite(&RtcHandle,RTC_BKP_DR0,0x32F2);

}


void RTC_CalendarShow(uint8_t* showtime, uint8_t* showdate)
{
	RTC_DateTypeDef RTC_DateStruct;
	RTC_TimeTypeDef RTC_TimeStruct;

	/* Get the RTC current Time */
	HAL_RTC_GetTime(&RtcHandle, &RTC_TimeStruct, RTC_FORMAT_BIN);
	/* Get the RTC current Date */
	HAL_RTC_GetDate(&RtcHandle, &RTC_DateStruct, RTC_FORMAT_BIN);
	/* Display time Format : hh:mm:ss */
	sprintf((char*)showtime,"%02d:%02d",RTC_TimeStruct.Hours, RTC_TimeStruct.Minutes);
	/* Display date Format : mm-dd-yy */
	sprintf((char*)showdate,"%02d-%02d-%02d", 2000 + RTC_DateStruct.Year, RTC_DateStruct.Month, RTC_DateStruct.Date);

}
