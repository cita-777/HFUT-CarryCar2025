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

    if (!action_started)
    {
        Vofa_FireWater("开始抓取物体到转盘测试 - 位置2(中间)\r\n");
        g_carAction->catchObjToTurntable(2);   // 中间位置
        action_started = true;
        return false;
    }

    if (g_carAction->catchObjToTurntable(2))
    {
        Vofa_FireWater("抓取物体到转盘测试完成\r\n");
        action_started = false;
        return true;
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
    State_CatchToTurntable,   // 抓取物体到转盘测试
    //    State_PutToMap,           // 放置物体到地图测试
    //    State_SequenceTest,       // 顺序执行测试
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