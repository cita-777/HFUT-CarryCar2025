/**
 * *****************************************************************************
 * @file        task_config.cpp
 * @brief       任务配置文件实现 (非RTOS版本)
 * @author      ciat-777
 * @date        2025-03-07
 * *****************************************************************************
 */

#include "task_config.h"
#include "1_Middleware/1_Driver/TIM/drv_tim.h"
#include "1_Middleware/1_Driver/WDG/drv_wdg.h"
#include "2_Device/Motor/Motor_ZDT42/ZDT_X42_V2.h"
#include "2_Device/Vofa/vofa.h"
#include "2_Device/tjc_screen/tjc.h"
#include "3_Car/main_proc.h"
#include "arm_math.h"
#include "main.h"
#include <string.h>
void feed_dog();
void test();
void delayed_task();
void tjc_start_detection();
// 任务配置表, 根据需要添加任务配置项，示例中默认使能状态为1（使能）
// 若暂不使用任务，可将taskFunction置为NULL
TaskConfig_t taskConfigTable[TASK_MAX_NUM] = {
    // 示例任务配置: {Task_Example, "Task_Example", 1},
    // 可继续添加任务项，比如:
    // {NULL, NULL, 0},
    {main_proc_run, "main_proc_run", 1},
    {feed_dog, "feed_dog", 1},
    {test, "test", 1},
    {tjc_start_detection, "tjc_start_detection", 1},
    //{delayed_task, "delayed_task", 1},
};
float a = 0.0f;
float b = 0.0f;
void  tjc_start_detection()
{
    if (TJC_Check_Receive())
    {
        TJC_Send_Command("t10.txt=\"111+222\"");
        // HAL_Delay(1);
        Task_DisableHandle("TJC_proc");
    }
}
void delayed_task()
{
    // 打印当前 Tick 和 状态, 方便观察
    Vofa_FireWater("Tick: %lu, Timer Status: %d\r\n", TIM_1ms.Get_Tick(), TIM_1ms.Get_Now_Status());

    // 调用 DelayNonBlocking，传入延时周期（例如 500ms）
    if (!TIM_1ms.DelayNonBlocking(500))
    {
        // 延时还在进行中，直接返回，非阻塞
        return;
    }

    // 延时结束后执行实际任务操作
    Vofa_FireWater("delayed task executed\r\n");

    // 执行完毕后禁用任务
    Task_DisableHandle("delayed_task");
}
void test()
{
    static uint16_t counter = 0;

    a += 0.01;
    b = arm_sin_f32(a);

    // 仅每50ms打印一次数据(每5次执行打印一次)
    // if (counter % 5 == 0)
    // {
    //     Vofa_FireWater("%f,%f\r\n", a, b);
    // }

    // 添加调试信息验证通信状态
    if (ZDT_X42_V2_Receive_Data_Right())
    {
        // 仅每30ms控制一次电机(每3次执行控制一次)
        if (counter % 1 == 0)
        {
            if (b> 0)
            {
                ZDT_X42_V2_Traj_Position_Control(1, 0, 2000, 2000, 3000, b * 10000, 1, 1);
                HAL_Delay(0);
                ZDT_X42_V2_Synchronous_motion(0);
            }
            else
            {
                ZDT_X42_V2_Traj_Position_Control(1, 1, 2000, 2000, 3000, b * 10000, 1, 1);
                HAL_Delay(0);
                ZDT_X42_V2_Synchronous_motion(0);
            }

        }
    }
    else
    {
        // 如果通信异常，每100次执行打印一次错误信息
        if (counter % 10 == 0)
        {
            Vofa_FireWater("Motor communication error\r\n");
        }
    }

    counter++;
    Task_DisableHandle("test");
}
void feed_dog()
{
    TIM_1ms_IWDG_PeriodElapsedCallback();
    Task_DisableHandle("feed_dog");
}
// 任务初始化, 可在此函数中初始化任务状态，默认为使能状态
void Task_InitAll(void)
{
    //  启动TIM6定时器
    HAL_TIM_Base_Start_IT(&htim6);
    while (ZDT_X42_V2_Init());

    HAL_Delay(1);
    ZDT_X42_V2_Traj_Position_Control(1, 1, 1000, 1000, 2000, 0, 1, 0);
    HAL_Delay(1);
    while (TJC_Init(&huart1));
    for (int i = 0; i < TASK_MAX_NUM; i++)
    {
        // 如果任务函数不为空，可以设置为默认启用状态
        if (taskConfigTable[i].taskFunction != NULL)
        {
            taskConfigTable[i].enabled = 1;
        }
    }
}

// 运行所有任务, 仅调用处于使能状态的任务函数
void Task_RunAll(void)
{
    for (int i = 0; i < TASK_MAX_NUM; i++)
    {
        if (taskConfigTable[i].taskFunction != NULL && taskConfigTable[i].enabled)
        {
            // 调用任务函数
            taskConfigTable[i].taskFunction();
        }
    }
}

// 开启指定任务（通过任务名称），匹配第一个符合的任务
void Task_EnableHandle(const char* taskName)
{
    for (int i = 0; i < TASK_MAX_NUM; i++)
    {
        if (taskConfigTable[i].taskFunction != NULL && taskConfigTable[i].taskName != NULL &&
            strcmp(taskConfigTable[i].taskName, taskName) == 0)
        {
            taskConfigTable[i].enabled = 1;
            break;
        }
    }
}

// 禁用指定任务（通过任务名称），匹配第一个符合的任务
void Task_DisableHandle(const char* taskName)
{
    for (int i = 0; i < TASK_MAX_NUM; i++)
    {
        if (taskConfigTable[i].taskFunction != NULL && taskConfigTable[i].taskName != NULL &&
            strcmp(taskConfigTable[i].taskName, taskName) == 0)
        {
            taskConfigTable[i].enabled = 0;
            break;
        }
    }
}