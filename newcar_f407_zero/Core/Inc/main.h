/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "stdbool.h"
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
#define KEY1_Pin GPIO_PIN_0
#define KEY1_GPIO_Port GPIOA
#define BUZZER_Pin GPIO_PIN_1
#define BUZZER_GPIO_Port GPIOA
#define IMU_Pin GPIO_PIN_2
#define IMU_GPIO_Port GPIOA
#define IMUA3_Pin GPIO_PIN_3
#define IMUA3_GPIO_Port GPIOA
#define LED_RED_Pin GPIO_PIN_5
#define LED_RED_GPIO_Port GPIOC
#define LED_BLUE_Pin GPIO_PIN_2
#define LED_BLUE_GPIO_Port GPIOB
#define usart3tx_Pin GPIO_PIN_10
#define usart3tx_GPIO_Port GPIOB
#define usart3rx_Pin GPIO_PIN_11
#define usart3rx_GPIO_Port GPIOB
#define Display_Pin GPIO_PIN_9
#define Display_GPIO_Port GPIOA
#define DisplayA10_Pin GPIO_PIN_10
#define DisplayA10_GPIO_Port GPIOA
#define MORTOR_Pin GPIO_PIN_11
#define MORTOR_GPIO_Port GPIOA
#define MORTORA12_Pin GPIO_PIN_12
#define MORTORA12_GPIO_Port GPIOA
#define SERVO_Pin GPIO_PIN_10
#define SERVO_GPIO_Port GPIOC
#define SERVOC11_Pin GPIO_PIN_11
#define SERVOC11_GPIO_Port GPIOC
#define JETSON_Pin GPIO_PIN_12
#define JETSON_GPIO_Port GPIOC
#define JETSOND2_Pin GPIO_PIN_2
#define JETSOND2_GPIO_Port GPIOD

/* USER CODE BEGIN Private defines */
    extern bool init_finished;
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
