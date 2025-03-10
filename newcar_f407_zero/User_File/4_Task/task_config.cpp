/**
 * *****************************************************************************
 * @file        task_config.cpp
 * @brief       任务配置文件实现
 * @author      ciat-777
 * @date        2025-03-07
 * *****************************************************************************
 */

#include "task_config.h"
#include "1_Middleware/1_Driver/TIM/drv_tim.h"
#include "1_Middleware/1_Driver/WDG/drv_wdg.h"
#include "2_Device/HWT101/dvc_hwt101.h"   // HWT101头文件
#include "2_Device/Jetson/dvc_jetson.h"
#include "2_Device/Motor/Motor_ZDT42/dvc_zdt_x42.h"
#include "2_Device/Servo/dvc_fsuServo.h"
#include "2_Device/Vofa/dvc_vofa.h"
#include "2_Device/tjc_screen/dvc_tjc.h"
#include "3_Car/main_proc.h"
#include "arm_math.h"
#include <string.h>
void feed_dog();
void test();
void delayed_task();
void tjc_start_detection();
void hwt101_proc();
void servo_proc();
// 任务配置表, 根据需要添加任务配置项，示例中默认使能状态为1（使能）
// 示例任务配置: {Task_Example, "Task_Example", 1},
// 可添加任务项，比如:{NULL, NULL, 0},
TaskConfig_t taskConfigTable[TASK_MAX_NUM] = {
    {main_proc_run, "main_proc_run", 1},               // 跑地图主逻辑任务
    {feed_dog, "feed_dog", 1},                         // 喂狗任务
    {tjc_start_detection, "tjc_start_detection", 1},   // TJC检测一键启动任务
    // {test, "test", 1},                                 // 电机等测试
    // {hwt101_proc, "hwt101_proc", 1},                   // HWT101处理yaw任务
    // {servo_proc, "servo_proc", 1},                     // 舵机控制任务（test）
    // {delayed_task, "delayed_task", 1},                 // 延时任务（test）
};
float                          a = 0.0f;
float                          b = 0.0f;
static HWT101Communicator      hwt101(&huart2);                                          // 使用UART2
static JetsonCommunicator      jc(&huart5);                                              // 使用UART5
static fsuservo::FSUS_Protocol servoProtocol(&huart4, fsuservo::FSUS_BAUDRATE_115200);   // 使用UART6
static fsuservo::FSUS_Servo    servo1(1, &servoProtocol);                                // ID为1的舵机
static fsuservo::FSUS_Servo    servo2(2, &servoProtocol);                                // ID为2的舵机
/**
 * @brief 任务执行频率配置
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
    if (htim == (&htim6))   // 0.001s触发一次中断
    {
        static int count_num1 = 0;
        static int count_num2 = 0;
        static int count_num3 = 0;   // 为舵机控制添加计数器

        count_num1++;
        count_num2++;
        count_num3++;

        if (count_num1 == 1)
        {
            count_num1 = 0;
            Task_EnableHandle("hwt101_proc");
            Task_EnableHandle("test");
            TIM_1ms.TIM_1ms_Calculate_PeriodElapsedCallback();
        }

        // 每5ms控制一次舵机
        if (count_num3 == 5)
        {
            count_num3 = 0;
            Task_EnableHandle("servo_proc");
        }

        if (count_num2 == 100)
        {
            Task_EnableHandle("feed_dog");
            count_num2 = 0;
        }
    }
}
/**
 * @brief 舵机控制任务
 */
void servo_proc()
{
    static uint16_t counter = 0;
    static float    angle   = 0.0f;

    // 生成一个正弦波角度
    angle = 45.0f * arm_sin_f32(a * 0.5f);

    if (servo1.isOnline)
    {
        // 每隔一段时间设置舵机角度
        if (counter % 50 == 0)
        {
            servo1.setAngle(angle);

            // 调试输出
            Vofa_FireWater("Servo angle: %.2f\r\n", angle);
            TJC_Send_Format("t4.txt=\"Servo: %.1f°\"", angle);
        }
    }
    else
    {
        // 如果舵机离线，尝试重新连接
        if (counter % 200 == 0)
        {
            servo1.ping();
        }
    }

    counter++;
    Task_DisableHandle("servo_proc");
}
/**
 * @brief 处理HWT101数据的任务
 */
void hwt101_proc()
{
    // 处理接收到的数据
    g_hwt101->processData();

    // 获取偏航角，可用于调试输出或控制逻辑
    float yaw = g_hwt101->getYawAngle();

    static uint16_t counter = 0;
    if (counter++ % 200 == 0)   // 每200ms输出一次角度
    {
        // 输出到VOFA或TJC屏幕
        Vofa_FireWater("Yaw: %.2f\r\n", yaw);
        TJC_Send_Format("t3.txt=\"Yaw: %.1f°\"", yaw);
    }
    Task_DisableHandle("hwt101_proc");
}
void tjc_start_detection()
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

    // 仅每5ms打印一次数据(每5次执行打印一次)
    if (counter % 50 == 0)
    {
        // Vofa_FireWater("%f,%f\r\n", a, b);
        // TJC_Send_Format("t2.txt=\"%.2f\"", b);
    }

    // 添加调试信息验证通信状态
    if (ZDT_X42_V2_Receive_Data_Right())
    {
        // 仅每1ms控制一次电机(每3次执行控制一次)
        if (counter % 1 == 0)
        {
            if (b > 0)
            {
                ZDT_X42_V2_Traj_Position_Control(1, 0, 500, 500, 3000, b * 10000, 1, 1);
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

    while (ZDT_X42_V2_Init());
    HAL_Delay(1);
    ZDT_X42_V2_Traj_Position_Control(1, 1, 1000, 1000, 2000, 0, 1, 0);
    HAL_Delay(1);
    while (TJC_Init(&huart1));
    // 初始化HWT101传感器
    uint8_t ret = hwt101.init();
    if (ret != 0)
    {
        Vofa_FireWater("HWT101 init failed: %d\r\n", ret);
    }
    jc.init();
    g_jetson->send(0x01);   // 发送数据到Jetson

    servo1.init();
    servo2.init();
    Vofa_FireWater("Servo init done, servo1 online: %d, servo2 online: %d\r\n", servo1.isOnline, servo2.isOnline);
    // 启动TIM6定时器
    HAL_TIM_Base_Start_IT(&htim6);
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