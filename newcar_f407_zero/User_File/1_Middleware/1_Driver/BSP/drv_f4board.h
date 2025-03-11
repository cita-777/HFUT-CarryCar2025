/**
 * *****************************************************************************
 * @file        drv_f4board.h
 * @brief
 * @author      ciat-777 (juricek.chen@gmail.com)
 * @date        2025-03-11
 * @copyright   cita
 * *****************************************************************************
 */

#ifndef DRV_F4BOARD_H
#define DRV_F4BOARD_H

/* Includes ------------------------------------------------------------------*/

#include "stm32f4xx_hal.h"
#include "tim.h"
/* Exported macros -----------------------------------------------------------*/

// 初始化LED被按位或的参数
#define BSP_LED_RED_ON (1 << 0)
#define BSP_LED_BLUE_ON (1 << 1)

// 魔女F407最小系统板引脚别名
#define Board_KEY1_Pin GPIO_PIN_0
#define Board_KEY1_GPIO_Port GPIOA
#define Board_BUZZER_Pin GPIO_PIN_1
#define Board_BUZZER_GPIO_Port GPIOA
#define Board_LED_RED_Pin GPIO_PIN_5
#define Board_LED_RED_GPIO_Port GPIOC
#define Board_LED_BLUE_Pin GPIO_PIN_2
#define Board_LED_BLUE_GPIO_Port GPIOB

/* Exported types ------------------------------------------------------------*/

/**
 * @brief 板上LED工作状态
 *
 */
enum Enum_BSP_LED_Status
{
    BSP_LED_Status_ENABLED = 0,
    BSP_LED_Status_DISABLED,
};

/**
 * @brief 按键状态
 *
 */
enum Enum_BSP_Key_Status
{
    BSP_Key_Status_FREE = 0,
    BSP_Key_Status_TRIG_FREE_PRESSED,
    BSP_Key_Status_TRIG_PRESSED_FREE,
    BSP_Key_Status_PRESSED,
};

/* Exported variables --------------------------------------------------------*/

extern TIM_HandleTypeDef htim1;   // 假设蜂鸣器使用TIM1，请根据实际配置修改

/* Exported function declarations --------------------------------------------*/

void BSP_Init(uint32_t Status, float Buzzer_Rate = 0);

Enum_BSP_LED_Status BSP_Get_LED_RED();
Enum_BSP_LED_Status BSP_Get_LED_BLUE();

Enum_BSP_Key_Status BSP_Get_Key();

void BSP_Set_LED_RED(Enum_BSP_LED_Status Status);
void BSP_Set_LED_BLUE(Enum_BSP_LED_Status Status);

void BSP_Set_PWM_Buzzer(float Rate);

#endif
