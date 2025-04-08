/**
 * *****************************************************************************
 * @file        task_config.cpp
 * @brief       任务配置文件实现
 * @author      ciat-777
 * @date        2025-03-07
 * *****************************************************************************
 */

#include "task_config.h"
#include "1_Middleware/1_Driver/BSP/drv_f4board.h"
#include "1_Middleware/1_Driver/TIM/drv_tim.h"
#include "1_Middleware/1_Driver/WDG/drv_wdg.h"
#include "2_Device/HWT101/dvc_hwt101.h"   // HWT101头文件
#include "2_Device/Jetson/dvc_jetson.h"
#include "2_Device/Motor/Motor_CyberGear/dvc_cybergear.h"
#include "2_Device/Motor/Motor_ZDT42/dvc_zdt_emmv5.h"
#include "2_Device/Servo/dvc_fsuServo.h"
#include "2_Device/Vofa/dvc_vofa.h"
#include "2_Device/tjc_screen/dvc_tjc.h"
#include "3_Car/1_Chassis/car_chassis.h"
#include "3_Car/2_Behavior_State_Machine/main_proc.h"
#include "3_Car/3_Action/car_action.h"
#include "arm_math.h"
#include <string.h>
/*------------------------------------task函数声明------------------------------------*/
void feed_dog();
void test();
void delayed_task();
void tjc_start_detection();
void hwt101_proc();
void servo_proc();
void jetson_test();
void chassis_move_task();
/*------------------------------------task注册表------------------------------------*/
// 任务配置表, 根据需要添加任务配置项，示例中默认使能状态为1（使能）
// 示例任务配置: {Task_Example, "Task_Example", 1},
// 可添加任务项，比如:{NULL, NULL, 0},
TaskConfig_t taskConfigTable[TASK_MAX_NUM] = {
    //{main_proc_run, "main_proc_run", 1},               // 跑地图主逻辑任务
    {feed_dog, "feed_dog", 1},                         // 喂狗任务
    {tjc_start_detection, "tjc_start_detection", 1},   // TJC检测一键启动任务
    {hwt101_proc, "hwt101_proc", 1},                   // HWT101处理yaw任务
    {chassis_move_task, "chassis_move_task", 1},
    //{test, "test", 1},                                 // 电机等测试（test）
    //{jetson_test, "jetson_test", 0},   // Jetson通信测试任务
    //{servo_proc, "servo_proc", 1},     // 舵机控制任务（test）
    // {delayed_task, "delayed_task", 1},                 // 延时任务（test）

};
enum ChassisTaskState
{
    IDLE,
    MOVING_BACKWARD,
    MOVING_LEFT,
    PLACING_ON_GROUND,
    CLOSING_GRIPPER,
    WAITING_AFTER_GRIP,   // 新增：夹爪关闭后等待状态
    ROTATING_GIMBAL,
    FINISHED
};

static ChassisTaskState chassis_state = IDLE;
/*------------------------------------静态类/全局变量------------------------------------*/
float                     a = 0.0f;
float                     b = 0.0f;
static HWT101Communicator hwt101(&huart2);             // hwt101通讯类
static JetsonCommunicator jc(&huart5);                 // jetson通讯类
fsuservo::FSUS_Protocol*  g_servoProtocol = nullptr;   // 舵机通讯类
fsuservo::FSUS_Servo*     g_servo1        = nullptr;   // ID为1的舵机实例
fsuservo::FSUS_Servo*     g_servo3        = nullptr;   // ID为3的舵机实例
/*------------------------------------task执行频率配置------------------------------------*/
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
            TIM_1ms.TIM_1ms_Calculate_PeriodElapsedCallback();
        }

        // 每5ms控制一次舵机
        if (count_num3 == 5)
        {
            count_num3 = 0;
            Task_EnableHandle("test");
            Task_EnableHandle("hwt101_proc");
            Task_EnableHandle("servo_proc");
        }

        if (count_num2 == 100)
        {
            Task_EnableHandle("jetson_test");
            Task_EnableHandle("feed_dog");
            count_num2 = 0;
        }
    }
}
/*------------------------------------task初始化------------------------------------*/
void Task_InitAll(void)
{

    // while (ZDT_X42_V2_Init());   // 初始化ZDT_X42电机
    while (EMMV5_Init());   // 初始化EMMV5电机
    while (TJC_Init(&huart1));
    // 初始化HWT101传感器
    uint8_t ret = hwt101.init(&huart2);
    if (ret != 0)
    {
        Vofa_FireWater("HWT101 init failed: %d\r\n", ret);
    }
    jc.init();

    g_servoProtocol = new fsuservo::FSUS_Protocol(&huart4, fsuservo::FSUS_BAUDRATE_115200);
    g_servo1        = new fsuservo::FSUS_Servo(1, g_servoProtocol);   // 转盘
    g_servo3        = new fsuservo::FSUS_Servo(3, g_servoProtocol);   // 夹爪

    g_servo1->init();
    HAL_Delay(1);
    g_servo1->setAngle(0, 200);
    HAL_Delay(1);
    g_servo3->init();
    HAL_Delay(1);
    g_servo3->setAngle(0, 200);
    HAL_Delay(1000);
    if (check_and_init_cybergear(&mi_motor[0], 0x7F, Motion_mode, 1))
    {
        Vofa_FireWater("Cybergear init success\r\n");
        // 添加小角度测试转动，参数：电机指针，力矩，位置，速度，kp，kd
        // motor_controlmode(&mi_motor[0], 0.5f, 0.1f, 1.0f, 0.5f, 0.1f);
        // HAL_Delay(100);   // 给电机一点响应时间
    }
    else
    {
        Vofa_FireWater("Cybergear init failed\r\n");
    }
    //  两个LED都开启
    BSP_Init(BSP_LED_RED_ON, 0.5f);
    // 初始化底盘控制类
    g_chassis = new Chassis();
    if (g_chassis->init())
    {
        Vofa_FireWater("底盘初始化成功\r\n");
        // while (!g_chassis->moveForward(10000));
    }
    else
    {
        Vofa_FireWater("底盘初始化失败\r\n");
    }
    // 初始化车辆动作控制类
    g_carAction = new CarAction();
    if (g_carAction->init())
    {
        Vofa_FireWater("车辆动作控制初始化成功\r\n");
        // g_chassis->moveLeft(1000);
        //  g_carAction->setSliderHeight(SliderHeight::PLACE_ON_GROUND);
    }
    else
    {
        Vofa_FireWater("车辆动作控制初始化失败\r\n");
    }
    Task_EnableHandle("chassis_move_task");
    // g_carAction->setSliderHeight(SliderHeight::PLACE_ON_TEMP);
    // while (!g_chassis->moveBackward(2000))
    // {
    //     HAL_Delay(10);   // 短暂延时避免过度占用CPU
    // }

    // // // 前进完成后等待2秒
    // // HAL_Delay(2000);

    // // // 开始右移并等待完成
    // while (!g_chassis->moveLeft(1000))
    // {
    //     HAL_Delay(10);
    // }
    // // HAL_Delay(500);
    // g_carAction->setSliderHeight(SliderHeight::PLACE_ON_GROUND);
    // // HAL_Delay(1000);
    // g_carAction->setGripperState(GripperState::CLOSE, 200);   // 打开爪子
    // // 右移完成后等待2秒
    // HAL_Delay(2000);
    for (int i = 0; i < TASK_MAX_NUM; i++)
    {
        // 如果任务函数不为空，可以设置为默认启用状态
        if (taskConfigTable[i].taskFunction != NULL)
        {
            taskConfigTable[i].enabled = 1;
        }
    }
}
/*------------------------------------task实现函数------------------------------------*/

// 实现底盘移动任务
void chassis_move_task()
{
    static uint32_t delay_start       = 0;
    static uint32_t debug_tick        = 0;
    static uint32_t gimbal_start_time = 0;   // 云台转动计时器

    // 添加定期日志记录，帮助调试
    if (HAL_GetTick() - debug_tick > 1000)
    {
        debug_tick = HAL_GetTick();
        Vofa_FireWater("当前状态机状态: %d\r\n", chassis_state);
    }

    switch (chassis_state)
    {
    case IDLE:
        // 开始后退任务
        g_carAction->setSliderHeight(SliderHeight::PLACE_ON_TEMP);
        g_chassis->moveBackward(20000);
        chassis_state = MOVING_BACKWARD;
        Vofa_FireWater("开始执行后退任务\r\n");
        break;

    case MOVING_BACKWARD:
        // 检查后退是否完成 - 无论成功或超时都认为是完成
        if (g_chassis->moveBackward(30000))
        {
            // 后退完成，开始左移
            Vofa_FireWater("后退任务完成，准备左移\r\n");
            chassis_state = MOVING_LEFT;
        }
        break;

    case MOVING_LEFT:
        // 检查左移是否完成
        if (g_chassis->moveRight(25000))
        {
            // 左移完成，放下滑架
            Vofa_FireWater("左移任务完成，准备放下滑架\r\n");
            chassis_state = PLACING_ON_GROUND;
        }
        break;

    case PLACING_ON_GROUND:
        // 放下滑架到地面
        g_carAction->setSliderHeight(SliderHeight::PLACE_ON_GROUND);
        Vofa_FireWater("滑架放下中...\r\n");
        delay_start   = HAL_GetTick();
        chassis_state = CLOSING_GRIPPER;
        break;

    case CLOSING_GRIPPER:
        // 等待滑架稳定后关闭夹爪
        if (HAL_GetTick() - delay_start > 200)
        {
            g_carAction->setGripperState(GripperState::CLOSE, 200);
            Vofa_FireWater("关闭夹爪\r\n");

            // 修改：切换到等待状态而不是直接转动云台
            delay_start   = HAL_GetTick();   // 重置计时器
            chassis_state = WAITING_AFTER_GRIP;
        }
        break;

    case WAITING_AFTER_GRIP:
        // 夹爪关闭后额外等待1秒，确保稳定
        if (HAL_GetTick() - delay_start > 1000)
        {
            Vofa_FireWater("夹爪稳定完成，准备转动云台\r\n");

            // 更新状态为云台旋转
            chassis_state     = ROTATING_GIMBAL;
            gimbal_start_time = HAL_GetTick();   // 记录云台操作开始时间

            // 使用CarAction中的方法控制云台
            g_carAction->setCybergearPosition(CybergearPosition::BACK);
            Vofa_FireWater("开始转动云台\r\n");
        }
        break;

    case ROTATING_GIMBAL:
        // 等待云台转动完成
        if (HAL_GetTick() - gimbal_start_time > 2500)   // 2.5秒完成转动
        {
            Vofa_FireWater("云台转动完成\r\n");
            chassis_state = FINISHED;
        }
        break;

    case FINISHED:
        // 所有操作完成，禁用此任务
        Vofa_FireWater("任务完成\r\n");
        Task_DisableHandle("chassis_move_task");
        break;
    }
}
void tjc_start_detection()
{
    if (TJC_Check_Receive())
    {
        TJC_Send_Command("t10.txt=\"iii+iii\"");
        // HAL_Delay(1);
        Task_DisableHandle("TJC_proc");
    }
}

void test()
{
    static uint16_t counter          = 0;
    static uint8_t  flag             = 0;
    static bool     waiting_response = false;   // 跟踪是否在等待响应
    static float    last_cmd_value   = 0.0f;    // 最后发送的命令值

    // 只有在不等待响应时才更新值
    if (!waiting_response)
    {
        a += 0.01;
        b              = arm_sin_f32(a);
        last_cmd_value = b;   // 记录本次发送的值
    }

    // 初始设置
    if (flag == 0)
    {
        flag = 1;
        // 修改前:
        // ZDT_X42_V2_Traj_Position_Control(1, 1, 1000, 1000, 2000, 0, 1, 0);

        // 修改后:
        EMMV5_Pos_Control(1, 1, 200, 200, 0, true, false);
    }

    // 检查通信状态
    if (waiting_response)
    {
        // 正在等待响应
        if (EMMV5_Receive_Data_Right())
        {
            waiting_response = false;   // 收到响应，可以发送新命令
        }
    }
    else
    {
        // 没有待响应命令，发送新命令
        if (last_cmd_value > 0)
        {
            // 修改前:
            // ZDT_X42_V2_Traj_Position_Control(1, 1, 201, 201, 3000, last_cmd_value * 3000, 1, 1);
            // ZDT_X42_V2_Traj_Position_Control(2, 1, 201, 201, 3000, last_cmd_value * 3000, 1, 1);
            // ZDT_X42_V2_Traj_Position_Control(3, 0, 201, 201, 3000, last_cmd_value * 3000, 1, 1);
            // ZDT_X42_V2_Traj_Position_Control(4, 0, 201, 201, 3000, last_cmd_value * 3000, 1, 1);
            // ZDT_X42_V2_Synchronous_motion(0);

            // 修改后:
            EMMV5_Pos_Control(1, 1, 3000, 201, (uint32_t)(last_cmd_value * 3000), true, true);
            HAL_Delay(1);
            EMMV5_Pos_Control(2, 1, 3000, 201, (uint32_t)(last_cmd_value * 3000), true, true);
            HAL_Delay(1);
            EMMV5_Pos_Control(3, 0, 3000, 201, (uint32_t)(last_cmd_value * 3000), true, true);
            HAL_Delay(1);
            EMMV5_Pos_Control(4, 0, 3000, 201, (uint32_t)(last_cmd_value * 3000), true, true);
            HAL_Delay(1);
            EMMV5_Synchronous_motion(0);
        }
        else
        {
            // 修改前:
            // ZDT_X42_V2_Traj_Position_Control(1, 0, 201, 201, 3000, last_cmd_value * 3000, 1, 1);
            // ZDT_X42_V2_Traj_Position_Control(2, 0, 201, 201, 3000, last_cmd_value * 3000, 1, 1);
            // ZDT_X42_V2_Traj_Position_Control(3, 1, 201, 201, 3000, last_cmd_value * 3000, 1, 1);
            // ZDT_X42_V2_Traj_Position_Control(4, 1, 201, 201, 3000, last_cmd_value * 3000, 1, 1);
            // ZDT_X42_V2_Synchronous_motion(0);

            // 修改后:
            EMMV5_Pos_Control(1, 0, 3000, 201, (uint32_t)(fabs(last_cmd_value) * 3000), true, true);
            HAL_Delay(1);
            EMMV5_Pos_Control(2, 0, 3000, 201, (uint32_t)(fabs(last_cmd_value) * 3000), true, true);
            HAL_Delay(1);
            EMMV5_Pos_Control(3, 1, 3000, 201, (uint32_t)(fabs(last_cmd_value) * 3000), true, true);
            HAL_Delay(1);
            EMMV5_Pos_Control(4, 1, 3000, 201, (uint32_t)(fabs(last_cmd_value) * 3000), true, true);
            HAL_Delay(1);
            EMMV5_Synchronous_motion(0);
        }
        waiting_response = true;   // 标记为等待响应状态
    }

    counter++;
    Task_DisableHandle("test");
}
void feed_dog()
{
    TIM_1ms_IWDG_PeriodElapsedCallback();
    Task_DisableHandle("feed_dog");
}

/*------------------------------------逻辑核心与事件函数------------------------------------*/
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
/*------------------------------------test任务------------------------------------*/
/**
 * @brief 舵机控制任务
 */
void servo_proc()
{
    // static uint16_t counter               = 0;
    // static float    angle                 = 0.0f;
    // static uint16_t position_hold_counter = 0;
    // static uint32_t last_ping_time        = 0;

    // // 每秒ping一次舵机确认连接
    // if (HAL_GetTick() - last_ping_time > 1000)
    // {
    //     if (g_servo1 && g_servo3)
    //     {
    //         bool online2 = g_servo1->ping();
    //         bool online3 = g_servo3->ping();
    //         Vofa_FireWater("舵机2状态: %s, 舵机3状态: %s\r\n", online2 ? "在线" : "离线", online3 ? "在线" : "离线");
    //         last_ping_time = HAL_GetTick();
    //     }
    // }

    // // 执行固定位置测试
    // position_hold_counter++;
    // if (position_hold_counter < 100)
    // {
    //     angle = 45.0f;   // 位置1 - 右侧
    // }
    // else if (position_hold_counter < 200)
    // {
    //     angle = 0.0f;   // 位置2 - 中间
    // }
    // else if (position_hold_counter < 300)
    // {
    //     angle = -45.0f;   // 位置3 - 左侧
    // }
    // else
    // {
    //     position_hold_counter = 0;   // 重新开始
    // }

    // // 如果舵机在线，设置角度
    // if (counter % 10 == 0)   // 每50ms发送一次命令
    // {
    //     // 同时控制两个舵机
    //     if (g_servo1 && g_servo1->isOnline)
    //     {
    //         g_servo1->setAngle(angle, 200);   // 200ms周期，平滑过渡
    //     }
    //     HAL_Delay(1);
    //     if (g_servo3 && g_servo3->isOnline)
    //     {
    //         g_servo3->setAngle(angle, 200);   // 200ms周期，平滑过渡
    //     }

    //     // 定期输出当前角度
    //     if (counter % 40 == 0)
    //     {
    //         Vofa_FireWater("固定位置测试: 角度=%.1f\r\n", angle);
    //     }
    // }

    // counter++;
    Task_DisableHandle("servo_proc");
}
/**
 * @brief Jetson通信测试任务
 */
void jetson_test()
{
    static uint8_t  test_stage = 0;
    static uint32_t last_time  = 0;
    static bool     first_run  = true;

    // 首次运行初始化
    if (first_run)
    {
        Vofa_FireWater("Jetson通信测试开始...\r\n");
        test_stage = 0;
        last_time  = HAL_GetTick();
        first_run  = false;
    }

    // 基于测试阶段执行不同操作
    switch (test_stage)
    {
    case 0:   // 发送区域到达通知
        if (HAL_GetTick() - last_time > 1000)
        {   // 等待1秒
            Vofa_FireWater("发送区域到达通知: ZONE_RAW_MATERIAL\r\n");
            jc.sendZoneReached(ZONE_RAW_MATERIAL);
            test_stage++;
            last_time = HAL_GetTick();
        }
        break;

    case 1:   // 发送不同区域通知
        if (HAL_GetTick() - last_time > 1000)
        {
            Vofa_FireWater("发送区域到达通知: ZONE_ROUGH_PROCESSING\r\n");
            jc.sendZoneReached(ZONE_ROUGH_PROCESSING);
            test_stage++;
            last_time = HAL_GetTick();
        }
        break;

    case 2:   // 获取二维码数据
        if (HAL_GetTick() - last_time > 4000)
        {
            Vofa_FireWater("当前二维码内容: %s\r\n", jc.getQRCodeString());
            test_stage++;
            last_time = HAL_GetTick();
        }
        break;

    case 3:   // 获取坐标数据
        if (HAL_GetTick() - last_time > 4000)
        {
            int16_t x, y;
            jc.getCoordinates(x, y);
            Vofa_FireWater("当前坐标: X=%d, Y=%d\r\n", x, y);
            test_stage++;
            last_time = HAL_GetTick();
        }
        break;

    case 4:   // 测试抓取状态
        if (HAL_GetTick() - last_time > 4000)
        {
            Vofa_FireWater("当前抓取状态: %s\r\n", jc.canGrab() ? "可以抓取" : "不可抓取");
            jc.setWaitGrab(true);   // 设置为等待抓取状态
            Vofa_FireWater("已设置为等待抓取状态\r\n");
            test_stage++;
            last_time = HAL_GetTick();
        }
        break;

    case 5:   // 测试结束
        if (HAL_GetTick() - last_time > 4000)
        {
            Vofa_FireWater("Jetson通信测试完成\r\n");
            // 重置测试以便下次运行
            first_run  = true;
            test_stage = 0;
            // 禁用任务
            Task_DisableHandle("jetson_test");
        }
        break;
    }
}
/**
 * @brief 处理HWT101数据的任务
 */
void hwt101_proc()
{
    // 处理接收到的数据
    // Vofa_FireWater("调用processData()");
    uint8_t result = g_hwt101->processData();
    // Vofa_FireWater("processData()返回: %d\r\n", result);

    // 获取偏航角，可用于调试输出或控制逻辑
    float yaw = g_hwt101->getYawAngle();

    static uint16_t counter = 0;
    if (counter++ % 5 == 0)   // 每200ms输出一次角度
    {
        // 输出到VOFA或TJC屏幕
        // Vofa_FireWater("%f\r\n", yaw);
        // TJC_Send_Format("t3.txt=\"Yaw: %.1f°\"", yaw);
    }
    Task_DisableHandle("hwt101_proc");
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