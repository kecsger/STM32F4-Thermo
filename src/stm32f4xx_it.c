/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @author  Ac6
  * @version V1.0
  * @date    02-Feb-2015
  * @brief   Default Interrupt Service Routines.
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"

#include "led.h"
#include "spi.h"
#include "ad7843_touch.h"
#include "uart.h"
#include "spwf01sa_wifi.h"
#include "Application.h"

#include "Timers.h"

/*
#define		USARTx_DMA_TX_IRQHandler		DMA2_Stream7_IRQHandler
#define		USARTx_DMA_RX_IRQHandler		DMA2_Stream5_IRQHandler
#define		USARTx_IRQHandler				USART1_IRQHandler
*/




#define 	SPIx_DMA_TX_IRQHandler       	DMA2_Stream1_IRQHandler
#define 	SPIx_DMA_RX_IRQHandler       	DMA2_Stream0_IRQHandler

#define		AD7843_PenIRQHandler			EXTI9_5_IRQHandler


/* Private variables ---------------------------------------------------------*/

//extern UART_HandleTypeDef UART_HandleDef;

uint16_t touchCnt = 0;
uint8_t LCDScreenTouched_cnt;

extern TIM_HandleTypeDef TIM_HandleDef_LCDFadeOut;
extern TIM_HandleTypeDef TIM_HandleDef_LCDBcklight;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            	  	    Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();

}


/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/


/*--------------------[ILI9341 DMA IRQHandle]-----------------------*/

void SPIx_DMA_TX_IRQHandler(void)
{
	HAL_DMA_IRQHandler(SPI_HandleDef_ILI9341.hdmatx);
}

void SPIx_DMA_RX_IRQHandler(void)
{
	HAL_DMA_IRQHandler(SPI_HandleDef_ILI9341.hdmarx);
}

/*--------------------[WiFi UART-DMA IRQHandle]-----------------------*/

/*
void USARTx_DMA_TX_IRQHandler(void)
{
	HAL_DMA_IRQHandler(UART_HandleDef_SPWF01SA.hdmatx);
}

void USARTx_DMA_RX_IRQHandler(void)
{
	HAL_DMA_IRQHandler(UART_HandleDef_SPWF01SA.hdmarx);
}

void USART1_IRQHandler(void)
{
	HAL_UART_IRQHandler(&UART_HandleDef_SPWF01SA);
}

*/

/*--------------------[AD7843 IRQHandle]-----------------------*/

void AD7843_PenIRQHandler(void)
{
	touchCnt++;

	/* EXTI line interrupt detected */

	if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_9) != RESET)
	{
		/* TODO: Kellene mérni egyet, h milyen fesz jel jön kijelzõ érintésekor */
		LCDScreenTouched_cnt = SET;
		//LCD_ScreenTouched();
		//while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_9) == (uint8_t)RESET);

		//HAL_NVIC_ClearPendingIRQ(EXTI9_5_IRQn);
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_9);
	}
}

/*--------------------[User Button IRQHandle]-----------------------*/


void EXTI0_IRQHandler(void)
{
	static uint8_t buttonState;
	static uint8_t buttonState_prev;

	if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_0) != RESET)
	{
		buttonState = 1;
	}

	/* Debouncing */
	if (buttonState == buttonState_prev)
	{
		buttonState = 0;
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);
		ButtonWasPressed();
	}

	buttonState_prev = buttonState;
}

void TIM2_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&TIM_HandleDef_Task1sec);
}

void TIM4_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&TIM_HandleDef_LCDBcklight);
}

void TIM5_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&TIM_HandleDef_LCDBcklPWM);
}

void TIM7_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&TIM_HandleDef_LCDFadeOut);
}
