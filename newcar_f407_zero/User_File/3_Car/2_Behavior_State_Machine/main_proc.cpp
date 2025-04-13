/**
 * *****************************************************************************
 * @file        main_proc.cpp
 * @brief       主处理逻辑状态机 - CarAction测试
 * @author      ciat-777
 * @date        2025-04-08
 * *****************************************************************************
 */

#include "main_proc.h"
#include "1_Middleware/1_Driver/TIM/drv_tim.h"
#include "2_Device/Jetson/dvc_jetson.h"
#include "2_Device/Vofa/dvc_vofa.h"
#include "3_Car/1_Chassis/car_chassis.h"   // 添加底盘头文件
#include "3_Car/3_Action/car_action.h"
#include <stdbool.h>
#include <stdint.h>
// 定义测试状态
enum
{
    PROC_STATE_INIT = 0,             // 初始状态
    PROC_STATE_SERVO_TEST,           // 舵机测试
    PROC_STATE_GRIPPER_TEST,         // 爪子测试
    PROC_STATE_CYBERGEAR_TEST,       // 云台测试
    PROC_STATE_SLIDER_TEST,          // 滑轨测试
    PROC_STATE_CHASSIS_TEST,         // 底盘测试（新增）
    PROC_STATE_PICK_MATERIAL,        // 测试抓取物料
    PROC_STATE_PLACE_MATERIAL,       // 测试放置物料
    PROC_STATE_CATCH_TO_TURNTABLE,   // 测试抓取物体到转盘
    PROC_STATE_PUT_TO_MAP,           // 测试放置物体到地图
    PROC_STATE_SEQUENCE_TEST,        // 测试顺序执行
    PROC_STATE_DONE,                 // 测试完成
    PROC_STATE_MAX                   // 状态最大值
};

// 初始化状态
static bool State_Init(void)
{
    static int stage = 0;

    switch (stage)
    {
    case 0:
        Vofa_FireWater("CarAction功能测试开始\r\n");
        stage = 1;
        return false;

    case 1:
        if (!TIM_1ms.DelayNonBlocking(1000)) return false;

        Vofa_FireWater("即将进行舵机测试\r\n");
        stage = 0;
        return true;
    }

    return false;
}

// 测试舵机
static bool State_ServoTest(void)
{
    static int stage = 0;

    switch (stage)
    {
    case 0:
        Vofa_FireWater("测试舵机 - 设置到左侧位置\r\n");
        g_carAction->setServo2Position(ServoPosition::A);
        stage = 1;
        return false;

    case 1:
        if (!TIM_1ms.DelayNonBlocking(1500)) return false;

        Vofa_FireWater("测试舵机 - 设置到中间位置\r\n");
        g_carAction->setServo2Position(ServoPosition::B);
        stage = 2;
        return false;

    case 2:
        if (!TIM_1ms.DelayNonBlocking(1500)) return false;

        Vofa_FireWater("测试舵机 - 设置到右侧位置\r\n");
        g_carAction->setServo2Position(ServoPosition::C);
        stage = 3;
        return false;

    case 3:
        if (!TIM_1ms.DelayNonBlocking(1500)) return false;

        Vofa_FireWater("舵机测试完成\r\n");
        stage = 0;
        return true;
    }

    return false;
}

// 测试爪子
static bool State_GripperTest(void)
{
    static int stage = 0;

    switch (stage)
    {
    case 0:
        Vofa_FireWater("测试爪子 - 打开\r\n");
        g_carAction->setGripperState(GripperState::OPEN);
        stage = 1;
        return false;

    case 1:
        if (!TIM_1ms.DelayNonBlocking(1500)) return false;

        Vofa_FireWater("测试爪子 - 关闭\r\n");
        g_carAction->setGripperState(GripperState::CLOSE);
        stage = 2;
        return false;

    case 2:
        if (!TIM_1ms.DelayNonBlocking(1500)) return false;

        Vofa_FireWater("爪子测试完成\r\n");
        stage = 0;
        return true;
    }

    return false;
}

// 测试云台
static bool State_CybergearTest(void)
{
    static int stage = 0;

    switch (stage)
    {
    case 0:
        Vofa_FireWater("测试云台 - 前方位置\r\n");
        g_carAction->setCybergearPosition(CybergearPosition::FRONT);
        stage = 1;
        return false;

    case 1:
        if (!TIM_1ms.DelayNonBlocking(2500)) return false;

        Vofa_FireWater("测试云台 - 后方位置\r\n");
        g_carAction->setCybergearPosition(CybergearPosition::BACK);
        stage = 2;
        return false;

    case 2:
        if (!TIM_1ms.DelayNonBlocking(2500)) return false;

        Vofa_FireWater("云台测试完成\r\n");
        stage = 0;
        return true;
    }

    return false;
}

// 测试滑轨
static bool State_SliderTest(void)
{
    static int stage = 0;

    switch (stage)
    {
    case 0:
        Vofa_FireWater("测试滑轨 - 最高位置\r\n");
        g_carAction->setSliderHeight(SliderHeight::MOST_HIGH);
        stage = 1;
        return false;

    case 1:
        if (!TIM_1ms.DelayNonBlocking(2000)) return false;

        Vofa_FireWater("测试滑轨 - 临时存放高度\r\n");
        g_carAction->setSliderHeight(SliderHeight::PLACE_ON_TEMP);
        stage = 2;
        return false;

        // case 2:
        //     if (!TIM_1ms.DelayNonBlocking(2000)) return false;

        //     Vofa_FireWater("测试滑轨 - 转盘取料高度\r\n");
        //     g_carAction->setSliderHeight(SliderHeight::PICK_FROM_TURNTABLE);
        //     stage = 3;
        //     return false;

        // case 3:
        //     if (!TIM_1ms.DelayNonBlocking(2000)) return false;

        //     Vofa_FireWater("测试滑轨 - 码垛高度\r\n");
        //     g_carAction->setSliderHeight(SliderHeight::STACKING);
        //     stage = 4;
        //     return false;

        // case 4:
        //     if (!TIM_1ms.DelayNonBlocking(2000)) return false;

        //     Vofa_FireWater("测试滑轨 - 地面放置高度\r\n");
        //     g_carAction->setSliderHeight(SliderHeight::PLACE_ON_GROUND);
        //     stage = 5;
        //     return false;

        // case 5:
        //     if (!TIM_1ms.DelayNonBlocking(2000)) return false;

        //     Vofa_FireWater("测试滑轨 - 恢复到临时存放高度\r\n");
        //     g_carAction->setSliderHeight(SliderHeight::PLACE_ON_TEMP);
        //     stage = 6;
        //     return false;

    case 2:
        if (!TIM_1ms.DelayNonBlocking(2000)) return false;

        Vofa_FireWater("滑轨测试完成\r\n");
        stage = 0;
        return true;
    }

    return false;
}

// 测试底盘（新增）
static bool State_ChassisTest(void)
{
    static int     stage            = 0;
    static bool    action_completed = false;
    const uint32_t MOVE_DISTANCE    = 15000;   // 移动距离单位

    switch (stage)
    {
    case 0:
        Vofa_FireWater("开始底盘测试\r\n");
        Vofa_FireWater("测试底盘 - 前进\r\n");
        action_completed = false;
        stage            = 1;
        return false;

    case 1:   // 前进测试
        if (!action_completed)
        {
            action_completed = g_chassis->moveForward(MOVE_DISTANCE);
            return false;
        }

        if (!TIM_1ms.DelayNonBlocking(1000)) return false;

        Vofa_FireWater("测试底盘 - 后退\r\n");
        action_completed = false;
        stage            = 2;
        return false;

    case 2:   // 后退测试
        if (!action_completed)
        {
            action_completed = g_chassis->moveBackward(MOVE_DISTANCE);
            return false;
        }

        if (!TIM_1ms.DelayNonBlocking(1000)) return false;

        Vofa_FireWater("测试底盘 - 左移\r\n");
        action_completed = false;
        stage            = 3;
        return false;

    case 3:   // 左移测试
        if (!action_completed)
        {
            action_completed = g_chassis->moveLeft(MOVE_DISTANCE);
            return false;
        }

        if (!TIM_1ms.DelayNonBlocking(1000)) return false;

        Vofa_FireWater("测试底盘 - 右移\r\n");
        action_completed = false;
        stage            = 4;
        return false;

    case 4:   // 右移测试
        if (!action_completed)
        {
            action_completed = g_chassis->moveRight(MOVE_DISTANCE);
            return false;
        }

        if (!TIM_1ms.DelayNonBlocking(1000)) return false;

        Vofa_FireWater("底盘测试完成\r\n");
        stage = 0;
        return true;
    }

    return false;
}

// 测试抓取物料
static bool State_PickMaterial(void)
{
    static bool action_started = false;

    if (!action_started)
    {
        Vofa_FireWater("开始抓取物料测试\r\n");
        g_carAction->pickMaterial();
        action_started = true;
        return false;
    }

    if (g_carAction->pickMaterial())
    {
        Vofa_FireWater("抓取物料测试完成\r\n");
        action_started = false;
        return true;
    }

    return false;
}

// 测试放置物料
static bool State_PlaceMaterial(void)
{
    static bool action_started = false;

    if (!action_started)
    {
        Vofa_FireWater("开始放置物料测试\r\n");
        g_carAction->placeMaterial();
        action_started = true;
        return false;
    }

    if (g_carAction->placeMaterial())
    {
        Vofa_FireWater("放置物料测试完成\r\n");
        action_started = false;
        return true;
    }

    return false;
}

// 测试抓取物体到转盘
static bool State_CatchToTurntable(void)
{
    static bool action_started = false;
    static int  test_position  = 1;   // 当前测试位置：1(左侧)、2(中间)、3(右侧)

    if (!action_started)
    {
        Vofa_FireWater("开始抓取物体到转盘测试 - 位置%d\r\n", test_position);
        g_carAction->catchObjToTurntable(test_position);
        action_started = true;
        return false;
    }

    if (g_carAction->catchObjToTurntable(test_position))
    {
        Vofa_FireWater("抓取物体到转盘位置%d测试完成\r\n", test_position);

        // 切换到下一个测试位置
        test_position++;

        if (test_position > 3)   // 测试完1、2、3三个位置后完成
        {
            Vofa_FireWater("所有位置抓取到转盘测试完成\r\n");
            test_position  = 1;   // 重置为初始位置，准备下次测试
            action_started = false;
            return true;
        }
        else
        {
            action_started = false;   // 重置标志，准备下一个位置测试
            return false;
        }
    }

    return false;
}

// 测试放置物体到地图
static bool State_PutToMap(void)
{
    static bool action_started = false;

    if (!action_started)
    {
        Vofa_FireWater("开始放置物体到地图测试 - 位置1(左侧)\r\n");
        g_carAction->putObjToMap(1);   // 左侧位置
        action_started = true;
        return false;
    }

    if (g_carAction->putObjToMap(1))
    {
        Vofa_FireWater("放置物体到地图测试完成\r\n");
        action_started = false;
        return true;
    }

    return false;
}

// 测试顺序执行
static bool State_SequenceTest(void)
{
    static bool action_started = false;

    if (!action_started)
    {
        Vofa_FireWater("开始顺序执行测试 - 3-2-1顺序\r\n");
        g_carAction->executeSequence(MaterialSequence::SEQ_321);
        action_started = true;
        return false;
    }

    if (g_carAction->executeSequence(MaterialSequence::SEQ_321))
    {
        Vofa_FireWater("顺序执行测试完成\r\n");
        action_started = false;
        return true;
    }

    return false;
}

// 测试完成状态
static bool State_Done(void)
{
    static int stage = 0;

    switch (stage)
    {
    case 0:
        Vofa_FireWater("所有测试完成!\r\n");
        stage = 1;
        return false;

    case 1:
        if (!TIM_1ms.DelayNonBlocking(3000)) return false;

        Vofa_FireWater("测试循环将重新开始...\r\n");
        stage = 0;
        return true;
    }

    return false;
}

static bool State_One(void)
{
    static int   stage            = 0;
    static bool  action_completed = false;
    static int   current_position = 1;        // 当前抓取位置：1、2、3
    static float move_scale       = 235.0f;   // 移动距离调整系数，可根据需要修改
    static bool  message_sent     = false;    // 消息是否已发送标志

    switch (stage)
    {
    case 0:   // 向左移动150个单位
        if (!message_sent)
        {
            Vofa_FireWater("开始任务: 向左移动150个单位，系数%.2f\r\n", move_scale);
            message_sent = true;
        }

        if (!action_completed)
        {
            action_completed = g_chassis->moveLeft((uint32_t)(175 * move_scale));
            return false;
        }

        if (!TIM_1ms.DelayNonBlocking(350)) return false;

        Vofa_FireWater("向左移动完成\r\n");
        action_completed = false;
        message_sent     = false;
        stage            = 1;
        return false;

    case 1:   // 向前移动第一段(一半距离)
        if (!message_sent)
        {
            Vofa_FireWater("向前移动第一段(635单位)，系数%.2f\r\n", move_scale);
            message_sent = true;
        }

        if (!action_completed)
        {
            action_completed = g_chassis->moveForward((uint32_t)(550 * move_scale));
            return false;
        }

        if (!TIM_1ms.DelayNonBlocking(400)) return false;

        Vofa_FireWater("向前移动第一段完成\r\n");
        action_completed = false;
        message_sent     = false;
        stage            = 2;
        return false;

    case 2:   // 向前移动第二段(一半距离)
        if (!message_sent)
        {
            Vofa_FireWater("向前移动第二段(635单位)，系数%.2f\r\n", move_scale);
            message_sent = true;
        }

        if (!action_completed)
        {
            action_completed = g_chassis->moveForward((uint32_t)(735 * move_scale));
            return false;
        }

        if (!TIM_1ms.DelayNonBlocking(600)) return false;

        Vofa_FireWater("向前移动第二段完成\r\n");
        action_completed = false;
        message_sent     = false;
        stage            = 3;
        return false;

    case 3:   // 向右移动50个单位 (原case 2)
        if (!message_sent)
        {
            Vofa_FireWater("向右移动50个单位，系数%.2f\r\n", move_scale);
            message_sent = true;
        }

        if (!action_completed)
        {
            action_completed = g_chassis->moveRight((uint32_t)(91 * move_scale));
            return false;
        }

        if (!TIM_1ms.DelayNonBlocking(500)) return false;

        Vofa_FireWater("向右移动完成\r\n");
        action_completed = false;
        message_sent     = false;
        stage            = 4;
        return false;

    case 4:   // 向Jetson发送已到达原料区的信号 (原case 3)
        Vofa_FireWater("已到达原料区，发送信号\r\n");
        //  HAL_Delay(1);
        g_jetson->sendZoneReached(ZONE_RAW_MATERIAL);
        stage = 5;
        return false;

    case 5:   // 等待Jetson发送可抓取信号 (原case 4)
        // 如果已经完成了3次抓取，任务完成
        if (current_position > 3)
        {
            Vofa_FireWater("所有抓取任务完成\r\n");
            stage            = 0;
            current_position = 1;
            return true;
        }

        if (g_jetson->canGrab())
        {
            Vofa_FireWater("收到可抓取信号，开始抓取位置%d\r\n", current_position);
            stage            = 6;
            message_sent     = false;
            action_completed = false;
        }
        else
        {
            static uint32_t last_print_time = 0;
            uint32_t        current_time    = HAL_GetTick();

            // 每1000ms打印一次等待消息，避免刷屏
            if (current_time - last_print_time >= 1000)
            {
                Vofa_FireWater("等待可抓取信号...(当前位置:%d)\r\n", current_position);
                last_print_time = current_time;
            }

            if (!TIM_1ms.DelayNonBlocking(200)) return false;
        }
        return false;

    case 6:   // 执行抓取到转盘的操作 (原case 5)
        if (!message_sent)
        {
            Vofa_FireWater("开始抓取物体到转盘位置%d\r\n", current_position);
            message_sent = true;
        }

        if (!action_completed)
        {
            action_completed = g_carAction->catchObjToTurntable(current_position);
            return false;
        }

        // 抓取完成，消息重置
        Vofa_FireWater("抓取到转盘位置%d完成\r\n", current_position);
        current_position++;
        action_completed = false;
        message_sent     = false;

        // 设置Jetson等待下一次抓取信号
        g_jetson->setWaitGrab(true);

        // 返回case 5，等待下一次信号
        stage = 5;
        return false;
    }
}
static bool State_Two(void)
{
    static int   stage            = 0;
    static bool  action_completed = false;
    static float move_scale       = 213.0f;   // 移动距离调整系数
    static bool  message_sent     = false;    // 消息是否已发送标志
    static char  qr_code[4]       = {0};      // 存储QR码内容
    const char*  qr_string        = nullptr;

    switch (stage)
    {
    case 0:   // 向左移动50个单位
        if (!message_sent)
        {
            Vofa_FireWater("开始任务2: 向左移动50个单位，系数%.2f\r\n", move_scale);
            message_sent = true;
        }

        if (!action_completed)
        {
            action_completed = g_chassis->moveLeft((uint32_t)(50 * move_scale));
            return false;
        }

        if (!TIM_1ms.DelayNonBlocking(500)) return false;

        Vofa_FireWater("向左移动完成\r\n");
        action_completed = false;
        message_sent     = false;
        stage            = 1;
        return false;

    case 1:   // 向后移动400个单位
        if (!message_sent)
        {
            Vofa_FireWater("向后移动400个单位，系数%.2f\r\n", move_scale);
            message_sent = true;
        }

        if (!action_completed)
        {
            action_completed = g_chassis->moveBackward((uint32_t)(400 * move_scale));
            return false;
        }

        if (!TIM_1ms.DelayNonBlocking(500)) return false;

        Vofa_FireWater("向后移动完成\r\n");
        action_completed = false;
        message_sent     = false;
        stage            = 2;
        return false;

    case 2:   // 转向到90度
        if (!message_sent)
        {
            Vofa_FireWater("转向到90度\r\n");
            message_sent = true;
        }

        if (!action_completed)
        {
            action_completed = g_chassis->rotateToAngle(90);
            return false;
        }

        if (!TIM_1ms.DelayNonBlocking(2500)) return false;

        Vofa_FireWater("转向完成\r\n");
        action_completed = false;
        message_sent     = false;
        stage            = 3;
        return false;

    case 3:   // 向后移动1720个单位
        if (!message_sent)
        {
            Vofa_FireWater("向后移动1720个单位，系数%.2f\r\n", move_scale);
            message_sent = true;
        }

        if (!action_completed)
        {
            action_completed = g_chassis->moveBackward((uint32_t)(1690 * move_scale));
            return false;
        }

        if (!TIM_1ms.DelayNonBlocking(3000)) return false;

        Vofa_FireWater("向后移动完成\r\n");
        action_completed = false;
        message_sent     = false;
        stage            = 4;
        return false;

    case 4:   // 转向到180度
        if (!message_sent)
        {
            Vofa_FireWater("转向到180度\r\n");
            message_sent = true;
        }

        if (!action_completed)
        {
            action_completed = g_chassis->rotateToAngle(180);
            return false;
        }

        if (!TIM_1ms.DelayNonBlocking(2500)) return false;

        Vofa_FireWater("转向完成\r\n");
        action_completed = false;
        message_sent     = false;
        stage            = 5;
        return false;

    case 5:   // 向右移动50个单位
        if (!message_sent)
        {
            Vofa_FireWater("向右移动50个单位，系数%.2f\r\n", move_scale);
            message_sent = true;
        }

        if (!action_completed)
        {
            action_completed = g_chassis->moveRight((uint32_t)(50 * move_scale));
            return false;
        }

        if (!TIM_1ms.DelayNonBlocking(500)) return false;

        Vofa_FireWater("向右移动完成\r\n");
        action_completed = false;
        message_sent     = false;
        stage            = 6;
        return false;

    case 6:   // 向Jetson发送已到达粗加工区的信号
        Vofa_FireWater("已到达粗加工区，发送信号\r\n");
        // g_jetson->sendZoneReached(ZONE_ROUGH_PROCESSING);
        stage = 7;
        return false;

    case 7:   // 等待Jetson发送QR码信息
        // 获取QR码字符串
        qr_string = g_jetson->getQRCodeString();
        if (qr_string && qr_string[0] != '\0')
        {
            // 复制前3个字符作为QR码顺序
            strncpy(qr_code, qr_string, 3);
            qr_code[3] = '\0';

            Vofa_FireWater("收到QR码信息: %s，开始执行相应操作\r\n", qr_code);
            stage        = 8;
            message_sent = false;
        }
        else
        {
            static uint32_t last_print_time = 0;
            uint32_t        current_time    = HAL_GetTick();

            // 每1000ms打印一次等待消息，避免刷屏
            if (current_time - last_print_time >= 1000)
            {
                Vofa_FireWater("等待QR码信息...\r\n");
                last_print_time = current_time;
            }

            if (!TIM_1ms.DelayNonBlocking(200)) return false;
        }
        return false;

    case 8:   // 根据QR码执行不同的放置序列
        if (!message_sent)
        {
            Vofa_FireWater("执行QR码 %s 对应的放置序列\r\n", qr_code);
            message_sent = true;
        }

        if (!action_completed)
        {
            // 根据QR码选择不同的放置顺序
            if (qr_code[0] == '2' && qr_code[1] == '1' && qr_code[2] == '3')
            {
                action_completed = g_carAction->executeSequence(MaterialSequence::SEQ_213);
            }
            else if (qr_code[0] == '2' && qr_code[1] == '3' && qr_code[2] == '1')
            {
                action_completed = g_carAction->executeSequence(MaterialSequence::SEQ_231);
            }
            else if (qr_code[0] == '1' && qr_code[1] == '2' && qr_code[2] == '3')
            {
                action_completed = g_carAction->executeSequence(MaterialSequence::SEQ_123);
            }
            else if (qr_code[0] == '1' && qr_code[1] == '3' && qr_code[2] == '2')
            {
                action_completed = g_carAction->executeSequence(MaterialSequence::SEQ_132);
            }
            else if (qr_code[0] == '3' && qr_code[1] == '1' && qr_code[2] == '2')
            {
                action_completed = g_carAction->executeSequence(MaterialSequence::SEQ_312);
            }
            else if (qr_code[0] == '3' && qr_code[1] == '2' && qr_code[2] == '1')
            {
                action_completed = g_carAction->executeSequence(MaterialSequence::SEQ_321);
            }
            else
            {
                Vofa_FireWater("错误: 无效的QR码 %s\r\n", qr_code);
                action_completed = true;   // 视为完成，跳过无效的QR码
            }
            return false;
        }

        Vofa_FireWater("放置序列执行完成\r\n");
        stage            = 0;   // 重置为初始阶段，准备下一次任务
        action_completed = false;
        message_sent     = false;
        return true;   // 整个State_Two任务完成
    }

    return false;
}
// 测试坐标移动
static bool State_CoordinatesMoveTest(void)
{
    static int     stage            = 0;
    static bool    action_completed = false;
    static float   currFactor       = 16.15f;   // 当前使用的系数
    static int32_t testDistance     = 100;      // 测试距离，单位mm

    switch (stage)
    {
    case 0:
        Vofa_FireWater("开始坐标移动测试，当前系数: %.2f\r\n", currFactor);
        Vofa_FireWater("测试前进 %ld mm\r\n", testDistance);
        action_completed = false;
        stage            = 1;
        return false;

    case 1:   // 前进测试
        if (!action_completed)
        {
            action_completed = g_chassis->moveToCoordinates(0, testDistance, CHASSIS_MOTOR_SPEED, CHASSIS_ACC_DEFAULT);
            return false;
        }

        if (!TIM_1ms.DelayNonBlocking(2000)) return false;

        Vofa_FireWater("测试后退 %ld mm\r\n", testDistance);
        action_completed = false;
        stage            = 2;
        return false;

    case 2:   // 后退测试
        if (!action_completed)
        {
            action_completed = g_chassis->moveToCoordinates(0, -testDistance, CHASSIS_MOTOR_SPEED, CHASSIS_ACC_DEFAULT);
            return false;
        }

        if (!TIM_1ms.DelayNonBlocking(2000)) return false;

        Vofa_FireWater("测试右移 %ld mm\r\n", testDistance);
        action_completed = false;
        stage            = 3;
        return false;

    case 3:   // 右移测试
        if (!action_completed)
        {
            action_completed = g_chassis->moveToCoordinates(testDistance, 0, CHASSIS_MOTOR_SPEED, CHASSIS_ACC_DEFAULT);
            return false;
        }

        if (!TIM_1ms.DelayNonBlocking(2000)) return false;

        Vofa_FireWater("测试左移 %ld mm\r\n", testDistance);
        action_completed = false;
        stage            = 4;
        return false;

    case 4:   // 左移测试
        if (!action_completed)
        {
            action_completed = g_chassis->moveToCoordinates(-testDistance, 0, CHASSIS_MOTOR_SPEED, CHASSIS_ACC_DEFAULT);
            return false;
        }

        if (!TIM_1ms.DelayNonBlocking(2000)) return false;

        // 对角线移动测试
        Vofa_FireWater("测试右前方对角线 %ld mm\r\n", testDistance);
        action_completed = false;
        stage            = 5;
        return false;

    case 5:   // 右前方对角线测试
        if (!action_completed)
        {
            action_completed =
                g_chassis->moveToCoordinates(testDistance, testDistance, CHASSIS_MOTOR_SPEED, CHASSIS_ACC_DEFAULT);
            return false;
        }

        if (!TIM_1ms.DelayNonBlocking(2000)) return false;

        Vofa_FireWater("测试左后方对角线 %ld mm\r\n", testDistance);
        action_completed = false;
        stage            = 6;
        return false;

    case 6:   // 左后方对角线测试
        if (!action_completed)
        {
            action_completed =
                g_chassis->moveToCoordinates(-testDistance, -testDistance, CHASSIS_MOTOR_SPEED, CHASSIS_ACC_DEFAULT);
            return false;
        }

        if (!TIM_1ms.DelayNonBlocking(2000)) return false;

        Vofa_FireWater("坐标移动测试完成。请测量实际移动距离，调整系数。\r\n");
        Vofa_FireWater("理论距离: %ld mm, 当前系数: %.2f\r\n", testDistance, currFactor);
        Vofa_FireWater("如果实际距离为X mm，新系数应为: %.2f * (%ld / X)\r\n", currFactor, testDistance);
        stage = 0;
        return true;
    }

    return false;
}
// 当前状态索引，初始为初始化状态
static uint8_t currentState = PROC_STATE_INIT;
// 手动设置状态的标志
static bool manualOverride = false;
// 状态函数数组（状态按顺序执行）
static ProcHandler_t procHandlers[PROC_STATE_MAX] = {
    State_Init,   // 初始化状态
                  // State_ServoTest,   // 舵机测试
                  //   State_GripperTest,     // 爪子测试
                  //  State_CybergearTest,   // 云台测试
                  // State_SliderTest,   // 滑轨测试
                  //    State_ChassisTest,   // 底盘测试（新增）
                  //    State_PickMaterial,       // 抓取物料测试
                  //    State_PlaceMaterial,      // 放置物料测试
                  // State_CatchToTurntable,   // 抓取物体到转盘测试
                  //     State_PutToMap,           // 放置物体到地图测试
                  //     State_SequenceTest,       // 顺序执行测试
    State_One,
    State_Two,
    // State_CoordinatesMoveTest,   // 坐标移动测试
    State_Done   // 测试完成
};
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

        // 查找下一个有效的状态处理函数
        uint8_t nextState = (currentState + 1) % PROC_STATE_MAX;
        while (procHandlers[nextState] == NULL && nextState != currentState)
        {
            nextState = (nextState + 1) % PROC_STATE_MAX;
        }

        // 更新到下一个状态
        currentState = nextState;

        // 记录状态切换
        Vofa_FireWater("状态切换到: %d\r\n", currentState);
    }
    else if (manualOverride)
    {
        // 检查手动设置的状态是否有效
        if (procHandlers[currentState] == NULL)
        {
            // 如果无效，找到下一个有效状态
            uint8_t validState = currentState;
            while (procHandlers[validState] == NULL)
            {
                validState = (validState + 1) % PROC_STATE_MAX;
                if (validState == currentState) break;   // 防止无限循环
            }
            currentState = validState;
            Vofa_FireWater("手动设置的状态无效，调整为: %d\r\n", currentState);
        }

        // 重置手动控制标志前重置定时器
        TIM_1ms.Set_Delay(0);
        manualOverride = false;
    }
}