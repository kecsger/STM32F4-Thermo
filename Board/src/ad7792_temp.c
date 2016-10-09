/*
 * ad7792_temp.c
 *
 *  Created on: 2015 okt. 20
 *      Author: kecsger12
 *
 *	Tudjuk, hogy:
 *		[#1] -> R_RTD = R0(1 + Alfa * dT)
 *
 *	Mivel Bipolarisan van felkonfigurálva az ADC, így:
 *
 *  	[#2] -> ADC_code = [(Ain * Gain/Vref) + 1] * 2^(N-1)
 *
 *  	ahol,
 *  		Ain - Analog Input Voltage (RTD végpontjáról)
 *  		Gain - defaul erõsítés (128)
 *  		N - 16  (16bites SzigmaDelta ADC)
 *
 *		vagyis:
 *
 *		-> ADC_code = [(U_RTD/Uref * Gain) + 1] * 2^15 			// U_ref = I * R_ref
 *																// U_RTD = I * R_RTD
 *
 *		Behelyettesíve a képletbe, majd 'I'-vel egyszerüsítve azt kapjuk, hogy:
 *
 *		-> R_RTD = [(ADC_code/2^15) - 1] * (R_ref/Gain) = R0(1 + Alfa*dT) = R0(1 + A*dT + B*dT^2)
 *
 *		Innen dT mar könnyen kifejezhetõ! --->
 *
 *		DE: MIvel ez nagyon szamitasigenyes, ezert helzette a meglevo RTD_t struktura tagjai segitsegevel
 *		interpollacioval szamolom a homersekletet.
 *
 */


/*================================[Includes] ======================================================================================*/

#include "ad7792_temp.h"

/*================================[Internal constants]================================================================*/

static const RTD_t RTD_LookUp[11] =
{
	{0,  100.000},
	{5,  101.953},
	{10, 103.903},
	{15, 105.849},
	{20, 107.794},
	{25, 109.735},
	{30, 111.673},
	{35, 113.608},
	{40, 115.541},
	{45, 117.470},
	{50, 119.397},
};

/*================================[Macros]==============================================================*/

/* RTD constants (P0k1.1206.2P.B) */
#define		ALFA				(3850.0/1000000.0) 			// Temperature coefficient (3850ppm/K)
#define		R0					100.0						// 100Ohm @ 0°C

#define 	GAIN				32.0						// AD default erosites
#define		R_REF				6200.0						// 6.2kOhm 1%-os pecizios ellenallas
#define		AD_CONST			(R_REF/GAIN)				// Make it easier to calculate for the Mcu

#define		AD7792_OFFSET		0x8000						// Default Offset which is in the output code
#define 	AD7792_ID			0x0A

/*================================[Internal functions]==============================================================*/



/*================================[Function definitions]==============================================================*/

HAL_StatusTypeDef AD7792_Init()
{
	/* Init the communication interface for AD7792 */
	SPI_Init_GPIO(AD7792_PORT, AD7792_MISO, AD7792_MOSI, AD7792_SCK, AD7792_nSS);
	SPI_Init_Config(SPI_AD7792, SPI_POLARITY_HIGH, SPI_PHASE_2EDGE, 5000000);

	AD7792_Reset();

	/* PowerUp/Reset default values */
	/*while(AD7792_ReadReg(AD7792_MODE_REG, 2) != 0xA);
	while(AD7792_ReadReg(AD7792_CONFIG_REG, 2)!= 0x710);
	while(AD7792_ReadReg(AD7792_STATUS_REG, 1)!= 0x80);
	while(AD7792_ReadReg(AD7792_DATA_REG, 2)!= 0x0);
	while(AD7792_ReadReg(AD7792_OFFSET_REG, 2)!= 0x8000);*/


	/*----------------[CONFIG register settings]----------------*/

	//AD7792_SetVBias(VBIAS_AIN1N_EN); //-> No need
	//AD7792_SetBurnoutCurrent(BURNOUT_CURR_EN); //-> No need
	//AD7792_SetUniBipolarity(UNIPOLAR_MODE);
	AD7792_SetGain(GAIN_32);
	AD7792_SetChannel(CH_AIN1P_AIN1N);
	AD7792_SetIntReference(REF_SEL_EXT);
	//AD7792_SetBuf(BUFFERED_ON);

	//uint16_t configReg = AD7792_ReadReg(AD7792_CONFIG_REG, 2);

	/*----------------[MODE register settings]----------------*/
	AD7792_SetUpdateRate(FILT_UPD_RATE_4Hz17);

	//uint16_t modeReg = AD7792_ReadReg(AD7792_MODE_REG, 2);

	/*----------------[IO register settings]----------------*/
	AD7792_SetExcCurrent(AD7792_DIR_IEXC1_IEXC2_IOUT1, AD7792_IEXCEN_10uA);

	//uint8_t ioReg = AD7792_ReadReg(AD7792_IO_REG, 1);

	/* Calibrate AD7792 to reduce the offset and full-scale error */
	AD7792_Calibrate();
	AD7792_ContConversionersion();

	//modeReg = AD7792_ReadReg(AD7792_MODE_REG, 2);

	/* Check init state */
	HAL_StatusTypeDef initState = AD7792_IDRead_Success();

	//while(AD7792_IsBusy());

	AD7792_SPI_nSS(1);
	return initState;
}


double AD7792_MeasureTemp()
{
	uint16_t ADC_code = 0;
	double R_RTD, Temperature;

	/* [#1] */
	ADC_code = AD7792_ReadReg(AD7792_DATA_REG, 2);

	/* [#2]
	 * Felhasznalva a fajl elejen ismertetet kepletet [#2]
	 *
	 * ldexp -> (ADC_code/ 2^15) meggyorsitva
	 */
	R_RTD = ((ldexp(ADC_code, -15) - 1) * AD_CONST);

	/* [#3]
	 * Homerseklek kiszamolasa interpollacio segitsegevel RTD_t strukturabol */
	Temperature = AD7792_Interpollate(R_RTD);

	return Temperature;
}

void AD7792_SetBuf(uint8_t buffered)
{
	uint16_t regVal = AD7792_ReadReg(AD7792_CONFIG_REG, 2);
	regVal &= (0xFFEF); //Mask out bit 4
	regVal |= buffered;

	AD7792_WriteReg(AD7792_CONFIG_REG, regVal, 2);
}

void AD7792_SetUpdateRate(uint8_t updateRate)
{
	uint16_t regVal = AD7792_ReadReg(AD7792_MODE_REG, 2);
	regVal &= (0xFFF0); //Mask out bits 3:0
	regVal |= updateRate;

	AD7792_WriteReg(AD7792_MODE_REG, regVal, 2);
}

void AD7792_SetChannel(uint8_t channel)
{
	uint16_t regVal = AD7792_ReadReg(AD7792_CONFIG_REG, 2);
	regVal &= (0xFFF8); //Mask out bits 2:0
	regVal |= channel;

	AD7792_WriteReg(AD7792_CONFIG_REG, regVal, 2);
}

void AD7792_SetVBias(uint16_t VBias)
{
	uint16_t regVal = AD7792_ReadReg(AD7792_CONFIG_REG, 2);
	regVal &= (0x3FFF); //Mask out bits 15:14
	regVal |= VBias;

	AD7792_WriteReg(AD7792_CONFIG_REG, regVal, 2);
}

void AD7792_SetGain(uint16_t gain)
{
	uint16_t regVal = AD7792_ReadReg(AD7792_CONFIG_REG, 2);
	regVal &= (0xF8FF); //Mask out bits 10:8
	regVal |= gain;

	AD7792_WriteReg(AD7792_CONFIG_REG, regVal, 2);
}

void AD7792_SetIntReference(uint8_t reference)
{
	uint16_t regVal = AD7792_ReadReg(AD7792_CONFIG_REG, 2);
	regVal &= (0xFF7F); //Mask out bit 7
	regVal |= reference;

	AD7792_WriteReg(AD7792_CONFIG_REG, regVal, 2);
}

void AD7792_SingleConversion()
{
	uint16_t regVal = AD7792_ReadReg(AD7792_MODE_REG, 2);
	regVal &= (0x1FFF); // Mask out bits 15:13
	regVal |= MODE_SINGLE_CONV;

	AD7792_WriteReg(AD7792_MODE_REG, regVal, 2);
}

void AD7792_ContConversionersion()
{
	uint16_t regVal = AD7792_ReadReg(AD7792_MODE_REG, 2);
	regVal &= (0x1FFF); // Mask out bits 15:13
	regVal |= MODE_CONT_CONV;

	AD7792_WriteReg(AD7792_MODE_REG, regVal, 2);
}

void AD7792_SetUniBipolarity(uint16_t polarity)
{
	uint16_t regVal = AD7792_ReadReg(AD7792_CONFIG_REG, 2);
	regVal &= (0xEFFF); // Mask out bit 12
	regVal |= polarity;

	AD7792_WriteReg(AD7792_CONFIG_REG, regVal, 2);
}

void AD7792_SetBurnoutCurrent(uint16_t burnoutCurr)
{
	uint16_t regVal = AD7792_ReadReg(AD7792_CONFIG_REG, 2);
	regVal &= (0xDFFF); // Mask out bit 13
	regVal |= burnoutCurr;

	AD7792_WriteReg(AD7792_CONFIG_REG, regVal, 2);
}

void AD7792_SetExcCurrent(uint8_t dirr, uint8_t value)
{
	uint8_t regVal = AD7792_ReadReg(AD7792_IO_REG, 1);
	regVal &= (0x00); // Mask out bit 13
	regVal |= (dirr | value);

	AD7792_WriteReg(AD7792_IO_REG, regVal, 1);
}


void AD7792_Calibrate()
{
	uint16_t RegVal;

	/* Internal Zero-Scale Calibration */
	RegVal = AD7792_ReadReg(AD7792_MODE_REG, 2);
	RegVal &= (0x1FFF); // Mask out 15:13 bits
	RegVal = (RegVal | MODE_CAL_INT_ZERO_SCALE);
	AD7792_WriteReg(AD7792_MODE_REG, RegVal, 2);

	//while(AD7792_IsBusy());

	/* Internal Full-Scale Calibration */
	RegVal = AD7792_ReadReg(AD7792_MODE_REG, 2);
	RegVal &= (0x1FFF); // Mask out 15:13 bits
	RegVal = (RegVal | MODE_CAL_INT_FULL_SCALE);
	AD7792_WriteReg(AD7792_MODE_REG, RegVal, 2);

	//while(AD7792_IsBusy());
}


void AD7792_WriteReg(uint8_t reg, uint16_t value, uint8_t size)
{
	uint8_t value_upper = ((value & 0xFF00) >> 8);
	uint8_t value_lower = (value & 0xFF);

	/* wait until ADC is Ready */
	//while(AD7792_IsBusy());

	AD7792_SPI_nSS(0);
	SPIx_Write(SPI_AD7792, AD7792_COMM_WRITE | reg);

	if(size > 1)
	{
		SPIx_Write(SPI_AD7792, value_upper);
		HAL_Delay(1);
		SPIx_Write(SPI_AD7792, value_lower);
	}
	else
		SPIx_Write(SPI_AD7792, value);

	AD7792_SPI_nSS(1);
}

uint16_t AD7792_ReadReg(uint8_t reg, uint8_t size)
{
	uint8_t regValue_u, regValue_l = 0;
	uint8_t regVal_1 = 0;
	uint16_t regVal_2 = 0;

	AD7792_SPI_nSS(0);
	SPIx_Write(SPI_AD7792, AD7792_COMM_READ | reg);


	if (size == 1)
	{
		regVal_1 = SPIx_Read(SPI_AD7792, 1);

		AD7792_SPI_nSS(1);
		return regVal_1;
	}
	else
	{
		regValue_u = SPIx_Read(SPI_AD7792, 1);
		regValue_l = SPIx_Read(SPI_AD7792, 1);
		regVal_2 = (regValue_u << 8) + regValue_l;
		AD7792_SPI_nSS(1);

		return regVal_2;
	}
}


/* STATUS registers 7th. bit is the /RDY status bit */
uint8_t AD7792_IsBusy()
{
	uint8_t status, isBusy;

	AD7792_SPI_nSS(0);
	status = AD7792_ReadReg(AD7792_STATUS_REG, 1);
	AD7792_SPI_nSS(1);

	isBusy = ((status & 0x80) >> 7);
	return isBusy;
}


HAL_StatusTypeDef AD7792_IDRead_Success()
{
	uint8_t id = AD7792_ReadReg(AD7792_ID_REG, 1);

	if(AD7792_ID == (id & 0x0F))
		return HAL_OK;
	else
		return HAL_ERROR;
}

/* Write at least 32 series of 1s, and allow min 500us wait */
void AD7792_Reset(void)
{
	AD7792_SPI_nSS(0);

	int i;

	SPIx_Write(SPI_AD7792, 0x03); //just some dummy
	for(i=0; i<4; i++)
	{
		SPIx_Write(SPI_AD7792, 0xFF);
		HAL_Delay(1);
	}
    AD7792_SPI_nSS(1);
}


void AD7792_SPI_nSS(uint8_t BitVal)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, BitVal);
}

/*===================[Tempereature interpollation]============================*/
/*
 *
 *  						(T_Upper - T_lower) * (R_Upper - R_act)
 *  Temp_act = T_upper - [	--------------------------------------- ]
 *  								(R_Upper - R_lower)
 *
 *
 *
 */
double AD7792_Interpollate(double RTD_act)
{
	uint8_t i;
	uint8_t elementNum = sizeof(RTD_LookUp)/sizeof(RTD_LookUp[0]);

	double diff_Celsius = 0;
	double diff_Resistance = 0;
	double diff_actResistance = 0;

	uint16_t T_Upper = 0;
	uint16_t T_lower = 0;
	double R_Upper = 0;
	double R_lower = 0;

	double Temp_act = 0;

	double RTD_ResUpper = 0;
	double RTD_ResLower = 0;

	for(i=0; i<elementNum-1; i++)
	{
		RTD_ResLower = RTD_LookUp[i].RTD_Resistance;
		RTD_ResUpper = RTD_LookUp[i+1].RTD_Resistance;

		if(RTD_ResLower <= RTD_act && RTD_ResUpper > RTD_act)
		{
			T_Upper = RTD_LookUp[i+1].RTD_Celsius;
			T_lower = RTD_LookUp[i].RTD_Celsius;

			R_Upper = RTD_LookUp[i+1].RTD_Resistance;
			R_lower = RTD_LookUp[i].RTD_Resistance;

			diff_Celsius = T_Upper - T_lower;
			diff_Resistance = R_Upper - R_lower;
			diff_actResistance = R_Upper - RTD_act;

			Temp_act = T_Upper - ((diff_Celsius/diff_Resistance) * (diff_actResistance));

			return Temp_act;
		}
	}
	return 0;
}
