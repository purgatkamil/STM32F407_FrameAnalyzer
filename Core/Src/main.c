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
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "i2c_analyze.h"
#include "hci.h"

#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

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

  int x = 5;
  int y = 5;

extern i2c_bit_buffer_s i2c_bits;

#define BITS_ON_SCREEN 32

typedef struct
{
	int data[BITS_ON_SCREEN];
	int head;
}lcd_buffer_s;

lcd_buffer_s lcd_buffer;

int counter = 32 - 1;

void lcd_buffer_init(lcd_buffer_s *buffer)
{
	buffer->head = BITS_ON_SCREEN - 1;
  	uint16_t i;
  	for (i = 0; i < BITS_ON_SCREEN; i++){
  		buffer->data[i] = i2c_bits.data[i];
  	}
}

void lcd_buffer_add_next(lcd_buffer_s *buffer, uint16_t value)
{
    buffer->data[buffer->head] = value;
    buffer->head = (buffer->head + 1) % BITS_ON_SCREEN;
}

void lcd_buffer_add_previous(lcd_buffer_s *buffer, uint16_t value)
{
	buffer->data[buffer->head] = value;
    buffer->head = (buffer->head - 1 + BITS_ON_SCREEN) % BITS_ON_SCREEN;
}

int lcd_buffer_read_from_head(lcd_buffer_s *buffer, int index)
{
	int cnt = ( (buffer->head) + index) % BITS_ON_SCREEN;
    int result = buffer->data[cnt];

    return result;
}

  void start_display_whole_frame()
  {
	fill_with(BLACK);

	lcd_draw_horizontal_line(10, 0, LCD_WIDTH - LCD_OFFSET_Y, WHITE);
	lcd_draw_horizontal_line(30, 0, LCD_WIDTH - LCD_OFFSET_Y, WHITE);
	const int one_bit_lcd_size = LCD_WIDTH / BITS_ON_SCREEN;

	int last_state = 0;

	for(int i = 0; i < BITS_ON_SCREEN; i++)
	{
		int state = lcd_buffer_read_from_head(&lcd_buffer, i);

		if(state == 0)
		{
			lcd_draw_horizontal_line(10, (i * one_bit_lcd_size) + 1, ((i+1) * one_bit_lcd_size), BLACK);
		}
		else if(state == 1)
		{
			lcd_draw_horizontal_line(30, (i * one_bit_lcd_size) + 1, ((i+1) * one_bit_lcd_size), BLACK);
		}

		if(last_state != state)
		{
			lcd_draw_vertical_line(i * one_bit_lcd_size, 10, 31, WHITE);
		}
		if( ( (i - counter) != 0) && ( (i  % 8) == 0) )
		{
			lcd_draw_vertical_line_dotted( (i - counter) * one_bit_lcd_size, 5, 37, RED);
		}
		last_state = state;

	}

	lcd_copy();
	HAL_Delay(10);

	//i2c_reset_all();
    }

int whole_frame_flag = 0;

  void whole_frame_scrolling()
  {
	fill_with(BLACK);

  	if(scroll_result == 1)
  	{
  		int size = i2c_get_bits_buffor_size();
  		if(counter < size)
  		{
  			counter++;
  			lcd_buffer_add_next(&lcd_buffer, i2c_bits.data[counter]);
  		}
  	}
  	else if(scroll_result == -1)
  	{
  		if((counter - BITS_ON_SCREEN) > 0)
  		{
  	  		lcd_buffer_add_previous(&lcd_buffer, i2c_bits.data[counter - BITS_ON_SCREEN]);
  	  		counter--;
  		}

  	}

    if( (hci_return_delay_timer_state() == 1) && (whole_frame_flag == 1) )
    {
  	  start_display_whole_frame();
    }
  }

void start_display_data_only()
{
	int choosen_color = WHITE;

	int size = i2c_get_bytes_buffor_size();
  	for(int i = 0; i < size; i++)
  	{
  	  switch(i)
  	  {
		  case 0:
		  	  new_data = i2c_get_ready_i2c_byte();
		  	  sprintf(new_data_char, "Device address: 0x%02X", new_data);
		  	  choosen_color = RED;
			  break;

		  case 1:
		  	  new_data = i2c_get_ready_i2c_byte();
		  	  sprintf(new_data_char, "Register address: 0x%02X", new_data);
		  	  choosen_color = YELLOW;
			  break;

		  default:
		  	  new_data = i2c_get_ready_i2c_byte();
		  	  sprintf(new_data_char, "%02d:0x%02X", (i - 1), new_data);
		  	  choosen_color = WHITE;
			  break;
  	  }

  	  LCD_DisplayString( x, y, new_data_char, choosen_color, LCD_FONT8);


  	  if(y + 15 < LCD_HEIGHT)
  	  {
  	      y = y + 10;
  	  }
  	  else
  	  {
  		  y = 25;
  		  x = x + 40;
  	  }
  	}
  	lcd_copy();
  	HAL_Delay(10);

  	i2c_reset_all();
  	new_data = 0;
  	for(int i = 0; i < 30; i++)
  	{
  		new_data_char[i] = 0;
  	}

  	fill_with(BLACK);
  	y = 5;
  	x = 5;
}

extern int8_t encoder_flag;
int debounce_active = 0;
extern volatile int menu_opened;



void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim == &htim13)
    {
    	HAL_TIM_Base_Stop_IT(&htim13);
    	debounce_active = 0;

    	if(HAL_GPIO_ReadPin(ENCODER_BUTTON_GPIO_Port, ENCODER_BUTTON_Pin) == GPIO_PIN_RESET)
    	{
    		if(menu_opened == 1)
    		{
    			menu_enter();
    			menu_refresh();
    		}
    		else
    		{
    			whole_frame_flag = 0;
    			i2c_reset_all();
    			hci_display_menu();
    		}
    	}
    }

    if (htim == &htim14) {
    	encoder_flag = 1;
    }
}

int start_display_flag = 0;
extern volatile bool hci_i2c_flag;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == ENCODER_BUTTON_Pin)
	{
        if (debounce_active == 0)
        {
            debounce_active = 1;
            HAL_TIM_Base_Start_IT(&htim13);
        }
	}

	if(hci_i2c_flag == true)
	{
		switch(GPIO_Pin)
		{
			case ANALYZER_SCL_IT_RISING_Pin:
				i2c_read_data();
			    break;

			case ANALYZER_SCL_IT_FALLING_Pin:
				i2c_scl_falling();
			    break;

			case ANALYZER_SDA_IT_RISING_Pin:
				if(i2c_check_for_stop() == I2C_STOP)
				{
					i2c_convert_i2c_bytes();
					lcd_buffer_init(&lcd_buffer);
					start_display_flag = 1;
				}
			    break;

			case ANALYZER_SDA_IT_FALLING_Pin:
				i2c_check_for_start();
			    break;

			default:
			    break;
		}
	}

	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_Pin);
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
  MX_TIM2_Init();
  MX_TIM14_Init();
  MX_TIM13_Init();
  /* USER CODE BEGIN 2 */


  hci_encoder_init();


  lcd_init();
  fill_with(BLACK);
  lcd_copy();

  extern display_mode mode;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if (start_display_flag == 1)
	  {
		  fill_with(BLACK);
		  switch(mode)
		  {
			  case DISPLAY_MODE_DATA_ONLY:
				  start_display_data_only();
				  start_display_flag = 0;
				  break;

			  case DISPLAY_MODE_WHOLE_FRAME:
				  start_display_whole_frame();
				  whole_frame_flag = 1;
				  start_display_flag = 0;
				  break;
		  }
	  }

	  hci_scroll();

	  if(menu_opened == 1)
	  {
		  menu_scrolling();
	  }

	  if(whole_frame_flag == 1)
	  {
		  whole_frame_scrolling();
	  }


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
