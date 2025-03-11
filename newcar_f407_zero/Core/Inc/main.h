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
#define IMU_TX_Pin GPIO_PIN_2
#define IMU_TX_GPIO_Port GPIOA
#define IMU_RX_Pin GPIO_PIN_3
#define IMU_RX_GPIO_Port GPIOA
#define LED_RED_Pin GPIO_PIN_5
#define LED_RED_GPIO_Port GPIOC
#define LED_BLUE_Pin GPIO_PIN_2
#define LED_BLUE_GPIO_Port GPIOB
#define WIFI_TX_Pin GPIO_PIN_10
#define WIFI_TX_GPIO_Port GPIOB
#define WIFI_RX_Pin GPIO_PIN_11
#define WIFI_RX_GPIO_Port GPIOB
#define DISPLAY_TX_Pin GPIO_PIN_9
#define DISPLAY_TX_GPIO_Port GPIOA
#define DISPLAY_RX_Pin GPIO_PIN_10
#define DISPLAY_RX_GPIO_Port GPIOA
#define MORTOR_RX_Pin GPIO_PIN_11
#define MORTOR_RX_GPIO_Port GPIOA
#define MORTOR_TX_Pin GPIO_PIN_12
#define MORTOR_TX_GPIO_Port GPIOA
#define SERVO_TX_Pin GPIO_PIN_10
#define SERVO_TX_GPIO_Port GPIOC
#define SERVO_RX_Pin GPIO_PIN_11
#define SERVO_RX_GPIO_Port GPIOC
#define JETSON_TX_Pin GPIO_PIN_12
#define JETSON_TX_GPIO_Port GPIOC
#define JETSON_RX_Pin GPIO_PIN_2
#define JETSON_RX_GPIO_Port GPIOD

/* USER CODE BEGIN Private defines */
extern bool init_finished;
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
