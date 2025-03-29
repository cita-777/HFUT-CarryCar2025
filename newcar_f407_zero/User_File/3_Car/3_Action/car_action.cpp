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
#include "2_Device/Vofa/dvc_vofa.h"

/* 静态成员初始化 ----------------------------------------------------------*/
// 全局实例定义
CarAction* g_carAction = nullptr;

/**
 * @brief 构造函数
 */
CarAction::CarAction()
    : _isInitialized(false)
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
    if (!g_servo2 || !g_servo3)
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
    bool servo2_online = g_servo2->ping();
    bool servo3_online = g_servo3->ping();

    Vofa_FireWater(
        "舵机2状态: %s, 舵机3状态: %s\r\n", servo2_online ? "在线" : "离线", servo3_online ? "在线" : "离线");

    return servo2_online && servo3_online;
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

    if (!g_servo2 || !g_servo2->isOnline)
    {
        Vofa_FireWater("舵机2未连接或未初始化\r\n");
        return false;
    }

    float angle;
    switch (position)
    {
    case ServoPosition::A:   // 左侧位置
        angle = SERVO2_A_ANGLE;
        Vofa_FireWater("设置舵机2到位置A(左侧): %.1f°\r\n", angle);
        break;
    case ServoPosition::B:   // 中间位置
        angle = SERVO2_B_ANGLE;
        Vofa_FireWater("设置舵机2到位置B(中间): %.1f°\r\n", angle);
        break;
    case ServoPosition::C:   // 右侧位置
        angle = SERVO2_C_ANGLE;
        Vofa_FireWater("设置舵机2到位置C(右侧): %.1f°\r\n", angle);
        break;
    default: Vofa_FireWater("未知的舵机2位置\r\n"); return false;
    }

    g_servo2->setAngle(angle, interval);
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
        Vofa_FireWater("设置爪子舵机到打开状态: %.1f°\r\n", angle);
        break;
    case GripperState::CLOSE:
        angle = SERVO3_CLOSE_ANGLE;
        Vofa_FireWater("设置爪子舵机到关闭状态: %.1f°\r\n", angle);
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

    float angle;
    switch (position)
    {
    case CybergearPosition::FRONT:
        angle = CYBERGEAR_FRONT_ANGLE;
        Vofa_FireWater("设置Cybergear云台到前方位置: %.1f\r\n", angle);
        break;
    case CybergearPosition::BACK:
        angle = CYBERGEAR_BACK_ANGLE;
        Vofa_FireWater("设置Cybergear云台到后方位置: %.1f\r\n", angle);
        break;
    default: Vofa_FireWater("未知的Cybergear云台位置\r\n"); return false;
    }

    // 控制Cybergear电机，使用位置控制模式
    motor_controlmode(&mi_motor[0], 0.5f, angle, 1.0f, 0.5f, 0.1f);
    return true;
}

/**
 * @brief ZDT_X42电机控制(滑轨高度)
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
    case SliderHeight::PICK_FROM_TURNTABLE:
        position = SLIDER_PICK_FROM_TURNTABLE;
        Vofa_FireWater("设置滑轨到转盘取料高度: %.1f\r\n", position);
        break;
    case SliderHeight::PLACE_ON_TEMP:
        position = SLIDER_PLACE_ON_TEMP;
        Vofa_FireWater("设置滑轨到临时存放高度: %.1f\r\n", position);
        break;
    case SliderHeight::PLACE_ON_GROUND:
        position = SLIDER_PLACE_ON_GROUND;
        Vofa_FireWater("设置滑轨到地面放置高度: %.1f\r\n", position);
        break;
    case SliderHeight::STACKING:
        position = SLIDER_STACKING;
        Vofa_FireWater("设置滑轨到码垛高度: %.1f\r\n", position);
        break;
    default: Vofa_FireWater("未知的滑轨高度\r\n"); return false;
    }

    // 控制ZDT_X42电机(滑轨电机)，使用绝对位置模式(raf=1)
    ZDT_X42_V2_Traj_Position_Control(SLIDER_MOTOR_ADDR, dir, 201, 201, 3000, position, 1, 1);
    return true;
}

/**
 * @brief 等待舵机2完成动作
 */
void CarAction::waitForServo2()
{
    if (!_isInitialized)
    {
        Vofa_FireWater("错误: CarAction未初始化\r\n");
        return;
    }

    if (g_servo2 && g_servo2->isOnline)
    {
        Vofa_FireWater("等待舵机2动作完成...\r\n");
        g_servo2->wait();
        Vofa_FireWater("舵机2动作完成\r\n");
    }
}

/**
 * @brief 等待舵机3完成动作
 */
void CarAction::waitForServo3()
{
    if (!_isInitialized)
    {
        Vofa_FireWater("错误: CarAction未初始化\r\n");
        return;
    }

    if (g_servo3 && g_servo3->isOnline)
    {
        Vofa_FireWater("等待舵机3动作完成...\r\n");
        g_servo3->wait();
        Vofa_FireWater("舵机3动作完成\r\n");
    }
}

/**
 * @brief 等待Cybergear电机完成动作
 */
void CarAction::waitForCybergear()
{
    if (!_isInitialized)
    {
        Vofa_FireWater("错误: CarAction未初始化\r\n");
        return;
    }

    // 由于我们没有直接的方法检查Cybergear电机是否完成动作，使用延时
    Vofa_FireWater("等待Cybergear电机动作完成...\r\n");
    HAL_Delay(1000);   // 假设1秒足够完成动作
    Vofa_FireWater("Cybergear电机动作完成\r\n");
}

/**
 * @brief 等待滑轨电机完成动作
 */
void CarAction::waitForSlider()
{
    if (!_isInitialized)
    {
        Vofa_FireWater("错误: CarAction未初始化\r\n");
        return;
    }

    Vofa_FireWater("等待滑轨电机动作完成...\r\n");

    // 等待ZDT_X42电机响应
    uint32_t startTime = HAL_GetTick();
    while (!ZDT_X42_V2_Receive_Data_Right())
    {
        // 超时检测，10秒
        if (HAL_GetTick() - startTime > 10000)
        {
            Vofa_FireWater("等待滑轨电机响应超时\r\n");
            return;
        }
        HAL_Delay(10);   // 小延时避免过于频繁检查
    }
    Vofa_FireWater("滑轨电机动作完成\r\n");
}

/**
 * @brief 等待所有执行器完成动作
 */
void CarAction::waitForAllActuators()
{
    if (!_isInitialized)
    {
        Vofa_FireWater("错误: CarAction未初始化\r\n");
        return;
    }

    waitForServo2();
    waitForServo3();
    waitForCybergear();
    waitForSlider();
    Vofa_FireWater("所有执行器动作完成\r\n");
}

/**
 * @brief 抓取物料组合动作
 * @return bool 操作结果，true成功，false失败
 */
bool CarAction::pickMaterial()
{
    if (!_isInitialized)
    {
        Vofa_FireWater("错误: CarAction未初始化\r\n");
        return false;
    }

    Vofa_FireWater("开始执行抓取物料动作序列\r\n");

    // 1. 设置云台到前方位置
    if (!setCybergearPosition(CybergearPosition::FRONT))
    {
        return false;
    }
    waitForCybergear();

    // 2. 设置舵机到中心位置
    if (!setServo2Position(ServoPosition::B))
    {   // 中间位置
        return false;
    }
    waitForServo2();

    // 3. 打开爪子
    if (!setGripperState(GripperState::OPEN))
    {
        return false;
    }
    waitForServo3();

    // 4. 下降滑轨到取料高度
    if (!setSliderHeight(SliderHeight::PICK_FROM_TURNTABLE))
    {
        return false;
    }
    waitForSlider();

    // 5. 关闭爪子抓取物料
    if (!setGripperState(GripperState::CLOSE))
    {
        return false;
    }
    waitForServo3();

    // 6. 提升滑轨到临时存放高度
    if (!setSliderHeight(SliderHeight::PLACE_ON_TEMP))
    {
        return false;
    }
    waitForSlider();

    Vofa_FireWater("抓取物料动作序列完成\r\n");
    return true;
}

/**
 * @brief 放置物料组合动作
 * @return bool 操作结果，true成功，false失败
 */
bool CarAction::placeMaterial()
{
    if (!_isInitialized)
    {
        Vofa_FireWater("错误: CarAction未初始化\r\n");
        return false;
    }

    Vofa_FireWater("开始执行放置物料动作序列\r\n");

    // 1. 设置云台到后方位置
    if (!setCybergearPosition(CybergearPosition::BACK))
    {
        return false;
    }
    waitForCybergear();

    // 2. 设置舵机到左侧或右侧位置
    if (!setServo2Position(ServoPosition::C))
    {   // 右侧位置
        return false;
    }
    waitForServo2();

    // 3. 下降滑轨到放置高度
    if (!setSliderHeight(SliderHeight::PLACE_ON_GROUND))
    {
        return false;
    }
    waitForSlider();

    // 4. 打开爪子释放物料
    if (!setGripperState(GripperState::OPEN))
    {
        return false;
    }
    waitForServo3();

    // 5. 提升滑轨
    if (!setSliderHeight(SliderHeight::PLACE_ON_TEMP))
    {
        return false;
    }
    waitForSlider();

    // 6. 关闭爪子
    if (!setGripperState(GripperState::CLOSE))
    {
        return false;
    }
    waitForServo3();

    Vofa_FireWater("放置物料动作序列完成\r\n");
    return true;
}

/************************ COPYRIGHT(C) CITA **************************/