/**
 * *****************************************************************************
 * @file        car_action.cpp
 * @brief       车辆机械动作控制类实现
 * @author      ciat-777 (juricek.chen@gmail.com)
 * @date        2025-03-28
 * @copyright   cita
 * *****************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "car_action.h"
#include "1_Middleware/1_Driver/TIM/drv_tim.h"
#include "2_Device/Jetson/dvc_jetson.h"
#include "2_Device/Vofa/dvc_vofa.h"
#include "3_Car/1_Chassis/car_chassis.h"
/* 静态成员初始化 ----------------------------------------------------------*/
// 全局实例定义
CarAction* g_carAction = nullptr;

/**
 * @brief 构造函数
 */
CarAction::CarAction()
    : _isInitialized(false)
    , _actionState(ActionState::IDLE)
    , _mapActionState(MapActionState::IDLE)
    , _currentPosition(0)
    , _mapStateCounter(0)
    , _currentSequence(MaterialSequence::SEQ_321)
    , _actionTimer(0)
    , _isSendOk(false)
{
    g_carAction = this;
}

/**
 * @brief 初始化
 * @return bool 初始化结果，true成功，false失败
 */
bool CarAction::init()
{
    // 检查各组件是否已经初始化
    if (!g_servo1 || !g_servo3)
    {
        Vofa_FireWater("CarAction初始化失败: 舵机组件未完全初始化\r\n");
        return false;
    }

    // 检查舵机连接状态
    if (!checkServoConnection())
    {
        Vofa_FireWater("CarAction初始化警告: 舵机连接异常\r\n");
    }

    _isInitialized = true;
    Vofa_FireWater("CarAction初始化完成\r\n");
    return true;
}

/**
 * @brief 检查舵机通信状态
 * @return bool 检查结果，true正常，false异常
 */
bool CarAction::checkServoConnection()
{
    bool servo1_online = g_servo1->ping();
    bool servo3_online = g_servo3->ping();

    Vofa_FireWater(
        "舵机1状态: %s, 舵机3状态: %s\r\n", servo1_online ? "在线" : "离线", servo3_online ? "在线" : "离线");

    return servo1_online && servo3_online;
}

/**
 * @brief 云台舵机控制
 * @param position 舵机位置
 * @param interval 运动时间间隔(ms)
 * @return bool 操作结果，true成功，false失败
 */
bool CarAction::setServo2Position(ServoPosition position, uint16_t interval)
{
    if (!_isInitialized)
    {
        Vofa_FireWater("错误: CarAction未初始化\r\n");
        return false;
    }

    if (!g_servo1 || !g_servo1->isOnline)
    {
        Vofa_FireWater("舵机2未连接或未初始化\r\n");
        return false;
    }

    float angle;
    switch (position)
    {
    case ServoPosition::A:   // 左侧位置
        angle = SERVO2_A_ANGLE;
        // Vofa_FireWater("设置舵机2到位置A(左侧): %.1f°\r\n", angle);
        break;
    case ServoPosition::B:   // 中间位置
        angle = SERVO2_B_ANGLE;
        // Vofa_FireWater("设置舵机2到位置B(中间): %.1f°\r\n", angle);
        break;
    case ServoPosition::C:   // 右侧位置
        angle = SERVO2_C_ANGLE;
        // Vofa_FireWater("设置舵机2到位置C(右侧): %.1f°\r\n", angle);
        break;
    default: Vofa_FireWater("未知的舵机2位置\r\n"); return false;
    }

    g_servo1->setAngle(angle, interval);
    return true;
}

/**
 * @brief 爪子舵机控制
 * @param state 爪子状态
 * @param interval 运动时间间隔(ms)
 * @return bool 操作结果，true成功，false失败
 */
bool CarAction::setGripperState(GripperState state, uint16_t interval)
{
    if (!_isInitialized)
    {
        Vofa_FireWater("错误: CarAction未初始化\r\n");
        return false;
    }

    if (!g_servo3 || !g_servo3->isOnline)
    {
        Vofa_FireWater("舵机3未连接或未初始化\r\n");
        return false;
    }

    float angle;
    switch (state)
    {
    case GripperState::OPEN:
        angle = SERVO3_OPEN_ANGLE;
        // Vofa_FireWater("设置爪子舵机到打开状态: %.1f°\r\n", angle);
        break;
    case GripperState::CLOSE:
        angle = SERVO3_CLOSE_ANGLE;
        // Vofa_FireWater("设置爪子舵机到关闭状态: %.1f°\r\n", angle);
        break;
    default: Vofa_FireWater("未知的爪子状态\r\n"); return false;
    }

    g_servo3->setAngle(angle, interval);
    return true;
}

/**
 * @brief Cybergear电机控制(抓取云台)
 * @param position 云台位置
 * @return bool 操作结果，true成功，false失败
 */
bool CarAction::setCybergearPosition(CybergearPosition position)
{
    if (!_isInitialized)
    {
        Vofa_FireWater("错误: CarAction未初始化\r\n");
        return false;
    }
    // static float temp_degree = 0.0f;
    float angle;
    switch (position)
    {
    case CybergearPosition::FRONT:
        angle = CYBERGEAR_FRONT_ANGLE;
        // Vofa_FireWater("设置Cybergear云台到前方位置: %.1f\r\n", angle);
        break;
    case CybergearPosition::BACK:
        angle = CYBERGEAR_BACK_ANGLE;
        // Vofa_FireWater("设置Cybergear云台到后方位置: %.1f\r\n", angle);
        break;
    default: Vofa_FireWater("未知的Cybergear云台位置\r\n"); return false;
    }
    // temp_degree += 0.15f;
    //  控制Cybergear电机，使用位置控制模式
    motor_controlmode(&mi_motor[0], 0.0f, angle, 0, 0.55f, 0.1f);
    return true;
}

/**
 * @brief 滑轨电机控制(高度)
 * @param height 滑轨高度
 * @return bool 操作结果，true成功，false失败
 */
bool CarAction::setSliderHeight(SliderHeight height)
{
    if (!_isInitialized)
    {
        Vofa_FireWater("错误: CarAction未初始化\r\n");
        return false;
    }

    float   position;
    uint8_t dir = 1;   // 默认方向为正向

    switch (height)
    {
    case SliderHeight::MOST_HIGH:
        position = SLIDER_MOSTHIGH;
        // Vofa_FireWater("设置滑轨到最高位置: %.1f\r\n", position);
        break;
    case SliderHeight::PICK_FROM_TURNTABLE:
        position = SLIDER_PICK_FROM_TURNTABLE;
        // Vofa_FireWater("设置滑轨到转盘取料高度: %.1f\r\n", position);
        break;
    case SliderHeight::PLACE_ON_TEMP:
        position = SLIDER_PLACE_ON_TEMP;
        // Vofa_FireWater("设置滑轨到临时存放高度: %.1f\r\n", position);
        break;
    case SliderHeight::PLACE_ON_GROUND:
        position = SLIDER_PLACE_ON_GROUND;
        // Vofa_FireWater("设置滑轨到地面放置高度: %.1f\r\n", position);
        break;
    case SliderHeight::STACKING:
        position = SLIDER_STACKING;
        // Vofa_FireWater("设置滑轨到码垛高度: %.1f\r\n", position);
        break;
    default: Vofa_FireWater("未知的滑轨高度\r\n"); return false;
    }

    // EMMV5_Pos_Control(addr, dir, vel, acc, clk, raF, snF)
    EMMV5_Pos_Control(SLIDER_MOTOR_ADDR, dir, 1500, 250, (uint32_t)position, true, false);

    return true;
}

/**
 * @brief 执行抓取物体到转盘动作 - 非阻塞状态机实现
 * @param position 物体位置(1-3)
 * @return bool 如果完成返回true，否则返回false
 */
bool CarAction::catchObjToTurntable(uint8_t position)
{
    // 声明静态变量（放在函数开头，便于管理）
    static bool msgSent  = false;
    static bool msgSent1 = false;
    static bool msgSent2 = false;
    if (!_isInitialized) return false;

    // 如果当前没有活动操作，初始化新操作
    if (_actionState == ActionState::IDLE)
    {
        Vofa_FireWater("开始抓取物体到转盘, 位置: %d\r\n", position);
        _currentPosition = position;
        _actionState     = ActionState::CATCH_OPEN_GRIPPER;
        _actionTimer     = HAL_GetTick();
        // 重置静态变量，确保每次新操作时滑轨命令都会发送
        msgSent  = false;
        msgSent1 = false;
        msgSent2 = false;
        return false;
    }

    // 执行状态机逻辑
    switch (_actionState)
    {
    case ActionState::CATCH_OPEN_GRIPPER:
        // 打开爪子
        if (setGripperState(GripperState::OPEN, 200))
        {
            if (!TIM_1ms.DelayNonBlocking(300)) return false;
            _actionState = ActionState::CATCH_LOWER_TO_OBJECT;
            Vofa_FireWater("爪子已打开, 准备下降滑轨\r\n");
        }
        break;

    case ActionState::CATCH_LOWER_TO_OBJECT:
        // 下降到物体位置
        if (!msgSent)
        {
            setSliderHeight(SliderHeight::PICK_FROM_TURNTABLE);
            msgSent = true;
        }
        if (!TIM_1ms.DelayNonBlocking(550)) return false;
        _actionState = ActionState::CATCH_CLOSE_GRIPPER;
        Vofa_FireWater("滑轨已下降, 准备关闭爪子\r\n");
        break;

    case ActionState::CATCH_CLOSE_GRIPPER:
        // 关闭爪子抓取物体
        if (setGripperState(GripperState::CLOSE, 200))
        {
            if (!TIM_1ms.DelayNonBlocking(300)) return false;
            _actionState = ActionState::CATCH_RAISE_SLIDER;
            Vofa_FireWater("爪子已关闭, 准备提升滑轨\r\n");
        }
        break;

    case ActionState::CATCH_RAISE_SLIDER:
        // 提升滑轨
        if (setSliderHeight(SliderHeight::MOST_HIGH))
        {
            if (!TIM_1ms.DelayNonBlocking(550)) return false;
            _actionState = ActionState::CATCH_POSITION_LOCATION;
            Vofa_FireWater("滑轨已提升, 准备定位\r\n");
        }
        break;

    case ActionState::CATCH_POSITION_LOCATION:
        // 定位到目标位置
        positionToLocation(_currentPosition);
        if (!TIM_1ms.DelayNonBlocking(550)) return false;
        // 直接进入旋转云台状态，跳过后退
        _actionState = ActionState::CATCH_ROTATE_TURNTABLE;
        Vofa_FireWater("已定位, 准备旋转云台\r\n");
        break;

    case ActionState::CATCH_ROTATE_TURNTABLE:
        // 旋转转盘
        if (setCybergearPosition(CybergearPosition::BACK))
        {
            if (!TIM_1ms.DelayNonBlocking(650)) return false;
            _actionState = ActionState::CATCH_DOWN_SLIDER;
            Vofa_FireWater("云台已旋转, 准备释放物体\r\n");
        }
        break;

    case ActionState::CATCH_DOWN_SLIDER:
        // 下降滑轨
        if (!msgSent1)
        {
            setSliderHeight(SliderHeight::PLACE_ON_TEMP);
            msgSent1 = true;
        }
        if (!TIM_1ms.DelayNonBlocking(550)) return false;
        _actionState = ActionState::CATCH_RELEASE_GRIPPER;
        Vofa_FireWater("滑轨已下\r\n");
        break;

    case ActionState::CATCH_RELEASE_GRIPPER:
        // 释放爪子
        if (setGripperState(GripperState::OPEN, 200))
        {
            if (!TIM_1ms.DelayNonBlocking(300)) return false;
            _actionState = ActionState::CATCH_FINAL_RAISE;
            Vofa_FireWater("物体已释放, 准备最终提升\r\n");
        }
        break;

    case ActionState::CATCH_FINAL_RAISE:
        // 最终提升
        if (!msgSent2)
        {
            setSliderHeight(SliderHeight::MOST_HIGH);
            msgSent2 = true;
        }

        if (!TIM_1ms.DelayNonBlocking(550)) return false;
        // 直接完成任务，跳过前进
        _actionState = ActionState::CATCH_POSITION_FRONT;
        Vofa_FireWater("滑轨最终提升完成, 抓取任务结束\r\n");
        break;

    case ActionState::CATCH_POSITION_FRONT:
        if (setCybergearPosition(CybergearPosition::FRONT))
        {
            if (!TIM_1ms.DelayNonBlocking(900)) return false;
            _actionState = ActionState::COMPLETED;
            Vofa_FireWater("云台已旋转, 准备释放物体\r\n");
        }
        break;

    case ActionState::COMPLETED:
        // 重置状态机
        _actionState = ActionState::IDLE;
        // 也可以在这里重置静态变量，但在IDLE初始化更合适
        return true;

    default:
        // 异常状态处理
        Vofa_FireWater("错误: 非法状态 %d\r\n", (int)_actionState);
        _actionState = ActionState::IDLE;
        // 出错时也重置静态变量
        msgSent  = false;
        msgSent1 = false;
        msgSent2 = false;
        break;
    }

    return false;
}

/**
 * @brief 执行物体放置到地图动作 - 非阻塞状态机实现
 * @param position 目标位置(1-3)
 * @return bool 如果完成返回true，否则返回false
 */
bool CarAction::putObjToMap(uint8_t position)
{
    static int16_t targetX = 0, targetY = 0;
    static bool    adjustmentRequested = false;
    static bool    locationReached     = false;
    // 添加滑轨控制标志
    static bool lowerFirstSent   = false;
    static bool raiseSliderSent  = false;
    static bool lowerToPlaceSent = false;
    static bool finalRaiseSent   = false;
    // 添加固定顺序计数器
    static uint8_t fixedPosition = 1;   // 固定的位置顺序计数器，从1开始

    if (!_isInitialized) return false;

    // 如果当前没有活动操作，初始化新操作
    if (_actionState == ActionState::IDLE)
    {
        Vofa_FireWater("开始放置物体到地图, 位置: %d (实际使用位置: %d)\r\n", position, fixedPosition);
        _currentPosition    = fixedPosition;                        // 使用固定顺序而非传入的position
        _actionState        = ActionState::PUT_POSITION_LOCATION;   // 直接从位置定位开始，跳过固定移动
        _actionTimer        = HAL_GetTick();
        adjustmentRequested = false;
        locationReached     = true;   // 假定已经到达位置，直接开始微调
        _isSendOk           = false;
        // 重置滑轨控制标志
        lowerFirstSent   = false;
        raiseSliderSent  = false;
        lowerToPlaceSent = false;
        finalRaiseSent   = false;
        return false;
    }

    // 执行状态机逻辑
    switch (_actionState)
    {
    case ActionState::PUT_POSITION_LOCATION:
        // 执行位置微调
        if (!adjustmentRequested)
        {
            // 获取微调坐标
            g_jetson->getCoordinates(targetX, targetY);

            // 检查是否收到有效坐标
            if (targetX != 0 || targetY != 0)
            {
                if (g_chassis->moveToCoordinates(targetX, targetY, 200, 150))
                {
                    adjustmentRequested = true;
                    // if (!TIM_1ms.DelayNonBlocking(100)) return false;
                    //  微调完成，进行舵机定位
                    positionToLocation(_currentPosition);
                    // if (!TIM_1ms.DelayNonBlocking(500)) return false;
                    _actionState = ActionState::PUT_ROTATE_BACK;
                    Vofa_FireWater("微调完成, 舵机已定位, 准备旋转云台\r\n");

                    // 关键修改：微调成功后直接break而不是返回false
                    break;   // 退出case语句，继续执行后续状态
                }
                return false;   // 仅在微调未完成时返回false
            }
            else
            {
                // 如果没有收到有效坐标，继续等待
                // 每100ms尝试一次，避免CPU占用过高
                if (!TIM_1ms.DelayNonBlocking(100)) return false;
                // 可以添加日志，但不要太频繁
                static uint32_t lastLogTime = 0;
                uint32_t        currentTime = HAL_GetTick();
                if (currentTime - lastLogTime > 1000)   // 每秒只打印一次日志
                {
                    Vofa_FireWater("等待微调坐标中...\r\n");
                    lastLogTime = currentTime;
                }
                return false;   // 继续循环等待，直到收到有效坐标
            }
        }
        break;

    case ActionState::PUT_ROTATE_BACK:
        // 旋转转盘到后方
        if (setCybergearPosition(CybergearPosition::BACK))
        {
            if (!TIM_1ms.DelayNonBlocking(1000)) return false;
            _actionState = ActionState::PUT_OPEN_GRIPPER_FIRST;
            Vofa_FireWater("云台已旋转到后方, 准备打开爪子\r\n");
        }
        break;

    case ActionState::PUT_OPEN_GRIPPER_FIRST:
        // 打开爪子
        if (setGripperState(GripperState::OPEN, 200))
        {
            if (!TIM_1ms.DelayNonBlocking(350)) return false;
            _actionState = ActionState::PUT_LOWER_FIRST;
            Vofa_FireWater("爪子已打开, 准备下降滑轨\r\n");
        }
        break;

    case ActionState::PUT_LOWER_FIRST:
        // 下降滑轨 - 只发送一次命令
        if (!lowerFirstSent)
        {
            setSliderHeight(SliderHeight::PLACE_ON_TEMP);
            lowerFirstSent = true;
            Vofa_FireWater("发送下降滑轨命令\r\n");
        }
        if (!TIM_1ms.DelayNonBlocking(700)) return false;
        _actionState = ActionState::PUT_CLOSE_GRIPPER;
        Vofa_FireWater("滑轨已下降, 准备关闭爪子\r\n");
        break;

    case ActionState::PUT_CLOSE_GRIPPER:
        // 关闭爪子
        if (setGripperState(GripperState::CLOSE, 200))
        {
            if (!TIM_1ms.DelayNonBlocking(350)) return false;
            _actionState = ActionState::PUT_RAISE_SLIDER;
            Vofa_FireWater("爪子已关闭, 准备提升滑轨\r\n");
        }
        break;

    case ActionState::PUT_RAISE_SLIDER:
        // 提升滑轨 - 只发送一次命令
        if (!raiseSliderSent)
        {
            setSliderHeight(SliderHeight::MOST_HIGH);
            raiseSliderSent = true;
            Vofa_FireWater("发送提升滑轨命令\r\n");
        }
        if (!TIM_1ms.DelayNonBlocking(700)) return false;
        _actionState = ActionState::PUT_ROTATE_FRONT;
        Vofa_FireWater("滑轨已提升, 准备旋转云台到前方\r\n");
        break;

    case ActionState::PUT_ROTATE_FRONT:
        // 旋转转盘到前方
        if (setCybergearPosition(CybergearPosition::FRONT))
        {
            if (!TIM_1ms.DelayNonBlocking(1000)) return false;
            _actionState = ActionState::PUT_LOWER_TO_PLACE;
            Vofa_FireWater("云台已旋转到前方, 准备下降放置\r\n");
        }
        break;

    case ActionState::PUT_LOWER_TO_PLACE:
        // 下降放置 - 只发送一次命令
        if (!lowerToPlaceSent)
        {
            setSliderHeight(SliderHeight::PLACE_ON_GROUND);
            lowerToPlaceSent = true;
            Vofa_FireWater("发送下降放置命令\r\n");
        }
        if (!TIM_1ms.DelayNonBlocking(1000)) return false;
        _actionState = ActionState::PUT_OPEN_GRIPPER;
        Vofa_FireWater("滑轨已下降, 准备打开爪子\r\n");
        break;

    case ActionState::PUT_OPEN_GRIPPER:
        // 打开爪子
        if (setGripperState(GripperState::OPEN, 200))
        {
            if (!TIM_1ms.DelayNonBlocking(350)) return false;
            _actionState = ActionState::PUT_FINAL_RAISE;
            Vofa_FireWater("爪子已打开, 准备最终提升\r\n");
        }
        break;

    case ActionState::PUT_FINAL_RAISE:
        // 最终提升 - 只发送一次命令
        if (!finalRaiseSent)
        {
            setSliderHeight(SliderHeight::MOST_HIGH);
            finalRaiseSent = true;
            Vofa_FireWater("发送最终提升命令\r\n");
        }
        if (!TIM_1ms.DelayNonBlocking(1000)) return false;
        _actionState = ActionState::COMPLETED;
        Vofa_FireWater("最终提升完成, 放置任务结束\r\n");
        break;

    case ActionState::COMPLETED:
        // 递增固定位置计数器，循环在1-3之间
        fixedPosition = fixedPosition % 3 + 1;
        Vofa_FireWater("完成放置位置 %d，下一次将放置位置 %d\r\n", _currentPosition, fixedPosition);

        // 重置状态机
        _actionState = ActionState::IDLE;
        return true;

    default:
        // 异常状态处理
        Vofa_FireWater("错误: 非法状态 %d\r\n", (int)_actionState);
        _actionState = ActionState::IDLE;
        break;
    }

    return false;
}

/**
 * @brief 执行码垛版本的物体放置到地图动作
 * @param position 目标位置(1-3)
 * @return bool 如果完成返回true，否则返回false
 */
// bool CarAction::putObjToMapMaDuo(uint8_t position)
// {
//     if (!_isInitialized) return false;

//     // 码垛版本与普通版本相似，但使用不同的高度
//     // 使用状态机简化实现，与putObjToMap类似但有不同的高度设置

//     // 如果当前没有活动操作，初始化新操作
//     if (_actionState == ActionState::IDLE)
//     {
//         Vofa_FireWater("开始码垛放置物体到地图, 位置: %d\r\n", position);
//         _currentPosition = position;
//         _actionState     = ActionState::PUT_MOVE_BACKWARD;
//         _actionTimer     = HAL_GetTick();
//         return false;
//     }

//     // 执行状态机逻辑 - 基本与putObjToMap相同，只是在放置高度上有差异
//     switch (_actionState)
//     {
//     case ActionState::PUT_MOVE_BACKWARD:
//         if (g_chassis->moveBackward(_currentPosition * DISTANCE_WITH_SEHUAN))
//         {
//             _actionState = ActionState::PUT_POSITION_LOCATION;
//             Vofa_FireWater("后退完成, 准备定位\r\n");
//         }
//         break;

//     case ActionState::PUT_POSITION_LOCATION:
//         positionToLocation(_currentPosition);
//         if (!TIM_1ms.DelayNonBlocking(500)) return false;
//         _actionState = ActionState::PUT_ROTATE_TURNTABLE;
//         Vofa_FireWater("已定位, 准备旋转云台\r\n");
//         break;

//     case ActionState::PUT_ROTATE_TURNTABLE:
//         if (setCybergearPosition(CybergearPosition::FRONT))
//         {
//             if (!TIM_1ms.DelayNonBlocking(1500)) return false;
//             _actionState = ActionState::PUT_CLOSE_GRIPPER;
//             Vofa_FireWater("云台已旋转, 准备关闭爪子\r\n");
//         }
//         break;

//     case ActionState::PUT_CLOSE_GRIPPER:
//         if (setGripperState(GripperState::CLOSE, 200))
//         {
//             if (!TIM_1ms.DelayNonBlocking(500)) return false;
//             _actionState = ActionState::PUT_RAISE_SLIDER;
//             Vofa_FireWater("爪子已关闭, 准备提升滑轨\r\n");
//         }
//         break;

//     case ActionState::PUT_RAISE_SLIDER:
//         if (setSliderHeight(SliderHeight::PLACE_ON_TEMP))
//         {
//             if (!TIM_1ms.DelayNonBlocking(1000)) return false;
//             _actionState = ActionState::PUT_MOVE_FORWARD;
//             Vofa_FireWater("滑轨已提升, 准备前进\r\n");
//         }
//         break;

//     case ActionState::PUT_MOVE_FORWARD:
//         if (g_chassis->moveForward(_currentPosition * DISTANCE_WITH_SEHUAN))
//         {
//             _actionState = ActionState::PUT_LOWER_TO_PLACE;
//             Vofa_FireWater("前进完成, 准备下降放置\r\n");
//         }
//         break;

//     case ActionState::PUT_LOWER_TO_PLACE:
//         // 码垛时使用码垛高度
//         if (setSliderHeight(SliderHeight::STACKING))
//         {
//             if (!TIM_1ms.DelayNonBlocking(1000)) return false;
//             _actionState = ActionState::PUT_OPEN_GRIPPER;
//             Vofa_FireWater("滑轨已下降到码垛高度, 准备打开爪子\r\n");
//         }
//         break;

//     case ActionState::PUT_OPEN_GRIPPER:
//         if (setGripperState(GripperState::OPEN, 200))
//         {
//             if (!TIM_1ms.DelayNonBlocking(500)) return false;
//             _actionState = ActionState::PUT_FINAL_RAISE;
//             Vofa_FireWater("爪子已打开, 准备最终提升\r\n");
//         }
//         break;

//     case ActionState::PUT_FINAL_RAISE:
//         if (setSliderHeight(SliderHeight::PLACE_ON_TEMP))
//         {
//             if (!TIM_1ms.DelayNonBlocking(1000)) return false;
//             _actionState = ActionState::COMPLETED;
//             Vofa_FireWater("最终提升完成, 码垛放置任务结束\r\n");
//         }
//         break;

//     case ActionState::COMPLETED: _actionState = ActionState::IDLE; return true;

//     default:
//         Vofa_FireWater("错误: 非法状态 %d\r\n", (int)_actionState);
//         _actionState = ActionState::IDLE;
//         break;
//     }

//     return false;
// }

/**
 * @brief 执行从地图获取物体动作 - 非阻塞状态机实现
 * @param position 目标位置(1-3)
 * @return bool 如果完成返回true，否则返回false
 */
bool CarAction::getObjFromMap(uint8_t position)
{
    if (!_isInitialized) return false;

    // 如果当前没有活动操作，初始化新操作
    if (_actionState == ActionState::IDLE)
    {
        Vofa_FireWater("开始从地图获取物体, 位置: %d\r\n", position);
        _currentPosition = position;
        _actionState     = ActionState::GET_POSITION_LOCATION;
        _actionTimer     = HAL_GetTick();
        return false;
    }

    // 执行状态机逻辑
    switch (_actionState)
    {
    case ActionState::GET_POSITION_LOCATION:
        // 定位到目标位置
        positionToLocation(_currentPosition);
        if (!TIM_1ms.DelayNonBlocking(500)) return false;
        _actionState = ActionState::GET_LOWER_TO_OBJECT;
        Vofa_FireWater("已定位, 准备下降到物体\r\n");
        break;

    case ActionState::GET_LOWER_TO_OBJECT:
        // 下降到物体位置
        if (setSliderHeight(SliderHeight::PLACE_ON_GROUND))
        {
            if (!TIM_1ms.DelayNonBlocking(700)) return false;
            _actionState = ActionState::GET_CLOSE_GRIPPER;
            Vofa_FireWater("滑轨已下降, 准备关闭爪子\r\n");
        }
        break;

    case ActionState::GET_CLOSE_GRIPPER:
        // 关闭爪子抓取物体
        if (setGripperState(GripperState::CLOSE, 200))
        {
            if (!TIM_1ms.DelayNonBlocking(350)) return false;
            _actionState = ActionState::GET_RAISE_SLIDER;
            Vofa_FireWater("爪子已关闭, 准备提升滑轨\r\n");
        }
        break;

    case ActionState::GET_RAISE_SLIDER:
        // 提升滑轨
        if (setSliderHeight(SliderHeight::MOST_HIGH))
        {
            if (!TIM_1ms.DelayNonBlocking(700)) return false;
            _actionState = ActionState::GET_ROTATE_TURNTABLE;
            Vofa_FireWater("滑轨已提升, 准备旋转云台\r\n");
        }
        break;


    case ActionState::GET_ROTATE_TURNTABLE:
        // 旋转转盘
        if (setCybergearPosition(CybergearPosition::BACK))
        {
            if (!TIM_1ms.DelayNonBlocking(800)) return false;
            _actionState = ActionState::GET_OPEN_GRIPPER;
            Vofa_FireWater("云台已旋转, 准备释放物体\r\n");
        }
        break;

    case ActionState::GET_OPEN_GRIPPER:
        // 打开爪子释放物体
        if (setGripperState(GripperState::OPEN, 200))
        {
            if (!TIM_1ms.DelayNonBlocking(350)) return false;
            _actionState = ActionState::GET_FINAL_RAISE;
            Vofa_FireWater("爪子已打开, 准备最终提升\r\n");
        }
        break;

    case ActionState::GET_FINAL_RAISE:
        // 最终提升
        if (setSliderHeight(SliderHeight::MOST_HIGH))
        {
            if (!TIM_1ms.DelayNonBlocking(700)) return false;
            _actionState = ActionState::GET_FINAL_FRONT;
            Vofa_FireWater("最终提升完成, 准备前进\r\n");
        }
        break;
    case ActionState::GET_FINAL_FRONT:
        // 最终提升
        if (setCybergearPosition(CybergearPosition::FRONT))
        {
            if (!TIM_1ms.DelayNonBlocking(800)) return false;
            _actionState = ActionState::COMPLETED;
            Vofa_FireWater("云台回正\r\n");
        }
        break;
    case ActionState::COMPLETED:
        // 重置状态机
        _actionState = ActionState::IDLE;
        return true;

    default:
        // 异常状态处理
        Vofa_FireWater("错误: 非法状态 %d\r\n", (int)_actionState);
        _actionState = ActionState::IDLE;
        break;
    }

    return false;
}

/**
 * @brief 按指定顺序执行物体放置和获取动作序列 - 非阻塞状态机实现
 * @param sequence 顺序枚举
 * @return bool 如果完成返回true，否则返回false
 */
bool CarAction::executeSequence(MaterialSequence sequence)
{
    // 静态变量保存状态
    static uint8_t positions[6];            // 保存位置序列
    static uint8_t currentStep   = 0;       // 当前步骤
    bool           moveCompleted = false;   // 提前声明，移到switch外部

    if (!_isInitialized) return false;

    // 初始化新序列
    if (_mapActionState == MapActionState::IDLE)
    {
        Vofa_FireWater("开始执行序列: %d\r\n", (int)sequence);

        // 根据不同序列设置位置数组 (前3个是放置顺序，后3个是获取顺序)
        switch (sequence)
        {
        case MaterialSequence::SEQ_123:
            positions[0] = 1;
            positions[1] = 2;
            positions[2] = 3;   // 放置顺序
            positions[3] = 1;
            positions[4] = 2;
            positions[5] = 3;   // 获取顺序
            break;
        case MaterialSequence::SEQ_132:
            positions[0] = 1;
            positions[1] = 3;
            positions[2] = 2;
            positions[3] = 1;
            positions[4] = 3;
            positions[5] = 2;
            break;
        case MaterialSequence::SEQ_213:
            positions[0] = 2;
            positions[1] = 1;
            positions[2] = 3;
            positions[3] = 2;
            positions[4] = 1;
            positions[5] = 3;
            break;
        case MaterialSequence::SEQ_231:
            positions[0] = 2;
            positions[1] = 3;
            positions[2] = 1;
            positions[3] = 2;
            positions[4] = 3;
            positions[5] = 1;
            break;
        case MaterialSequence::SEQ_312:
            positions[0] = 3;
            positions[1] = 1;
            positions[2] = 2;
            positions[3] = 3;
            positions[4] = 1;
            positions[5] = 2;
            break;
        case MaterialSequence::SEQ_321:
        default:
            positions[0] = 3;
            positions[1] = 2;
            positions[2] = 1;
            positions[3] = 3;
            positions[4] = 2;
            positions[5] = 1;
            break;
        }

        _mapActionState = MapActionState::MOVE_TO_POSITION;
        currentStep     = 0;
        _isSendOk       = false;
        return false;
    }

    // 当前位置
    uint8_t currentPos = positions[currentStep];

    // 执行简化的状态机
    switch (_mapActionState)
    {
    case MapActionState::MOVE_TO_POSITION:
        // 移动到目标位置 - 简化版
        moveCompleted = false;   // 重置变量，而不是重新声明

        if (currentStep == 0)
        {
            // 第一个位置，直接去
            int16_t moveDistance = (currentPos - 2) * DISTANCE_WITH_SEHUAN;

            if (moveDistance > 0)
            {
                moveCompleted = g_chassis->moveForward(moveDistance);
            }
            else if (moveDistance < 0)
            {
                moveCompleted = g_chassis->moveBackward(-moveDistance);
            }
            else
            {
                moveCompleted = true;
            }
        }
        else
        {
            // 从上一个位置移动到当前位置
            uint8_t prevPos      = positions[currentStep - 1];
            int16_t moveDistance = (currentPos - prevPos) * DISTANCE_WITH_SEHUAN;

            if (moveDistance > 0)
            {
                moveCompleted = g_chassis->moveForward(moveDistance);
            }
            else if (moveDistance < 0)
            {
                moveCompleted = g_chassis->moveBackward(-moveDistance);
            }
            else
            {
                moveCompleted = true;
            }
        }

        if (moveCompleted)
        {
            Vofa_FireWater("移动到位置 %d 完成\r\n", currentPos);

            // 根据当前是放置还是获取阶段决定下一步
            if (currentStep < 3)
            {
                _mapActionState = MapActionState::PLACE_OBJECT;
                // 通知Jetson我们到达了粗加工区
                g_jetson->sendZoneReached(ZONE_ROUGH_PROCESSING);
            }
            else
            {
                _mapActionState = MapActionState::GET_OBJECT;
                // 通知Jetson我们到达了原料区
                // g_jetson->sendZoneReached(ZONE_RAW_MATERIAL);
            }
        }
        break;

    case MapActionState::PLACE_OBJECT:
        // 放置物体
        if (putObjToMap(currentPos))
        {
            _mapActionState = MapActionState::MOVE_TO_NEXT;
            Vofa_FireWater("物体放置完成, 位置 %d\r\n", currentPos);
        }
        break;

    case MapActionState::GET_OBJECT:
        // 获取物体
        if (getObjFromMap(currentPos))
        {
            _mapActionState = MapActionState::MOVE_TO_NEXT;
            Vofa_FireWater("物体获取完成, 位置 %d\r\n", currentPos);
        }
        break;

    case MapActionState::MOVE_TO_NEXT:
        // 完成当前步骤，移动到下一步
        currentStep++;

        // 检查是否完成所有步骤
        if (currentStep >= 6)
        {
            _mapActionState = MapActionState::COMPLETED;
            Vofa_FireWater("所有位置处理完毕\r\n");
        }
        else
        {
            _mapActionState = MapActionState::MOVE_TO_POSITION;
            Vofa_FireWater("准备处理下一个位置\r\n");
        }
        break;

    case MapActionState::COMPLETED:
        // 序列完成
        Vofa_FireWater("序列执行完成\r\n");
        _mapActionState = MapActionState::IDLE;
        return true;

    default:
        Vofa_FireWater("错误: 非法序列状态\r\n");
        _mapActionState = MapActionState::IDLE;
        break;
    }

    return false;
}

/**
 * @brief 仅放置物体到地图（简化版本）
 * @param sequence 顺序枚举
 * @return bool 如果完成返回true，否则返回false
 */
bool CarAction::placeOnlySequence(MaterialSequence sequence)
{
    // 简化版本的executeSequence，只执行放置操作
    return executeSequence(sequence);
}

/**
 * @brief 非阻塞抓取物料 - 状态机实现
 * @return bool 如果完成返回true，否则返回false
 */
bool CarAction::pickMaterial()
{
    if (!_isInitialized) return false;

    // 如果当前没有活动操作，初始化新操作
    if (_actionState == ActionState::IDLE)
    {
        Vofa_FireWater("开始执行抓取物料动作序列\r\n");
        _actionState = ActionState::PICK_SET_CYBERGEAR;
        _actionTimer = HAL_GetTick();
        return false;
    }

    // 执行状态机逻辑
    switch (_actionState)
    {
    case ActionState::PICK_SET_CYBERGEAR:
        // 1. 设置云台到前方位置
        if (setCybergearPosition(CybergearPosition::FRONT))
        {
            if (!TIM_1ms.DelayNonBlocking(1000)) return false;
            _actionState = ActionState::PICK_SET_SERVO_POSITION;
            Vofa_FireWater("云台已设置到前方位置, 准备设置舵机位置\r\n");
        }
        break;

    case ActionState::PICK_SET_SERVO_POSITION:
        // 2. 设置舵机到中心位置
        if (setServo2Position(ServoPosition::A, 200))
        {
            if (!TIM_1ms.DelayNonBlocking(300)) return false;
            _actionState = ActionState::PICK_OPEN_GRIPPER;
            Vofa_FireWater("舵机已设置到中心位置, 准备打开爪子\r\n");
        }
        break;

    case ActionState::PICK_OPEN_GRIPPER:
        // 3. 打开爪子
        if (setGripperState(GripperState::OPEN, 200))
        {
            if (!TIM_1ms.DelayNonBlocking(300)) return false;
            _actionState = ActionState::PICK_LOWER_SLIDER;
            Vofa_FireWater("爪子已打开, 准备下降滑轨\r\n");
        }
        break;

    case ActionState::PICK_LOWER_SLIDER:
        // 4. 下降滑轨到取料高度
        if (setSliderHeight(SliderHeight::PICK_FROM_TURNTABLE))
        {
            if (!TIM_1ms.DelayNonBlocking(1000)) return false;
            _actionState = ActionState::PICK_CLOSE_GRIPPER;
            Vofa_FireWater("滑轨已下降到取料高度, 准备关闭爪子\r\n");
        }
        break;

    case ActionState::PICK_CLOSE_GRIPPER:
        // 5. 关闭爪子抓取物料
        if (setGripperState(GripperState::CLOSE, 200))
        {
            if (!TIM_1ms.DelayNonBlocking(500)) return false;
            _actionState = ActionState::PICK_RAISE_SLIDER;
            Vofa_FireWater("爪子已关闭抓取物料, 准备提升滑轨\r\n");
        }
        break;

    case ActionState::PICK_RAISE_SLIDER:
        // 6. 提升滑轨到临时存放高度
        if (setSliderHeight(SliderHeight::PLACE_ON_TEMP))
        {
            if (!TIM_1ms.DelayNonBlocking(1000)) return false;
            _actionState = ActionState::COMPLETED;
            Vofa_FireWater("滑轨已提升到临时存放高度, 抓取物料完成\r\n");
        }
        break;

    case ActionState::COMPLETED:
        // 完成操作
        _actionState = ActionState::IDLE;
        return true;

    default:
        // 异常状态处理
        Vofa_FireWater("错误: 非法状态 %d\r\n", (int)_actionState);
        _actionState = ActionState::IDLE;
        break;
    }

    return false;
}

/**
 * @brief 非阻塞放置物料 - 状态机实现
 * @return bool 如果完成返回true，否则返回false
 */
bool CarAction::placeMaterial()
{
    if (!_isInitialized) return false;

    // 如果当前没有活动操作，初始化新操作
    if (_actionState == ActionState::IDLE)
    {
        Vofa_FireWater("开始执行放置物料动作序列\r\n");
        _actionState = ActionState::PLACE_SET_CYBERGEAR;
        _actionTimer = HAL_GetTick();
        return false;
    }

    // 执行状态机逻辑
    switch (_actionState)
    {
    case ActionState::PLACE_SET_CYBERGEAR:
        // 1. 设置云台到后方位置
        if (setCybergearPosition(CybergearPosition::BACK))
        {
            if (!TIM_1ms.DelayNonBlocking(1000)) return false;
            _actionState = ActionState::PLACE_SET_SERVO_POSITION;
            Vofa_FireWater("云台已设置到后方位置, 准备设置舵机位置\r\n");
        }
        break;

    case ActionState::PLACE_SET_SERVO_POSITION:
        // 2. 设置舵机到右侧位置
        if (setServo2Position(ServoPosition::C, 200))
        {
            if (!TIM_1ms.DelayNonBlocking(300)) return false;
            _actionState = ActionState::PLACE_LOWER_SLIDER;
            Vofa_FireWater("舵机已设置到右侧位置, 准备下降滑轨\r\n");
        }
        break;

    case ActionState::PLACE_LOWER_SLIDER:
        // 3. 下降滑轨到放置高度
        if (setSliderHeight(SliderHeight::PLACE_ON_GROUND))
        {
            if (!TIM_1ms.DelayNonBlocking(1000)) return false;
            _actionState = ActionState::PLACE_OPEN_GRIPPER;
            Vofa_FireWater("滑轨已下降到放置高度, 准备打开爪子\r\n");
        }
        break;

    case ActionState::PLACE_OPEN_GRIPPER:
        // 4. 打开爪子释放物料
        if (setGripperState(GripperState::OPEN, 200))
        {
            if (!TIM_1ms.DelayNonBlocking(500)) return false;
            _actionState = ActionState::PLACE_RAISE_SLIDER;
            Vofa_FireWater("爪子已打开释放物料, 准备提升滑轨\r\n");
        }
        break;

    case ActionState::PLACE_RAISE_SLIDER:
        // 5. 提升滑轨
        if (setSliderHeight(SliderHeight::PLACE_ON_TEMP))
        {
            if (!TIM_1ms.DelayNonBlocking(1000)) return false;
            _actionState = ActionState::PLACE_CLOSE_GRIPPER;
            Vofa_FireWater("滑轨已提升, 准备关闭爪子\r\n");
        }
        break;

    case ActionState::PLACE_CLOSE_GRIPPER:
        // 6. 关闭爪子
        if (setGripperState(GripperState::CLOSE, 200))
        {
            if (!TIM_1ms.DelayNonBlocking(500)) return false;
            _actionState = ActionState::COMPLETED;
            Vofa_FireWater("爪子已关闭, 放置物料完成\r\n");
        }
        break;

    case ActionState::COMPLETED:
        // 完成操作
        _actionState = ActionState::IDLE;
        return true;

    default:
        // 异常状态处理
        Vofa_FireWater("错误: 非法状态 %d\r\n", (int)_actionState);
        _actionState = ActionState::IDLE;
        break;
    }

    return false;
}

/**
 * @brief 根据位置选择相应的定位
 * @param position 目标位置(1-3)
 */
void CarAction::positionToLocation(uint8_t position)
{
    if (!_isInitialized) return;

    ServoPosition servoPos;

    // 根据位置选择对应的舵机位置
    switch (position)
    {
    case 1:
        servoPos = ServoPosition::A;   // 左侧
        // Vofa_FireWater("定位到位置1(左侧)\r\n");
        break;
    case 2:
        servoPos = ServoPosition::B;   // 中间
        // Vofa_FireWater("定位到位置2(中间)\r\n");
        break;
    case 3:
        servoPos = ServoPosition::C;   // 右侧
        // Vofa_FireWater("定位到位置3(右侧)\r\n");
        break;
    default: Vofa_FireWater("错误: 无效位置 %d\r\n", position); return;
    }

    // 设置舵机位置
    setServo2Position(servoPos);
}

/************************ COPYRIGHT(C) CITA **************************/