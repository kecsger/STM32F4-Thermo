/*
 * ad7792_temp.h
 *
 *  Created on: 2015 okt. 20
 *      Author: kecsger12
 */

#ifndef INC_AD7792_TEMP_H_
#define INC_AD7792_TEMP_H_

/*================================[Includes] =========================================================================*/

#include "stm32f4xx.h"

#include "spi.h"
#include "math.h"

/*================================[Macros]============================================================================*/

/* AD7792 Communication Register definitions
 *
 *   ---------------------------------------------------------------
 * 	|	  7		|	6	|	   5:3		|	  2		|	1	|	0	|
 * 	 ---------------------------------------------------------------
 * 	|	/WEN	|  R/W	|	AD7792 REG	|	CREAD	|	0	|	0	|
 * 	 ---------------------------------------------------------------
 */
//===================================================================
#define			nWEN					(0<<7)			// Start bit: Calling COM REG (Mandatory!)

#define			AD7792_COMM_READ		(1<<6)			// Register will be read
#define			AD7792_COMM_WRITE		(0<<6)			// Data will be written to the given register

#define			AD7792_STATUS_REG		(0b000<<3)			// AD7792 Registers
#define			AD7792_MODE_REG			(0b001<<3)
#define			AD7792_CONFIG_REG		(0b010<<3)
#define			AD7792_DATA_REG			(0b011<<3)
#define			AD7792_ID_REG			(0b100<<3)
#define			AD7792_IO_REG			(0b101<<3)
#define			AD7792_OFFSET_REG		(0b110<<3)
#define			AD7792_FULLSCALE_REG	(0b111<<3)

#define 		CREAD_EN				(1<<2)			// Continues Read of DATA REG Enabled
#define 		CREAD_DIS				(0<<2)			// Continues Read of DATA REG Disabled

//===================================================================



/* AD7792 Status Register definitions */
//===================================================================
#define			READ_STAT				(0b0100.0000)


//===================================================================
/* AD7792 Mode Register definitions
 *
 *   -------------------------------------------
 * 	|	15:13	|  12:8	|  7:6	|  5:4	|  3:0	|
 * 	 -------------------------------------------
 * 	|	MODE	|	0	|  CLK	|	0	|  FR	|
 * 	 -------------------------------------------
 */
//===================================================================
#define			MODE_CONT_CONV			(0b000<<13)		//MODE Definition - CREAD has to be 1 in COMREG !!
#define			MODE_SINGLE_CONV		(0b001<<13)
#define			MODE_IDLE				(0b010<<13)
#define			MODE_POWER_DOWN			(0b011<<13)
#define			MODE_CAL_INT_ZERO_SCALE	(0b100<<13)		/* Internal Zero-Scale Calibration */
#define			MODE_CAL_INT_FULL_SCALE	(0b101<<13)		/* Internal Full-Scale Calibration */
#define			MODE_CAL_SYS_ZERO_SCALE	(0b110<<13)		/* System Zero-Scale Calibration */
#define			MODE_CAL_SYS_FULL_SCALE	(0b111<<13)		/* System Full-Scale Calibration */


#define			INTERNAL_CLOCK			(0b00<<6)		// Use internal 64kHz oscillator

#define 		FILT_UPD_RATE_242		(0b0010)		// fADC=242Hz, tSETTLE = 8.2ms
#define 		FILT_UPD_RATE_123		(0b0011)		// fADC=123Hz, tSETTLE = 16ms
#define			FILT_UPD_RATE_16Hz7		(0b1010)		// Default,  fADC=16.7Hz, tSETTLE = 120ms @50-60Hz
#define			FILT_UPD_RATE_4Hz17		(0b1111)		// fADC=4.7Hz, tSETTLE = 480ms @50-60Hz

//===================================================================



/* AD7792 Configuration Register definitions
 *
 *   -------------------------------------------------------------------------------
 * 	| 15:14	|   13	|  12	|  11	| 10:8	|	7	|  6:5	|	4	|	3	|  2:0	|
 * 	 -------------------------------------------------------------------------------
 * 	| VBIAS	|   BO	|  U/B	| BOOST	|  GAIN	| RefSel|   0	|  BUF	|	0	|  CH	|
 * 	 -------------------------------------------------------------------------------
 */
//===================================================================

#define			VBIAS_DIS				(0b00<<14)		//Bias voltage connected to AIN1(-)
#define			VBIAS_AIN1N_EN			(0b01<<14)		//Bias voltage connected to AIN1(-)
#define			VBIAS_AIN2N_EN			(0b10<<14)		//Bias voltage connected to AIN1(-)

#define 		BURNOUT_CURR_EN			(1<<13)			// 100nA current souce ENABLED
#define 		BURNOUT_CURR_DIS		(0<<13)

#define			BIPOLAR_MODE			(0<<12)
#define 		UNIPOLAR_MODE			(1<<12)

#define			GAIN_1					(0b000<<8)		// Use Gain 1 (In-amp not used), ADC INput Range: 2.5V
#define			GAIN_2					(0b001<<8)		// Use Gain 2 (In-amp not used) - 1.25V
#define			GAIN_4					(0b010<<8)		// Use Gain 4 ...
#define			GAIN_8					(0b011<<8)
#define			GAIN_16					(0b100<<8)
#define			GAIN_32					(0b101<<8)
#define			GAIN_64					(0b110<<8)
#define			GAIN_128				(0b111<<8)		// USE default GAIN 128 - 19.53mV

#define			REF_SEL_EXT				(0<<7)			// USE external refrense - 6.2kOhm resistor
#define			REF_SEL_INT				(1<<7)

#define 		BUFFERED_ON				(1<<4)
#define 		BUFFERED_OFF			(0<<4)

#define			CH_AIN1P_AIN1N			(0b000)			// Default: Ain1(+) - Ain1(-) channel
#define			CH_AIN3P_AIN3N			(0b010)			// Ain3(+) - Ain3(-) channel
#define			CH_AIN1N_AIN1N			(0b011)			// Ain1(-) - Ain1(-) channel
#define			CH_TEMP					(0b110)			// Temp Sensor
#define			CH_AVDD_MONITOR			(0b111)			// AVDD Monitor


//===================================================================



/* AD7792 I/O Register definitions
 *
 *   -------------------------------
 * 	|  7:4	|    3:2	|    1:0	|
 * 	 -------------------------------
 * 	|	0	| IEXCDIR1/0| IEXCEN1/0	|
 * 	 -------------------------------
 */
//===================================================================

#define 		AD7792_DIR_IEXC1_IEXC2_IOUT1		(0b10<<2)		/* Both current sources connected to Pin IOUT1 */
#define 		AD7792_DIR_IEXC1_IOUT1_IEXC2_IOUT2  (0b00<<2)		/* IEXC1 connect to IOUT1, IEXC2 connect to IOUT2 */

#define 		AD7792_IEXCEN_DIS  			(0b00)			/* Excitation Current Disabled */
#define 		AD7792_IEXCEN_10uA   		(0b01)			/* Excitation Current 210uA */
#define 		AD7792_IEXCEN_210uA   		(0b10)			/* Excitation Current 210uA */
#define 		AD7792_IEXCEN_1mA   		(0b11)			/* Excitation Current 1mA */


/* AD7792 HArdware Porn definitions */
//===================================================================
#define 	AD7792_PORT			GPIOA

#define 	AD7792_SPI			SPI1
#define 	AD7792_nSS			GPIO_PIN_4
#define 	AD7792_SCK			GPIO_PIN_5
#define 	AD7792_MISO			GPIO_PIN_6
#define 	AD7792_MOSI			GPIO_PIN_7


/*================================[Type definitions]==================================================================*/

typedef struct{
	uint8_t 	RTD_Celsius;
	double 		RTD_Resistance;
}RTD_t;

/*================================[Internal constants]================================================================*/

SPI_HandleTypeDef SPI_HandleDef_AD7792;	// SPI1

/* This LookUpTable is calculated from Callendar-Van Dusen equation for the RTD
 *
 * 		R_RTD = R0(1 + AdT + B*dT^2)
 * 		where
 * 		A = 3.9083e-03
 * 		B = -5.775e-07
 *
 * 	Values are from an excel sheet
 *
 * */


/*================================[Prototype function declarations]===================================================*/

uint8_t 		AD7792_Init();							// if return 1, Init was successful
void 			AD7792_Reset(void);

void			AD7792_SetBuf(uint8_t buffered);
void			AD7792_SetUpdateRate(uint8_t updateRate);
void 			AD7792_SetChannel(uint8_t channel);
void 			AD7792_SetGain(uint16_t gain);
void 			AD7792_SetIntReference(uint8_t reference);
void			AD7792_SetVBias(uint16_t VBias);
void			AD7792_SetUniBipolarity(uint16_t polarity);
void			AD7792_SetBurnoutCurrent(uint16_t burnoutCurr);
void			AD7792_SetExcCurrent(uint8_t dirr, uint8_t value);

void			AD7792_SingleConversion();
void			AD7792_ContConversionersion();
void 			AD7792_Calibrate();

void 			AD7792_WriteReg(uint8_t reg, uint16_t value, uint8_t size);
uint16_t 		AD7792_ReadReg(uint8_t reg,  uint8_t size);

double			AD7792_MeasureTemp();

uint8_t 		AD7792_IDRead_Success();

uint8_t 		AD7792_IsBusy();						//Read StatusReg 7th /BUSY bit
void 			AD7792_SPI_nSS(uint8_t BitVal);			// AD7792 SPI /SS

double			AD7792_Interpollate(double RTD_act);

#endif /* INC_AD7792_TEMP_H_ */
