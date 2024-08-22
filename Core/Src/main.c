/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "dma.h"
#include "spi.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "lcd.h"
#include "i2c_analyze.h"

#include <math.h>
#include <string.h>
#include <stdio.h>

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

  int new_data = 0;
  char new_data_char[30];

  int x = 10;
  int y = 10;

  void start_display()
  {
  	for(int i = 0; i <= 2; i++)
  	{
  	  new_data = i2c_get_ready_i2c_byte();
  	  sprintf(new_data_char, "Device address: 0x%X", new_data);
  	 // fill_with(BLACK);
  	  LCD_DisplayString( x, y, new_data_char, WHITE);
  	  y = y + 10;
  	  //LCD_DisplayString( 5, 5, device_address_char, WHITE);
  	 // LCD_DisplayString( 5, 15, register_address_char, WHITE);
  	 // LCD_DisplayString( 5, 25, iic_data_char, WHITE);
  	  lcd_copy();
  	  HAL_Delay(100);
  	}

  	i2c_reset_all();
  	new_data = 0;
  	for(int i = 0; i < 30; i++)
  	{
  		new_data_char[i] = 0;
  	}
  }

  int start_display_flag = 0;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	switch(GPIO_Pin)
	{
		case ANALYZER_SCL_IT_RISING_Pin:

			i2c_read_data();
		    __HAL_GPIO_EXTI_CLEAR_IT(ANALYZER_SCL_IT_RISING_Pin);
		    break;

		case ANALYZER_SCL_IT_FALLING_Pin:
			i2c_scl_falling();
			__HAL_GPIO_EXTI_CLEAR_IT(ANALYZER_SCL_IT_FALLING_Pin);
		    break;

		case ANALYZER_SDA_IT_RISING_Pin:
			if(i2c_check_for_stop() == I2C_STOP)
			{
				start_display_flag = 1;
			}
			__HAL_GPIO_EXTI_CLEAR_IT(ANALYZER_SDA_IT_RISING_Pin);
		    break;

		case ANALYZER_SDA_IT_FALLING_Pin:
			i2c_check_for_start();
			__HAL_GPIO_EXTI_CLEAR_IT(ANALYZER_SDA_IT_FALLING_Pin);
		    break;

		default:
		    break;
	}
}



/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */

  lcd_init();
  fill_with(BLACK);
  lcd_copy();



  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if (start_display_flag == 1)
	  {
		  start_display();
		  start_display_flag = 0;
	  }
	 // if(i2c_is_data_ready() == CONVERSION_READY)//data_pointer >= 28)
	//  {
		/*  uint8_t device_address = 0;
		  uint8_t register_address = 0;
		  uint8_t iic_data = 0;

		  char device_address_char[30];
		  char register_address_char[30];
		  char iic_data_char[30];

		  uint8_t power = 0;*/

		 /* for(int i = 0; i < 8; i++)
		  {
			  power = pow(2, 7 - i);
			  device_address += data[i] * power;
			  register_address += data[9 + i] * power;
			  iic_data += data[18 + i] * power;
		  }*/

		 // sprintf(device_address_char, "Device address: 0x%X", device_address);
		 // sprintf(register_address_char, "Register address: 0x%X", register_address);
		//  sprintf(iic_data_char, "Data: 0x%X", iic_data);

		  //data_pointer = 0;
	 // }
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
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
