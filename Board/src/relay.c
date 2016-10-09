/*
 * relay.c
 *
 *  Created on: 2015 nov. 10
 *      Author: kecsger12
 */

/*================================[Includes] ======================================================================================*/

#include "relay.h"

/*================================[Internal constants]================================================================*/


/*================================[Macros]==============================================================*/


/*================================[Internal functions]==============================================================*/



/*================================[Function definitions]==============================================================*/



void BD6211_InitGPIO()
{
	GPIO_InitTypeDef GPIO_relay;

	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();

	GPIO_relay.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_relay.Pull = GPIO_NOPULL;
	GPIO_relay.Speed = GPIO_SPEED_HIGH;
	GPIO_relay.Pin = GPIO_PIN_0;

	HAL_GPIO_Init(GPIOB, &GPIO_relay);

	GPIO_relay.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_relay.Pull = GPIO_NOPULL;
	GPIO_relay.Pin = GPIO_PIN_7;

	HAL_GPIO_Init(GPIOC, &GPIO_relay);

	/* Start from known state */
	BD6211_StandBy();
}

void Relay_On()
{
	/* Set time is max 10ms */
	BD6211_StandBy();
	HAL_Delay(10);

	/* Min set signal width has to be 20ms */
	BD6211_Forward();
	HAL_Delay(50);

	BD6211_StandBy();
}


void Relay_Off()
{
	/* Reset time is max 10ms */
	BD6211_StandBy();
	HAL_Delay(10);

	/* Min reset signal width has to be 20ms */
	BD6211_Reverse();
	HAL_Delay(50);

	BD6211_StandBy();
}



void BD6211_StandBy()
{
	BD6211_FIN(0);
	BD6211_RIN(0);
}

void BD6211_Forward()
{
	BD6211_FIN(1);
	BD6211_RIN(0);
}


void BD6211_Reverse()
{
	BD6211_FIN(0);
	BD6211_RIN(1);
}


void BD6211_Break()
{
	BD6211_FIN(1);
	BD6211_RIN(1);
}


void BD6211_FIN(uint8_t BitVal)
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, BitVal);
}

void BD6211_RIN(uint8_t BitVal)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, BitVal);
}

