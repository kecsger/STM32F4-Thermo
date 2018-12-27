/*
 * spi.h
 *
 *  Created on: 2015 okt. 9
 *      Author: kecsger12
 */

#ifndef SPI_H_
#define SPI_H_

/*================================[Includes] =========================================================================*/

#include "stm32f4xx.h"
#include "led.h"


/*================================[Macros]============================================================================*/

#define 		GPIO_AF_SPI				((uint8_t)0x05)

/*================================[Type definitions]============================================================================*/

typedef enum{
	SPI_AD7792,
	SPI_RFM73,
	SPI_AD7843,
	SPI_ILI9341
}SPI_Periph_t;



/*================================[Internal constants]================================================================*/

extern SPI_HandleTypeDef SPI_HandleDef_AD7792;		// SPI1
extern SPI_HandleTypeDef SPI_HandleDef_RFM73;		// SPI2
extern SPI_HandleTypeDef SPI_HandleDef_ILI9341;		// SPI4
extern SPI_HandleTypeDef SPI_HandleDef_AD7843;		// SPI4

/*================================[Function prototypes]============================================================================*/

void 		SPI_Init_GPIO(GPIO_TypeDef  *GPIOx, uint32_t GPIO_PinMISO, uint32_t GPIO_PinMOSI, uint32_t GPIO_PinSCK, uint32_t GPIO_PinnSS);
void 		SPI_Init_Config(SPI_Periph_t SPIx_type, uint32_t CLKPolarity, uint32_t CLKPhase, uint32_t DataSize, uint32_t BaudRatePrescaler);


uint32_t 	SPIx_Read(SPI_Periph_t SPIx_type, uint8_t size);
void 		SPIx_Write(SPI_Periph_t SPIx_type, uint8_t Data);

void 		SPIx_ClockEnable(SPI_TypeDef *SPIx);
void 		GPIOx_ClockEnable(GPIO_TypeDef  *GPIOx);

uint16_t 	SPI_GetPrescaleraxFreq(SPI_TypeDef* SPIx, uint32_t MAX_SPI_Frequency);

/* ------------------------------------------------------------ */
uint8_t 	SPI_Write(SPI_HandleTypeDef SPI_HandleDef, uint8_t data);
void 		SPI_Wait_TX(SPI_TypeDef *SPIx);
void 		SPI_Wait_RX(SPI_TypeDef *SPIx);
void 		SPI_Check_Enabled(SPI_HandleTypeDef SPI_HandleDef, SPI_TypeDef *SPIx);

/* ------------------------------------------------------------ */
void  		ILI9341_DeInit();
void  		AD7843_DeInit();

#endif /* SPI_H_ */
