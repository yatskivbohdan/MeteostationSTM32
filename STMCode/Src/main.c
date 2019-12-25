/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "i2s.h"
#include "rtc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "usb_host.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include "bmp180.h"
#include "i2c-lcd.h"
#include "dhtxx.h"
#include "DS1302.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define USE_TIM10_TIMING 1
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
DHTxx_hygrometer_t hr11_1;
DHTxx_errors res;
int DHT_temp, DHT_press;

bmp_t bmp;





/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_USB_HOST_Process(void);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void WriteDHTData(void)
{
	/* Reading the data */
	char humidity[30];
	char temp_dht[30];
	DHT_temp = get_temperature_DHTxx(&hr11_1);
	DHT_press = get_humidity_DHTxx(&hr11_1);
	/* Sending data to display */
	sprintf(humidity, "Humidity: %02d.%d%%", DHT_press/10, DHT_press%10);
	sprintf(temp_dht, "Temp: %d.%dC", DHT_temp/10, abs(DHT_temp)%10);
	lcd_clear();
	lcd_display_first_row(temp_dht);
	lcd_display_second_row(humidity);
	/* Sending data to the server */
	TransmitOneValue(0, DHT_temp/10, DHT_temp/10);
	TransmitOneValue(1, abs(DHT_temp)%10, abs(DHT_temp)%10);
	TransmitOneValue(2, DHT_press/10, DHT_press/10);
	TransmitOneValue(3, DHT_press%10, DHT_press%10);
	HAL_Delay(5000);
}


void TransmitOneValue(int num, int value, int check)
{
	uint8_t buffer[3] = {num, (uint8_t)value, (uint8_t) check};
	HAL_UART_Transmit(&huart2, buffer, sizeof(buffer), HAL_MAX_DELAY);
}

void DHTError(char * str)
{
	lcd_clear();
	lcd_display_first_row(str);
}

void WriteBMPData(void)
{
	bmp.uncomp.temp = get_ut();
	bmp.data.temp = get_temp (&bmp);
	bmp.uncomp.press = get_up (bmp.oss);
	bmp.data.press = get_pressure (bmp);
		//bmp.data.altitude = get_altitude (&bmp);
	char temp[30];
	char press[30];
	char rofl[16];
	sprintf(temp, "Temp: %2.1f C", bmp.data.temp);
	sprintf(press, "Press: %6d Pa", bmp.data.press);
	sprintf(rofl, "%d", bmp.data.press);
	lcd_clear();
	lcd_display_first_row(temp);
	lcd_display_second_row(rofl);
	int temp_to_transmit = bmp.data.temp*10;
	TransmitOneValue(4, temp_to_transmit/10, temp_to_transmit/10);
	TransmitOneValue(5, abs(temp_to_transmit%10), abs(temp_to_transmit%10));
	// Bug fix needed
	TransmitOneValue(6, bmp.data.press/1000, bmp.data.press/1000);
	TransmitOneValue(7, bmp.data.press/10 - (int)(bmp.data.press/1000)*100, bmp.data.press/10 - (int)(bmp.data.press/1000)*100);
	TransmitOneValue(8, bmp.data.press - (int)(bmp.data.press/10)*10, bmp.data.press - (int)(bmp.data.press/10)*10);
	HAL_Delay(3000);

}



void get_time(void)
{
	uint8_t time[8];
	uint8_t buf [8] = {0, 0x13, 12, 23, 21, 1, 0, 0};
	DS1302_WriteTimeBurst(buf);
	char toDisplay[8];
	char toDisplay2[10];
	DS1302_ReadTimeBurst(time);
	sprintf(toDisplay,"%02d:%02d:%02d", time[4], time[5], time[6]);
	sprintf(toDisplay2,"%02d-%02d-%2d",time[3], time[2], 2000 + time[1]);
	lcd_clear();
	lcd_display_first_row(toDisplay);
	lcd_display_second_row(toDisplay2);

}


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_I2S2_Init();
  MX_I2S3_Init();
  MX_SPI1_Init();
  MX_USART2_UART_Init();
  MX_USB_HOST_Init();
  MX_I2C3_Init();
  MX_TIM10_Init();
  MX_RTC_Init();
  /* USER CODE BEGIN 2 */
  bmp_init(&bmp);
  lcd_init();
  init_DHTxx(&hr11_1, DHT22, GPIO_PIN_6, GPIOC);
  HAL_Delay(ms_before_first_read(&hr11_1));
  DS1302_Init();
  DS1302_ClockStart();
  TIM10_reinit();

  //set_time(19, 12, 23, 21, 56, 7);
  //set_time(0x16, 0x12, 0x13, 0x06, 0x03, 0x10);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
    MX_USB_HOST_Process();

    /* USER CODE BEGIN 3 */
    get_time();
    HAL_Delay(2000);
	/*Getting hygrometer data*/
	res = read_raw_DHTxx(&hr11_1, 1);
		 if (res==DHTXX_OK)
		 {
			 WriteDHTData();
		 }
		 if (res==DHTXX_CS_ERROR)
		 {
			 DHTError("DHT CHEKSUM ERROR");
		 }
		 if (res==DHTXX_NO_CONN)
		 {
			 DHTError("DHT NO CONN");
		 }
		 //HAL_Delay(2000);

	/*Getting barometer data*/
	WriteBMPData();

	/* Some delay */
	HAL_Delay(2000);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 120;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV6;
  RCC_OscInitStruct.PLL.PLLQ = 5;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2S|RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.PLLI2S.PLLI2SN = 200;
  PeriphClkInitStruct.PLLI2S.PLLI2SM = 5;
  PeriphClkInitStruct.PLLI2S.PLLI2SR = 2;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
