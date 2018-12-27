/*
 * rfm73.c
 *
 *  Created on: 2015 nov. 9
 *      Author: kecsger12
 */

/*================================[Includes] =========================================================================*/

#include "rfm73.h"
#include "Application.h"
#include "led.h"

/*================================[Macros]============================================================================*/

#define 	RFM_OUT_PWR_0DBM        2 //Set output power to 0 dBm.
#define 	RFM_OUT_PWR_PLUS5DBM	3 //Set output power to +5 dBm

#define 	RFM_LNA_GAIN_HIGH       1 //Set low noise amplifier gain to 0 dBm
#define 	RFM_LNA_GAIN_LOW        0 //Set low noise amplifier gain to -20 dBm


#define 	RFM_MOSI				GPIO_PIN_15
#define		RFM_MISO				GPIO_PIN_14
#define 	RFM_SCK					GPIO_PIN_13
#define		RFM_nSS					GPIO_PIN_12

#define 	RFM_SPI					SPI2
#define 	RFM_GPIO_PORT			GPIOB


/*================================[Function definitions]==============================================================*/

// { address, init_value }
uint8_t Bank0Reg[BANK0_ENTRIES][2] = {
								   { 0x00, 0x0F }, // receive, enabled, CRC 2, enable interupts
								   { 0x01, 0x3F }, // auto-ack on all pipes enabled
								   { 0x02, 0x03 }, // Enable pipes 0 and 1
								   { 0x03, 0x03 }, // 5 bytes addresses
								   { 0x04, 0xFF }, // auto retransmission delay 4000 us, 15 times
								   { 0x05, 0x02 }, // channel 2
								   { 0x06, 0x07 }, // data rate 1Mbit, power 5dbm, LNA gain high
								   { 0x07, 0x07 }, // why write this at all?? but seems required to work...
								   { 0x17, 0x00 }, // fifo status
};

// { address, init_byte1, init_byte2, ... }
uint8_t Bank1Reg[BANK1_ENTRIES][5] = {
								   { 0x00, 0x40, 0x4B, 0x01, 0xE2 },
								   { 0x01, 0xC0, 0x4B, 0x00, 0x00 },
								   { 0x02, 0xD0, 0xFC, 0x8C, 0x02 },
								   { 0x03, 0x99, 0x00, 0x39, 0x41 },
								   { 0x04, 0xD9, 0x9E, 0x86, 0x0B },
								   { 0x05, 0x24, 0x06, 0x7F, 0xA6 },
								   { 0x06, 0x00, 0x00, 0x00, 0x00 },
								   { 0x07, 0x00, 0x00, 0x00, 0x00 },
								   { 0x08, 0x00, 0x00, 0x00, 0x00 },
								   { 0x09, 0x00, 0x00, 0x00, 0x00 },
								   { 0x0A, 0x00, 0x00, 0x00, 0x00 },
								   { 0x0B, 0x00, 0x00, 0x00, 0x00 },
								   { 0x0C, 0x00, 0x12, 0x73, 0x00 },
								   { 0x0D, 0x36, 0xB4, 0x80, 0x00 }
};

uint8_t Bank1LongReg[BANK1LONG_ENTRIES] = {
									0x41, 0x10, 0x04, 0x82, 0x20, 0x08, 0x08, 0xF2, 0x7D, 0xEF, 0xFF };

const unsigned long Bank1_Reg0_13[] = {
   0xE2014B40,
   0x00004BC0,
   0x028CFCD0,
   0x41390099,
   0x1B8296D9,
   0xA67F0624,
   0x00000000,
   0x00000000,
   0x00000000,
   0x00000000,
   0x00000000,
   0x00000000,
   0x00127300,
   0x36B48000 };

   // more magic bank1 register initialization values
unsigned char Bank1_Reg14[] = {
   0x41, 0x20, 0x08, 0x04, 0x81, 0x20, 0xCF, 0xF7, 0xFE, 0xFF, 0xFF };

// default receive address data pipe 0:
// just a bunch of bytes, nothing magical
uint8_t RX0_Address[]={ 0x34, 0x43, 0x10, 0x10, 0x01 };


/*================================[Function definitions]==============================================================*/


void RFM_Init()
{
	RFM_GPIO_Init();

	SPI_Init_GPIO(RFM_GPIO_PORT,RFM_MISO, RFM_MOSI, RFM_SCK, RFM_nSS);
	SPI_Init_Config(SPI_RFM73, SPI_POLARITY_LOW, SPI_PHASE_1EDGE, SPI_DATASIZE_8BIT, 8000000);

	RFM_CE(0);
	HAL_Delay(1);
	RFM_CS(1);

	RFM_Bank0_Init();

	RFM_RecvAddr_P0(RX0_Address);
	RFM_RecvAddr_P1(RX0_Address);
	RFM_TranAddr(RX0_Address);

	// Enable the extra features only when they are not yet activated!
	if (RFM_ReadReg(0, RFM_REG_FEATURE) == 0)
	{
		RFM_CS(0);
		SPIx_Write(SPI_RFM73, RFM_ACTIVATE);
		SPIx_Write(SPI_RFM73, 0x73);
		RFM_CS(1);
	}

	// select dynamic payload length data pipe5\4\3\2\1\0
	RFM_WriteReg(0, RFM_REG_DYNPD, 0x3F);

	// select Dynamic Payload Length, Payload with ACK, W_TX_PAYLOAD_NOACK
	RFM_WriteReg(0, RFM_REG_FEATURE, 0x07);

	// dynamic payload sizing on channels 0 and 1
	RFM_Channel_PayloadSize(0, 0);
	RFM_Channel_PayloadSize(1, 0);

	RFM_Bank1_Init();

	//TODO: biztosan meg akarom ezeket hivni ???
/*
	RFM_Set_AutoACK();
	RFM_Set_AutoRetransmit(4000, 15);
*/

	RFM_SwitchToRxMode();
}

//! This function tests whether the rfm73 module is present.
//! It does so by reading the status register, and verifying that
//! an activate 0x53 command toggles the RBANK bit
//! in the status register.

uint8_t RFM_isPresent(void)
{
	uint8_t st1, st2;

	st1 = CheckBank();
	ChangeBank();

	st2 = CheckBank();
	ChangeBank();

	return ((st1 ^ st2) == 0x01);
}


/*
This function sets main RF params of the module.
	@param out_pwr - defines output power of the module. It tooks one of these values:
						  #RFM73_OUT_PWR_MINUS10DBM,
						  #RFM73_OUT_PWR_MINUS5DBM,
						  #RFM73_OUT_PWR_0DBM,
						  #RFM73_OUT_PWR_PLUS5DBM;
	@param lna_gain - this parameter determines low noise amplifier gain and can take one of two values:
						  #RFM73_LNA_GAIN_HIGH,
						  #RFM73_LNA_GAIN_LOW;
	@param data_rate - air data rate is set within these values:
						   #RFM73_DATA_RATE_1MBPS,
						   #RFM73_DATA_RATE_2MBPS,
						   #RFM73_DATA_RATE_250KBPS
*/

void RFM_SetRFParams(uint8_t out_pwr, uint8_t lna_gain, uint8_t data_rate)
{
	volatile uint8_t c;

	/* BitField shifts*/
	uint8_t RFM_LNA_HCURR_bf = 0;
	uint8_t RFM_RF_PWR_bf = 1;
	uint8_t RFM_RF_DR_HIGH_bf = 3;
	uint8_t RFM_RF_DR_LOW_bf = 5;

	// read config
	c = RFM_ReadReg(0, RFM_REG_RF_SETUP);

	// clear all used bits
	c &=~(RFM_RF_DR | RFM_LNA_HCURR | RFM_RF_PWR);

	// set appropriate bits
	c |= (out_pwr & 3) << RFM_RF_PWR_bf;
	c |= (lna_gain & 1) << RFM_LNA_HCURR_bf;
	c |= (((data_rate & 2) >> 1) << RFM_RF_DR_HIGH_bf | ((data_rate & 1) << RFM_RF_DR_LOW_bf));

	// write config
	RFM_WriteReg(0, RFM_REG_RF_SETUP, c);
}

uint8_t RFM_CheckStatus()
{
	unsigned char st1 = 0, st2 = 0;

	st1 = RFM_ReadReg(0, RFM_REG_STATUS);
	RFM_WriteReg(0, RFM_ACTIVATE, 0x53);

	st2 = RFM_ReadReg(0, RFM_REG_STATUS);
	RFM_WriteReg(0, RFM_ACTIVATE, 0x53);

	return ( st1 ^ st2 ) == 0x80;

}

/*
 	This function returns some parameters used to observe quality of channel.
	@param packet_lost   - number of packets lost from switching to current
						   channel. The counter is overflow protected to 15, and
						   discontinues at max until reset. The counter is reset by
						   changing channel. For some unknown reason this counter
						   counts only to 1.
	@param retrans_count - number of times last packet was retransmitted. The
						   counter is reset when transmission of a new packet
						   starts.
*/

void RFM_Observe(uint8_t packetLost, uint8_t packetRetr)
{
	uint8_t result = RFM_ReadReg(0, RFM_REG_OBSERVE_TX);

	// count lost packets
	packetLost = ((result & 0xF0) >> 4);

	// count retransmitted packets
	packetRetr = ((result & 0x0F));
}

/*
This function scans air with auto-acknowledge message and returns channel and datarate of the first answer.
	@param chanel	- starting channel of the scan; in this variable channel number
				 	  would be returned in case of answer;
	@param datarate	- in this variable would be returned datarate;
	@return 		1 (and change ch and dr params) if acknowledge received;
					0 if nothing received.
*/

uint8_t RFM_Find_Reciever(uint8_t chanel, uint8_t datarate)
{
	uint8_t result;
	uint8_t dummy = 0xAA;

	uint8_t cur_chanel = chanel;
	uint8_t cur_datarate = datarate;

	RFM_Set_AutoRetransmit(4000, 15);

	/* Scan every channel */
	for(; cur_chanel<0x80; cur_chanel++)
	{
		RFM_SetChanel(cur_chanel);

		/* Scan every data-rate */
		for(cur_datarate = 0; cur_datarate<3; cur_datarate++)
		{
			RFM_SetRFParams(RFM_OUT_PWR_PLUS5DBM, RFM_LNA_GAIN_HIGH, cur_datarate);

			/* clear RX_DR, TX_DS, MAX_RT */
			uint8_t stat = RFM_ReadReg(0, RFM_REG_STATUS);
			RFM_WriteReg(0, RFM_REG_STATUS, stat);

			result = RFM_Transmit(RFM_W_TX_PAYLOAD_NOACK, &dummy, 1);
			if(result == 1)
			{
				chanel = cur_chanel;
				datarate = cur_datarate;
				return 1;
			}
		}
	}
	return 0;
}

/* This function is used to get new packet from FIFO buffer.
	@param type 	- #RFM73_RX_WITH_ACK if explicit acknowledge of the packet is
				  	  needed;
				  	  #RFM73_RX_WITH_NOACK to only get data from FIFO buffer.
	@param data_buf - pointer to start of the input buffer;
	@param len  	- length of received packet (this data is read from RFM73,
				  	  and does not exceed 32).

	@return
			- 0 - if received data is correct;
			- 1 - if #RFM73_MAX_PACKET_LEN is exceeded, input FIFO buffer is
				  flushed, data_buf unchanged;

\warning Possible hangouts in certain conditions.*/


uint8_t RFM_Receive(uint8_t *RX_buf)
{
	uint8_t length, RFMstatus, fifoStatus;
	int8_t result = 0;

	//uint8_t *RX_buf[MAX_PACKET_LEN]

	/* Read register status */
	RFMstatus = RFM_ReadReg(0, RFM_REG_STATUS);

	/* if receive data ready (RX_DR) interrupt */
	if((STATUS_RX_DR & RFMstatus) == 0x40)
	{
		GreenLED_On();

		do
		{
			/* Reading length */
			length = RFM_ReadReg(0, RFM_R_RX_PL_WID);

			if(length <= MAX_PACKET_LEN)
			{
				/* read receive payload from RX_FIFO buffer */
				RFM_ReadBuf(0, RFM_R_RX_PAYLOAD, RX_buf, length);

				/* RETURN: Some data has been recieved */
				result = 0;
			}
			else
			{
				/* Flush RX FIFO */
				RFM_WriteReg(0, RFM_FLUSH_RX, 0);
				length = 0;

				/*RETURN: Data was flushed */
				result = 1;
			}

			fifoStatus = RFM_ReadReg(0, RFM_REG_FIFO_STATUS);

		}while ((fifoStatus & FIFO_STATUS_RX_EMPTY) == 0); /* Do until FIFO is not empty */


		//RFM_Transmit(RFM_W_TX_PAYLOAD_NOACK, RX_buf,length); --> Only in PRX mode!!

		//RFM_SwitchToRxMode(); //switch to RX mode
		GreenLED_Off();
	}


	/* Clear RX_DR or TX_DS or MAX_RT interrupt flag */
	RFM_WriteReg(0, RFM_REG_STATUS, RFMstatus);

	return result;
}

/**********************************************************************************
Function: RFM_Transmit
Description:
	fill FIFO to send a packet
Parameter:
	reg:		WR_TX_PLOAD or  W_TX_PAYLOAD_NOACK_CMD
	pbuf: 		a buffer pointer
	len:		packet length
Return:
		- 1 - data sent successfully (acknowledge received if enabled);
        - 0 - no reply from receiver (delivery probably failed).
***********************************************************************************/

uint8_t RFM_Transmit(uint8_t reg, uint8_t *buffer, uint8_t length)
{
	uint8_t fifoStatus, Tx_Status = 0;
	uint8_t result = 1;

	/* Switch to Tx mode */
	RFM_SwitchToTxMode();

	/* read register FIFO_STATUS's value */
	fifoStatus = RFM_ReadReg(0, RFM_REG_FIFO_STATUS);

	if((fifoStatus & FIFO_STATUS_TX_FULL) == 0)
	{
		GreenLED_On();

		RFM_WriteBuf(0, reg, buffer, length);

		/*Wait for MAX_RT and/or TX_DS flags */
		do {

			Tx_Status = RFM_ReadReg(0, RFM_REG_STATUS);

		}while(! (Tx_Status & (STATUS_MAX_RT | STATUS_TX_DS)));

		if (Tx_Status & STATUS_MAX_RT)
			result = 0;

		GreenLED_Off();
	}

	/* read  FIFO_STATUS register's value again */
	fifoStatus = RFM_ReadReg(0, RFM_REG_FIFO_STATUS);

	return result;
}



/* ======================================================================================================= */

void RFM_SwitchToRxMode(void)
{
	uint8_t value;

	// Flush receive queue
	RFM_WriteReg(0, RFM_FLUSH_RX, 0);

	// Clear interrupt status
	value = RFM_ReadReg(0, RFM_REG_STATUS);
	RFM_WriteReg(0, RFM_REG_STATUS, value);

	// switch to receive mode
	RFM_CE(0);
	value = RFM_ReadReg(0, RFM_REG_CONFIG);
	value |= 0x01; // set RX bit
	value |= 0x02; // set PWR_UP bit
	RFM_WriteReg(0, RFM_REG_CONFIG, value);
	RFM_CE(1);
}

void RFM_SwitchToTxMode(void)
{
	uint8_t value;

	// Flush transmit queue
	RFM_WriteReg(0, RFM_FLUSH_TX, 0);

	// Clear interrupt status
	value = RFM_ReadReg(0, RFM_REG_STATUS);
	RFM_WriteReg(0, RFM_REG_STATUS, value);

	// Switch to transmit mode
	RFM_CE(0);
	value = RFM_ReadReg(0, RFM_REG_CONFIG);
	value &= 0xFE; // clear RX bit
	value |= 0x02; // set PWR_UP bit
	RFM_WriteReg(0, RFM_REG_CONFIG, value);
	RFM_CE(1);
}

void RFM_Mode_Standby(void)
{
	uint8_t value;
	RFM_CE(0);
	value = RFM_ReadReg(0, RFM_REG_CONFIG );
	value |= 0x02; // set PWR_UP bit
	RFM_WriteReg(0, RFM_REG_CONFIG, value );
}

void RFM_Mode_Powerdown(void)
{
	uint8_t value;
	RFM_CE(0);
	value = RFM_ReadReg(0, RFM_REG_CONFIG );
	value &= 0xFD; // clear PWR_UP bit
	RFM_WriteReg(0, RFM_REG_CONFIG, value );
}

void RFM_Pipe_AutoAck(uint8_t pipe, uint8_t enabled)
{
   uint8_t val = RFM_ReadReg(0, RFM_REG_EN_AA);
   if ( pipe > 5 ) { pipe = 5; }
   if ( enabled )
   {
      val |= 1 << pipe;
   }
   else
   {
      val &= ~ ( 1 << pipe );
   }
   RFM_WriteReg(0, RFM_REG_EN_AA, val);
}

/* Gives back whitch RegisterBank is active */
uint8_t CheckBank()
{
	uint8_t RetVal = 0;

	RFM_CS(0);

	SPIx_Write(SPI_RFM73, RFM_REG_STATUS);
	RetVal = SPIx_Read(SPI_RFM73, 1);

	RFM_CS(1);

	return ((RetVal & 0x80) >> 7);
}

void ChangeBank()
{
	RFM_CS(0);

	SPIx_Write(SPI_RFM73, RFM_ACTIVATE);
	SPIx_Write(SPI_RFM73, 0x53);

	RFM_CS(1);
}

void RFM_WriteReg(uint8_t bank, uint8_t reg, uint8_t value)
{
	bank %= 2;
	if (bank != CheckBank()) { ChangeBank(); }
	if ( reg < RFM_W_REGISTER ) { reg |= RFM_W_REGISTER; }

	RFM_CS(0);

	SPIx_Write(SPI_RFM73, reg);
	SPIx_Write(SPI_RFM73, value);

	RFM_CS(1);
}

uint8_t RFM_ReadReg(uint8_t bank, uint8_t reg)
{
	uint8_t RetVal;
	bank %= 2;
	if (bank != CheckBank()) { ChangeBank(); }
	if ( reg < RFM_W_REGISTER ) { reg |= RFM_R_REGISTER; }

	RFM_CS(0);
	SPIx_Write(SPI_RFM73, reg);

	RetVal = SPIx_Read(SPI_RFM73, 1);

	RFM_CS(1);

	return RetVal;
}


void RFM_WriteBuf(uint8_t bank, uint8_t reg, uint8_t* buf, uint8_t length)
{
	uint8_t i;

	bank %= 2;
	if (bank != CheckBank()) { ChangeBank(); }
	if ( reg < RFM_W_REGISTER ) { reg |= RFM_W_REGISTER; }

	RFM_CS(0);
	SPIx_Write(SPI_RFM73, reg);
	for (i = 0; i < length; i++)
	{
		SPIx_Write(SPI_RFM73, *(buf++));
	}
	RFM_CS(1);
}

void RFM_ReadBuf(uint8_t bank, uint8_t reg, uint8_t* buf, uint8_t length)
{
	uint8_t i;
	bank %= 2;
	if (bank != CheckBank()) { ChangeBank(); }
	if ( reg < RFM_W_REGISTER ) { reg |= RFM_R_REGISTER; }

	RFM_CS(0);
	SPIx_Write(SPI_RFM73, reg);
	for (i = 0; i < length; i++)
	{
		*(buf++) = SPIx_Read(SPI_RFM73, 1);
	}
	RFM_CS(1);
}


void RFM_Bank0_Init()
{
	uint8_t j;
	for (j = 0; j < BANK0_ENTRIES; j++)
	{
		  RFM_WriteReg(0, Bank0Reg[j][0], Bank0Reg[j][1]);
	}
}



// initialize bank1 like in the example code.
// don't ask why, just do it
void RFM_Bank1_Init()
{
	unsigned char i, j;
	unsigned char WriteArr[ 12 ];

	//reverse!
	for( i = 0; i <= 8; i++ )
	{
		for( j = 0; j < 4; j++ )
		{
			WriteArr[j] = (Bank1_Reg0_13[i] >> (8*(j))) & 0xff;
	  	}

		RFM_WriteBuf(1, i, WriteArr, 4);
	}

	for( i = 9; i <= 13; i++ )
	{
		for( j = 0; j < 4; j++ )
		{
			WriteArr[j] = (Bank1_Reg0_13[i] >> (8*(3-j))) & 0xff;
		}

		RFM_WriteBuf(1, i, WriteArr, 4);
	}

	RFM_WriteBuf(1, 14, Bank1_Reg14, 11);

	//toggle REG4<25,26>
	for(j=0;j<4;j++)
	{
		WriteArr[j] = (Bank1_Reg0_13[4] >> (8*(j))) & 0xff;
	}

	WriteArr[0] = WriteArr[0] | 0x06;
	RFM_WriteBuf(1, 4, WriteArr, 4);

	WriteArr[0] = WriteArr[0] & 0xf9;
	RFM_WriteBuf(1, 4, WriteArr, 4);
}


// Masikat inkabb helyette

/*void RFM_Bank1_Init()
{
	uint8_t j;
	for (j = 0; j < BANK1_ENTRIES; j++)
	{
		RFM_WriteBuf(1, Bank1Reg[j][0], &(Bank1Reg[j][1]), 4);
	}
}

void RFM_Bank1Long_Init()
{
	RFM_WriteBuf(1, 0x0E, Bank1LongReg, BANK1LONG_ENTRIES);
}
*/

void RFM_RecvAddr_P0(uint8_t address[5])
{
	RFM_WriteBuf(0, RFM_REG_RX_ADDR_P0, address, 5);
}

void RFM_RecvAddr_P1(uint8_t address[5])
{
	RFM_WriteBuf(0, RFM_REG_RX_ADDR_P1, address, 5);
}

void RFM_RecvAddr_Pn(uint8_t channel, uint8_t address)
{
	if( address > 5 ) { address = 5; }
	if( address < 2 ) { address = 2; }
	RFM_WriteReg(0, RFM_REG_RX_ADDR_P0 + channel, address);
}

void RFM_TranAddr(uint8_t address[5])
{
	RFM_WriteBuf(0, RFM_REG_TX_ADDR, address, 5);
}

void RFM_Channel_PayloadSize(uint8_t channel, uint8_t size)
{
   uint8_t val;

   if( size > 32 ) { size = 32; }

   val = RFM_ReadReg(0, RFM_REG_DYNPD);
   if( size == 0 )
   {
      val |= 1 << channel;
   }
   else
   {
      val &= ~ ( 1 << channel );
   }

   RFM_WriteReg(0, RFM_REG_DYNPD, val );
   RFM_WriteReg(0, RFM_REG_RX_PW_P0 + channel, size );
}

/*
This function enables auto-acknowledge feature of all receive pipelines.
*/

void RFM_Set_AutoACK()
{
	RFM_WriteReg(0, RFM_REG_EN_AA, 0x3F);
}


/*
This function sets auto re-trnasmition parameters.
	@param rt_time - time in milliseconds (250-4000) between consequent
					 retransmitions. This value is set up with 250 ms steps (that
					 is e.g. 255 and 350 input values will result in the same
					 delay).
	@param rt_count - number of re-transmit tries. When this value is reached
					  transmition would be broken (MAX_RT flag would be set).
*/

void RFM_Set_AutoRetransmit(uint16_t rt_time, uint8_t rt_count)
{
	uint8_t time = rt_time/250-1;
	uint8_t count = rt_count;

	// set up constrains
	if (rt_time<250) time = 0;
	if (rt_time>4000) time = 15;
	if (rt_count>15) count = 15;
	RFM_WriteReg(0, RFM_REG_SETUP_RETR, ((time >> 4) | count) );

}

void RFM_SetChanel(uint8_t chanel)
{
	RFM_WriteReg(0, RFM_REG_RF_CH, chanel);
}


void RFM_CS(uint8_t BitVal)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, BitVal);
}


void RFM_CE(uint8_t BitVal)
{
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, BitVal);
}


/* Initialize RFM73 CE pin */
void RFM_GPIO_Init()
{
	__HAL_RCC_GPIOD_CLK_ENABLE();

	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pin = GPIO_PIN_8;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;

	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
}

float RFM_GetTemperature()
{
	float tempretureRFM;

	if(Rx_buf[0] != '\0')
		tempretureRFM = 10 * (Rx_buf[0]-48) + (Rx_buf[1]-48) + ((Rx_buf[3]-48.0)/10.0);

	return tempretureRFM;
}

