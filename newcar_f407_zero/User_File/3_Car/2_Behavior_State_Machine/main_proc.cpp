/**
 * *****************************************************************************
 * @file        main_proc.cpp
 * @brief
 * @author      ciat-777
 * @date        2025-03-07
 * *****************************************************************************
 */

#include "main_proc.h"
#include "1_Middleware/1_Driver/TIM/drv_tim.h"
#include "2_Device/Vofa/dvc_vofa.h"
#include <stdbool.h>
#include <stdint.h>

// 复杂任务1
static bool Task1(void)
{
    static int stage = 0;

    switch (stage)
    {
    case 0:
        Vofa_FireWater("Task1:1\r\n");
        stage = 1;
        return false;   // 继续执行

    case 1:
        if (!TIM_1ms.DelayNonBlocking(300)) return false;   // 延时中

        Vofa_FireWater("Task1:2\r\n");
        stage = 2;
        return false;   // 继续执行

    case 2:
        if (!TIM_1ms.DelayNonBlocking(200)) return false;   // 延时中

        Vofa_FireWater("Task1:3\r\n");
        stage = 0;     // 重置状态
        return true;   // 任务完成
    }

    return true;
}

// 复杂任务2
static bool Task2(void)
{
    static int stage = 0;

    switch (stage)
    {
    case 0:
        Vofa_FireWater("Task2:1\r\n");
        stage = 1;
        return false;   // 继续执行

    case 1:
        if (!TIM_1ms.DelayNonBlocking(500))   // 不同的延时时间
            return false;                     // 延时中

        Vofa_FireWater("Task2:2\r\n");
        stage = 2;
        return false;   // 继续执行

    case 2:
        if (!TIM_1ms.DelayNonBlocking(400)) return false;   // 延时中

        Vofa_FireWater("Task2:3\r\n");
        stage = 0;     // 重置状态
        return true;   // 任务完成
    }

    return true;
}

// 复杂任务3
static bool Task3(void)
{
    static int stage = 0;

    switch (stage)
    {
    case 0:
        Vofa_FireWater("Task3:1\r\n");
        stage = 1;
        return false;   // 继续执行

    case 1:
        if (!TIM_1ms.DelayNonBlocking(150)) return false;   // 延时中

        Vofa_FireWater("Task3:2\r\n");
        stage = 2;
        return false;   // 继续执行

    case 2:
        if (!TIM_1ms.DelayNonBlocking(350)) return false;   // 延时中

        Vofa_FireWater("Task3:3\r\n");
        stage = 0;     // 重置状态
        return true;   // 任务完成
    }

    return true;
}

// 状态"结束"
static bool State_Done(void)
{
    static int stage = 0;

    switch (stage)
    {
    case 0:
        Vofa_FireWater("State_Done:0\r\n");
        stage = 1;
        return false;   // 继续执行

    case 1:
        if (!TIM_1ms.DelayNonBlocking(1000))   // 较长延时
            return false;                      // 延时中

        Vofa_FireWater("State_Done: 1\r\n");
        stage = 0;     // 重置状态
        return true;   // 任务完成
    }

    return true;
}

// 状态函数数组（状态按顺序执行）
static ProcHandler_t procHandlers[PROC_STATE_MAX] = {
    Task1,       // 状态0：任务1
    Task2,       // 状态1：任务2
    Task3,       // 状态2：任务3
    State_Done   // 状态3：结束状态
};

// 当前状态索引，初始为任务1
static uint8_t currentState = 0;
// 手动设置状态的标志
static bool manualOverride = false;

// 状态机处理函数
void main_proc_run(void)
{
    // 执行当前状态函数，并获取任务完成状态
    bool taskCompleted = procHandlers[currentState]();

    // 仅当任务完成且没有手动设置时，自动递增状态
    if (taskCompleted && !manualOverride)
    {
        // 切换状态前重置定时器
        TIM_1ms.Set_Delay(0);

        currentState = (currentState + 1) % PROC_STATE_MAX;
    }
    else if (manualOverride)
    {
        // 重置手动控制标志前重置定时器
        TIM_1ms.Set_Delay(0);
        manualOverride = false;
    }
}

// 调试接口：手动设置当前状态
void Proc_SetState(uint8_t state)
{
    if (state < PROC_STATE_MAX)
    {
        currentState   = state;
        manualOverride = true;
    }
}