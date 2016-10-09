/*
 * Application.c
 *
 *  Created on: 2015 nov. 16
 *      Author: kecsger12
 */

/*================================[Includes] ======================================================================================*/

#include "Application.h"

/*================================[Internal constants]================================================================*/


/*================================[Macros]==============================================================*/


/*================================[Internal functions]==============================================================*/

int count = 0;
uint8_t internetStatus = 0;
uint8_t buttonPressedCnt = 0;

uint8_t tempDes_changed = 0;

uint8_t POST_success = 0;
uint8_t POSTcnt = 0;

/*================================[Function definitions]==============================================================*/

void Application_RUN()
{
	WatchDog_Refresh();

	WiFi_GET_TempDesired(mode, 10000);

	RFM_Receive(Rx_buf);

	tempAct1 = AD7792_MeasureTemp();

	if(tempAct1 < tempDesired)
	{
		if(isHeating == 0)
		{
			isHeating = 1;
			isReady = 0;

			Relay_On();

			WiFi_POST_ThermoStatus(isOn, isHeating, isReady, tempAct1, tempAct2, mode);
		}
		else
		{
			WiFi_GET_TempDesired(mode, 10000);
			LCD_UpdateStatus();
		}
	}

	else if(isReady == 0)
	{
		isReady = 1;
		isHeating = 0;

		Relay_Off();

		WiFi_POST_ThermoStatus(isOn, isHeating, isReady, tempAct1, tempAct2, mode);
	}

	LCD_UpdateStatus();

	if(tempDes_changed == 1)
	{
		tempDes_changed = 0;
		HAL_Delay(100);

		if(tempDes_changed == 1)
		{tempDes_changed = 0; return;}

		WiFi_POST_TempDesired(tempDesired, mode);
		HAL_Delay(10);
	}
}

void LCD_ScreenTouched()
{
	uint16_t x_coord, y_coord;

	x_coord = AD7843_GetTouch_X();
	y_coord = AD7843_GetTouch_Y();

	/* Turn on backlight again, and start BacklightTimer */
	LCD_BacklightWithPWM(100);
	LCD_BacklightTimerStart(30);



	/* Termosztat MultiPage */
	if(x_coord > 0 && x_coord < 105 && \
			y_coord > 0 && y_coord < 35)
	{
		if(WM_GetActiveWindow() != termosztat)
		{
			WM_HideWindow(WM_GetActiveWindow());
			MULTIPAGE_SelectPage(hMultiPage, 0);
			WM_SelectWindow(termosztat);
			WM_ShowWindow(termosztat);

			mainWin = termosztat;
			WM_InvalidateWindow(mainWin);
			//GUI_Exec();
		}
	}

	/* Grafikon MultiPage */
	if(x_coord > 105 && x_coord < 210 && \
			y_coord > 0 && y_coord < 35)
	{
		if(WM_GetActiveWindow() != grafikon)
		{
			WM_HideWindow(WM_GetActiveWindow());
			MULTIPAGE_SelectPage(hMultiPage, 1);
			WM_SelectWindow(grafikon);
			WM_ShowWindow(grafikon);

			mainWin = grafikon;
			WM_InvalidateWindow(mainWin);
			//GUI_Exec();
		}
	}

	/* Beallitasok MultiPage */
	if(x_coord > 210 && x_coord < 320 && \
			y_coord > 0 && y_coord < 35)
	{
		if(WM_GetActiveWindow() != beallitasok)
		{
			WM_HideWindow(WM_GetActiveWindow());
			MULTIPAGE_SelectPage(hMultiPage, 2);
			WM_SelectWindow(beallitasok);
			WM_ShowWindow(beallitasok);
			mainWin = beallitasok;
			WM_InvalidateWindow(mainWin);
			//GUI_Exec();
		}

	}


	/* Button '-' */
	if(x_coord > 160 && x_coord < 200 && \
			y_coord > 198 && y_coord < 238)
	{
		if(WM_GetActiveWindow() == termosztat)
		{
			tempDesired = tempDesired - 0.5;
			tempDes_changed = 1;
		}
	}

	/* Button '+' */
	if(x_coord > 270 && x_coord < 310 && \
			y_coord > 198 && y_coord < 238)
	{
		if(WM_GetActiveWindow() == termosztat)
		{
			tempDesired = tempDesired + 0.5;
			tempDes_changed = 1;
		}
	}

	/* Idõ és dátum beállitása */
	if(x_coord > 60 && x_coord < 260 && \
				y_coord > 60 && y_coord < 100)
	{
		if(WM_GetActiveWindow() == beallitasok)
		{
			WiFi_GET_TimeAndDate();
		}
	}


}

void LCD_UpdateStatus()
{
	WM_InvalidateWindow(mainWin);
	//WM_UpdateWindowAndDescs(mainWin);
	GUI_Exec();
}




void InitBoard()
{


/*--------------------[ GPIO Init ]----------------------------*/

	LED_Init();
	Button_init();


/*--------------------[ ILI9342 Init ]----------------------------*/



	LCD_BacklightPWMInit();
	//LCD_BacklightTimerStart(5);

	ILI9341_Init();



/*  LCD is beeing initialized in LCDConf.c

		if(ILI9341_Init() != ILI9341_DEV_ID)
			ErrorHandler();
*/

	//LCD_BacklightWithPWM(0);

/*--------------------[ WiFi Init ]-----------------------------*/


	internetStatus = WiFi_Init();


/*--------------------[ RTC Config ]----------------------------*/

	if(internetStatus == Success)
		WiFi_GET_TimeAndDate();

	else
		RTC_CalendarConfig(0x01, RTC_MONTH_JANUARY, 0x01, 0x01, 0x01, 0x01);




/*--------------------[ RFM73 Init ]----------------------------*/

	RFM_Init();

/*--------------------[ AD7792 Init ]----------------------------*/

	if(AD7792_Init() != HAL_OK)
		ErrorHandler();

/*--------------------[ AD7843 Init ]----------------------------*/

	AD7843_Init();

/*--------------------[ Relay Init ]-----------------------------*/

	BD6211_InitGPIO();
	Relay_Off();

}

void InitVariables()
{
	isOn = 1;
	isHeating = 0;
	isReady = 1;
	tempDesired = 21.0;
	tempDesired_Current = tempDesired;
	mode = "manual";
}



/*--------------------[ MISC ]----------------------------*/

void Button_init()
{
	GPIO_InitTypeDef Gpio_Init;

	__HAL_RCC_GPIOC_CLK_ENABLE();

	Gpio_Init.Mode =  GPIO_MODE_IT_FALLING; // GPIO_MODE_INPUT;
	Gpio_Init.Pin = GPIO_PIN_0;
	Gpio_Init.Pull = GPIO_NOPULL;
	Gpio_Init.Speed = GPIO_SPEED_FAST;

	HAL_GPIO_Init(GPIOC, &Gpio_Init);

	//HAL_NVIC_SetPriority(EXTI0_IRQn, 4, 0);
	//HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}

void ButtonWasPressed()
{
	if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_0))
	{
		HAL_Delay(80);
		if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_0))
		{
				LCD_BacklightWithPWM(1);

		}

	}
}



