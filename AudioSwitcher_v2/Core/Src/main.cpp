/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "iwdg.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include <AudioAnalyzer/Analyzer.h>
#include <AudioSwitcher/SimpleAudioSwitcher.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
volatile uint16_t adc_buffer[8];
#define CHANNELS_COUNT (4)
#define AMP_ON() 		(HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET))
#define AMP_OFF() 		(HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET))
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
AudioSwitcher::SimpleAudioSwitcher sw1(GPIOA, GPIO_PIN_11);
AudioSwitcher::SimpleAudioSwitcher sw2(GPIOA, GPIO_PIN_12);
AudioSwitcher::SimpleAudioSwitcher sw3(GPIOB, GPIO_PIN_9);
AudioSwitcher::SimpleAudioSwitcher sw4(GPIOC, GPIO_PIN_15);

AudioAnalyzer::Analyzer analyzer1(&adc_buffer[0], &adc_buffer[1]);
AudioAnalyzer::Analyzer analyzer2(&adc_buffer[2], &adc_buffer[3]);
AudioAnalyzer::Analyzer analyzer3(&adc_buffer[4], &adc_buffer[5]);
AudioAnalyzer::Analyzer analyzer4(&adc_buffer[6], &adc_buffer[7]);

struct Channel
{
	AudioSwitcher::SimpleAudioSwitcher* switcher;
	AudioAnalyzer::Analyzer* analyzer;
};

Channel channels[CHANNELS_COUNT] = {{&sw1, &analyzer1}, {&sw2, &analyzer2}, {&sw3, &analyzer3}, {&sw4, &analyzer4}};
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* USER CODE BEGIN 0 */
uint8_t i = 0;
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{

	analyzer1.UpdateData();
	analyzer2.UpdateData();
	analyzer3.UpdateData();
	analyzer4.UpdateData();
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
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_USART1_UART_Init();
  MX_IWDG_Init();
  /* USER CODE BEGIN 2 */

  uint32_t last_tick = HAL_GetTick();
  /* USER CODE END 2 */
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
AMP_ON();
/*while(1){

	sw3.SetCommutationState(true);
	HAL_IWDG_Refresh(&hiwdg);
}*/
	bool amp_state = false;
  while (1)
  {
	  HAL_IWDG_Refresh(&hiwdg);
    /* USER CODE END WHILE */
	  if(HAL_GetTick() - last_tick > 5)
	  {
		  last_tick = HAL_GetTick();
		  uint8_t ch_buf[100];
		  //(amp_state ? 200 : 10)
		  sprintf(reinterpret_cast<char*>(ch_buf), "$%u %u %u;", analyzer1.last_freq[0],  adc_buffer[2], adc_buffer[4]);
		  HAL_UART_Transmit_DMA(&huart1, ch_buf, strlen(reinterpret_cast<char*>(ch_buf)));
	  }

	  amp_state = false;
	  int8_t active_ch = -1;
	  for(uint8_t i = 0; i < CHANNELS_COUNT; i++)
	  {
		  if(channels[i].analyzer->IsAudio())
		  {
			  amp_state = true;
			  active_ch = i;
			  break;
		  }
	  }

	  if(amp_state)
	  {
		  for(uint8_t i = 0; i < CHANNELS_COUNT; i++)
		  {
			  channels[i].switcher->SetCommutationState(i == active_ch);
		  }

		  AMP_ON();
	  }
	  else
	  {
		  AMP_OFF();
	  }


	  /*for(uint8_t i = 0; i < CHANNELS_COUNT; i++)
	  {
		  if(channels[i].analyzer->IsAudio())
		  {
			  for(uint8_t y = i + 1; y < CHANNELS_COUNT; y++)
			  {
				  channels[y].switcher->SetCommutationState(false);
			  }
			  channels[i].switcher->SetCommutationState(true);
			  amp_state = true;
			  break;
		  }
		  else
		  {
			  channels[i].switcher->SetCommutationState(false);
		  }
	  }

	  if(amp_state)
	  {
		  AMP_ON();
	  }
	  else
	  {
		  AMP_OFF();
	  }*/
    /* USER CODE BEGIN 3 */
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
  RCC_OscInitStruct.PLL.PLLN = 8;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the peripherals clocks
  */
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_ADC;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_SYSCLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
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
  __disable_irq();
  while (1)
  {
  }
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
