/*
 * relay.h
 *
 *  Created on: 2015 nov. 10
 *      Author: kecsger12
 */

#ifndef INC_RELAY_H_
#define INC_RELAY_H_

/*================================[Includes] =========================================================================*/

#include "stm32f4xx.h"

/*================================[Macros]============================================================================*/



/*================================[Type definitions]==================================================================*/



/*================================[Internal constants]================================================================*/


/*================================[Prototype function declarations]===================================================*/

void 	BD6211_InitGPIO(); 	//H-bridge GPIO Init


void	Relay_On();
void 	Relay_Off();


/*-----------[BD6211 Modes]---------------------*/

void 	BD6211_FIN(uint8_t BitVal);
void 	BD6211_RIN(uint8_t BitVal);

void 	BD6211_StandBy();
void 	BD6211_Forward();
void 	BD6211_Reverse();
void 	BD6211_Break();


#endif /* INC_RELAY_H_ */
