/*
 * Task.h
 *
 *  Created on: 2016 jan. 5
 *      Author: kecsger12
 */

#ifndef INC_TASK_H_
#define INC_TASK_H_

/*================================[Includes] =========================================================================*/

#include "stm32f4xx.h"
#include "led.h"

#include "spwf01sa_wifi.h"

/*================================[Macros]============================================================================*/


/*================================[Type definitions]==================================================================*/


/*================================[Internal constants]================================================================*/



/*================================[Prototype function declarations]===================================================*/

void 		TaskManager();

void		Task_1Seconds();
void		Task_10Seconds();
void 		Task_1minute();
void 		Task_5minute();
void 		Task_10minute();

#endif /* INC_TASK_H_ */
