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
#define BoardA_Pin_J2_Pin GPIO_PIN_4
#define BoardA_Pin_J2_GPIO_Port GPIOE
#define BoardA_Pin_J1_Pin GPIO_PIN_5
#define BoardA_Pin_J1_GPIO_Port GPIOE
#define BoardA_Pin_K1_Pin GPIO_PIN_6
#define BoardA_Pin_K1_GPIO_Port GPIOE
#define BoardA_Pin_N2_Pin GPIO_PIN_0
#define BoardA_Pin_N2_GPIO_Port GPIOC
#define BoardA_Pin_O2_Pin GPIO_PIN_1
#define BoardA_Pin_O2_GPIO_Port GPIOC
#define BoardA_Pin_L1_Pin GPIO_PIN_2
#define BoardA_Pin_L1_GPIO_Port GPIOC
#define BoardA_Pin_M1_Pin GPIO_PIN_3
#define BoardA_Pin_M1_GPIO_Port GPIOC
#define BoardA_UART7_Tx_Pin GPIO_PIN_0
#define BoardA_UART7_Tx_GPIO_Port GPIOA
#define BoardA_UART7_Rx_Pin GPIO_PIN_1
#define BoardA_UART7_Rx_GPIO_Port GPIOA
#define BoardA_Pin_U_Pin GPIO_PIN_2
#define BoardA_Pin_U_GPIO_Port GPIOA
#define BoardA_Pin_P2_Pin GPIO_PIN_4
#define BoardA_Pin_P2_GPIO_Port GPIOA
#define BoardA_SPI5_Clk_Pin GPIO_PIN_5
#define BoardA_SPI5_Clk_GPIO_Port GPIOA
#define BoardA_SPI5_MISO_Pin GPIO_PIN_6
#define BoardA_SPI5_MISO_GPIO_Port GPIOA
#define BoardA_SPI5_MOSI_Pin GPIO_PIN_7
#define BoardA_SPI5_MOSI_GPIO_Port GPIOA
#define BoardA_Pin_N1_Pin GPIO_PIN_4
#define BoardA_Pin_N1_GPIO_Port GPIOC
#define BoardA_Pin_O1_Pin GPIO_PIN_5
#define BoardA_Pin_O1_GPIO_Port GPIOC
#define BoardA_Pin_L2_Pin GPIO_PIN_0
#define BoardA_Pin_L2_GPIO_Port GPIOB
#define BoardA_Pin_M2_Pin GPIO_PIN_1
#define BoardA_Pin_M2_GPIO_Port GPIOB
#define BoardA_LED_R_Pin GPIO_PIN_11
#define BoardA_LED_R_GPIO_Port GPIOE
#define BoardA_Pin_K2_Pin GPIO_PIN_12
#define BoardA_Pin_K2_GPIO_Port GPIOE
#define BoardA_CAN2_Rx_Pin GPIO_PIN_12
#define BoardA_CAN2_Rx_GPIO_Port GPIOB
#define BoardA_CAN2_Tx_Pin GPIO_PIN_13
#define BoardA_CAN2_Tx_GPIO_Port GPIOB
#define BoardA_PWM_Buzzer_Pin GPIO_PIN_14
#define BoardA_PWM_Buzzer_GPIO_Port GPIOB
#define BoardA_USART3_Tx_Pin GPIO_PIN_8
#define BoardA_USART3_Tx_GPIO_Port GPIOD
#define BoardA_USART3_Rx_Pin GPIO_PIN_9
#define BoardA_USART3_Rx_GPIO_Port GPIOD
#define BoardA_Pin_H_Pin GPIO_PIN_12
#define BoardA_Pin_H_GPIO_Port GPIOD
#define BoardA_Pin_G_Pin GPIO_PIN_13
#define BoardA_Pin_G_GPIO_Port GPIOD
#define BoardA_Pin_F_Pin GPIO_PIN_14
#define BoardA_Pin_F_GPIO_Port GPIOD
#define BoardA_Pin_E_Pin GPIO_PIN_15
#define BoardA_Pin_E_GPIO_Port GPIOD
#define BoardA_USART6_Tx_Pin GPIO_PIN_6
#define BoardA_USART6_Tx_GPIO_Port GPIOC
#define BoardA_USART6_Rx_Pin GPIO_PIN_7
#define BoardA_USART6_Rx_GPIO_Port GPIOC
#define BoardA_USB_DM_Pin GPIO_PIN_11
#define BoardA_USB_DM_GPIO_Port GPIOA
#define BoardA_USB_DP_Pin GPIO_PIN_12
#define BoardA_USB_DP_GPIO_Port GPIOA
#define BoardA_CAN1_Rx_Pin GPIO_PIN_0
#define BoardA_CAN1_Rx_GPIO_Port GPIOD
#define BoardA_CAN1_Tx_Pin GPIO_PIN_1
#define BoardA_CAN1_Tx_GPIO_Port GPIOD
#define BoardA_Bluetooth_Tx_Pin GPIO_PIN_5
#define BoardA_Bluetooth_Tx_GPIO_Port GPIOD
#define BoardA_Bluetooth_Rx_Pin GPIO_PIN_6
#define BoardA_Bluetooth_Rx_GPIO_Port GPIOD
#define BoardA_PWM_MPU6500_Heater_Pin GPIO_PIN_5
#define BoardA_PWM_MPU6500_Heater_GPIO_Port GPIOB
#define BoardA_DBUS_Idle_Pin GPIO_PIN_6
#define BoardA_DBUS_Idle_GPIO_Port GPIOB
#define BoardA_DBUS_Rx_Pin GPIO_PIN_7
#define BoardA_DBUS_Rx_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
