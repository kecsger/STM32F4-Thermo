/*
 * ili9341.c
 *
 *  Created on: 2015 okt. 12
 *      Author: kecsger12
 */

#include "ili9341.h"
#include "led.h"



typedef struct {
	uint16_t width;
	uint16_t height;
	ILI9341_Orientation_t orientation; // 1 = portrait; 0 = landscape
} TM_ILI931_Options_t;



/* Private structure */
typedef struct {
	uint32_t TX_Channel;
	DMA_Stream_TypeDef* TX_Stream;
	uint32_t RX_Channel;
	DMA_Stream_TypeDef* RX_Stream;
	uint32_t Dummy32;
	uint16_t Dummy16;
} TM_SPI_DMA_INT_t;

/* Pin functions */
uint16_t ILI9341_x;
uint16_t ILI9341_y;
TM_ILI931_Options_t ILI9341_Opts;
uint8_t ILI9341_INT_CalledFromPuts = 0;


static TM_SPI_DMA_INT_t SPI4_DMA_INT = {SPI4_DMA_TX_CHANNEL, SPI4_DMA_TX_STREAM, SPI4_DMA_RX_CHANNEL, SPI4_DMA_RX_STREAM};
uint8_t isILI9341_Enabled;


static DMA_HandleTypeDef DMA_HandleDef_TX;
static DMA_HandleTypeDef DMA_HandleDef_RX;

/* ===================================================================================== */

uint16_t ILI9341_Init()
{
	uint16_t devicetype;
	isILI9341_Enabled = 1;

	/* Init D/C, Reset, backlight pins*/
	ILI9341_GPIO_Init();

	/* Set LCD BackLight 100% ON --->TODO */
	//ILI9341_BackLightON();

	/* Init SPI */
	SPI_Init_GPIO(ILI9341_GPIO_PORT, ILI9341_MISO, ILI9341_MOSI, ILI9341_SCK, ILI9341_nSS);
	SPI_Init_Config(SPI_ILI9341, SPI_POLARITY_LOW, SPI_PHASE_1EDGE, SPI_DATASIZE_8BIT, 10000000);

	/* CS high */
	ILI9341_SPI_nSS(1);
	HAL_Delay(1);

	/* Init DMA for SPI */
	ILI9341_SPI_DMA_Init();

	/* Init LCD */
	ILI9341_InitLCD();

	/* Set default settings */
	ILI9341_x = ILI9341_y = 0;
	ILI9341_Opts.width = ILI9341_WIDTH;
	ILI9341_Opts.height = ILI9341_HEIGHT;
	ILI9341_Opts.orientation = ILI9341_Orientation_Landscape_1;

	ILI9341_Rotate(ILI9341_Orientation_Landscape_1);

	//TM_ILI9341_INT_Fill(0, 0, 320, 240, ILI9341_COLOR_BLACK);

	//ILI9341_Fill(0, 0, ILI9341_HEIGHT, ILI9341_WIDTH, ILI9341_COLOR_WHITE);

	//devicetype = ILI9341_ReadID();
	return devicetype;
}



void ILI9341_InitLCD(void)
{
	/* Force reset */
	ILI9341_Reset();

	/* Software reset */
	ILI9341_SendCommand(ILI9341_SWRESET);
	HAL_Delay(120);

	ILI9341_SendCommand(ILI9341_POWERA);
	ILI9341_SendData(0x39);
	ILI9341_SendData(0x2C);
	ILI9341_SendData(0x00);
	ILI9341_SendData(0x34);
	ILI9341_SendData(0x02);
	ILI9341_SendCommand(ILI9341_POWERB);
	ILI9341_SendData(0x00);
	ILI9341_SendData(0xC1);
	ILI9341_SendData(0x30);
	ILI9341_SendCommand(ILI9341_DTCA);
	ILI9341_SendData(0x84);
	ILI9341_SendData(0x00);
	ILI9341_SendData(0x7A);
	ILI9341_SendCommand(ILI9341_DTCB);
	ILI9341_SendData(0x00);
	ILI9341_SendData(0x00);
	ILI9341_SendCommand(ILI9341_POWER_SEQ);
	ILI9341_SendData(0x64);
	ILI9341_SendData(0x03);
	ILI9341_SendData(0x12);
	ILI9341_SendData(0x81);
	ILI9341_SendCommand(ILI9341_PRC);
	ILI9341_SendData(0x20);
	ILI9341_SendCommand(ILI9341_POWER1);
	ILI9341_SendData(0x23);
	ILI9341_SendCommand(ILI9341_POWER2);
	ILI9341_SendData(0x10);
	ILI9341_SendCommand(ILI9341_VCOM1);
	ILI9341_SendData(0x3E);
	ILI9341_SendData(0x28);
	ILI9341_SendCommand(ILI9341_VCOM2);
	ILI9341_SendData(0x86);
	ILI9341_SendCommand(ILI9341_MAC);
	ILI9341_SendData(0x48);
	ILI9341_SendCommand(ILI9341_PIXEL_FORMAT);
	ILI9341_SendData(0x55);
	ILI9341_SendCommand(ILI9341_FRC);
	ILI9341_SendData(0x00);
	ILI9341_SendData(0x1B);
	ILI9341_SendCommand(ILI9341_DFC);
	ILI9341_SendData(0x08);
	ILI9341_SendData(0x82);
	ILI9341_SendData(0x27);
	ILI9341_SendCommand(ILI9341_3GAMMA_EN);
	ILI9341_SendData(0x00);
	ILI9341_SendCommand(ILI9341_COLUMN_ADDR);
	ILI9341_SendData(0x00);
	ILI9341_SendData(0x00);
	ILI9341_SendData(0x00);
	ILI9341_SendData(0xEF);
	ILI9341_SendCommand(ILI9341_PAGE_ADDR);
	ILI9341_SendData(0x00);
	ILI9341_SendData(0x00);
	ILI9341_SendData(0x01);
	ILI9341_SendData(0x3F);
	ILI9341_SendCommand(ILI9341_GAMMA);
	ILI9341_SendData(0x01);
	ILI9341_SendCommand(ILI9341_PGAMMA);
	ILI9341_SendData(0x0F);
	ILI9341_SendData(0x31);
	ILI9341_SendData(0x2B);
	ILI9341_SendData(0x0C);
	ILI9341_SendData(0x0E);
	ILI9341_SendData(0x08);
	ILI9341_SendData(0x4E);
	ILI9341_SendData(0xF1);
	ILI9341_SendData(0x37);
	ILI9341_SendData(0x07);
	ILI9341_SendData(0x10);
	ILI9341_SendData(0x03);
	ILI9341_SendData(0x0E);
	ILI9341_SendData(0x09);
	ILI9341_SendData(0x00);
	ILI9341_SendCommand(ILI9341_NGAMMA);
	ILI9341_SendData(0x00);
	ILI9341_SendData(0x0E);
	ILI9341_SendData(0x14);
	ILI9341_SendData(0x03);
	ILI9341_SendData(0x11);
	ILI9341_SendData(0x07);
	ILI9341_SendData(0x31);
	ILI9341_SendData(0xC1);
	ILI9341_SendData(0x48);
	ILI9341_SendData(0x08);
	ILI9341_SendData(0x0F);
	ILI9341_SendData(0x0C);
	ILI9341_SendData(0x31);
	ILI9341_SendData(0x36);
	ILI9341_SendData(0x0F);
	ILI9341_SendCommand(ILI9341_SLEEP_OUT);

	HAL_Delay(100);

	ILI9341_SendCommand(ILI9341_DISPLAY_ON);
	//ILI9341_SendCommand(ILI9341_GRAM);
}

void ILI9341_Reset()
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);  	// nRESET(1)
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);  // nRESET(0)
	/* Delay for RST response */
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);		// nRESET(1)

	/* It is necessary to wait 5msec after releasing /RES before sending commands */
	HAL_Delay(1);
}

uint16_t ILI9341_ReadID()
{
	uint16_t val;
	uint16_t temp;

	uint8_t reg = 0xD3;

	ILI9341_SendCommand(0xD9);
	ILI9341_SendData(0x11);
	temp = ILI9341_ReadReg(reg, 1);

	ILI9341_SendCommand(0xD9);
	ILI9341_SendData(0x12);
	temp = ILI9341_ReadReg(reg, 1);

	ILI9341_SendCommand(0xD9);
	ILI9341_SendData(0x13);
	val = ILI9341_ReadReg(reg, 1);
	ILI9341_ReadReg(reg, 1);

	val &= 0x00FF;
	val |= (uint16_t)temp<<8;

	return val;
}

/*=======================================================================================*/

void ILI9341_SendData(uint8_t data)
{
/*
	if(HAL_SPI_GetState(&SPI_HandleDef_AD7843) != HAL_SPI_STATE_RESET)
	{
		AD7843_DeInit();
		SPI_Init_Config(SPI_ILI9341, SPI_POLARITY_LOW, SPI_PHASE_1EDGE, 11000000);
		SPI_Init_GPIO(ILI9341_GPIO_PORT, ILI9341_MISO, ILI9341_MOSI, ILI9341_SCK, ILI9341_nSS);
	}
*/

	ILI9341_DC(1);
	ILI9341_SPI_nSS(0);

	/*TODO: BUSY figyeles!! */
	SPIx_Write(SPI_ILI9341, data);

	ILI9341_SPI_nSS(1);
}

void ILI9341_SendMultipleData(uint16_t pData)
{
	uint32_t counter = 0;

	ILI9341_DC(1);
	ILI9341_SPI_nSS(0);

	 uint8_t size = 2;
	 uint32_t timeout = 1000;

	 HAL_SPI_Transmit(&SPI_HandleDef_ILI9341, (uint8_t*)&pData, size, timeout);

	 ILI9341_SPI_nSS(1);
}

void ILI9341_SendCommand(uint8_t data)
{
/*
	if(HAL_SPI_GetState(&SPI_HandleDef_AD7843) != HAL_SPI_STATE_RESET)
	{
		AD7843_DeInit();
		SPI_Init_Config(SPI_ILI9341, SPI_POLARITY_LOW, SPI_PHASE_1EDGE, 11000000);
		SPI_Init_GPIO(ILI9341_GPIO_PORT, ILI9341_MISO, ILI9341_MOSI, ILI9341_SCK, ILI9341_nSS);
	}
*/

	ILI9341_DC(0);
	ILI9341_SPI_nSS(0);

	/*TODO: BUSY figyeles!! */
	SPIx_Write(SPI_ILI9341, data);

	ILI9341_SPI_nSS(1);
	ILI9341_DC(1);
}

uint32_t ILI9341_ReadReg(uint8_t reg, uint8_t size)
{
	uint16_t tempReg = 0;
	uint32_t regVal = 0;
	int i;

/*
	if(HAL_SPI_GetState(&SPI_HandleDef_AD7843) == HAL_SPI_STATE_READY)
	{
		AD7843_DeInit();
		ILI9341_Init();
	}

*/

	/*for(i=0; i<size; i++)
	{
		ILI9341_SendCommand(0xD9);		// ---> ????
		ILI9341_SendData(0x10 + i+1);

		ILI9341_SendCommand(reg);
		ILI9341_DC(1);

		tempReg = SPIx_Read(1);
		regVal = (regVal << 8) + tempReg;
		HAL_Delay(1);
	}*/

	ILI9341_SPI_nSS(0);
	ILI9341_DC(0);

	SPIx_Write(SPI_ILI9341, reg);

	ILI9341_DC(1);
	tempReg = SPIx_Read(SPI_ILI9341, size);

	ILI9341_SPI_nSS(1);
	return tempReg;
}


void ILI9341_BackLightON()
{
	//ILI9341_GPIO_Init();

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
}

void ILI9341_BackLightOFF()
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
}

/* ------------------------------------------------------------------------------------------------------ */

TM_SPI_DataSize_t TM_SPI_SetDataSize(SPI_TypeDef* SPIx, TM_SPI_DataSize_t DataSize)
{
	TM_SPI_DataSize_t status = (SPIx->CR1 & SPI_CR1_DFF) ? TM_SPI_DataSize_16b : TM_SPI_DataSize_8b;

	/* Disable SPI first */
	SPIx->CR1 &= ~SPI_CR1_SPE;

	/* Set proper value */
	if (DataSize == TM_SPI_DataSize_16b) {
		/* Set bit for frame */
		SPIx->CR1 |= SPI_CR1_DFF;
	} else {
		/* Clear bit for frame */
		SPIx->CR1 &= ~SPI_CR1_DFF;
	}

	/* Enable SPI back */
	SPIx->CR1 |= SPI_CR1_SPE;

	/* Return status */
	return status;
}


void ILI9341_SPI_DMA_Init()
{
	//DMA_HandleTypeDef DMA_HandleDef_TX = DMA_HandleDef_TX;;
	//DMA_HandleTypeDef DMA_HandleDef_RX = DMA_HandleDef_RX;

	SPI_HandleTypeDef *hspi = &SPI_HandleDef_ILI9341;

	/* Enable DMA2 clock */
	__HAL_RCC_DMA2_CLK_ENABLE();

	/* Configure the DMA handler for Transmission process */
	DMA_HandleDef_TX.Instance = DMA2_Stream1;

	DMA_HandleDef_TX.Init.Channel 				= DMA_CHANNEL_4;
	DMA_HandleDef_TX.Init.Direction 			= DMA_MEMORY_TO_PERIPH;
	DMA_HandleDef_TX.Init.PeriphInc 			= DMA_PINC_DISABLE;
	DMA_HandleDef_TX.Init.MemInc 				= DMA_MINC_ENABLE;
	DMA_HandleDef_TX.Init.PeriphDataAlignment	= DMA_PDATAALIGN_BYTE;
	DMA_HandleDef_TX.Init.MemDataAlignment 		= DMA_MDATAALIGN_BYTE;
	DMA_HandleDef_TX.Init.Mode 					= DMA_NORMAL;
	DMA_HandleDef_TX.Init.Priority 				= DMA_PRIORITY_MEDIUM;
	DMA_HandleDef_TX.Init.FIFOMode 				= DMA_FIFOMODE_DISABLE;
	DMA_HandleDef_TX.Init.FIFOThreshold 		= DMA_FIFO_THRESHOLD_FULL;
	DMA_HandleDef_TX.Init.MemBurst 				= DMA_MBURST_SINGLE;
	DMA_HandleDef_TX.Init.PeriphBurst 			= DMA_PBURST_SINGLE;

	//DMA_HandleDef_TX.StreamBaseAddress = (uint32_t)&(hspi->Instance->DR);

	HAL_DMA_Init(&DMA_HandleDef_TX);


	/* Associate the initialized DMA handle to the the SPI handle */
	__HAL_LINKDMA(hspi, hdmatx, DMA_HandleDef_TX);



	/* Configure the DMA handler for Recieving process */
	DMA_HandleDef_RX.Instance = DMA2_Stream0;

	DMA_HandleDef_RX.Init.Channel 				= DMA_CHANNEL_4;
	DMA_HandleDef_RX.Init.Direction 			= DMA_PERIPH_TO_MEMORY;
	DMA_HandleDef_RX.Init.PeriphInc 			= DMA_PINC_ENABLE;
	DMA_HandleDef_RX.Init.MemInc 				= DMA_MINC_ENABLE;
	DMA_HandleDef_RX.Init.PeriphDataAlignment	= DMA_PDATAALIGN_BYTE;
	DMA_HandleDef_RX.Init.MemDataAlignment 		= DMA_PDATAALIGN_BYTE;
	DMA_HandleDef_RX.Init.Mode 					= DMA_NORMAL;
	DMA_HandleDef_RX.Init.Priority 				= DMA_PRIORITY_HIGH;
	DMA_HandleDef_RX.Init.FIFOMode 				= DMA_FIFOMODE_ENABLE;
	DMA_HandleDef_RX.Init.FIFOThreshold 		= DMA_FIFO_THRESHOLD_FULL;
	DMA_HandleDef_RX.Init.MemBurst 				= DMA_MBURST_INC16;
	DMA_HandleDef_RX.Init.PeriphBurst 			= DMA_MBURST_INC16;

	HAL_DMA_Init(&DMA_HandleDef_RX);

	/* Associate the initialized DMA handle to the the SPI handle */
	__HAL_LINKDMA(hspi, hdmarx, DMA_HandleDef_RX);


	 /* NVIC configuration for DMA transfer complete interrupt (SPI4_TX) */
	  HAL_NVIC_SetPriority(DMA2_Stream1_IRQn, 0, 1);
	  HAL_NVIC_EnableIRQ(DMA2_Stream1_IRQn);

	  /* NVIC configuration for DMA transfer complete interrupt (SPI4_RX) */
	  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
	  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
}


uint8_t TM_SPI_DMA_SendHalfWord(SPI_TypeDef* SPIx, uint16_t value, uint16_t count)
{
	/* Get SPI4 settings*/
	TM_SPI_DMA_INT_t* Settings = &SPI4_DMA_INT;

	//DMA_HandleTypeDef DMA_HandleDef = DMA_HandleDef_TX;

	/* Check if DMA available*/
	if (Settings->TX_Stream->NDTR) {
		return 0;
	}

	/* Set dummy memory to value we specify*/
	Settings->Dummy16 = value;



	/* Set DMA peripheral address, number of bytes*/
	if(HAL_DMA_Start(&DMA_HandleDef_TX, (uint32_t) Settings->Dummy16, (uint32_t)SPIx->DR, count) != HAL_OK)
	{
		RedLED_Toggle();
	}


	DMA_HandleDef_TX.Init.MemInc = DMA_MINC_DISABLE;

	/* Configure TX DMA*/
	DMA_HandleDef_TX.Init.Channel = SPI4_DMA_TX_CHANNEL;
	DMA_HandleDef_TX.Init.Direction = DMA_MEMORY_TO_PERIPH;

	/* Set memory size */
	DMA_HandleDef_TX.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
	DMA_HandleDef_TX.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;

	/* Deinit first TX stream */
	HAL_DMA_DeInit(&DMA_HandleDef_TX);

	/* Init TX stream */
	HAL_DMA_Init(&DMA_HandleDef_TX);

	/* Enable TX stream*/
	Settings->TX_Stream->CR |= DMA_SxCR_EN;

	/* Enable SPI TX DMA*/
	SPIx->CR2 |= SPI_CR2_TXDMAEN;

	/* Return OK*/
	return 1;
}

uint8_t TM_SPI_DMA_Working(SPI_TypeDef* SPIx)
{
	/* Get SPI settings*/
	TM_SPI_DMA_INT_t* Settings = &SPI4_DMA_INT;

	/* Check if TX or RX DMA are working*/
	/* Check if TX or RX DMA are working */
	return (
		Settings->RX_Stream->NDTR || /*!< RX is working */
		Settings->TX_Stream->NDTR || /*!< TX is working */
		SPI_IS_BUSY(SPIx)            /*!< SPI is busy */
	);
}

void TM_ILI9341_INT_Fill(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
{
	uint32_t pixels_count;

	/* Set cursor position */
	ILI9341_SetCursorPosition(x0, y0, x1, y1);

	/* Set command for GRAM data */
	ILI9341_SendCommand(ILI9341_GRAM);

	/* Calculate pixels count */
	pixels_count = (x1 - x0 +1) * (y1 - y0 +1);

	/* Send everything */
	ILI9341_DC(1);
	ILI9341_SPI_nSS(0);

	/* Go to 16-bit SPI mode */
	TM_SPI_SetDataSize(ILI9341_SPI, TM_SPI_DataSize_16b);

	/* Check again */
	while (pixels_count > 0xFFFF)
	{
		/* Send first 65535 bytes, SPI MUST BE IN 16-bit MODE */
		TM_SPI_DMA_SendHalfWord(ILI9341_SPI, color, 0xFFFF);

		pixels_count -= 0xFFFF;

		/* Wait till done */
		while (TM_SPI_DMA_Working(ILI9341_SPI));
	}

	/* Send first 65535 bytes, SPI MUST BE IN 16-bit MODE */
	TM_SPI_DMA_SendHalfWord(ILI9341_SPI, color, pixels_count);

	/* Wait till done */
	while (TM_SPI_DMA_Working(ILI9341_SPI));

	ILI9341_SPI_nSS(1);;

	/* Go back to 8-bit SPI mode */
	TM_SPI_SetDataSize(ILI9341_SPI, TM_SPI_DataSize_8b);

}


/*=======================================================================================*/

void ILI9341_SetCursorPosition(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	ILI9341_SendCommand(ILI9341_COLUMN_ADDR);
	ILI9341_SendData(x1 >> 8);
	ILI9341_SendData(x1 & 0xFF);
	ILI9341_SendData(x2 >> 8);
	ILI9341_SendData(x2 & 0xFF);

	ILI9341_SendCommand(ILI9341_PAGE_ADDR);
	ILI9341_SendData(y1 >> 8);
	ILI9341_SendData(y1 & 0xFF);
	ILI9341_SendData(y2 >> 8);
	ILI9341_SendData(y2 & 0xFF);
}

void ILI9341_DrawPixel(uint16_t x, uint16_t y, uint16_t color)
{
	ILI9341_SetCursorPosition(x, y, x, y);

	ILI9341_SendCommand(ILI9341_GRAM);
	ILI9341_SendData(color >> 8);
	ILI9341_SendData(color & 0xFF);

	//ILI9341_SendMultipleData(color);
	//TM_SPI_DMA_SendHalfWord(SPI_HandleDef_ILI9341.Instance, color, 2);
}

void ILI9341_Putc(uint16_t x, uint16_t y, char c, TM_FontDef_t *font, uint16_t foreground, uint16_t background)
{
	uint32_t i, b, j;
	ILI9341_x = x;
	ILI9341_y = y;

	if ((ILI9341_x + font->FontWidth) > ILI9341_Opts.width)
	{
		//If at the end of a line of display, go to new line and set x to 0 position
		ILI9341_y += font->FontHeight;
		ILI9341_x = 0;
	}

	for (i = 0; i < font->FontHeight; i++)
	{
		b = font->data[(c - 32) * font->FontHeight + i];

		for (j = 0; j < font->FontWidth; j++)
		{
			if ((b << j) & 0x8000)
			{
				ILI9341_DrawPixel(ILI9341_x + j, (ILI9341_y + i), foreground);
			}
			else
			{
				ILI9341_DrawPixel(ILI9341_x + j, (ILI9341_y + i), background);
			}
		}
	}
	ILI9341_x += font->FontWidth;
}

void ILI9341_Puts(uint16_t x, uint16_t y, char *str, TM_FontDef_t *font, uint16_t foreground, uint16_t background)
{
	uint16_t startX = x;
	ILI9341_x = x;
	ILI9341_y = y;

	while (*str)
	{
		//New line
		if (*str == '\n')
		{
			ILI9341_y += font->FontHeight + 1;
			//if after \n is also \r, than go to the left of the screen

			if (*(str + 1) == '\r')
			{
				ILI9341_x = 0;
				str++;
			}
			else
			{
				ILI9341_x = startX;
			}
			str++;
			continue;

		}
		else if (*str == '\r')
		{
			str++;
			continue;
		}

		ILI9341_Putc(ILI9341_x, ILI9341_y, *str++, font, foreground, background);
	}
}



void ILI9341_Fill(int x0, int y0, int x1, int y1, uint32_t color)
{
	unsigned int n, i, j;
	i = color >> 8;
	j = color & 0xFF;

	ILI9341_SetCursorPosition(x0, y0, x1, y1);

	ILI9341_SendCommand(ILI9341_GRAM);

	for (n = 0; n < ILI9341_PIXEL; n++)
	{
		ILI9341_SendData(i);
		ILI9341_SendData(j);
	}
}



void ILI9341_DrawCircle(int16_t x0, int16_t y0, int16_t r, uint32_t color) {
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

    ILI9341_DrawPixel(x0, y0 + r, color);
    ILI9341_DrawPixel(x0, y0 - r, color);
    ILI9341_DrawPixel(x0 + r, y0, color);
    ILI9341_DrawPixel(x0 - r, y0, color);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        ILI9341_DrawPixel(x0 + x, y0 + y, color);
        ILI9341_DrawPixel(x0 - x, y0 + y, color);
        ILI9341_DrawPixel(x0 + x, y0 - y, color);
        ILI9341_DrawPixel(x0 - x, y0 - y, color);

        ILI9341_DrawPixel(x0 + y, y0 + x, color);
        ILI9341_DrawPixel(x0 - y, y0 + x, color);
        ILI9341_DrawPixel(x0 + y, y0 - x, color);
        ILI9341_DrawPixel(x0 - y, y0 - x, color);
    }
}

void ILI9341_DrawFilledCircle(int16_t x0, int16_t y0, int16_t r, uint32_t color) {
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

    ILI9341_DrawPixel(x0, y0 + r, color);
    ILI9341_DrawPixel(x0, y0 - r, color);
    ILI9341_DrawPixel(x0 + r, y0, color);
    ILI9341_DrawPixel(x0 - r, y0, color);
    ILI9341_DrawLine(x0 - r, y0, x0 + r, y0, color);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        ILI9341_DrawLine(x0 - x, y0 + y, x0 + x, y0 + y, color);
        ILI9341_DrawLine(x0 + x, y0 - y, x0 - x, y0 - y, color);

        ILI9341_DrawLine(x0 + y, y0 + x, x0 - y, y0 + x, color);
        ILI9341_DrawLine(x0 + y, y0 - x, x0 - y, y0 - x, color);
    }
}

void ILI9341_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint32_t color) {
	/* Code by dewoller: https://github.com/dewoller */

	int16_t dx, dy, sx, sy, err, e2;
	uint16_t tmp;

	/* Check for overflow */
	if (x0 >= ILI9341_Opts.width) {
		x0 = ILI9341_Opts.width - 1;
	}
	if (x1 >= ILI9341_Opts.width) {
		x1 = ILI9341_Opts.width - 1;
	}
	if (y0 >= ILI9341_Opts.height) {
		y0 = ILI9341_Opts.height - 1;
	}
	if (y1 >= ILI9341_Opts.height) {
		y1 = ILI9341_Opts.height - 1;
	}

	/* Check correction */
	if (x0 > x1) {
		tmp = x0;
		x0 = x1;
		x1 = tmp;
	}
	if (y0 > y1) {
		tmp = y0;
		y0 = y1;
		y1 = tmp;
	}

	dx = x1 - x0;
	dy = y1 - y0;

	/* Vertical or horizontal line */
	if (dx == 0 || dy == 0) {
		ILI9341_Fill(x0, y0, x1, y1, color);
		return;
	}

	sx = (x0 < x1) ? 1 : -1;
	sy = (y0 < y1) ? 1 : -1;
	err = ((dx > dy) ? dx : -dy) / 2;

	while (1) {
		ILI9341_DrawPixel(x0, y0, color);
		if (x0 == x1 && y0 == y1) {
			break;
		}
		e2 = err;
		if (e2 > -dx) {
			err -= dy;
			x0 += sx;
		}
		if (e2 < dy) {
			err += dx;
			y0 += sy;
		}
	}
}



void ILI9341_Rotate(ILI9341_Orientation_t orientation)
{
	ILI9341_SendCommand(ILI9341_MAC);

	if (orientation == ILI9341_Orientation_Portrait_1) {
		ILI9341_SendData(0x58);
	} else if (orientation == ILI9341_Orientation_Portrait_2) {
		ILI9341_SendData(0x88);
	} else if (orientation == ILI9341_Orientation_Landscape_1) {
		ILI9341_SendData(0x28);
	} else if (orientation == ILI9341_Orientation_Landscape_2) {
		ILI9341_SendData(0xE8);
	}

	if (orientation == ILI9341_Orientation_Portrait_1 || orientation == ILI9341_Orientation_Portrait_2) {
		ILI9341_Opts.width = ILI9341_WIDTH;
		ILI9341_Opts.height = ILI9341_HEIGHT;
		ILI9341_Opts.orientation = ILI9341_Portrait;
	} else {
		ILI9341_Opts.width = ILI9341_HEIGHT;
		ILI9341_Opts.height = ILI9341_WIDTH;
		ILI9341_Opts.orientation = ILI9341_Landscape;
	}
}



/* ------------------------------------------------------------------------------------------------------ */
void ILI9341_DC(uint8_t BitVal)
{
	HAL_GPIO_WritePin(ILI9341_DC_PORT, ILI9341_DC_PIN, BitVal);
}

void ILI9341_SPI_nSS(uint8_t BitVal)
{
	HAL_GPIO_WritePin(ILI9341_GPIO_PORT, ILI9341_nSS, BitVal);
}

void ILI9341_GPIO_Init()
{
	GPIO_InitTypeDef GPIO_InitStrut;

	__HAL_RCC_GPIOA_CLK_ENABLE();

	/* D/C, BackLight and nReset pin initialization */
	GPIO_InitStrut.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStrut.Pin = (ILI9341_DC_PIN /*| ILI9341_BACKLIGHT_PIN*/ | ILI9341_nRESET_PIN);
	GPIO_InitStrut.Pull = GPIO_NOPULL;
	GPIO_InitStrut.Speed = GPIO_SPEED_HIGH;

	HAL_GPIO_Init(GPIOA, &GPIO_InitStrut);
}

void  ILI9341_DeInit()
{
	__HAL_RCC_SPI4_FORCE_RESET();
	__HAL_RCC_SPI4_RELEASE_RESET();

	HAL_GPIO_DeInit(ILI9341_GPIO_PORT, ILI9341_SCK);
	HAL_GPIO_DeInit(ILI9341_GPIO_PORT, ILI9341_MISO);
	HAL_GPIO_DeInit(ILI9341_GPIO_PORT, ILI9341_MOSI);

	HAL_SPI_DeInit(&SPI_HandleDef_ILI9341);

	isILI9341_Enabled = 0;
}


