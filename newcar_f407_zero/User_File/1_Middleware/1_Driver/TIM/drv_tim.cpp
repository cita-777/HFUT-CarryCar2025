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
#include "4_Task\task_config.h"
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
bool Class_Timer::DelayNonBlocking(uint32_t __Delay)
{
    // 移除静态变量，改为使用状态机逻辑
    if (Get_Now_Status() == Timer_Status_RESET)
    {
        // 未启动则初始化延时
        Set_Delay(__Delay);
        return false;   // 延时开始，但还未完成
    }

    // 检查当前延时状态
    if (Get_Now_Status() == Timer_Status_TRIGGER || Get_Now_Status() == Timer_Status_TIMEOUT)
    {
        // 延时结束，复位定时器
        Set_Delay(0);
        return true;
    }
    return false;
}
/*------------------------------------test------------------------------------*/
