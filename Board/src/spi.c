/*
 * spi.c
 *
 *  Created on: 2015 okt. 8
 *      Author: kecsger12
 */

/*================================[Includes]==========================================================================*/

#include "spi.h"

/*================================[Internal constants]================================================================*/

SPI_HandleTypeDef SPI_HandleDef_AD7792;		// SPI1
SPI_HandleTypeDef SPI_HandleDef_ILI9341;	// SPI4
SPI_HandleTypeDef SPI_HandleDef_RFM73;		// SPI2

/*================================[Function definitions]==============================================================*/
void SPI_Init_GPIO(GPIO_TypeDef  *GPIOx, uint32_t GPIO_PinMISO, uint32_t GPIO_PinMOSI, uint32_t GPIO_PinSCK, uint32_t GPIO_PinnSS)
{
	/* Enable clocks */
	GPIOx_ClockEnable(GPIOx);

	GPIO_InitTypeDef GPIO_InitStruct;

	/* SPI SCK GPIO pin configuration  */
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pin = GPIO_PinSCK;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	GPIO_InitStruct.Alternate = GPIO_AF_SPI;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);

	 /* SPI MISO GPIO pin configuration  */
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Pin = GPIO_PinMISO;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);

	/* SPI MOSI GPIO pin configuration  */
	GPIO_InitStruct.Pin = GPIO_PinMOSI;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);

	/* SPI nSS pin configuration */
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pin = GPIO_PinnSS;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;

	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

/*
 * Function to initialize SPI communication
 *
 * @param:
 * 		SPIx: number os the SPI interface beeing used
 * 		CLKPolarity:
 * 				- SPI_POLARITY_LOW
 * 				- SPI_POLARITY_HIGH
 *
 * 		CLKPhase:
 * 				- SPI_PHASE_1EDGE
 * 				- SPI_PHASE_2EDGE
 *
 * 		MAX_SPI_Frequency
 * 				MAX Frequency in which the given device is operates *
 */

void SPI_Init_Config(SPI_Periph_t SPIx_type, uint32_t CLKPolarity, uint32_t CLKPhase, uint32_t MAX_SPI_Frequency)
{
	HAL_StatusTypeDef SPI_Status;
	SPI_HandleTypeDef *SPI_HandleDef;
	SPI_TypeDef *SPIx;

	/*if(SPIx == SPI1)
		SPI_HandleDef = &SPI_HandleDef_AD7792;
	if(SPIx == SPI2)
		SPI_HandleDef = &SPI_HandleDef_RFM73;
	if(SPIx == SPI4)
		SPI_HandleDef = &SPI_HandleDef_ILI9341;
	*/

	if(SPIx_type == SPI_AD7792){
		SPI_HandleDef = &SPI_HandleDef_AD7792; SPIx = SPI1;
	}
	else if(SPIx_type == SPI_RFM73){
		SPI_HandleDef = &SPI_HandleDef_RFM73; SPIx = SPI2;
	}
	else if(SPIx_type == SPI_ILI9341){
		SPI_HandleDef = &SPI_HandleDef_ILI9341; SPIx = SPI4;
	}
	else if(SPIx_type == SPI_AD7843){
			SPI_HandleDef = &SPI_HandleDef_AD7843; SPIx = SPI4;
	}

		/* Enable clocks */
		SPIx_ClockEnable(SPIx);

		/* Configure SPIx interface for gived device ------------------------------------*/
		SPI_HandleDef->Instance = SPIx;

		SPI_HandleDef->Init.Direction = SPI_DIRECTION_2LINES;
		SPI_HandleDef->Init.Mode = SPI_MODE_MASTER;
		SPI_HandleDef->Init.DataSize = SPI_DATASIZE_8BIT;
		SPI_HandleDef->Init.CLKPolarity = CLKPolarity;
		SPI_HandleDef->Init.CLKPhase = CLKPhase;
		SPI_HandleDef->Init.NSS = SPI_NSS_SOFT;
		SPI_HandleDef->Init.BaudRatePrescaler = SPI_GetPrescaleraxFreq(SPIx, MAX_SPI_Frequency);
		SPI_HandleDef->Init.FirstBit = SPI_FIRSTBIT_MSB;
		SPI_HandleDef->Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
		SPI_HandleDef->Init.CRCPolynomial = 7;
		SPI_HandleDef->Init.TIMode = SPI_TIMODE_DISABLE;

		SPI_Status = HAL_SPI_Init(SPI_HandleDef);

		if(SPI_Status != HAL_OK)
		{
			ErrorHandler();
		}

}


uint32_t SPIx_Read(SPI_Periph_t SPIx_type, uint8_t size)
{
  HAL_StatusTypeDef status = HAL_OK;
  uint32_t readvalue = 0;
  SPI_HandleTypeDef *SPI_HandleDef;
  uint8_t option;

/*
	if(SPIx == SPI1)
		SPI_HandleDef = &SPI_HandleDef_AD7792;
	if(SPIx == SPI2)
		SPI_HandleDef = &SPI_HandleDef_RFM73;
	if(SPIx == SPI4)
		SPI_HandleDef = &SPI_HandleDef_ILI9341;
*/
	if(SPIx_type == SPI_AD7792){
		SPI_HandleDef = &SPI_HandleDef_AD7792;
		option = 1;
	}
	if(SPIx_type == SPI_RFM73){
		SPI_HandleDef = &SPI_HandleDef_RFM73;
		option = 1;
	}
	if(SPIx_type == SPI_ILI9341){
		SPI_HandleDef = &SPI_HandleDef_ILI9341;
		option = 0;
	}
	if(SPIx_type == SPI_AD7843){
			SPI_HandleDef = &SPI_HandleDef_AD7843;
			option = 1;
	}


	switch (option)
	{
	case 0:
		status = HAL_SPI_Receive_DMA(SPI_HandleDef, (uint8_t*)&readvalue, size);
		break;
	case 1:

	default:
		status = HAL_SPI_Receive(SPI_HandleDef, (uint8_t*)&readvalue, size, 1000);
		break;
	}

	/* Check the communication status */
	if(status != HAL_OK)
	{
		/* Execute user timeout callback */
		ErrorHandler();
	}

	return readvalue;
}


/**
  * @brief SPI Write a byte to device
  * @param Value: value to be written
  * @retval None
  */
void SPIx_Write(SPI_Periph_t SPIx_type, uint8_t Value)
{
  HAL_StatusTypeDef status = HAL_ERROR;
  uint8_t size = 1;
  uint32_t timeout = 1000;
  SPI_HandleTypeDef *SPI_HandleDef;

  uint8_t option;

/*
	if(SPIx == SPI1)
		SPI_HandleDef = &SPI_HandleDef_AD7792;
	if(SPIx == SPI2)
		SPI_HandleDef = &SPI_HandleDef_RFM73;
	if(SPIx == SPI4)
		SPI_HandleDef = &SPI_HandleDef_ILI9341;
*/
	if(SPIx_type == SPI_AD7792){
		SPI_HandleDef = &SPI_HandleDef_AD7792;
		option = 1;
	}
	if(SPIx_type == SPI_RFM73){
		SPI_HandleDef = &SPI_HandleDef_RFM73;
		option = 1;
	}
	if(SPIx_type == SPI_ILI9341){
		SPI_HandleDef = &SPI_HandleDef_ILI9341;
		option = 1;
	}
	if(SPIx_type == SPI_AD7843){
			SPI_HandleDef = &SPI_HandleDef_AD7843;
			option = 1;
	}

	switch (option)
	{
	case 0:
		status = HAL_SPI_Transmit_DMA(SPI_HandleDef, &Value, size);
		break;
	case 1:

	default:
		status = HAL_SPI_Transmit(SPI_HandleDef, (uint8_t*)&Value, size, timeout);
		break;
	}


	/* Check the communication status */
	if(status != HAL_OK)
	{
		/* Execute user timeout callback */
		//ErrorHandler();
	}
}



/* This function calculates the maximum frequency which
 * 	can be used with the given device
 */

uint16_t SPI_GetPrescaleraxFreq(SPI_TypeDef* SPIx, uint32_t MAX_SPI_Frequency)
{
	uint32_t APB2_Freq = HAL_RCC_GetPCLK2Freq();

	int i;
	for (i = 0; i < 7; i++)
	{
		if (APB2_Freq / (1 << (i + 1)) <= MAX_SPI_Frequency)
		{
			/* Bits for BP are 5:3 in CR1 register */
			return (i << 3);
		}
	}

	/* Use max prescaler possible */
	return SPI_BAUDRATEPRESCALER_256;

}

void SPIx_ClockEnable(SPI_TypeDef *SPIx)
{
	if(SPIx == SPI1)
		__HAL_RCC_SPI1_CLK_ENABLE();
	if(SPIx == SPI2)
		__HAL_RCC_SPI2_CLK_ENABLE();
	if(SPIx == SPI3)
		__HAL_RCC_SPI3_CLK_ENABLE();
	if(SPIx == SPI4)
		__HAL_RCC_SPI4_CLK_ENABLE();
	if(SPIx == SPI5)
		__HAL_RCC_SPI5_CLK_ENABLE();
	if(SPIx == SPI6)
		__HAL_RCC_SPI6_CLK_ENABLE();
}

void GPIOx_ClockEnable(GPIO_TypeDef  *GPIOx)
{
	if(GPIOx == GPIOA)
		__HAL_RCC_GPIOA_CLK_ENABLE();
	if(GPIOx == GPIOB)
			__HAL_RCC_GPIOB_CLK_ENABLE();
	if(GPIOx == GPIOC)
			__HAL_RCC_GPIOC_CLK_ENABLE();
	if(GPIOx == GPIOD)
			__HAL_RCC_GPIOD_CLK_ENABLE();
	if(GPIOx == GPIOE)
			__HAL_RCC_GPIOE_CLK_ENABLE();
}

/* ------------------------------------------------------------------------------------------------------ */

/* Function to send and receive a byte over SPI channel */
uint8_t SPI_Write(SPI_HandleTypeDef SPI_HandleDef, uint8_t data)
{
	int32_t spixbase = 0x00;
	SPI_TypeDef *SPIx = SPI_HandleDef.Instance;

	// Check if SPI is enabled
	//SPI_Check_Enabled(SPI_HandleDef, SPIx);

	 // Wait for previous transmissions to complete if DMA TX enabled for SPI
	SPI_Wait_TX(SPIx);

	 // Fill output buffer with data
	spixbase = (uint32_t)SPIx;
	spixbase += 0x0C;
	/**(__IO uint8_t *)&*/SPIx->DR = data;

	 // Wait for transmission to complete
	//SPI_Wait_RX(SPIx);

	 // Return data from buffer
	//return SPIx->DR;
}

void SPI_Wait_TX(SPI_TypeDef *SPIx)
{
	while ((SPIx->SR & SPI_FLAG_TXE) == 0 || (SPIx->SR & SPI_FLAG_BSY))
	{};

}

void SPI_Wait_RX(SPI_TypeDef *SPIx)
{
	while ((SPIx->SR & SPI_FLAG_RXNE) == 0 || (SPIx->SR & SPI_FLAG_BSY))
	{};

}

/* Checks if SPI is enabled and returns value from function if not */
void SPI_Check_Enabled(SPI_HandleTypeDef SPI_HandleDef, SPI_TypeDef *SPIx)
{
	// Check if the SPI is already enabled
	if((SPIx->CR1 &SPI_CR1_SPE) != SPI_CR1_SPE)
	{
		// Enable SPI peripheral
		__HAL_SPI_ENABLE(&SPI_HandleDef);
	}
}
















