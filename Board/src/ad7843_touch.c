/*
 * ad7843_touch.c
 *
 *  Created on: 2015 okt. 26
 *      Author: kecsger12
 */

/*================================[Includes] ======================================================================================*/

#include "ad7843_touch.h"
#include "ili9341.h"

/*================================[Internal constants]================================================================*/

SPI_HandleTypeDef SPI_HandleDef_AD7843;

uint8_t isAD7843_Enabled;

/*================================[Macros]==============================================================*/


/*================================[Internal functions]==============================================================*/



/*================================[Function definitions]==============================================================*/

void AD7843_Init()
{
	/* Init the BSY and /PENIRQ pins */
	AD7843_GPIO_Init();

	/* Init the communication interface for AD7843 */
	//AD7843_SPI_GPIOInit();
	//AD7843_SPI_ConfigInit();
	SPI_Init_GPIO(AD7843_SPI_PORT, AD7843_MISO, AD7843_MOSI, AD7843_SCK, AD7843_nSS);
	SPI_Init_Config(SPI_AD7843, SPI_POLARITY_LOW, SPI_PHASE_1EDGE, 1000000);

	AD7843_SPI_nSS(1);

	isAD7843_Enabled = 1;
}

void AD7843_SPI_ConfigInit()
{
	HAL_StatusTypeDef SPI_Status;

	if(HAL_SPI_GetState(&SPI_HandleDef_AD7843) == HAL_SPI_STATE_RESET)
	{
		/* Enable clocks */
		__HAL_RCC_SPI4_CLK_ENABLE();

		/* Configure SPIx interface for gived device ------------------------------------*/
		SPI_HandleDef_AD7843.Instance = AD7843_SPI;

		SPI_HandleDef_AD7843.Init.Direction = SPI_DIRECTION_2LINES;
		SPI_HandleDef_AD7843.Init.Mode = SPI_MODE_MASTER;
		SPI_HandleDef_AD7843.Init.DataSize = SPI_DATASIZE_8BIT;
		SPI_HandleDef_AD7843.Init.CLKPolarity = SPI_POLARITY_LOW;
		SPI_HandleDef_AD7843.Init.CLKPhase = SPI_PHASE_1EDGE;
		SPI_HandleDef_AD7843.Init.NSS = SPI_NSS_SOFT;
		SPI_HandleDef_AD7843.Init.BaudRatePrescaler = SPI_GetPrescaleraxFreq(AD7843_SPI, 1000000);
		SPI_HandleDef_AD7843.Init.FirstBit = SPI_FIRSTBIT_MSB;
		SPI_HandleDef_AD7843.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
		SPI_HandleDef_AD7843.Init.CRCPolynomial = 7;
		SPI_HandleDef_AD7843.Init.TIMode = SPI_TIMODE_DISABLE;

		SPI_Status = HAL_SPI_Init(&SPI_HandleDef_AD7843);

		if(SPI_Status != HAL_OK)
		{
			ErrorHandler();
		}
	}
}

void AD7843_SPI_GPIOInit()
{
	/* Enable clocks */
	__HAL_RCC_GPIOE_CLK_ENABLE();

	GPIO_InitTypeDef GPIO_InitStruct;

	/* SPI SCK GPIO pin configuration  */
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pin = AD7843_SCK;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	GPIO_InitStruct.Alternate = GPIO_AF_SPI;
	HAL_GPIO_Init(AD7843_SPI_PORT, &GPIO_InitStruct);

	 /* SPI MISO GPIO pin configuration  */
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Pin = AD7843_MISO;
	HAL_GPIO_Init(AD7843_SPI_PORT, &GPIO_InitStruct);

	/* SPI MOSI GPIO pin configuration  */
	GPIO_InitStruct.Pin = AD7843_MOSI;
	HAL_GPIO_Init(AD7843_SPI_PORT, &GPIO_InitStruct);

	/* SPI nSS pin configuration */
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pin = AD7843_nSS;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;

	HAL_GPIO_Init(AD7843_SPI_PORT, &GPIO_InitStruct);
}


uint16_t AD7843_GetTouch_X()
{
	if(HAL_SPI_GetState(&SPI_HandleDef_ILI9341) != HAL_SPI_STATE_RESET)
	{
		ILI9341_DeInit();
		AD7843_Init();
	}

	uint16_t x_coordinate = 0;

	AD7843_SPI_nSS(0);

	AD7843_Write(AD7843_CH_X);
	//while(AD7843_isBusy());

	x_coordinate = AD7843_Read();
	//while(AD7843_isBusy());

	AD7843_SPI_nSS(1);

	return x_coordinate * (320.0/127.0);
}


uint16_t AD7843_GetTouch_Y()
{

	uint16_t y_coordinate = 0;

	AD7843_SPI_nSS(0);

	AD7843_Write(AD7843_CH_Y);
	//while(AD7843_isBusy());

	y_coordinate = AD7843_Read();
	//while(AD7843_isBusy());

	AD7843_SPI_nSS(1);

	if(HAL_SPI_GetState(&SPI_HandleDef_AD7843) != HAL_SPI_STATE_RESET)
	{
		AD7843_DeInit();
		SPI_Init_Config(SPI_ILI9341, SPI_POLARITY_LOW, SPI_PHASE_1EDGE, 11000000);
		SPI_Init_GPIO(ILI9341_GPIO_PORT, ILI9341_MISO, ILI9341_MOSI, ILI9341_SCK, ILI9341_nSS);
	}

	return 240 - (y_coordinate * (240.0/127.0));
}

void AD7843_Write(uint8_t value)
{

	HAL_StatusTypeDef status = HAL_OK;

	uint8_t Value = (AD7843_START |	value |	AD7843_MODE_8BIT |	AD7843_DIFF_REF_SELECT | AD7843_PWRUP_PENIRQ_EN);

	SPIx_Write(SPI_AD7843, Value);
	//status = HAL_SPI_Transmit(&SPI_HandleDef_AD7843, &Value, 1, 1000);
	/* Check the communication status */
	if(status != HAL_OK)
	{
		/* Execute user timeout callback */
		ErrorHandler();
	}
}

uint16_t AD7843_Read()
{
	if(HAL_SPI_GetState(&SPI_HandleDef_ILI9341) == HAL_SPI_STATE_READY)
	{
		ILI9341_DeInit();
		AD7843_Init();
	}

	HAL_StatusTypeDef status = HAL_OK;
	uint8_t retVal = 0;

	retVal = SPIx_Read(SPI_AD7843, 1);

	//status = HAL_SPI_Receive(&SPI_HandleDef_AD7843, (uint8_t*)&retVal, 1, 1000);
	/* Check the communication status */
	if(status != HAL_OK)
	{
		/* Execute user timeout callback */
		ErrorHandler();
	}

	return retVal;
}

void AD7843_SPI_nSS(uint8_t BitVal)
{
	HAL_GPIO_WritePin(AD7843_SPI_PORT, AD7843_nSS, BitVal);
}



uint8_t AD7843_isBusy()
{
	return (HAL_GPIO_ReadPin(AD7843_GPIO_PORT, AD7843_BUSY));
}


void AD7843_GPIO_Init()
{
	GPIO_InitTypeDef GPIO_InitStruct;

	__HAL_RCC_GPIOC_CLK_ENABLE();

	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pin = AD7843_BUSY;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;

	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/* Set up /PENIRQ pin as External Interrupt Line */
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pin = AD7843_PENIRQ;

	HAL_GPIO_Init(AD7843_GPIO_PORT, &GPIO_InitStruct);

	/* Enable and set EXTI Line9:5 Interrupt to the lowest priority */
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_1);
	HAL_NVIC_SetPriority(EXTI9_5_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
}

void AD7843_DeInit()
{
	__HAL_RCC_SPI4_FORCE_RESET();
	__HAL_RCC_SPI4_RELEASE_RESET();

	HAL_GPIO_DeInit(AD7843_SPI_PORT, AD7843_SCK);
	HAL_GPIO_DeInit(AD7843_SPI_PORT, AD7843_MISO);
	HAL_GPIO_DeInit(AD7843_SPI_PORT, AD7843_MOSI);

	HAL_SPI_DeInit(&SPI_HandleDef_AD7843);

	isAD7843_Enabled = 0;
}


