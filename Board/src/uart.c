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
//USART_HandleTypeDef USART_HandleTypeDef_SPWF01SA;

DMA_HandleTypeDef DMA_TxHandleDef;
DMA_HandleTypeDef DMA_RxHandleDef;

/*================================[Function definitions]==============================================================*/

void UART_Init()
{
	UART_GPIO_Init();
	UART_InitConfig();
}




void UART_TransmitData(uint16_t dataBuf)
{
	UART_HandleTypeDef *UART_HandleDef = &UART_HandleDef_SPWF01SA;

	if(HAL_UART_Transmit(UART_HandleDef, (uint8_t*)&dataBuf, 2, 1000) != HAL_OK)
	{
		ErrorHandler();
	}
}

void UART_RecieveData(char *c)
{
	UART_HandleTypeDef *UART_HandleDef = &UART_HandleDef_SPWF01SA;

	char RxBuff[1600] = {0};
	int i = 0;
	uint32_t timeOut = 1000;

	while(i<1600)
	{
		timeOut = 1000;
		RedLED_Off();

		while((UART_HandleDef->Instance->SR & USART_FLAG_RXNE) == RESET)
		{
			timeOut--;
			if(timeOut == 0)
				return;
		}

		if(HAL_UART_Receive(UART_HandleDef, (uint8_t*)c, 1, 1000) != HAL_OK)
		{
			RedLED_On();
		}

		else
		{
			RxBuff[i] = *(c++);
			i++;
		}

		if(i>=6)
		{
			  // If <CR><LF> OK <CR><LF> is found, then transmit shuld be done
			if(strstr(&RxBuff, "\r\nOK\r\n") != NULL)
				break;

			else if(strstr(&RxBuff, "WiFi Scan Complete (0x0)") != NULL) // No internet
			{
				ILI9341_Puts(2, 25, "Could not connect to known network, no internet", &TM_Font_7x10, ILI9341_COLOR_BLACK, ILI9341_COLOR_YELLOW);
				break;
			}
		}
	}
}

void UART_RecieveData2(char *c, int numOfChar)
{
	UART_HandleTypeDef *UART_HandleDef = &UART_HandleDef_SPWF01SA;

	char RxBuff[1600] = {0};
	int i = 0;
	int lostCharcnt = 0;
	int messageState = -1;
	uint32_t timeOut = 1000;



	while((UART_HandleDef->Instance->SR & USART_FLAG_RXNE) == RESET)
	{
		timeOut--;
		if(timeOut == 0)
			break;
	}

	UART_Ready = RESET;


	if(HAL_UART_Receive(UART_HandleDef, c, numOfChar, 1000) != HAL_OK)
	{
		lostCharcnt++;
	}




/*	while(messageState != SUCCESS || messageState != ERROR)
	{
		timeOut = 1000;
		RedLED_Off();

		if(HAL_UART_Receive(UART_HandleDef, (uint8_t*)c, 1, 1000) != HAL_OK)
		{
			RedLED_On();
		}

		else
		{
			RxBuff[i] = *(c++);
			i++;
		}

		if(i>=6)
		{
			if(strstr(&RxBuff, "\r\nOK\r\n") != NULL)
				messageState = SUCCESS;
			else if(strstr(&RxBuff,"\r\nO\r")!= NULL)
					messageState = SUCCESS;
			else if(strstr(&RxBuff, "\r\nERROR\r\n") != NULL)
				messageState = ERROR;
		}
	}*/
}




void UART_TransmitString(const char *str)
{
	UART_HandleTypeDef *UART_HandleDef = &UART_HandleDef_SPWF01SA;

	UART_Ready = RESET;

	while(*str)
	{
		while((UART_HandleDef->Instance->SR & UART_FLAG_TXE) == RESET);


		if(HAL_UART_Transmit(UART_HandleDef, (uint8_t*)str++, 1, 1000) != HAL_OK)
		{
			//ErrorHandler();
			//RedLED_On();
		}


		//while((UART_HandleDef->Instance->SR & UART_FLAG_TC) == RESET);
		//RedLED_Off();

	}


}

// TODO: !!!!

void UART_RecieveString(char *RxBufer)
{
	UART_HandleTypeDef *UART_HandleDef = &UART_HandleDef_SPWF01SA;

	if(HAL_UART_Receive(UART_HandleDef, (uint8_t*)RxBufer, 1, 5000) != HAL_OK)
	{
		ErrorHandler();
	}
}


void UART_InitConfig()
{
	/* Enable USART2 clock */
	__HAL_RCC_USART1_CLK_ENABLE();

	UART_HandleTypeDef *UART_HandleDef = &UART_HandleDef_SPWF01SA;

	/* Enable DMA1 clock */
	//__HAL_RCC_DMA2_CLK_ENABLE();

	/*##-1- Configure the UART peripheral ######################################*/
	/* Put the USART peripheral in the Asynchronous mode (UART Mode) */
	/* UART1 configured as follow:
	  - Word Length = 8 Bits
	  - Stop Bit = One Stop bit
	  - Parity = None
	  - BaudRate = 9600 baud
	  - Hardware flow control disabled (RTS and CTS signals) */

	UART_HandleDef->Instance = USART1;

	UART_HandleDef->Init.BaudRate = 115200;
	UART_HandleDef->Init.WordLength = UART_WORDLENGTH_8B;
	UART_HandleDef->Init.StopBits = UART_STOPBITS_1;
	UART_HandleDef->Init.Parity = UART_PARITY_NONE;
	UART_HandleDef->Init.HwFlowCtl = UART_HWCONTROL_NONE; //UART_HWCONTROL_RTS_CTS;
	UART_HandleDef->Init.Mode = UART_MODE_TX_RX;
	UART_HandleDef->Init.OverSampling = UART_OVERSAMPLING_16;

	if(HAL_UART_Init(UART_HandleDef) != HAL_OK)
	{
		ErrorHandler();
	}


	/*##-2a- Configure the DMA streams ##########################################*/

	/* Configure the DMA handler for Transmission process */
/*	DMA_TxHandleDef.Instance = DMA2_Stream7;

	DMA_TxHandleDef.Init.Channel = DMA_CHANNEL_4;
	DMA_TxHandleDef.Init.Direction = DMA_MEMORY_TO_PERIPH;
	DMA_TxHandleDef.Init.PeriphInc = DMA_PINC_DISABLE;
	DMA_TxHandleDef.Init.MemInc = DMA_MINC_ENABLE;
	DMA_TxHandleDef.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	DMA_TxHandleDef.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	DMA_TxHandleDef.Init.Mode = DMA_NORMAL;
	DMA_TxHandleDef.Init.Priority = DMA_PRIORITY_LOW;
	DMA_TxHandleDef.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	DMA_TxHandleDef.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
	DMA_TxHandleDef.Init.MemBurst = DMA_MBURST_INC4;
	DMA_TxHandleDef.Init.PeriphBurst = DMA_PBURST_INC4;

	HAL_DMA_Init(&DMA_TxHandleDef);*/

	/* Associate the initialized DMA handle to the the UART handle */
	//__HAL_LINKDMA(UART_HandleDef, hdmatx, DMA_TxHandleDef);


	/*##-2b- Configure the DMA streams ##########################################*/

	/* Configure the DMA handler for Reception process */
	/*DMA_RxHandleDef.Instance = DMA2_Stream5;

	DMA_RxHandleDef.Init.Channel = DMA_CHANNEL_4;
	DMA_RxHandleDef.Init.Direction = DMA_PERIPH_TO_MEMORY;
	DMA_RxHandleDef.Init.PeriphInc = DMA_PINC_DISABLE;
	DMA_RxHandleDef.Init.MemInc = DMA_MINC_ENABLE;
	DMA_RxHandleDef.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	DMA_RxHandleDef.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	DMA_RxHandleDef.Init.Mode = DMA_CIRCULAR;
	DMA_RxHandleDef.Init.Priority = DMA_PRIORITY_HIGH;
	DMA_RxHandleDef.Init.FIFOMode = DMA_FIFOMODE_DISABLE; //TODO: ENABLE?
	DMA_RxHandleDef.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
	DMA_RxHandleDef.Init.MemBurst = DMA_MBURST_INC4;
	DMA_RxHandleDef.Init.PeriphBurst = DMA_PBURST_INC4;

	HAL_DMA_Init(&DMA_RxHandleDef);*/

	/* Associate the initialized DMA handle to the the UART handle */
	//__HAL_LINKDMA(UART_HandleDef, hdmarx, DMA_RxHandleDef);


	/*##-4- Configure the NVIC for DMA #########################################*/
	/*NVIC configuration for DMA transfer complete interrupt (USARTx_TX) */
	/*HAL_NVIC_SetPriority(DMA2_Stream7_IRQn, 0, 1);
	HAL_NVIC_EnableIRQ(DMA2_Stream7_IRQn);*/

	/* NVIC configuration for DMA transfer complete interrupt (USARTx_RX) */
	/*HAL_NVIC_SetPriority(DMA2_Stream5_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA2_Stream5_IRQn);*/

	/* NVIC configuration for USART TC interrupt */
	HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(USART1_IRQn);

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







