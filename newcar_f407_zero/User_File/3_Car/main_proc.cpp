/**
 * *****************************************************************************
 * @file        main_proc.cpp
 * @brief
 * @author      ciat-777
 * @date        2025-03-07
 * *****************************************************************************
 */

#include "main_proc.h"
#include "1_Middleware/1_Driver/TIM/drv_tim.h"   // 如果需要使用定时器可以使用此头文件
#include <stdbool.h>
#include <stdint.h>

// 示例任务1
static void Task1(void)
{
    // 在此执行任务1操作
}

// 示例任务2
static void Task2(void)
{
    // 在此执行任务2操作
}

// 示例任务3
static void Task3(void)
{
    // 在此执行任务3操作
}

// 状态“结束”或其他状态，比如重置状态机
static void State_Done(void)
{
    // 可在此执行状态机结束后的操作
}

// 状态函数数组（状态按顺序执行）
static ProcHandler_t procHandlers[PROC_STATE_MAX] = {
    Task1,       // 状态0：任务1
    Task2,       // 状态1：任务2
    Task3,       // 状态2：任务3
    State_Done   // 状态3：结束状态，可用于重置状态机
};

// 当前状态索引，初始为任务1
static uint8_t currentState = 0;
// 手动设置状态的标志，当设置后下一次 Proc_Run() 将不自动递增状态
static bool manualOverride = false;

// 状态机处理函数，每次调用执行当前状态函数
// 如果未手动修改状态，则按照循环顺序自动递增状态
void main_proc_run(void)
{
    // 执行当前状态函数
    procHandlers[currentState]();

    // 仅当没有手动设置时，自动递增状态索引（循环方式）
    if (!manualOverride)
    {
        currentState = (currentState + 1) % PROC_STATE_MAX;
    }
    else
    {
        // 重置手动控制标志，不覆盖当前状态
        manualOverride = false;
    }
}

// 调试接口：手动设置当前状态（同时设置手动控制标志）
void Proc_SetState(uint8_t state)
{
    if (state < PROC_STATE_MAX)
    {
        currentState   = state;
        manualOverride = true;
    }
}