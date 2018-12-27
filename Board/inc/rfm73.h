/*
 * rfm73.h
 *
 *  Created on: 2015 nov. 9
 *      Author: kecsger12
 */

#ifndef INC_RFM73_H_
#define INC_RFM73_H_

/*================================[Includes] =========================================================================*/

#include "stm32f4xx.h"
#include "spi.h"

/*================================[Macros]============================================================================*/

// Commands
#define RFM_R_REGISTER			0x00
#define RFM_W_REGISTER			0x20
#define RFM_R_RX_PAYLOAD		0x61
#define RFM_W_TX_PAYLOAD		0xA0
#define RFM_FLUSH_TX			0xE1
#define RFM_FLUSH_RX			0xE2
#define RFM_REUSE_TX_PL			0xE3
#define RFM_ACTIVATE			0x50
#define RFM_R_RX_PL_WID			0x60
#define RFM_W_ACK_PAYLOAD		0xA8
#define RFM_W_TX_PAYLOAD_NOACK	0xB0

// Registers
#define RFM_REG_CONFIG			0x00
#define RFM_REG_EN_AA         	0x01
#define RFM_REG_EN_RXADDR     	0x02
#define RFM_REG_SETUP_AW     	0x03
#define RFM_REG_SETUP_RETR    	0x04
#define RFM_REG_RF_CH         	0x05
#define RFM_REG_RF_SETUP      	0x06
#define RFM_REG_STATUS        	0x07
#define RFM_REG_OBSERVE_TX    	0x08
#define RFM_REG_CD            	0x09
#define RFM_REG_RX_ADDR_P0   	0x0A
#define RFM_REG_RX_ADDR_P1    	0x0B
#define RFM_REG_RX_ADDR_P2    	0x0C
#define RFM_REG_RX_ADDR_P3    	0x0D
#define RFM_REG_RX_ADDR_P4    	0x0E
#define RFM_REG_RX_ADDR_P5    	0x0F
#define RFM_REG_TX_ADDR       	0x10
#define RFM_REG_RX_PW_P0      	0x11
#define RFM_REG_RX_PW_P1      	0x12
#define RFM_REG_RX_PW_P2      	0x13
#define RFM_REG_RX_PW_P3      	0x14
#define RFM_REG_RX_PW_P4      	0x15
#define RFM_REG_RX_PW_P5      	0x16
#define RFM_REG_FIFO_STATUS   	0x17
#define RFM_REG_DYNPD         	0x1C
#define RFM_REG_FEATURE       	0x1D

//


#define RFM_LNA_HCURR         (0x01) // 0: Low gain(20dB down); 1: High gain

#define RFM_RF_PWR            (0x06) //"00": -10 dBm; "01": -5 dBm;"10": 0 dBm;"11": 5 dBm

#define RFM_RF_DR_HIGH        (0x08) //"00": 1Mbps;"01": 250Kbps;"10": 2Mbps (default);"11": 2Mbps

#define RFM_PLL_LOCK          (0x10) //Only used in test (default is 0)

#define RFM_RF_DR_LOW         (0x20)
#define RFM_RF_DR             (0x28)


//Interrupt status
#define STATUS_RX_DR    0x40
#define STATUS_TX_DS    0x20
#define STATUS_MAX_RT   0x10
#define STATUS_TX_FULL  0x01

//FIFO status
#define FIFO_STATUS_TX_REUSE  0x40
#define FIFO_STATUS_TX_FULL   0x20
#define FIFO_STATUS_TX_EMPTY  0x10

#define FIFO_STATUS_RX_FULL   0x02
#define FIFO_STATUS_RX_EMPTY  0x01

#define BANK0_ENTRIES 			9
#define BANK1_ENTRIES 			14
#define BANK1LONG_ENTRIES 		11


#define MAX_PACKET_LEN  32// max value is 32



/*================================[Typedef declarations]===================================================*/



/*================================[Internal constants]================================================================*/

SPI_HandleTypeDef SPI_HandleDef_RFM73;


/*================================[Prototype function declarations]===================================================*/

/* ===== RFM73 init functions ===== */

void 	RFM_Init();
void 	RFM_SetRFParams(uint8_t out_pwr, uint8_t lna_gain, uint8_t data_rate);

/* ===== RFM73 check functions ===== */

uint8_t RFM_isPresent(void);
uint8_t RFM_CheckStatus();
void 	RFM_Observe(uint8_t packetLost, uint8_t packetRetr);
uint8_t RFM_Find_Reciever(uint8_t chanel, uint8_t datarate);

/* ===== RFM73 in work ===== */

uint8_t RFM_Receive(uint8_t *RX_buf);
uint8_t RFM_Transmit(uint8_t reg, uint8_t *buffer, uint8_t length);

/* ===== RFM73 Modes ===== */

void 	RFM_SwitchToRxMode(void);
void 	RFM_SwitchToTxMode(void);
void 	RFM_Mode_Standby(void);
void 	RFM_Mode_Powerdown(void);

/* ===== Data moving operarations ===== */

uint8_t RFM_ReadReg(uint8_t bank, uint8_t reg);
void 	RFM_ReadBuf(uint8_t bank, uint8_t reg, uint8_t* value, uint8_t length);
void 	RFM_WriteReg(uint8_t bank, uint8_t reg, uint8_t value);
void 	RFM_WriteBuf(uint8_t bank, uint8_t reg, uint8_t* value, uint8_t length);

/* ===== GPIO Settings ===== */

void 	RFM_GPIO_Init();
void 	RFM_CS(uint8_t BitVal);
void 	RFM_CE(uint8_t BitVal);

/* ===== BANK Operations ===== */
uint8_t CheckBank();

void 	ChangeBank();
void 	RFM_Bank0_Init();
void 	RFM_Bank1_Init();
void 	RFM_Bank1Long_Init();

/* ===== MISC ===== */

void 	RFM_RecvAddr_P0(uint8_t address[5]);
void 	RFM_RecvAddr_P1(uint8_t address[5]);
void 	RFM_RecvAddr_Pn(uint8_t channel, uint8_t address);
void 	RFM_TranAddr(uint8_t address[5]);

void 	RFM_Channel_PayloadSize(uint8_t channel, uint8_t size);

void 	RFM_Set_AutoACK();
void 	RFM_Set_AutoRetransmit(uint16_t rt_time, uint8_t rt_count);
void 	RFM_SetChanel(uint8_t chanel);


float 	RFM_GetTemperature();



#endif /* INC_RFM73_H_ */
