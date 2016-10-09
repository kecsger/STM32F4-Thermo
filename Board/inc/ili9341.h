/*
 * ili9341.h
 *
 *  Created on: 2015 okt. 12
 *      Author: kecsger12
 */

#ifndef INC_ILI9341_H_
#define INC_ILI9341_H_





/*================================[Includes] =========================================================================*/

#include "stm32f4xx.h"
#include "spi.h"

#include "font_def.h"

/*================================[Macros]============================================================================*/

/* Private defines */
#define ILI9341_SWRESET				0x01
#define ILI9341_RD_ID_INFO			0x04
#define ILI9341_RD_DISP_STATUS		0x09
#define ILI9341_RD_PWR_MODE			0x0A
#define ILI9341_SLEEP_OUT			0x11
#define ILI9341_SELF_DIAG			0x0F
#define ILI9341_GAMMA				0x26
#define ILI9341_DISPLAY_OFF			0x28
#define ILI9341_DISPLAY_ON			0x29
#define ILI9341_COLUMN_ADDR			0x2A
#define ILI9341_PAGE_ADDR			0x2B
#define ILI9341_GRAM				0x2C
#define ILI9341_MAC					0x36
#define ILI9341_PIXEL_FORMAT		0x3A
#define ILI9341_WDB					0x51
#define ILI9341_WCD					0x53
#define ILI9341_RGB_INTERFACE		0xB0
#define ILI9341_FRC					0xB1
#define ILI9341_BPC					0xB5
#define ILI9341_DFC					0xB6
#define ILI9341_POWER1				0xC0
#define ILI9341_POWER2				0xC1
#define ILI9341_VCOM1				0xC5
#define ILI9341_VCOM2				0xC7
#define ILI9341_POWERA				0xCB
#define ILI9341_POWERB				0xCF
#define ILI9341_RD_ID4				0xD3
#define ILI9341_PGAMMA				0xE0
#define ILI9341_NGAMMA				0xE1
#define ILI9341_DTCA				0xE8
#define ILI9341_DTCB				0xEA
#define ILI9341_POWER_SEQ			0xED
#define ILI9341_3GAMMA_EN			0xF2
#define ILI9341_INTERFACE			0xF6
#define ILI9341_PRC					0xF7



/* LCD settings */
#define 	ILI9341_WIDTH        	240
#define 	ILI9341_HEIGHT       	320
#define 	ILI9341_PIXEL       	76800

/* Colors */
#define	 	ILI9341_COLOR_WHITE		0xFFFF
#define	 	ILI9341_COLOR_BLACK		0x0000
#define 	ILI9341_COLOR_RED       0xF800
#define 	ILI9341_COLOR_GREEN		0x0400
#define 	ILI9341_COLOR_GREEN2	0xB723
#define 	ILI9341_COLOR_BLUE		0x001F
#define 	ILI9341_COLOR_BLUE2		0x051D
#define 	ILI9341_COLOR_YELLOW	0xFFE0
#define 	ILI9341_COLOR_ORANGE	0xFBE4
#define 	ILI9341_COLOR_CYAN		0x07FF
#define 	ILI9341_COLOR_MAGENTA	0xA254
#define 	ILI9341_COLOR_GRAY		0x7BEF
#define 	ILI9341_COLOR_BROWN		0xBBCA

#define 	ILI9341_TRANSPARENT		0x80000000

/*---------[SPI interface configs]---------*/

#define		ILI9341_DC_PORT			GPIOA
#define		ILI9341_nRESET_PORT		GPIOA
#define		ILI9341_BACKLIGHT_PORT	GPIOA
#define		ILI9341_DC_PIN			GPIO_PIN_0
#define		ILI9341_BACKLIGHT_PIN	GPIO_PIN_1
#define		ILI9341_nRESET_PIN		GPIO_PIN_2

#define		ILI9341_SPI				SPI4
#define		ILI9341_GPIO_PORT		GPIOE

#define		ILI9341_nSS				GPIO_PIN_11
#define		ILI9341_SCK				GPIO_PIN_12
#define		ILI9341_MISO			GPIO_PIN_13
#define		ILI9341_MOSI			GPIO_PIN_14

#define		ILI9341_DEV_ID			0x9341

/* SPI4 TX and RX default settings */
#define SPI4_DMA_TX_STREAM			DMA2_Stream1
#define SPI4_DMA_TX_CHANNEL   		DMA_CHANNEL_4

#define SPI4_DMA_RX_STREAM    		DMA2_Stream0
#define SPI4_DMA_RX_CHANNEL   		DMA_CHANNEL_4

#define SPI_IS_BUSY(SPIx) 			(((SPIx)->SR & (SPI_SR_TXE | SPI_SR_RXNE)) == 0 || ((SPIx)->SR & SPI_SR_BSY))


/*================================[Type definitions]============================================================================*/

typedef enum {
	TM_SPI_DataSize_8b, /*!< SPI in 8-bits mode */
	TM_SPI_DataSize_16b /*!< SPI in 16-bits mode */
} TM_SPI_DataSize_t;


typedef enum {
	ILI9341_Orientation_Portrait_1,
	ILI9341_Orientation_Portrait_2,
	ILI9341_Orientation_Landscape_1,
	ILI9341_Orientation_Landscape_2
} ILI9341_Orientation_t;


typedef enum {
	ILI9341_Landscape,
	ILI9341_Portrait
} ILI9341_Orientation;


/*================================[Internal constants]================================================================*/

SPI_HandleTypeDef SPI_HandleDef_ILI9341;

extern uint8_t isReady;

/*extern DMA_HandleTypeDef DMA_HandleDef_TX;
extern DMA_HandleTypeDef DMA_HandleDef_RX;*/


/*================================[Function prototypes]============================================================================*/

uint16_t	ILI9341_Init();
void 		ILI9341_InitLCD(void);
void 		ILI9341_GPIO_Init();

void		ILI9341_Reset();
void 		ILI9341_DC(uint8_t BitVal);
void		ILI9341_SPI_nSS(uint8_t BitVal);

uint16_t 	ILI9341_ReadID();

void 		ILI9341_SendData(uint8_t data);
void 		ILI9341_SendCommand(uint8_t data);
uint32_t	ILI9341_ReadReg(uint8_t reg, uint8_t size);

void 		ILI9341_BackLightON();
void		ILI9341_BackLightOFF();

void 		ILI9341_SPI_DMA_Init();
uint8_t 	TM_SPI_DMA_Working(SPI_TypeDef* SPIx);
uint8_t 	TM_SPI_DMA_SendHalfWord(SPI_TypeDef* SPIx, uint16_t value, uint16_t count);
TM_SPI_DataSize_t TM_SPI_SetDataSize(SPI_TypeDef* SPIx, TM_SPI_DataSize_t DataSize);

/* Graphical functions */
void 		ILI9341_SetCursorPosition(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void 		ILI9341_Fill(int x0, int y0, int x1, int y1, uint32_t color);
void 		ILI9341_Puts(uint16_t x, uint16_t y, char *str, TM_FontDef_t *font, uint16_t foreground, uint16_t background);
void 		ILI9341_DrawPixel(uint16_t x, uint16_t y, uint16_t color);
void 		ILI9341_Rotate(ILI9341_Orientation_t orientation);
void 		ILI9341_DrawCircle(int16_t x0, int16_t y0, int16_t r, uint32_t color);
void 		ILI9341_DrawFilledCircle(int16_t x0, int16_t y0, int16_t r, uint32_t color);
void 		ILI9341_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint32_t color);




void 		TM_ILI9341_INT_Fill(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);


#endif /* INC_ILI9341_H_ */
