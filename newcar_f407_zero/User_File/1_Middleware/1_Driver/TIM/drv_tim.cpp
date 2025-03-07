/**
 * *****************************************************************************
 * @file        drv_tim.cpp
 * @brief
 * @author      cita (juricek.chen@gmail.com)
 * @date        2025-03-07
 * @copyright   cita
 * *****************************************************************************
 */

/*----------------------------------include-----------------------------------*/
#include "drv_tim.h"
/*-----------------------------------macro------------------------------------*/

/*----------------------------------typedef-----------------------------------*/

/*----------------------------------variable----------------------------------*/
Class_Timer TIM_1ms;
/*-------------------------------------os-------------------------------------*/

/*----------------------------------function----------------------------------*/

/**
 * @brief 初始化
 *
 * @param __Delay 增长最大幅度
 */
void Class_Timer::Init(uint32_t __Delay)
{
    Delay = __Delay;
}

/**
 * @brief 定时器调整状态, 计时周期1ms
 *
 */
void Class_Timer::TIM_1ms_Calculate_PeriodElapsedCallback()
{
    Tick++;

    if (Delay == 0)
    {
        Now_Status = Timer_Status_RESET;
    }
    else if (Tick < Delay)
    {
        Now_Status = Timer_Status_WAIT;
    }
    else if (Tick == Delay)
    {
        Now_Status = Timer_Status_TRIGGER;
    }
    else if (Tick > Delay)
    {
        Now_Status = Timer_Status_TIMEOUT;
    }
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
    if (htim == (&htim6))   // 0.001s触发一次中断
    {
        TIM_1ms.TIM_1ms_Calculate_PeriodElapsedCallback();
    }
}
/*------------------------------------test------------------------------------*/
