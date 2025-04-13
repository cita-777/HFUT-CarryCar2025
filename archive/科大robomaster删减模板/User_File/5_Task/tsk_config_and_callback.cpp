/**
 * @file tsk_config_and_callback.cpp
 * @author yssickjgd (1345578933@qq.com)
 * @brief 临时任务调度测试用函数, 后续用来存放个人定义的回调函数以及若干任务
 * @version 0.1
 * @date 2023-08-29 0.1 23赛季定稿
 * @date 2023-01-17 1.1 调试到机器人层
 *
 * @copyright USTC-RoboWalker (c) 2023-2024
 *
 */

/**
 * @brief 注意, 每个类的对象分为专属对象Specialized, 同类可复用对象Reusable以及通用对象Generic
 *
 * 专属对象:
 * 单对单来独打独
 * 比如交互类的底盘对象, 只需要交互对象调用且全局只有一个, 这样看来, 底盘就是交互类的专属对象
 * 这种对象直接封装在上层类里面, 初始化在上层类里面, 调用在上层类里面
 *
 * 同类可复用对象:
 * 各调各的
 * 比如电机的对象, 底盘可以调用, 云台可以调用, 而两者调用的是不同的对象, 这种就是同类可复用对象
 * 电机的pid对象也算同类可复用对象, 它们都在底盘类里初始化
 * 这种对象直接封装在上层类里面, 初始化在最近的一个上层专属对象的类里面, 调用在上层类里面
 *
 * 通用对象:
 * 多个调用同一个
 * 比如底盘陀螺仪对象, 底盘类要调用它做小陀螺, 云台要调用它做方位感知, 因此底盘陀螺仪是通用对象.
 * 这种对象以指针形式进行指定, 初始化在包含所有调用它的上层的类里面, 调用在上层类里面
 *
 */

/* Includes ------------------------------------------------------------------*/

#include "tsk_config_and_callback.h"

#include "4_Interaction/ita_robot.h"
#include "2_Device/Serialplot/dvc_serialplot.h"
#include "1_Middleware/1_Driver/BSP/drv_djiboarda.h"
#include "1_Middleware/1_Driver/TIM/drv_tim.h"
#include "1_Middleware/1_Driver/WDG/drv_wdg.h"

/* Private macros ------------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

// 全局初始化完成标志位
bool init_finished = false;
uint32_t flag = 0;

// 机器人战车!!!
Class_Robot robot;

// 串口绘图
Class_Serialplot serialplot;
static char Serialplot_Variable_Assignment_List[][SERIALPLOT_RX_VARIABLE_ASSIGNMENT_MAX_LENGTH] = {
        // 电机调PID
        "pa",
        "ia",
        "da",
        "po",
        "io",
        "do",};

/* Private function declarations ---------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

/**
 * @brief CAN1回调函数
 *
 * @param CAN_RxMessage CAN1收到的消息
 */
void Device_CAN1_Callback(Struct_CAN_Rx_Buffer *CAN_RxMessage)
{
    switch (CAN_RxMessage->Header.StdId)
    {
    case (0x202):
    {
        robot.Booster.Motor_Driver.CAN_RxCpltCallback(CAN_RxMessage->Data);

        break;
    }
    case (0x203):
    {
        robot.Booster.Motor_Friction_Right.CAN_RxCpltCallback(CAN_RxMessage->Data);

        break;
    }
    case (0x204):
    {
        robot.Booster.Motor_Friction_Left.CAN_RxCpltCallback(CAN_RxMessage->Data);

        break;
    }
    case (0x205):
    {
        robot.Gimbal.Motor_Yaw.CAN_RxCpltCallback(CAN_RxMessage->Data);

        break;
    }
    case (0x206):
    {
        robot.Gimbal.Motor_Pitch.CAN_RxCpltCallback(CAN_RxMessage->Data);

        break;
    }
    }
}

/**
 * @brief CAN2回调函数
 *
 * @param CAN_RxMessage CAN2收到的消息
 */
void Device_CAN2_Callback(Struct_CAN_Rx_Buffer *CAN_RxMessage)
{
    switch (CAN_RxMessage->Header.StdId)
    {
    case (0x030):
    {
        robot.Supercap.CAN_RxCpltCallback(CAN_RxMessage->Data);

        break;
    }
    case (0x201):
    {
        robot.Chassis.Motor_Wheel[2].CAN_RxCpltCallback(CAN_RxMessage->Data);

        break;
    }
    case (0x202):
    {
        robot.Chassis.Motor_Wheel[3].CAN_RxCpltCallback(CAN_RxMessage->Data);

        break;
    }
    case (0x203):
    {
        robot.Chassis.Motor_Wheel[0].CAN_RxCpltCallback(CAN_RxMessage->Data);

        break;
    }
    case (0x204):
    {
        robot.Chassis.Motor_Wheel[1].CAN_RxCpltCallback(CAN_RxMessage->Data);

        break;
    }
    case (0x205):
    {
        robot.Chassis.Motor_Steer[2].CAN_RxCpltCallback(CAN_RxMessage->Data);

        break;
    }
    case (0x206):
    {
        robot.Chassis.Motor_Steer[3].CAN_RxCpltCallback(CAN_RxMessage->Data);

        break;
    }
    case (0x207):
    {
        robot.Chassis.Motor_Steer[0].CAN_RxCpltCallback(CAN_RxMessage->Data);

        break;
    }
    case (0x208):
    {
        robot.Chassis.Motor_Steer[1].CAN_RxCpltCallback(CAN_RxMessage->Data);

        break;
    }
    }
}

/**
 * @brief UART1遥控器回调函数
 *
 * @param Buffer UART1收到的消息
 * @param Length 长度
 */
void DR16_UART1_Callback(uint8_t *Buffer, uint16_t Length)
{
    robot.DR16.UART_RxCpltCallback(Buffer, Length);
}

/**
 * @brief UART2串口绘图回调函数
 *
 * @param Buffer UART2收到的消息
 * @param Length 长度
 */
void Serialplot_UART2_Callback(uint8_t *Buffer, uint16_t Length)
{
    serialplot.UART_RxCpltCallback(Buffer, Length);
    // 电机调PID
    switch (serialplot.Get_Variable_Index())
    {
    case (0):
    {
        robot.Gimbal.Motor_Yaw.PID_AHRS_Angle.Set_K_P(serialplot.Get_Variable_Value());

        break;
    }
    case (1):
    {
        robot.Gimbal.Motor_Yaw.PID_AHRS_Angle.Set_K_I(serialplot.Get_Variable_Value());

        break;
    }
    case (2):
    {
        robot.Gimbal.Motor_Yaw.PID_AHRS_Angle.Set_K_D(serialplot.Get_Variable_Value());

        break;
    }
    case (3):
    {
        robot.Gimbal.Motor_Yaw.PID_AHRS_Omega.Set_K_P(serialplot.Get_Variable_Value());

        break;
    }
    case (4):
    {
        robot.Gimbal.Motor_Yaw.PID_AHRS_Omega.Set_K_I(serialplot.Get_Variable_Value());

        break;
    }
    case (5):
    {
        robot.Gimbal.Motor_Yaw.PID_AHRS_Omega.Set_K_D(serialplot.Get_Variable_Value());

        break;
    }
    }
}

/**
 * @brief UART3妙算回调函数
 *
 * @param Buffer UART3收到的消息
 * @param Length 长度
 */
void Manifold_UART3_Callback(uint8_t *Buffer, uint16_t Length)
{
    robot.Manifold.UART_RxCpltCallback(Buffer, Length);
}

/**
 * @brief UART6裁判系统回调函数
 *
 * @param Buffer UART6收到的消息
 * @param Length 长度
 */
void Referee_UART6_Callback(uint8_t *Buffer, uint16_t Length)
{
    robot.Referee.UART_RxCpltCallback(Buffer, Length);
}

/**
 * @brief UART7底盘姿态传感器回调函数
 *
 * @param Buffer UART7收到的消息
 * @param Length 长度
 */
void Chassis_AHRS_UART7_Callback(uint8_t *Buffer, uint16_t Length)
{
    robot.Posture.AHRS_Chassis.UART_RxCpltCallback(Buffer, Length);
}

/**
 * @brief UART8云台姿态传感器回调函数
 *
 * @param Buffer UART8收到的消息
 * @param Length 长度
 */
void Gimbal_AHRS_UART8_Callback(uint8_t *Buffer, uint16_t Length)
{
    robot.Posture.AHRS_Gimbal.UART_RxCpltCallback(Buffer, Length);
}

/**
 * @brief TIM4任务回调函数
 *
 */
void Task100us_TIM4_Callback()
{

}

/**
 * @brief TIM5任务回调函数
 *
 */
void Task1ms_TIM5_Callback()
{
    // 模块保持存活

    static int alive_mod100 = 0;
    alive_mod100++;
    if (alive_mod100 == 100)
    {
        alive_mod100 = 0;

        robot.TIM_100ms_Alive_PeriodElapsedCallback();
    }

    static int alive_mod1000 = 0;
    alive_mod1000++;
    if (alive_mod1000 == 1000)
    {
        alive_mod1000 = 0;

        robot.TIM_1000ms_Alive_PeriodElapsedCallback();
    }

    // 交互层回调函数

    static int interaction_mod100 = 0;
    interaction_mod100++;
    if (interaction_mod100 == 100)
    {
        interaction_mod100 = 0;

        robot.TIM_100ms_Calculate_Callback();
    }

    static int interaction_mod10 = 0;
    interaction_mod10++;
    if (interaction_mod10 == 10)
    {
        interaction_mod10 = 0;

        robot.TIM_10ms_Calculate_PeriodElapsedCallback();
    }

    static int interaction_mod2 = 0;
    interaction_mod2++;
    if (interaction_mod2 == 2)
    {
        interaction_mod2 = 0;

        robot.TIM_2ms_Calculate_PeriodElapsedCallback();
    }

    robot.TIM_1ms_Calculate_Callback();

    // 设备层回调函数

    // 遥控器调试
    float lx = robot.DR16.Get_Left_X();
    float ly = robot.DR16.Get_Left_Y();
    float rx = robot.DR16.Get_Right_X();
    float ry = robot.DR16.Get_Right_Y();
    float ls = robot.DR16.Get_Left_Switch();
    float rs = robot.DR16.Get_Right_Switch();
    float yaw = robot.DR16.Get_Yaw();
    serialplot.Set_Data(7, &lx, &ly, &rx, &ry, &ls, &rs, &yaw);
    serialplot.TIM_1ms_Write_PeriodElapsedCallback();

    // 临时调试

    // 驱动层回调函数

    TIM_1ms_CAN_PeriodElapsedCallback();
    TIM_1ms_UART_PeriodElapsedCallback();
    TIM_1ms_IWDG_PeriodElapsedCallback();
    flag++;
}

/**
 * @brief 初始化任务
 *
 */
void Task_Init()
{
    // 驱动层初始化

    // 点俩灯, 开24V
    BSP_Init(BSP_LED_R_ON | BSP_LED_G_ON);
    // CAN总线初始化
    CAN_Init(&hcan1, Device_CAN1_Callback);
    CAN_Init(&hcan2, Device_CAN2_Callback);
    // UART初始化
    UART_Init(&huart1, DR16_UART1_Callback, 36);
    UART_Init(&huart2, Serialplot_UART2_Callback, SERIALPLOT_RX_VARIABLE_ASSIGNMENT_MAX_LENGTH);
    UART_Init(&huart3, Manifold_UART3_Callback, 128);
    UART_Init(&huart6, Referee_UART6_Callback, 512);
    UART_Init(&huart7, Chassis_AHRS_UART7_Callback, 128);
    UART_Init(&huart8, Gimbal_AHRS_UART8_Callback, 128);
    // 定时器初始化
    TIM_Init(&htim4, Task100us_TIM4_Callback);
    TIM_Init(&htim5, Task1ms_TIM5_Callback);
    // 喂狗
    IWDG_Independent_Feed();

    // 设备层初始化

    // 串口绘图初始化
    serialplot.Init(&huart2, Serialplot_Checksum_8_ENABLE, 9, (char **) Serialplot_Variable_Assignment_List);

    // 战车层初始化

    // 交互层初始化

    // 机器人战车初始化
    robot.Init();

    // 使能调度时钟
    HAL_TIM_Base_Start_IT(&htim4);
    HAL_TIM_Base_Start_IT(&htim5);
    // 标记初始化完成
    init_finished = true;

    // 等待系统
    HAL_Delay(2000);
}

/**
 * @brief 前台循环任务
 *
 */
void Task_Loop()
{
    robot.Loop();
}

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
