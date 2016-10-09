/*
 * Task.c
 *
 *  Created on: 2016 jan. 5
 *      Author: kecsger12
 */

/*================================[Includes] ======================================================================================*/

#include "Task.h"


/*================================[Internal constants]================================================================*/

uint16_t bufIndex = 0;
uint8_t timeBuf[2] = {0,0};



/*================================[Macros]==============================================================*/


/*================================[Internal functions]==============================================================*/



/*================================[Function definitions]==============================================================*/
void TaskManager()
{

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


void Task_1minute()
{
	/* Do nothing for now */
	asm("nop");
}

void Task_5minute()
{
	if(bufIndex == 0)
	{
		timeBuf[0] = aShowTime[0];
		timeBuf[1] = aShowTime[1];

		bufIndex = (uint16_t)(12 * (timeBuf[0]) + (timeBuf[1]/5.0));
	}


	oneDayTemp1[bufIndex] = tempAct1;
	oneDayTemp2[bufIndex] = tempAct2;
	bufIndex++;

	if(bufIndex == 288)
		bufIndex = 0;
}

void Task_10minute()
{
	WiFi_POST_ThermoStatus(isOn, isHeating, isReady, tempAct1, tempAct2, mode);
}
