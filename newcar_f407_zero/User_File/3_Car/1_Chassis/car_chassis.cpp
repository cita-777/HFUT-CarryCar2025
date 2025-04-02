/**
 * *****************************************************************************
 * @file        car_chassis.cpp
 * @brief       底盘控制类实现
 * @author      ciat-777 (juricek.chen@gmail.com)
 * @date        2025-03-28
 * @copyright   cita
 * *****************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "car_chassis.h"
#include "1_Middleware/1_Driver/TIM/drv_tim.h"
#include <math.h>   // 使用fabs而不是arm_abs_f32

/* 静态成员初始化 ----------------------------------------------------------*/
// 全局实例定义
Chassis* g_chassis = nullptr;

/**
 * @brief 构造函数
 */
Chassis::Chassis()
    : _initialized(false)
    , _motorBusy(false)
    , _angleError(0)
    , _motorReadyTime(0)
    , _waitingDelay(false)
{
    g_chassis = this;
}

/**
 * @brief 析构函数
 */
Chassis::~Chassis()
{
    stop();
}

/**
 * @brief 初始化底盘
 * @return 初始化结果，true成功，false失败
 */
bool Chassis::init()
{
    Vofa_FireWater("底盘初始化中...\r\n");

    // 三号电机四号电机使能和重置有bug，暂时注释掉
    //  // 使能所有电机
    //  setEnable(true);
    //
    //  // 重置电机位置
    //  resetPosition();

    // 确保IMU已经初始化（通常在系统初始化时完成）
    if (g_hwt101 == nullptr)
    {
        Vofa_FireWater("IMU未初始化\r\n");
        return false;
    }

    _initialized = true;
    Vofa_FireWater("底盘初始化完成\r\n");
    return true;
}

/**
 * @brief 设置电机使能状态
 * @param state 使能状态，true使能，false禁用
 */
void Chassis::setEnable(bool state)
{
    ZDT_X42_V2_En_Control(CHASSIS_MOTOR_RF, state, 0);
    // HAL_Delay(10);   // 延时10ms，确保电机使能稳定
    ZDT_X42_V2_En_Control(CHASSIS_MOTOR_RB, state, 0);
    // HAL_Delay(10);   // 延时10ms，确保电机使能稳定
    ZDT_X42_V2_En_Control(CHASSIS_MOTOR_LB, state, 0);
    // HAL_Delay(10);
    ZDT_X42_V2_En_Control(CHASSIS_MOTOR_LF, state, 0);
    // HAL_Delay(10);   // 延时10ms，确保电机使能稳定
}

/**
 * @brief 重置电机位置为零
 */
void Chassis::resetPosition()
{
    ZDT_X42_V2_Reset_CurPos_To_Zero(CHASSIS_MOTOR_RF);
    // HAL_Delay(10);   // 延时10ms，确保电机使能稳定
    ZDT_X42_V2_Reset_CurPos_To_Zero(CHASSIS_MOTOR_RB);
    // HAL_Delay(10);   // 延时10ms，确保电机使能稳定
    // ZDT_X42_V2_Reset_CurPos_To_Zero(CHASSIS_MOTOR_LB);
    // HAL_Delay(10);   // 延时10ms，确保电机使能稳定
    ZDT_X42_V2_Reset_CurPos_To_Zero(CHASSIS_MOTOR_LF);
    // HAL_Delay(10);   // 延时10ms，确保电机使能稳定
}

/**
 * @brief 前进指定距离
 * @param distance 距离值，正值
 * @return 是否完成动作，true完成，false未完成
 */
bool Chassis::moveForward(float distance)
{
    if (!_initialized) return false;

    // 如果电机繁忙，检查是否已完成
    if (_motorBusy)
    {
        if (isMotorReady())
        {
            // 电机已到位，但需要额外等待确保所有电机都到位
            if (!_waitingDelay)
            {
                _motorReadyTime = HAL_GetTick();
                _waitingDelay   = true;
            }

            // 额外等待一段时间
            if (HAL_GetTick() - _motorReadyTime >= CHASSIS_MOTOR_DELAY)
            {
                _motorBusy    = false;
                _waitingDelay = false;
                Vofa_FireWater("前进完成\r\n");
                return true;
            }
        }
        return false;
    }

    // 重置位置，开始新的移动
    resetPosition();

    // 控制四个电机同步运动 - 前进模式
    // 右侧电机(1,2)逆时针，左侧电机(3,4)顺时针
    ZDT_X42_V2_Traj_Position_Control(CHASSIS_MOTOR_RF,
                                     CHASSIS_DIR_CCW,
                                     CHASSIS_ACC_DEFAULT,
                                     CHASSIS_DEC_DEFAULT,
                                     CHASSIS_MOTOR_SPEED,
                                     distance,
                                     0,
                                     1);
    HAL_Delay(1);
    ZDT_X42_V2_Traj_Position_Control(CHASSIS_MOTOR_RB,
                                     CHASSIS_DIR_CCW,
                                     CHASSIS_ACC_DEFAULT,
                                     CHASSIS_DEC_DEFAULT,
                                     CHASSIS_MOTOR_SPEED,
                                     distance,
                                     0,
                                     1);
    HAL_Delay(1);
    ZDT_X42_V2_Traj_Position_Control(CHASSIS_MOTOR_LB,
                                     CHASSIS_DIR_CW,
                                     CHASSIS_ACC_DEFAULT,
                                     CHASSIS_DEC_DEFAULT,
                                     CHASSIS_MOTOR_SPEED,
                                     distance,
                                     0,
                                     1);
    HAL_Delay(1);
    ZDT_X42_V2_Traj_Position_Control(CHASSIS_MOTOR_LF,
                                     CHASSIS_DIR_CW,
                                     CHASSIS_ACC_DEFAULT,
                                     CHASSIS_DEC_DEFAULT,
                                     CHASSIS_MOTOR_SPEED,
                                     distance,
                                     0,
                                     1);
    HAL_Delay(1);
    // HAL_Delay(10);

    // 触发同步运动
    syncMotion();
    // HAL_Delay(1);
    //  HAL_Delay(10);
    _motorBusy = true;
    Vofa_FireWater("开始前进 %.1f 单位\r\n", distance);

    return false;
}

/**
 * @brief 后退指定距离
 * @param distance 距离值，正值
 * @return 是否完成动作，true完成，false未完成
 */
bool Chassis::moveBackward(float distance)
{
    if (!_initialized) return false;

    // 如果电机繁忙，检查是否已完成
    if (_motorBusy)
    {
        if (isMotorReady())
        {
            // 电机已到位，但需要额外等待确保所有电机都到位
            if (!_waitingDelay)
            {
                _motorReadyTime = HAL_GetTick();
                _waitingDelay   = true;
            }

            // 额外等待一段时间
            if (HAL_GetTick() - _motorReadyTime >= CHASSIS_MOTOR_DELAY)
            {
                _motorBusy    = false;
                _waitingDelay = false;
                Vofa_FireWater("后退完成\r\n");
                return true;
            }
        }
        return false;
    }

    // 重置位置，开始新的移动
    resetPosition();

    // 控制四个电机同步运动 - 后退模式
    // 右侧电机(1,2)顺时针，左侧电机(3,4)逆时针
    ZDT_X42_V2_Traj_Position_Control(CHASSIS_MOTOR_RF,
                                     CHASSIS_DIR_CW,
                                     CHASSIS_ACC_DEFAULT,
                                     CHASSIS_DEC_DEFAULT,
                                     CHASSIS_MOTOR_SPEED,
                                     distance,
                                     1,
                                     1);
    HAL_Delay(1);
    ZDT_X42_V2_Traj_Position_Control(CHASSIS_MOTOR_RB,
                                     CHASSIS_DIR_CW,
                                     CHASSIS_ACC_DEFAULT,
                                     CHASSIS_DEC_DEFAULT,
                                     CHASSIS_MOTOR_SPEED,
                                     distance,
                                     1,
                                     1);
    HAL_Delay(1);
    ZDT_X42_V2_Traj_Position_Control(CHASSIS_MOTOR_LB,
                                     CHASSIS_DIR_CCW,
                                     CHASSIS_ACC_DEFAULT,
                                     CHASSIS_DEC_DEFAULT,
                                     CHASSIS_MOTOR_SPEED,
                                     distance,
                                     1,
                                     1);
    HAL_Delay(1);
    ZDT_X42_V2_Traj_Position_Control(CHASSIS_MOTOR_LF,
                                     CHASSIS_DIR_CCW,
                                     CHASSIS_ACC_DEFAULT,
                                     CHASSIS_DEC_DEFAULT,
                                     CHASSIS_MOTOR_SPEED,
                                     distance,
                                     1,
                                     1);
    HAL_Delay(1);

    // 触发同步运动
    syncMotion();

    _motorBusy = true;
    Vofa_FireWater("开始后退 %.1f 单位\r\n", distance);

    return false;
}
/**
 * @brief 向右横移指定距离
 * @param distance 距离值，正值
 * @return 是否完成动作，true完成，false未完成
 */
bool Chassis::moveRight(float distance)
{
    if (!_initialized) return false;

    // 如果电机繁忙，检查是否已完成
    if (_motorBusy)
    {
        if (isMotorReady())
        {
            // 电机已到位，但需要额外等待确保所有电机都到位
            if (!_waitingDelay)
            {
                _motorReadyTime = HAL_GetTick();
                _waitingDelay   = true;
            }

            // 额外等待一段时间
            if (HAL_GetTick() - _motorReadyTime >= CHASSIS_MOTOR_DELAY)
            {
                _motorBusy    = false;
                _waitingDelay = false;
                Vofa_FireWater("右移完成\r\n");
                return true;
            }
        }
        return false;
    }

    // 重置位置，开始新的移动
    resetPosition();

    // 控制四个电机同步运动 - 右移模式 (麦克纳姆轮)
    // RF和LB顺时针, RB和LF逆时针
    ZDT_X42_V2_Traj_Position_Control(CHASSIS_MOTOR_RF,
                                     CHASSIS_DIR_CW,
                                     CHASSIS_ACC_DEFAULT,
                                     CHASSIS_DEC_DEFAULT,
                                     CHASSIS_MOTOR_SPEED,
                                     distance,
                                     0,
                                     1);
    HAL_Delay(1);
    ZDT_X42_V2_Traj_Position_Control(CHASSIS_MOTOR_RB,
                                     CHASSIS_DIR_CCW,
                                     CHASSIS_ACC_DEFAULT,
                                     CHASSIS_DEC_DEFAULT,
                                     CHASSIS_MOTOR_SPEED,
                                     distance,
                                     0,
                                     1);
    HAL_Delay(1);
    ZDT_X42_V2_Traj_Position_Control(CHASSIS_MOTOR_LB,
                                     CHASSIS_DIR_CW,
                                     CHASSIS_ACC_DEFAULT,
                                     CHASSIS_DEC_DEFAULT,
                                     CHASSIS_MOTOR_SPEED,
                                     distance,
                                     0,
                                     1);
    HAL_Delay(1);
    ZDT_X42_V2_Traj_Position_Control(CHASSIS_MOTOR_LF,
                                     CHASSIS_DIR_CCW,
                                     CHASSIS_ACC_DEFAULT,
                                     CHASSIS_DEC_DEFAULT,
                                     CHASSIS_MOTOR_SPEED,
                                     distance,
                                     0,
                                     1);
    HAL_Delay(1);

    // 触发同步运动
    syncMotion();

    _motorBusy = true;
    Vofa_FireWater("开始右移 %.1f 单位\r\n", distance);

    return false;
}

/**
 * @brief 向左横移指定距离
 * @param distance 距离值，正值
 * @return 是否完成动作，true完成，false未完成
 */
bool Chassis::moveLeft(float distance)
{
    if (!_initialized) return false;

    // 如果电机繁忙，检查是否已完成
    if (_motorBusy)
    {
        if (isMotorReady())
        {
            // 电机已到位，但需要额外等待确保所有电机都到位
            if (!_waitingDelay)
            {
                _motorReadyTime = HAL_GetTick();
                _waitingDelay   = true;
            }

            // 额外等待一段时间
            if (HAL_GetTick() - _motorReadyTime >= CHASSIS_MOTOR_DELAY)
            {
                _motorBusy    = false;
                _waitingDelay = false;
                Vofa_FireWater("左移完成\r\n");
                return true;
            }
        }
        return false;
    }

    // 重置位置，开始新的移动
    resetPosition();

    // 控制四个电机同步运动 - 左移模式 (麦克纳姆轮)
    // RF和LB逆时针, RB和LF顺时针
    ZDT_X42_V2_Traj_Position_Control(CHASSIS_MOTOR_RF,
                                     CHASSIS_DIR_CCW,
                                     CHASSIS_ACC_DEFAULT,
                                     CHASSIS_DEC_DEFAULT,
                                     CHASSIS_MOTOR_SPEED,
                                     distance,
                                     0,
                                     1);
    HAL_Delay(1);
    ZDT_X42_V2_Traj_Position_Control(CHASSIS_MOTOR_RB,
                                     CHASSIS_DIR_CW,
                                     CHASSIS_ACC_DEFAULT,
                                     CHASSIS_DEC_DEFAULT,
                                     CHASSIS_MOTOR_SPEED,
                                     distance,
                                     0,
                                     1);
    HAL_Delay(1);
    ZDT_X42_V2_Traj_Position_Control(CHASSIS_MOTOR_LB,
                                     CHASSIS_DIR_CCW,
                                     CHASSIS_ACC_DEFAULT,
                                     CHASSIS_DEC_DEFAULT,
                                     CHASSIS_MOTOR_SPEED,
                                     distance,
                                     0,
                                     1);
    HAL_Delay(1);
    ZDT_X42_V2_Traj_Position_Control(CHASSIS_MOTOR_LF,
                                     CHASSIS_DIR_CW,
                                     CHASSIS_ACC_DEFAULT,
                                     CHASSIS_DEC_DEFAULT,
                                     CHASSIS_MOTOR_SPEED,
                                     distance,
                                     0,
                                     1);
    HAL_Delay(1);

    // 触发同步运动
    syncMotion();

    _motorBusy = true;
    Vofa_FireWater("开始左移 %.1f 单位\r\n", distance);

    return false;
}
/**
 * @brief 左转指定角度
 * @param angle 角度值，正值
 * @return 是否完成动作，true完成，false未完成
 */
bool Chassis::turnLeft(float angle)
{
    if (!_initialized) return false;

    // 如果电机繁忙，检查是否已完成
    if (_motorBusy)
    {
        if (isMotorReady())
        {
            // 电机已到位，但需要额外等待确保所有电机都到位
            if (!_waitingDelay)
            {
                _motorReadyTime = HAL_GetTick();
                _waitingDelay   = true;
            }

            // 额外等待一段时间
            if (HAL_GetTick() - _motorReadyTime >= CHASSIS_MOTOR_DELAY)
            {
                _motorBusy    = false;
                _waitingDelay = false;
                Vofa_FireWater("左转完成\r\n");
                return true;
            }
        }
        return false;
    }

    // 重置位置，开始新的移动
    resetPosition();

    // 控制四个电机同步运动 - 左转模式
    // 右侧电机(1,2)逆时针，左侧电机(3,4)逆时针
    ZDT_X42_V2_Traj_Position_Control(
        CHASSIS_MOTOR_RF, CHASSIS_DIR_CCW, CHASSIS_ACC_DEFAULT, CHASSIS_DEC_DEFAULT, CHASSIS_MOTOR_SPEED, angle, 1, 1);
    ZDT_X42_V2_Traj_Position_Control(
        CHASSIS_MOTOR_RB, CHASSIS_DIR_CCW, CHASSIS_ACC_DEFAULT, CHASSIS_DEC_DEFAULT, CHASSIS_MOTOR_SPEED, angle, 1, 1);
    ZDT_X42_V2_Traj_Position_Control(
        CHASSIS_MOTOR_LB, CHASSIS_DIR_CCW, CHASSIS_ACC_DEFAULT, CHASSIS_DEC_DEFAULT, CHASSIS_MOTOR_SPEED, angle, 1, 1);
    ZDT_X42_V2_Traj_Position_Control(
        CHASSIS_MOTOR_LF, CHASSIS_DIR_CCW, CHASSIS_ACC_DEFAULT, CHASSIS_DEC_DEFAULT, CHASSIS_MOTOR_SPEED, angle, 1, 1);

    // 触发同步运动
    syncMotion();

    _motorBusy = true;
    Vofa_FireWater("开始左转 %.1f 单位\r\n", angle);

    return false;
}

/**
 * @brief 右转指定角度
 * @param angle 角度值，正值
 * @return 是否完成动作，true完成，false未完成
 */
bool Chassis::turnRight(float angle)
{
    if (!_initialized) return false;

    // 如果电机繁忙，检查是否已完成
    if (_motorBusy)
    {
        if (isMotorReady())
        {
            // 电机已到位，但需要额外等待确保所有电机都到位
            if (!_waitingDelay)
            {
                _motorReadyTime = HAL_GetTick();
                _waitingDelay   = true;
            }

            // 额外等待一段时间
            if (HAL_GetTick() - _motorReadyTime >= CHASSIS_MOTOR_DELAY)
            {
                _motorBusy    = false;
                _waitingDelay = false;
                Vofa_FireWater("右转完成\r\n");
                return true;
            }
        }
        return false;
    }

    // 重置位置，开始新的移动
    resetPosition();

    // 控制四个电机同步运动 - 右转模式
    // 右侧电机(1,2)顺时针，左侧电机(3,4)顺时针
    ZDT_X42_V2_Traj_Position_Control(
        CHASSIS_MOTOR_RF, CHASSIS_DIR_CW, CHASSIS_ACC_DEFAULT, CHASSIS_DEC_DEFAULT, CHASSIS_MOTOR_SPEED, angle, 1, 1);
    ZDT_X42_V2_Traj_Position_Control(
        CHASSIS_MOTOR_RB, CHASSIS_DIR_CW, CHASSIS_ACC_DEFAULT, CHASSIS_DEC_DEFAULT, CHASSIS_MOTOR_SPEED, angle, 1, 1);
    ZDT_X42_V2_Traj_Position_Control(
        CHASSIS_MOTOR_LB, CHASSIS_DIR_CW, CHASSIS_ACC_DEFAULT, CHASSIS_DEC_DEFAULT, CHASSIS_MOTOR_SPEED, angle, 1, 1);
    ZDT_X42_V2_Traj_Position_Control(
        CHASSIS_MOTOR_LF, CHASSIS_DIR_CW, CHASSIS_ACC_DEFAULT, CHASSIS_DEC_DEFAULT, CHASSIS_MOTOR_SPEED, angle, 1, 1);

    // 触发同步运动
    syncMotion();

    _motorBusy = true;
    Vofa_FireWater("开始右转 %.1f 单位\r\n", angle);

    return false;
}

/**
 * @brief 停止动作
 */
void Chassis::stop()
{
    if (!_initialized) return;

    // 所有电机立即停止
    ZDT_X42_V2_Stop_Now(CHASSIS_MOTOR_RF, 1);
    ZDT_X42_V2_Stop_Now(CHASSIS_MOTOR_RB, 1);
    ZDT_X42_V2_Stop_Now(CHASSIS_MOTOR_LB, 1);
    ZDT_X42_V2_Stop_Now(CHASSIS_MOTOR_LF, 1);

    // 触发同步运动
    syncMotion();

    // 重置状态
    _motorBusy    = false;
    _waitingDelay = false;

    Vofa_FireWater("底盘停止\r\n");
}

/**
 * @brief 旋转到绝对角度（使用imu）
 * @param targetAngle 目标角度，范围0-360度
 * @return 是否完成旋转，true完成，false未完成
 */
bool Chassis::rotateToAngle(float targetAngle)
{
    if (!_initialized) return false;

    // 如果电机繁忙，检查是否已完成
    if (_motorBusy)
    {
        if (isMotorReady())
        {
            // 电机已到位，但需要额外等待确保所有电机都到位
            if (!_waitingDelay)
            {
                _motorReadyTime = HAL_GetTick();
                _waitingDelay   = true;
            }

            // 额外等待一段时间
            if (HAL_GetTick() - _motorReadyTime >= CHASSIS_MOTOR_DELAY)
            {
                _motorBusy    = false;
                _waitingDelay = false;

                // 检查是否达到目标角度
                float currentAngle = g_hwt101->getYawAngle();
                float angleError   = calculateAngleError(currentAngle, targetAngle);

                if (fabs(angleError) < 3.0f)   // 使用fabs代替arm_abs_f32
                {
                    Vofa_FireWater("旋转完成，当前角度:%.1f\r\n", currentAngle);
                    return true;
                }

                // 未达到目标角度，需要继续旋转
                Vofa_FireWater(
                    "继续调整角度，当前:%.1f 目标:%.1f 误差:%.1f\r\n", currentAngle, targetAngle, angleError);

                // 根据误差决定旋转方向
                if (angleError > 0)
                {
                    // 顺时针旋转(右转)
                    float rotationAngle = fabs(angleError) * 5.0f;      // 使用fabs代替arm_abs_f32
                    if (rotationAngle > 50.0f) rotationAngle = 50.0f;   // 限制最大旋转单位
                    turnRight(rotationAngle);
                }
                else
                {
                    // 逆时针旋转(左转)
                    float rotationAngle = fabs(angleError) * 5.0f;      // 使用fabs代替arm_abs_f32
                    if (rotationAngle > 50.0f) rotationAngle = 50.0f;   // 限制最大旋转单位
                    turnLeft(rotationAngle);
                }

                return false;
            }
        }
        return false;
    }

    // 电机不忙，开始新的旋转
    // 获取当前角度
    float currentAngle = g_hwt101->getYawAngle();

    // 计算角度误差
    _angleError = calculateAngleError(currentAngle, targetAngle);

    // 如果误差很小，认为已经到达目标角度
    if (fabs(_angleError) < 3.0f)   // 使用fabs代替arm_abs_f32
    {
        Vofa_FireWater("已在目标角度附近，无需旋转\r\n");
        return true;
    }

    Vofa_FireWater("开始旋转到 %.1f 度，当前角度 %.1f 度\r\n", targetAngle, currentAngle);

    // 根据误差决定旋转方向
    if (_angleError > 0)
    {
        // 顺时针旋转(右转)
        float rotationAngle = fabs(_angleError) * 5.0f;     // 使用fabs代替arm_abs_f32
        if (rotationAngle > 50.0f) rotationAngle = 50.0f;   // 限制最大旋转单位
        turnRight(rotationAngle);
    }
    else
    {
        // 逆时针旋转(左转)
        float rotationAngle = fabs(_angleError) * 5.0f;     // 使用fabs代替arm_abs_f32
        if (rotationAngle > 50.0f) rotationAngle = 50.0f;   // 限制最大旋转单位
        turnLeft(rotationAngle);
    }

    return false;
}

/**
 * @brief 获取IMU航向角
 * @return 当前航向角，范围0-360度
 */
float Chassis::getYawAngle() const
{
    if (g_hwt101)
    {
        return g_hwt101->getYawAngle();
    }
    return 0.0f;
}

/**
 * @brief 计算角度误差
 * @param current 当前角度
 * @param target 目标角度
 * @return 最短路径的角度误差
 */
float Chassis::calculateAngleError(float current, float target)
{
    float error = target - current;

    // 调整为最短路径
    if (error > 180.0f)
    {
        error -= 360.0f;
    }
    else if (error < -180.0f)
    {
        error += 360.0f;
    }

    return error;   // 正值表示需要顺时针旋转，负值表示需要逆时针旋转
}

/**
 * @brief 触发电机同步运动
 */
void Chassis::syncMotion()
{
    ZDT_X42_V2_Synchronous_motion(CHASSIS_MOTOR_ALL);
}

/**
 * @brief 检查电机是否到位
 * @return 是否到位，true已到位，false未到位
 */
bool Chassis::isMotorReady() const
{
    static uint32_t lastDebugTime      = 0;
    static uint32_t callCount          = 0;
    static uint32_t operationStartTime = 0;

    if (callCount == 0 && _motorBusy)
    {
        // 记录操作开始时间(第一次检查时)
        operationStartTime = HAL_GetTick();
    }

    callCount++;
    uint8_t status = ZDT_X42_V2_Receive_Data_Right();

    // 每500ms打印一次状态信息，避免刷屏
    if (HAL_GetTick() - lastDebugTime > 500)
    {
        lastDebugTime = HAL_GetTick();
        Vofa_FireWater("电机状态检查: status=%d, 调用次数=%lu\r\n", status, callCount);

        // 如果长时间不返回ready状态，打印更多调试信息
        if (_motorBusy)
        {
            uint32_t busyTime = HAL_GetTick() - _motorReadyTime;
            Vofa_FireWater("电机繁忙已持续: %lu ms\r\n", busyTime);

            // 如果超过5秒，强制认为电机就绪
            if (HAL_GetTick() - operationStartTime > 2000)
            {
                Vofa_FireWater("警告: 电机操作超时(5秒)，强制认为就绪!\r\n");
                return true;   // 关键修改：超时后强制返回true
            }
        }
    }

    return status == 1;
}
/************************ COPYRIGHT(C) CITA **************************/