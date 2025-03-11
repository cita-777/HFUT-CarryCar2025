/**
 * *****************************************************************************
 * @file        drv_f4board.cpp
 * @brief
 * @author      ciat-777 (juricek.chen@gmail.com)
 * @date        2025-03-11
 * @copyright   cita
 * *****************************************************************************
 */

/* Includes ------------------------------------------------------------------*/

#include "drv_f4board.h"

/* Private macros ------------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function declarations ---------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

/**
 * @brief 初始化全部板级支持包引脚
 *
 * @param Status 各个状态的按位或
 */
void BSP_Init(uint32_t Status, float Buzzer_Rate)
{
    BSP_Set_LED_RED(static_cast<Enum_BSP_LED_Status>((Status & BSP_LED_RED_ON) == 0 ? BSP_LED_Status_DISABLED
                                                                                    : BSP_LED_Status_ENABLED));
    BSP_Set_LED_BLUE(static_cast<Enum_BSP_LED_Status>((Status & BSP_LED_BLUE_ON) == 0 ? BSP_LED_Status_DISABLED
                                                                                      : BSP_LED_Status_ENABLED));
    // 启动TIM6定时器
    HAL_TIM_Base_Start_IT(&htim6);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);   // 假设蜂鸣器使用TIM1的通道2，请根据实际配置修改

    BSP_Set_PWM_Buzzer(Buzzer_Rate);
}

/**
 * @brief 获取红色LED状态
 *
 * @return Enum_BSP_LED_Status 状态
 */
Enum_BSP_LED_Status BSP_Get_LED_RED()
{
    return (static_cast<Enum_BSP_LED_Status>(HAL_GPIO_ReadPin(Board_LED_RED_GPIO_Port, Board_LED_RED_Pin)));
}

/**
 * @brief 获取蓝色LED状态
 *
 * @return Enum_BSP_LED_Status 状态
 */
Enum_BSP_LED_Status BSP_Get_LED_BLUE()
{
    return (static_cast<Enum_BSP_LED_Status>(HAL_GPIO_ReadPin(Board_LED_BLUE_GPIO_Port, Board_LED_BLUE_Pin)));
}

/**
 * @brief 获取按键状态
 *
 * @return Enum_BSP_Key_Status 状态
 */
Enum_BSP_Key_Status BSP_Get_Key()
{
    static GPIO_PinState pre_key_status;
    GPIO_PinState        key_status;
    Enum_BSP_Key_Status  return_value;

    key_status = HAL_GPIO_ReadPin(Board_KEY1_GPIO_Port, Board_KEY1_Pin);

    switch (pre_key_status)
    {
    case (GPIO_PIN_RESET):
    {
        switch (key_status)
        {
        case (GPIO_PIN_RESET):
        {
            pre_key_status = key_status;
            return_value   = BSP_Key_Status_FREE;

            break;
        }
        case (GPIO_PIN_SET):
        {
            pre_key_status = key_status;
            return_value   = BSP_Key_Status_TRIG_FREE_PRESSED;

            break;
        }
        }

        break;
    }
    case (GPIO_PIN_SET):
    {
        switch (key_status)
        {
        case (GPIO_PIN_RESET):
        {
            pre_key_status = key_status;
            return_value   = BSP_Key_Status_TRIG_PRESSED_FREE;

            break;
        }
        case (GPIO_PIN_SET):
        {
            pre_key_status = key_status;
            return_value   = BSP_Key_Status_PRESSED;

            break;
        }
        }

        break;
    }
    }

    return (return_value);
}

/**
 * @brief 设置红色LED
 *
 * @param Status 状态
 */
void BSP_Set_LED_RED(Enum_BSP_LED_Status Status)
{
    HAL_GPIO_WritePin(Board_LED_RED_GPIO_Port, Board_LED_RED_Pin, static_cast<GPIO_PinState>(Status));
}

/**
 * @brief 设置蓝色LED
 *
 * @param Status 状态
 */
void BSP_Set_LED_BLUE(Enum_BSP_LED_Status Status)
{
    HAL_GPIO_WritePin(Board_LED_BLUE_GPIO_Port, Board_LED_BLUE_Pin, static_cast<GPIO_PinState>(Status));
}

/**
 * @brief 设置蜂鸣器
 *
 * @param Rate 蜂鸣器响度占空比
 */
void BSP_Set_PWM_Buzzer(float Rate)
{
    __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2, Rate * 1000);   // 1000为ARR值，rate固定属于0~1
}