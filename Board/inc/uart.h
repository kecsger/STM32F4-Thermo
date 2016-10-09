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
void 		UART_InitConfig();

void 		UART_TransmitData(uint16_t dataBuf);
void 		UART_TransmitString(const char *str);
void 		UART_TransmitRecieveString(char *Txstr, char *Rxstr);

void 		UART_RecieveData(char *c);
void 		UART_RecieveData2(char *c, int numOfChar);
void 		UART_RecieveString(char *RxBufer);

void 		UART_Wait(USART_TypeDef *USARTx);

/*================================[Prototype function declarations]===================================================*/


#endif /* INC_UART_H_ */


