/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>

#include "4x4-keypad.h"
#include "keypad_gpio_impl.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

SKeypad tKeypad;
SKey tKey[KEYPAD_ROW_SIZE * KEYPAD_COLUMN_SIZE];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void keyEventCallback(SKeypad* ptKeypad, EKeyState eKeyState, uint8_t u8KeyChar)
{
	char msg[50];
	if(ptKeypad == &tKeypad)
	{
		switch (eKeyState) {
			case KEY_PRESSED:
				snprintf((char*)msg, 100, "Pressed the %c key\n", (char)u8KeyChar);
				HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 100);
				break;
			case KEY_RELEASED:
				snprintf((char*)msg, 100, "Released the %c key\n", (char)u8KeyChar);
				HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 100);
				break;
			case KEY_HOLD:
				snprintf((char*)msg, 100, "Hold the %c key\n", (char)u8KeyChar);
				HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 100);
				break;
			case KEY_CLICKED:
				snprintf((char*)msg, 100, "Clicked the %c key\n", (char)u8KeyChar);
				HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 100);
				break;
			default:

				break;
		}
	}
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
  MX_USART3_UART_Init();
  MX_TIM17_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  SKeyGPIO tKeypadColumnPort[KEYPAD_COLUMN_SIZE] = {
	{
		.vpGpioPort = GPIOE,
		.u16GpioPin = GPIO_PIN_3,
		.eGpioContactType = IO_NORMALLY_CLOSE,
	},
	{
		.vpGpioPort = GPIOF,
		.u16GpioPin = GPIO_PIN_8,
		.eGpioContactType = IO_NORMALLY_CLOSE,
	},
	{
		.vpGpioPort = GPIOF,
		.u16GpioPin = GPIO_PIN_7,
		.eGpioContactType = IO_NORMALLY_CLOSE,
	},
	{
		.vpGpioPort = GPIOF,
		.u16GpioPin = GPIO_PIN_9,
		.eGpioContactType = IO_NORMALLY_CLOSE,
	}
  };

  SKeyGPIO tKeypadRowPort[KEYPAD_ROW_SIZE] = {
	{
		.vpGpioPort = GPIOE,
		.u16GpioPin = GPIO_PIN_2,
		.eGpioContactType = IO_NORMALLY_CLOSE,
	},
	{
		.vpGpioPort = GPIOE,
		.u16GpioPin = GPIO_PIN_4,
		.eGpioContactType = IO_NORMALLY_CLOSE,
	},
	{
		.vpGpioPort = GPIOE,
		.u16GpioPin = GPIO_PIN_5,
		.eGpioContactType = IO_NORMALLY_CLOSE,
	},
	{
		.vpGpioPort = GPIOE,
		.u16GpioPin = GPIO_PIN_6,
		.eGpioContactType = IO_NORMALLY_CLOSE,
	}
  };

  SKeypadIO tKeypadIO = {
	.ioColumn = tKeypadColumnPort,
	.ioRow = tKeypadRowPort,
	.gpioRead = gpio_read,
	.gpioWrite = gpio_write,
	.gpioOutput = gpio_setPinOutput,
	.gpioInput = gpio_setPinInput,
  };

  uint8_t u8KeyMap[KEYPAD_ROW_SIZE][KEYPAD_COLUMN_SIZE] =
  {
	{'1', '2', '3', 'A'},
	{'4', '5', '6', 'B'},
	{'7', '8', '9', 'C'},
	{'*', '0', '#', 'D'},
  };


  keypad_init(&tKeypad, tKey, &tKeypadIO, (uint8_t*)u8KeyMap);
  keypad_addEventCallback(&tKeypad, keyEventCallback);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1) {

		keypad_scan(&tKeypad, HAL_GetTick());

    /* USER CODE END WHILE */

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

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 50;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
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
	while (1) {
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
