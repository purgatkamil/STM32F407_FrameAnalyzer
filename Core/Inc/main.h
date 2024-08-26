/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define ENCODER_BUTTON_Pin GPIO_PIN_1
#define ENCODER_BUTTON_GPIO_Port GPIOC
#define ENCODER_BUTTON_EXTI_IRQn EXTI1_IRQn
#define LCD_RST_Pin GPIO_PIN_3
#define LCD_RST_GPIO_Port GPIOC
#define LCD_DC_Pin GPIO_PIN_1
#define LCD_DC_GPIO_Port GPIOA
#define LCD_SS_Pin GPIO_PIN_3
#define LCD_SS_GPIO_Port GPIOA
#define LCD_SPI1_SCK_Pin GPIO_PIN_5
#define LCD_SPI1_SCK_GPIO_Port GPIOA
#define LCD_SPI1_MOSI_Pin GPIO_PIN_7
#define LCD_SPI1_MOSI_GPIO_Port GPIOA
#define ANALYZER_SCL_IT_RISING_Pin GPIO_PIN_7
#define ANALYZER_SCL_IT_RISING_GPIO_Port GPIOC
#define ANALYZER_SCL_IT_RISING_EXTI_IRQn EXTI9_5_IRQn
#define ANALYZER_SCL_IT_FALLING_Pin GPIO_PIN_8
#define ANALYZER_SCL_IT_FALLING_GPIO_Port GPIOC
#define ANALYZER_SCL_IT_FALLING_EXTI_IRQn EXTI9_5_IRQn
#define ANALYZER_SDA_IT_RISING_Pin GPIO_PIN_10
#define ANALYZER_SDA_IT_RISING_GPIO_Port GPIOC
#define ANALYZER_SDA_IT_RISING_EXTI_IRQn EXTI15_10_IRQn
#define ANALYZER_SDA_IT_FALLING_Pin GPIO_PIN_11
#define ANALYZER_SDA_IT_FALLING_GPIO_Port GPIOC
#define ANALYZER_SDA_IT_FALLING_EXTI_IRQn EXTI15_10_IRQn

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
