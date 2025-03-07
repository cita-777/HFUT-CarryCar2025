/**
 * *****************************************************************************
 * @file        drv_tim.h
 * @brief
 * @author      cita (juricek.chen@gmail.com)
 * @date        2025-03-07
 * @copyright   cita
 * *****************************************************************************
 */


#ifndef DRV_TIM_H
#define DRV_TIM_H

#ifdef __cplusplus
extern "C" {
#endif


/*----------------------------------include-----------------------------------*/
#include "stm32f4xx_hal.h"
#include "tim.h"
#include <string.h>
/*-----------------------------------macro------------------------------------*/

/*----------------------------------typedef-----------------------------------*/
/**
 * @brief 状态
 *
 */
enum Enum_Timer_Status
{
    Timer_Status_RESET = 0,
    Timer_Status_WAIT,
    Timer_Status_TRIGGER,
    Timer_Status_TIMEOUT,
};

/**
 * @brief Reusable, 定时器本体, 一般以1ms为界
 *
 */
class Class_Timer
{
public:
    void Init(uint32_t __Delay);

    inline uint32_t Get_Tick();

    inline Enum_Timer_Status Get_Now_Status();

    inline void Set_Delay(uint32_t __Delay);

    void TIM_1ms_Calculate_PeriodElapsedCallback();

protected:
    // 初始化相关常量

    // 常量

    // 内部变量

    // 读变量

    // 时钟计数
    uint32_t Tick = 0;

    // 写变量

    // 延迟时间单元数
    uint32_t Delay = 0;

    // 读写变量

    // 当前状态
    Enum_Timer_Status Now_Status = Timer_Status_RESET;

    // 内部函数
};

/*----------------------------------variable----------------------------------*/
extern Class_Timer TIM_1ms;
/*-------------------------------------os-------------------------------------*/

/*----------------------------------function----------------------------------*/
/**
 * @brief 获取时钟计数
 *
 * @return 时钟计数
 */
inline uint32_t Class_Timer::Get_Tick()
{
    return (Tick);
}

/**
 * @brief 获取当前状态
 *
 * @return 当前状态
 */
inline Enum_Timer_Status Class_Timer::Get_Now_Status()
{
    return (Now_Status);
}

/**
 * @brief 设定当前状态
 *
 * @param __Now_Status 当前状态
 */
inline void Class_Timer::Set_Delay(uint32_t __Delay)
{
    Delay = __Delay;
    Tick  = 0;
    if (Delay != 0)
    {
        Now_Status = Timer_Status_WAIT;
    }
    else
    {
        Now_Status = Timer_Status_RESET;
    }
}

/*------------------------------------test------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* DRV_TIM_H */
