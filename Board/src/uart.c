/*
 * uart.c
 *
 *  Created on: 2015 okt. 15
 *      Author: kecsger12
 */

/*================================[Includes] ======================================================================================*/

#include "uart.h"
#include  "led.h"

#include "string.h"

#include "ili9341.h"
#include "font_Default.h"

/*================================[Internal constants]================================================================*/

__IO ITStatus UART_TxReady = RESET;
__IO ITStatus UART_RxReady = RESET;

__IO ITStatus UART_Ready = RESET;

/*================================[Macros]==============================================================*/



/*================================[Internal functions]==============================================================*/

UART_HandleTypeDef UART_HandleDef_SPWF01SA;

DMA_HandleTypeDef DMA_TxHandleDef;
DMA_HandleTypeDef DMA_RxHandleDef;

/*================================[Function definitions]==============================================================*/

void UART_Init()
{
	UART_GPIO_Init();
	UART_InitConfig(115200);
}


void UART_InitConfig(uint32_t BaudRateSpeed)
{
	/* Enable USART2 clock */
	__HAL_RCC_USART1_CLK_ENABLE();

	//UART_HandleTypeDef *UART_HandleDef = &UART_HandleDef_SPWF01SA;

	/*##-1- Configure the UART peripheral ######################################*/
	/* Put the USART peripheral in the Asynchronous mode (UART Mode) */
	/* UART1 configured as follow:
	  - Word Length = 8 Bits
	  - Stop Bit = One Stop bit
	  - Parity = None
	  - BaudRate = 9600 baud
	  - Hardware flow control disabled (RTS and CTS signals) */

	UART_HandleDef_SPWF01SA.Instance = USART1;

	UART_HandleDef_SPWF01SA.Init.BaudRate = BaudRateSpeed;
	UART_HandleDef_SPWF01SA.Init.WordLength = UART_WORDLENGTH_8B;
	UART_HandleDef_SPWF01SA.Init.StopBits = UART_STOPBITS_1;
	UART_HandleDef_SPWF01SA.Init.Parity = UART_PARITY_NONE;
	UART_HandleDef_SPWF01SA.Init.HwFlowCtl = UART_HWCONTROL_NONE; //UART_HWCONTROL_RTS_CTS;
	UART_HandleDef_SPWF01SA.Init.Mode = UART_MODE_TX_RX;
	UART_HandleDef_SPWF01SA.Init.OverSampling = UART_OVERSAMPLING_16;

	if(HAL_UART_Init(&UART_HandleDef_SPWF01SA) != HAL_OK)
	{
		ErrorHandler();
	}
}


void UART_GPIO_Init()
{
	GPIO_InitTypeDef  GPIO_InitStruct;

	__HAL_RCC_GPIOA_CLK_ENABLE();

	/* USART Tx/Rx Pin config */

	/**USART1 GPIO Configuration
		PA12     ------> USART1_RTS
		PA11     ------> USART1_CTS
		PA10     ------> USART1_RX
		PD9     ------> USART1_Tx
	*/

	GPIO_InitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_9;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	GPIO_InitStruct.Alternate = GPIO_AF7_USART1;

	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/*
 * UART command to send AT commands to Wifi module
 *
 * IMPORTANT: length should be the num of chars to be sent
 *
 * eg.: "AT\r" 			--> 3
 * 		"AT+S.HELP\r" 	--> 10
 */

void UART_TransmitString(char *AT_Command)
{
	uint16_t charLength = strlen((char*)AT_Command);

	HAL_UART_Transmit(&UART_HandleDef_SPWF01SA, (uint8_t *)AT_Command, charLength, 1000);
}

/*
 * UART command to recieve data from Wifi module
 *
 * IMPORTANT: expectedNumOfChars is the expected number of character recived after the command
 *
 * eg.: "AT\r" 			--> \r\nOK\r\n 	==> min 6, max 100(?)
 * 		"AT+S.HELP\r" 	--> 			==> 1024 was OK
 */

void UART_ReceiveString(char *RxBuff, uint16_t expectedNumOfCharsToRecieve)
{
	HAL_UART_Receive(&UART_HandleDef_SPWF01SA, (uint8_t *)RxBuff, expectedNumOfCharsToRecieve, 1000);
}



/**
  * @brief  Tx/Rx Transfer completed callbacks
  * @param  UartHandle: UART handle.
  * @note   This example shows a simple way to report end of DMA Tx transfer, and
  *         you can add your own implementation.
  * @retval None
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	/* Set transmission flag: transfer complete */
	//UART_TxReady = SET;
	UART_Ready = SET;
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
	/* Set transmission flag: transfer complete */
	//UART_RxReady = SET;
	UART_Ready = SET;
}

