/*
 * Task.c
 *
 *  Created on: 2016 jan. 5
 *      Author: kecsger12
 */

/*================================[Includes] ======================================================================================*/

#include "Task.h"


#define FLT_DECIMAL_DIG 2
#define FLT_STRING_SIZE (1+1+1+(FLT_DECIMAL_DIG-1)+1+1+ 4   +1)//  - d .  dddddddd           e - dddd \0


/*================================[Internal constants]================================================================*/

uint16_t bufIndex = 0;
uint8_t timeBuf[4] = {0,0, 0,0};

/* Benchmar task times */
uint32_t sleep_tickTime = 0;
uint32_t task_1s_tickTime = 0;
uint32_t task_10s_tickTime = 0;
uint32_t task_1min_tickTime = 0;
uint32_t tmpSysTick = 0;
static uint32_t tmpSleepTick = 0;

/* CPU load calculation
 *
 *  CPU load = counterVal/(counterVal + sleepCounterVal) *
 *
 */

float cpuLoad_1s_Task = 0;

/*================================[Macros]==============================================================*/


/*================================[Internal functions]==============================================================*/



/*================================[Function definitions]==============================================================*/
void TaskManager()
{

	if(is1Second == RESET && is10Second == RESET && is1Minute == RESET && is5Minute == RESET && is10Minute == RESET)
	{
		if(tmpSleepTick == 0)
			tmpSleepTick = HAL_GetTick();

		sleep_tickTime = HAL_GetTick() - tmpSleepTick;
	}


	if(is1Second == SET)
	{
		is1Second = RESET;
		Task_1Seconds();
	}

	if(is10Second == SET)
	{
		is10Second = RESET;
		Task_10Seconds();

		//char buf[20];
		//sprintf(buf, "%d", task_10s_tickTime);
		//ILI9341_Puts(2, 25, &buf, &TM_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_YELLOW);
	}

	if(is1Minute == SET)
	{
		is1Minute = RESET;
		Task_1minute();
	}

	if(is5Minute == SET)
	{
		is5Minute = RESET;
		Task_5minute();
	}

	if(is10Minute == SET)
	{
		is10Minute = RESET;
		Task_10minute();
	}

}

/* ================== Task list ============================= */

void Task_1Seconds()
{
	tmpRuntime_var = HAL_GetTick();
	task_idle_runtime_ms = sleep_tickTime;

	/* Refresh the screen */
	GUI_Exec();
	WM_InvalidateWindow(mainWin);

	task_1sec_runtime_ms = HAL_GetTick() - tmpRuntime_var;

	upTimeCounter_1sec++;
	tmpSleepTick = 0;
	sleep_tickTime = 0;
}

void Task_10Seconds()
{
	tmpRuntime_var = HAL_GetTick();
		//GreenLED_On();

		Application_RUN();
		RFM_Receive(Rx_buf);
		WatchDog_Refresh();

		//GreenLED_Off();
	task_10sec_runtime_ms = HAL_GetTick() - tmpRuntime_var;
}

void Task_1minute()
{
	tmpRuntime_var = HAL_GetTick();

	//WiFi_GET_TempDesired(mode, 10000);

	/* Checking internet connectivity periodically */
	internetStatus = WiFi_Send_PING();

	task_1min_runtime_ms = HAL_GetTick() - tmpRuntime_var;
}

void Task_5minute()
{
	/* Updated 'Grafikon' curve */

	if(bufIndex == 0)
	{
		/* ASCII 0 = 0x30, timeBuf returns ASCII chararcter */
		timeBuf[0] = aShowTime[0]-0x30;
		timeBuf[1] = aShowTime[1]-0x30;
		timeBuf[2] = aShowTime[3]-0x30;
		timeBuf[3] = aShowTime[4]-0x30;

		bufIndex = (uint16_t)((10.0 * timeBuf[0] + timeBuf[1] + (timeBuf[2]*10.0 + timeBuf[3])/60.0) * (288.0/24.0));
	}


	oneDayTemp1[bufIndex] = tempAct_mainModule;
	oneDayTemp2[bufIndex] = tempAct_smallModule;
	bufIndex++;

	if(bufIndex >= 288)
		bufIndex = 0;
}

void Task_10minute()
{
	//WiFi_POST_ThermoStatus(isOn, isHeating, isReady, tempAct_mainModule, tempAct_smallModule, mode);
}
