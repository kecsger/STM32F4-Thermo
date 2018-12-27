/*
 * uart.h
 *
 *  Created on: 2015 okt. 15
 *      Author: kecsger12
 */

#ifndef INC_UART_H_
#define INC_UART_H_

/*================================[Includes] =========================================================================*/

#include "stm32f4xx.h"

/*================================[Macros]============================================================================*/

extern UART_HandleTypeDef UART_HandleDef_SPWF01SA;


/*================================[Type definitions]==================================================================*/

void 		UART_Init();

void 		UART_GPIO_Init();
void 		UART_InitConfig(uint32_t BaudRateSpeed);

void 		UART_Wait(USART_TypeDef *USARTx);


void UART_TransmitString(char *AT_Command);
void UART_ReceiveString(char *RxBuff, uint16_t expectedNumOfCharsToRecieve);



/*================================[Prototype function declarations]===================================================*/


#endif /* INC_UART_H_ */


