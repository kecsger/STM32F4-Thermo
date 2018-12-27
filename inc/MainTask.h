/*
 * MainTask.h
 *
 *  Created on: 2015 nov. 16
 *      Author: kecsger12
 */

#ifndef MAINTASK_H_
#define MAINTASK_H_

#include "GUI.h"
#include "MULTIPAGE.h"

#include "Application.h"

extern void callbackTermosztat(WM_MESSAGE * pMsg);
extern void callbackGrafikon(WM_MESSAGE * pMsg);
extern void callbackBeallitasok(WM_MESSAGE * pMsg);
extern void callbackDiagnosztika(WM_MESSAGE * pMsg);

void 		LCD_DrawScreen(void);

WM_HWIN hGraph;
WM_HWIN hMultiPage;
WM_HWIN hDialog;

WM_HWIN mainWin;
WM_HWIN termosztat;
WM_HWIN grafikon;
WM_HWIN beallitasok;
WM_HWIN diagnosztika;




#endif /* MAINTASK_H_ */
