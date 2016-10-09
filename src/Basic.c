/*********************************************************************
*                SEGGER MICROCONTROLLER SYSTEME GmbH                 *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2004  SEGGER Microcontroller Systeme GmbH        *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

***** emWin - Graphical user interface for embedded applications *****
emWin is protected by international copyright laws.   Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with a license and should not be re-
distributed in any way. We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : WIDGET_Multipage.c
Purpose     : Demonstrates the use of the MULTIPAGE widget
Requirements: WindowManager - (x)
              MemoryDevices - (x)
              AntiAliasing  - ( )
              VNC-Server    - ( )
              PNG-Library   - ( )
              TrueTypeFonts - ( )
---------------------------END-OF-HEADER------------------------------
*/

#include <stdlib.h>

#include "MainTask.h"


#include "rfm73.h"
#include "ad7792_temp.h"
#include "rtc.h"


/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
//
// Recommended memory to run the sample with adequate performance
//
#define 		RECOMMENDED_MEMORY 		(1024L * 20)
#define 		ID_GRAPH_0             	(GUI_ID_USER )

/*----------------------------------------------------------------------------------------------------------------------*/


BUTTON_Handle buttonLCD, buttonWiFi, buttonTimeDate, buttonModeMan, buttonModeAuto;

/*----------------------------------------------------------------------------------------------------------------------*/





double oneDayTemp1[288] =
{
		22.2,22.3,22.2,22.1,22.1,22.0,22.0,22.1,22.1,22.0,22.5,22.5,22.4,22.3,22.2,
		22.1,22.1,22.0,22.0,22.1,22.1,22.0,21.8,21.8,21.7,21.6,21.5,21.4,21.3,21.3,
		21.2,21.1,21.1,21.0,21.0,21.0,21.0,21.0,21.0,21.0,21.0,21.0,21.0,20.9,20.9,
		20.9,20.9,20.8,20.8,20.8,20.7,20.7,20.7,20.7,20.5,20.7,20.8,20.8,20.7,20.8,
		20.7,20.8,20.7,20.6,20.7,20.5,20.6,20.5,20.5,20.5,20.4,20.3,20.3,20.3,20.3,
		20.3,20.3,20.3,20.4,20.4,20.4,20.4,20.4,20.6,20.6,20.6,20.5,20.5,20.3,20.4,
		20.4,20.3,20.3,20.3,20.5,20.6,20.5,20.6,20.6,20.5,20.6,20.5,20.5,19.2,19.4,
		19.4,19.4,19.7,19.7,19.7,19.8,20.1,20.1,20.2,20.3,20.3,20.3,20.3,20.3,20.3,
		20.5,20.5,20.5,20.5,20.8,20.8,20.7,20.7,20.9,20.9,21.0,21.0,21.0,21.0,21.2,
		21.2,21.2,21.3,21.3,21.3,21.4,21.4,21.5,21.5,21.6,21.6,22.8,22.7,22.7,22.8,
		22.7,22.9,22.8,22.8,22.7,22.7,22.8,22.9,22.9,23.0,23.0,22.8,22.9,22.9,23.0,
		23.1,23.0,22.9,23.0,23.1,23.1,23.1,23.1,23.0,23.1,23.0,23.0,23.2,23.0,23.0,
		23.1,22.9,22.9,23.0,23.0,22.7,23.0,23.0,23.1,23.1,22.9,22.7,22.7,22.8,22.9,
		23.2,23.2,23.2,23.2,23.2,23.1,23.2,23.2,23.1,23.4,23.2,23.2,22.9,23.0,22.8,
		22.8,22.7,22.7,22.6,22.7,22.5,22.6,22.4,22.2,22.3,22.2,22.2,22.4,22.2,21.9,
		22.0,21.8,21.7,21.6,21.4,21.5,21.8,21.9,22.1,22.3,22.3,22.5,22.5,22.1,22.3,
		22.5,22.5,22.4,22.4,22.4,22.4,22.3,22.3,22.2,22.1,21.8,21.9,21.9,21.9,21.8,
		21.7,21.6,21.5,21.5,21.4,21.3,21.2,21.3,21.4,21.3,21.2,21.3,21.2,21.2,21.1,
		21.1,21.0,21.0,21.1,21.0,21.3,21.4,21.5,21.5,22.0,21.9,21.8,21.7,21.8,21.8,
		21.8,21.8,21.8
};


double oneDayTemp2[288] =
{
		21.3,21.3,21.3,21.2,21.2,21.1,21.1,21.1,21.1,21.0,21.0,21.0,21.0,21.0,20.9,
		20.9,20.9,20.9,20.8,20.8,20.8,20.8,20.8,20.7,20.7,20.8,20.7,20.6,20.6,20.6,
		20.6,20.6,20.6,20.6,20.6,20.5,20.5,20.6,20.5,20.5,20.5,20.5,20.5,20.5,20.5,
		20.5,20.5,20.5,20.5,20.5,20.5,20.5,20.4,20.4,20.4,20.4,20.4,20.3,20.3,20.3,
		20.3,20.3,20.3,20.3,20.3,20.3,20.3,20.3,20.3,20.3,20.3,20.3,20.3,20.2,20.2,
		20.3,20.4,20.3,20.3,20.3,20.3,20.3,20.3,20.2,20.2,20.2,20.3,20.2,20.2,20.2,
		20.1,20.2,20.2,20.1,20.2,20.2,20.1,20.1,20.1,20.1,20.1,20.1,20.1,20.1,20.1,
		20.1,20.1,20.1,20.1,20.0,20.0,20.0,20.0,20.0,20.0,20.0,20.0,20.0,20.0,20.0,
		20.0,20.0,20.0,20.0,20.0,20.0,20.0,20.0,20.0,20.0,20.3,20.1,20.1,20.1,20.2,
		20.1,20.3,20.3,20.4,20.4,20.5,20.5,20.6,20.6,20.7,20.7,20.8,20.8,20.9,20.9,
		21.0,21.0,21.1,21.0,21.0,21.1,21.0,21.1,21.0,21.1,21.1,21.1,21.1,21.1,21.1,
		21.1,21.1,21.1,21.1,21.1,21.1,21.2,21.2,21.1,21.2,21.1,21.1,21.1,21.1,21.1,
		21.1,21.2,21.1,21.1,21.1,21.2,21.2,21.2,21.2,21.3,21.3,21.2,21.3,21.3,21.2,
		21.3,21.3,21.3,21.3,21.4,21.4,21.4,21.4,21.5,21.5,21.5,21.5,21.5,21.4,21.4,
		21.4,21.4,21.4,21.4,21.5,21.4,21.4,21.3,21.3,21.3,21.3,21.3,21.2,21.2,21.2,
		21.1,21.1,21.0,21.0,21.0,21.0,21.0,21.0,21.0,21.0,21.0,21.0,21.1,21.3,21.2,
		21.2,21.2,21.2,21.2,21.1,21.1,21.1,22.0,21.9,21.8,21.7,21.7,21.6,21.5,21.5,
		21.5,21.5,21.3,21.4,21.3,21.3,21.5,21.8,21.6,21.6,21.5,21.4,21.4,21.3,21.3,
		21.3,21.2,21.2,21.2,21.2,21.2,21.1,21.1,21.1,21.1,21.2,21.2,21.2,21.3,21.3,
		21.3,21.3,21.3
};



/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
/*********************************************************************
*
*       Dialog resource
*
*  These tables contain the information required to create the dialogs.
*  It has been created manually, but could also be created by the GUIBuilder.
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate1[] =
{
		{ WINDOW_CreateIndirect,    "Dialog 1", 0, 0, 0, 320, 240, FRAMEWIN_CF_MOVEABLE }
};

static const GUI_WIDGET_CREATE_INFO _aDialogCreate2[] =
{
		{ WINDOW_CreateIndirect,    "Dialog 2", 0, 0, 0, 320, 240, FRAMEWIN_CF_MOVEABLE }

};

static const GUI_WIDGET_CREATE_INFO _aDialogCreate3[] =
{
		{ WINDOW_CreateIndirect,    "Dialog 3", 0, 0, 0, 320, 240, FRAMEWIN_CF_MOVEABLE }
};



/*********************************************************************
*
*       _cbBkWindow
*
*  Function description
*    Callback routine of the background window.
*    It shows the sample title and draws the background.
*/


void _cbWin0(WM_MESSAGE * pMsg)
{
	BUTTON_Handle buttonPlus, buttonNeg;

	switch (pMsg->MsgId)
	{

		case WM_CREATE:



			BUTTON_SetDefaultFont(&GUI_Font24B_1);
			buttonNeg = BUTTON_CreateEx(160, 198, 35, 35, termosztat,  WM_CF_SHOW, 0, 0);
			BUTTON_SetText(buttonNeg, "-");
			buttonPlus = BUTTON_CreateEx(270, 198, 35, 35, termosztat,  WM_CF_SHOW, 0, 0);
			BUTTON_SetText(buttonPlus, "+");

			break;

		case WM_PAINT:

			GUI_DrawGradientV(0, 0, 320, 220, GUI_DARKGRAY, GUI_LIGHTBLUE);

			GUI_SetColor(GUI_WHITE);
			GUI_SetTextMode(GUI_TM_TRANS);

			GUI_SetFont(&GUI_Font24_1);
			GUI_DispStringAt("Szoba hõfoka", 160, 20);
			GUI_DrawVLine(150, 8, 205);
			GUI_SetFont(&GUI_Font32B_1);
			GUI_DispStringAt("°", 304, 50);

			GUI_SetFont(&GUI_Font16B_1);
			GUI_DispStringAt("Kívánt hõfok", 191, 142);

			GUI_SetFont(&GUI_Font16B_1);
			GUI_DispStringAt("Státus", 5, 82);
			GUI_DrawHLine(100, 5, 110);

			GUI_DispStringAt("Hõmérõ modul", 5, 142);
			GUI_DrawHLine(160, 5, 110);
			GUI_SetFont(&GUI_Font8x16_1);
			GUI_DispStringAt("°", 93, 172);


			GUI_DispStringAt(isHeating==1?("Fûtés"):(isReady==1?("Készen"):(" - ")), 40, 110);   	//x

			/* tempAct */
			GUI_GotoX(160);
			GUI_GotoY(57);
			GUI_SetFont(&GUI_FontD36x48);
			GUI_DispFloat(tempAct1, 4);

			RTC_CalendarShow(aShowTime, aShowDate);

			GUI_SetFont(&GUI_Font20B_ASCII);
			GUI_DispStringAt((char*)&aShowDate, 2, 8);
			GUI_SetFont(&GUI_FontD24x32);
			GUI_DispStringAt((char*)&aShowTime, 15, 35);

			if(Rx_buf[0] != '\0')
				tempAct2 = 10 * (Rx_buf[0]-48) + (Rx_buf[1]-48) + ((Rx_buf[3]-48.0)/10.0);

			GUI_SetFont(&GUI_Font32B_1);
			GUI_DispStringAt((char*)&Rx_buf, 40, 170);

			GUI_GotoX(208);
			GUI_GotoY(171);
			GUI_SetFont(&GUI_Font32B_1);
			GUI_DispFloat(tempDesired, 4);

			GUI_SetFont(&GUI_Font13_ASCII);
			internetStatus==Success?GUI_DispStringAt("ONLINE", 160, 5):GUI_DispStringAt("OFFLINE", 160, 5);

		break;
  }
}

void _cbWin1(WM_MESSAGE * pMsg)
{
	I16 y_tempDataToCoord[288];
	int i;

	switch (pMsg->MsgId)
	{

		case WM_PAINT:

			GUI_SetColor(GUI_BLACK);
			GUI_FillRect(0,0,320,210);


			GUI_SetColor(GUI_GRAY);
			for(i=0; i<7; i++)
			{
				GUI_DrawVLine(i*51+12, 0, 200);
			}

			GUI_SetColor(GUI_YELLOW);
			GUI_DispStringAt("0", 12, 200);
			GUI_DispStringAt("4", 60, 200);
			GUI_DispStringAt("8", 112, 200);
			GUI_DispStringAt("12", 160, 200);
			GUI_DispStringAt("16", 211, 200);
			GUI_DispStringAt("20", 262, 200);
			GUI_DispStringAt("24", 308, 200);

			GUI_SetColor(GUI_GRAY);
			for(i=0; i<5; i++)
			{
				GUI_DrawHLine(i*48, 0, 320);
			}

			GUI_SetColor(GUI_YELLOW);
			GUI_DispStringAt("23", 0, 1);
			GUI_DispStringAt("22", 0, 45);
			GUI_DispStringAt("21", 0, 92);
			GUI_DispStringAt("20", 0, 141);
			GUI_DispStringAt("19", 0, 189);


			/* Grafikon #1 */
			for(i=0; i< GUI_COUNTOF(oneDayTemp1); i++)
			{
				y_tempDataToCoord[i] = (((23.0-oneDayTemp1[i])/(23.0-19.0)) * 189);
			}
			GUI_SetColor(GUI_GREEN);
			GUI_DrawGraph(y_tempDataToCoord, GUI_COUNTOF(y_tempDataToCoord), 12, 0);



			/* Grafikon #2 */
			for(i=0; i< GUI_COUNTOF(oneDayTemp2); i++)
			{
				y_tempDataToCoord[i] = (((23.0-oneDayTemp2[i])/(23.0-19.0)) * 189);
			}
			GUI_SetColor(GUI_RED);
			GUI_DrawGraph(y_tempDataToCoord, GUI_COUNTOF(y_tempDataToCoord), 12, 0);

			break;
  }
}



void _cbWin2(WM_MESSAGE * pMsg)
{

	switch (pMsg->MsgId)
	{
		case WM_PAINT:

			GUI_DrawGradientV(0, 0, 320, 220, GUI_DARKGRAY, GUI_LIGHTBLUE);

			GUI_SetColor(GUI_WHITE);
			GUI_SetTextMode(GUI_TM_TRANS);

			BUTTON_SetDefaultFont(&GUI_Font16B_1);

			buttonLCD = BUTTON_CreateEx(60, 10, 200, 40, beallitasok,  WM_CF_SHOW, 0, 0);
			BUTTON_SetText(buttonLCD, "Kijelzõ és fényerõ");

			buttonTimeDate = BUTTON_CreateEx(60, 60, 200, 40, beallitasok,  WM_CF_SHOW, 0, 0);
			BUTTON_SetText(buttonTimeDate, "Dátum és idõ módosítása");

			buttonWiFi = BUTTON_CreateEx(60, 110, 200, 40, beallitasok,  WM_CF_SHOW, 0, 0);
			BUTTON_SetText(buttonWiFi, "WiFi beálltások");

			buttonModeMan = BUTTON_CreateEx(40, 160, 100, 40, beallitasok,  WM_CF_SHOW, 0, 0);
			if(strcmp(mode, "manual") == 0)
				BUTTON_SetTextColor(buttonModeMan, BUTTON_CI_UNPRESSED, GUI_DARKGREEN);
			BUTTON_SetText(buttonModeMan, "Manuális");


			buttonModeAuto = BUTTON_CreateEx(180, 160, 100, 40, beallitasok,  WM_CF_SHOW, 0, 0);
			if(strcmp(mode, "auto") == 0)
				BUTTON_SetTextColor(buttonModeAuto, BUTTON_CI_UNPRESSED, GUI_DARKGREEN);
			BUTTON_SetText(buttonModeAuto, "Automatikus");

			break;

  }
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       MainTask
*/
void LCD_DrawScreen(void)
{
	//WM_SetCallback(WM_HBKWIN, _cbBkWindow);


/*--------------------[ Create and attach the MULTIPAGE dialog windows ]---------------------------*/

	hMultiPage = MULTIPAGE_CreateEx(0, 0, 320, 240, 0, WM_CF_SHOW, 0, 0);

	hDialog = GUI_CreateDialogBox(_aDialogCreate1, GUI_COUNTOF(_aDialogCreate1), NULL,       WM_UNATTACHED, 0, 0);
	MULTIPAGE_AddPage(hMultiPage, hDialog, "Termosztát");

	hDialog = GUI_CreateDialogBox(_aDialogCreate2, GUI_COUNTOF(_aDialogCreate2), NULL,       WM_UNATTACHED, 0, 0);
	MULTIPAGE_AddPage(hMultiPage, hDialog, "Grafikon");

	hDialog = GUI_CreateDialogBox(_aDialogCreate3, GUI_COUNTOF(_aDialogCreate3), NULL,       WM_UNATTACHED, 0, 0);
	MULTIPAGE_AddPage(hMultiPage, hDialog, "Beállítások");

	MULTIPAGE_SetTabWidth(hMultiPage, 105, 0);
	MULTIPAGE_SetTabWidth(hMultiPage, 105, 1);
	MULTIPAGE_SetTabWidth(hMultiPage, 106, 2);
	MULTIPAGE_SetTabHeight(hMultiPage, 30);
	MULTIPAGE_SetFont(hMultiPage, &GUI_Font16B_1);


	MULTIPAGE_SKINFLEX_PROPS multipageSkinEnabled =
	{
				GUI_DARKGRAY,// backcolor for selected items
				GUI_BLACK,// top color of top gradient
				GUI_GRAY,// buttom color of buttom gradient
				GUI_GRAY, // top color of buttom gradient
				GUI_LIGHTBLUE,// buttom color of buttom gradient
				GUI_GRAY,// frame color
				GUI_WHITE // text color
	};

	MULTIPAGE_SKINFLEX_PROPS multipageSkinSelected =
	{
				GUI_DARKGRAY,// backcolor for selected items
				GUI_BLACK,// top color of top gradient
				GUI_GRAY,// buttom color of buttom gradient
				GUI_BLUE, // top color of buttom gradient
				GUI_LIGHTBLUE,// buttom color of buttom gradient
				GUI_WHITE,// frame color
				GUI_WHITE // text color
	};


	MULTIPAGE_SetSkinFlexProps( &multipageSkinEnabled, MULTIPAGE_SKINFLEX_PI_ENABLED);
	MULTIPAGE_SetSkinFlexProps( &multipageSkinSelected, MULTIPAGE_SKINFLEX_PI_SELECTED);


}

/*

static void _WriteByte2File(U8 Data, void * p)
{
	U32 nWritten;
	WriteFile(*((HANDLE *)p), &Data, 1, &nWritten, NULL);
}
static void _ExportToFile(void)
{
	HANDLE hFile = CreateFile("C:\\GUI_BMP_Serialize.bmp", GENERIC_WRITE, 0, 0,	CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	GUI_BMP_Serialize(_WriteByte2File, &hFile);
	CloseHandle(hFile);
}
*/




void MainTask(void)
{
	/* Enable use of memory devices */
	WM_SetCreateFlags(WM_CF_MEMDEV);

	/*Initialize STemWin GUI */
	GUI_Init();

	LCD_DrawScreen();

	/* Megrajzolja a megadott ablak tartalmat */
	termosztat = WM_CreateWindow(0, 30, 320, 220, WM_CF_HIDE, _cbWin0, 0);
	grafikon = WM_CreateWindow(0, 30, 320, 220, WM_CF_HIDE, _cbWin1, 0);
	beallitasok = WM_CreateWindow(0, 30, 320, 220, WM_CF_HIDE, _cbWin2, 0);

	//_ExportToFile();

/*
	while(1)
	{}
*/



}

/*************************** End of file ****************************/
