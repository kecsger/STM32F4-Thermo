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
#include "stm32f4xx_it.h"


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


	//WatchDog_Init();

	/* Configure the system clock to 180 MHz */
	SystemClock_Config();

	/* Enable clock for GUI_Init() */
	__HAL_RCC_CRC_CLK_ENABLE();

	/* Configure the RTC peripheral */
	RTC_Config();

/*--------------------[Board Init ]----------------------------*/

	InitVariables();

	InitBoard();

/*--------------------[ STemWin Init ]---------------------------*/

	EmWin_InitGUI();

/*--------------------[ while(1) ]----------------------------*/

	/* Start LCD backlight Timer to dim after given time */
	LCD_BacklightTimerStart(12);

	/* Start 1 second Tasktimer */
	TIM_Task1Sec_Init();

	/* Start 1 minute Tasktimer */
	//TIM_Task1Min_Init();

	while(1)
	{
		/*
		if(LCDScreenTouched_cnt == SET)
		{
			LCDScreenTouched_cnt = RESET;
			LCD_ScreenTouched();
		}
		*/
		TaskManager();

	}
}



/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 180000000
  *            HCLK(Hz)                       = 180000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 8
  *            PLL_N                          = 360
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
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct; // In case of RTC peripheral


	/* Enable Power Control clock */
	__PWR_CLK_ENABLE();
	__HAL_RCC_PWR_CLK_ENABLE();


	/* The voltage scaling allows optimizing the power consumption when the device is
	 clocked below the maximum system frequency, to update the voltage scaling value
	 regarding system frequency refer to product datasheet.  */
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);


	/* Enable HSE Oscillator and activate PLL with HSE as source */
	/*
	 * Max SysClockConfig:
	 *
	 * SysClock = (HSE(external oscill) /M) * N) /P)
	 * HSE = 8MHz --> max Sysclock := 180 MHz
	 *
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_LSI;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.LSIState = RCC_LSI_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 8;
	RCC_OscInitStruct.PLL.PLLN = 360;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 7; // not really needed

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
	/*
	 *
	 * maxAPB1 = 45MHz
	 * maxAPB2 = 90MHz
	 * Hclk := SysClk --> AHB = 1
	 */

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

	/*##-1- Enables the PWR Clock and Enables access to the backup domain ###################################*/
	/* To change the source clock of the RTC feature (LSE, LSI), You have to:
	 - Enable the power clock using __HAL_RCC_PWR_CLK_ENABLE()
	 - Enable write access using HAL_PWR_EnableBkUpAccess() function before to
	   configure the RTC clock source (to be done once after reset).
	 - Reset the Back up Domain using __HAL_RCC_BACKUPRESET_FORCE() and
	   __HAL_RCC_BACKUPRESET_RELEASE().
	 - Configure the needed RTc clock source */

	__HAL_RCC_PWR_CLK_ENABLE();

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
	PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_HSE_DIV8;
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


	/* fCK_SPRE := 1Hz
	 *
	 * 					fCK_RTC				1000 Khz (HSE/8)
	 * fCK_SPRE = -------------------   = -------------------
	 * 			  (Async+1) * (Synch+1)		(63 +1) * (15624 +1)
	 *
	 */

	RtcHandle.Init.HourFormat = RTC_HOURFORMAT_24;
	RtcHandle.Init.AsynchPrediv = 63;
	RtcHandle.Init.SynchPrediv = 15624; //0xFF; //
	RtcHandle.Init.OutPut = RTC_OUTPUT_DISABLE;
	RtcHandle.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
	RtcHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;


	if(HAL_RTC_Init(&RtcHandle) != HAL_OK)
	{
		ErrorHandler();
	}

}
