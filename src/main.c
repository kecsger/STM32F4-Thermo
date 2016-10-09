/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/

/*================================[Includes] ======================================================================================*/

#include "stm32f4xx.h"
#include "stdlib.h"
#include "string.h"

#include "Application.h"

#include "rtc.h"

#include "MainTask.h"
#include "WM.h"
#include "GUI.h"



/*================================[Type definitions]==================================================================*/


/*================================[Internal constants]================================================================*/

uint8_t Rx_buf[MAX_PACKET_LEN];
RTC_HandleTypeDef RtcHandle;



/*================================[Function prototypes]==============================================================*/

static void SystemClock_Config(void);
static void RTC_Config(void);

/*================================[MAIN Function definitions]==============================================================*/


int main(void)
{
	/* STM32F4xx HAL library initialization:
	   - Configure the Flash prefetch, instruction and Data caches
	   - Configure the Systick to generate an interrupt each 1 msec
	   - Set NVIC Group Priority to 4
	   - Global MSP (MCU Support Package) initialization
	*/
	HAL_Init();

	WatchDog_Init();

	/* Configure the system clock to 168 MHz */
	SystemClock_Config();

	/* Enable clock for GUI_Init() */
	__HAL_RCC_CRC_CLK_ENABLE();

	/* Configure the RTC peripheral */
	RTC_Config();

/*--------------------[Board Init ]----------------------------*/

	InitVariables();

	InitBoard();

/*--------------------[ STemWin Init ]---------------------------*/

	MainTask();

	/* Kezdo MULTIPAGE - termosztat beallitasa */
	MULTIPAGE_SelectPage(hMultiPage, 0);
	WM_SelectWindow(termosztat);
	WM_ShowWindow(termosztat);
	mainWin = termosztat;
	GUI_Exec();

/*--------------------[ while(1) ]----------------------------*/

	/* Start LCD backlight Timer to dim after given time */
	LCD_BacklightTimerStart(60);

	/* Start 1minute Tasktimer */
	TIM_Task1min_Init();

	while(1)
	{
		GUI_Exec();

		if(WM_GetActiveWindow() == termosztat)
			WM_InvalidateWindow(mainWin);

		Application_RUN();

		TaskManager();

	}

}



/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 168000000
  *            HCLK(Hz)                       = 168000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 8
  *            PLL_N                          = 336
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitTypeDef RCC_OscInitStruct;


	/* Enable Power Control clock */
	__HAL_RCC_PWR_CLK_ENABLE();


	/* The voltage scaling allows optimizing the power consumption when the device is
	 clocked below the maximum system frequency, to update the voltage scaling value
	 regarding system frequency refer to product datasheet.  */
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);


	/* Enable HSE Oscillator and activate PLL with HSE as source */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_LSI;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.LSIState = RCC_LSI_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 8;
	RCC_OscInitStruct.PLL.PLLN = 336;
	RCC_OscInitStruct.PLL.PLLP = 2;
	RCC_OscInitStruct.PLL.PLLQ = 7;

	if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		ErrorHandler();
	}

/*
	 Activate the Over Drive feature (available only for STM32F42xxx/43xxx devices)
	if(HAL_PWREx_EnableOverDrive() != HAL_OK)
	{
		 Initialization Error
		ErrorHandler();
	}
*/


	/* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
	 clocks dividers */
	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

	if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
	{
		ErrorHandler();
	}

}


static void RTC_Config()
{
	RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;

	/* Allow access to RTC */
	HAL_PWR_EnableBkUpAccess();

    /* RTC Clock selection can be changed only if the Backup Domain is reset */
    __HAL_RCC_BACKUPRESET_FORCE();
    __HAL_RCC_BACKUPRESET_RELEASE();

	/* Chech if RTC Clock is Enabled */
	if(!(*(volatile uint32_t *) (BDCR_RTCEN_BB)))
	{
		__HAL_RCC_RTC_ENABLE();
	}



	/*Configue HSE/LSI as RTC clock soucre */
	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
	PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_HSE_DIV8; //RCC_OSCILLATORTYPE_LSI; //
	if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
	{
		ErrorHandler();
	}

	RtcHandle.Instance = RTC;

	/*if(HAL_RTC_WaitForSynchro(&RtcHandle) != HAL_OK)
	{
		ErrorHandler();
	}*/

	/* Configure RTC prescaler and RTC data registers */
	/* RTC configured as follow:
		- Hour Format    = Format 24
		- Asynch Prediv  = Value according to source clock
		- Synch Prediv   = Value according to source clock
		- OutPut         = Output Disable
		- OutPutPolarity = High Polarity
		- OutPutType     = Open Drain */

	RtcHandle.Init.HourFormat = RTC_HOURFORMAT_24;
	RtcHandle.Init.AsynchPrediv = 0x7C;
	RtcHandle.Init.SynchPrediv = 0x1F3F; //0xFF; //
	RtcHandle.Init.OutPut = RTC_OUTPUT_DISABLE;
	RtcHandle.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
	RtcHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;


	if(HAL_RTC_Init(&RtcHandle) != HAL_OK)
	{
		ErrorHandler();
	}

}
