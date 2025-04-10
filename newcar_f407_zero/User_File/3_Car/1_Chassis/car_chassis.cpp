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
#include "2_Device/Vofa/dvc_vofa.h"
#include <math.h>   // 使用fabs而不是arm_abs_f32

/* 静态成员初始化 ----------------------------------------------------------*/
// 全局实例定义
Chassis* g_chassis = nullptr;

// 全局静态变量，记录电机响应状态
static bool g_motorResponse = false;

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
    //  //resetPosition();

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
    EMMV5_En_Control(CHASSIS_MOTOR_RF, state, false);
    // HAL_Delay(10);   // 延时10ms，确保电机使能稳定
    EMMV5_En_Control(CHASSIS_MOTOR_RB, state, false);
    // HAL_Delay(10);   // 延时10ms，确保电机使能稳定
    EMMV5_En_Control(CHASSIS_MOTOR_LB, state, false);
    // HAL_Delay(10);
    EMMV5_En_Control(CHASSIS_MOTOR_LF, state, false);
    // HAL_Delay(10);   // 延时10ms，确保电机使能稳定
}

/**
 * @brief 重置电机位置为零
 */
void Chassis::resetPosition()
{
    EMMV5_Reset_CurPos_To_Zero(CHASSIS_MOTOR_RF);
    // HAL_Delay(10);   // 延时10ms，确保电机使能稳定
    EMMV5_Reset_CurPos_To_Zero(CHASSIS_MOTOR_RB);
    // HAL_Delay(10);   // 延时10ms，确保电机使能稳定
    EMMV5_Reset_CurPos_To_Zero(CHASSIS_MOTOR_LB);
    // HAL_Delay(10);   // 延时10ms，确保电机使能稳定
    EMMV5_Reset_CurPos_To_Zero(CHASSIS_MOTOR_LF);
    // HAL_Delay(10);   // 延时10ms，确保电机使能稳定
}

/**
 * @brief 重置电机响应标志
 */
void Chassis::resetMotorResponse()
{
    g_motorResponse = false;
}

/**
 * @brief 前进指定距离
 * @param distance 距离值，正值
 * @return 是否完成动作，true完成，false未完成
 */
bool Chassis::moveForward(uint32_t distance)
{
    if (!_initialized) return false;

    // 新动作初始化
    if (!_motorBusy)
    {
        // 重置电机响应标志
        resetMotorResponse();

        // 重置位置，配置电机
        // resetPosition();
        // 右侧电机(1,2)逆时针，左侧电机(3,4)顺时针
        EMMV5_Pos_Control(
            CHASSIS_MOTOR_RF, CHASSIS_DIR_CCW, CHASSIS_MOTOR_SPEED, CHASSIS_ACC_DEFAULT, distance, false, true);
        HAL_Delay(1);
        EMMV5_Pos_Control(
            CHASSIS_MOTOR_RB, CHASSIS_DIR_CCW, CHASSIS_MOTOR_SPEED, CHASSIS_ACC_DEFAULT, distance, false, true);
        HAL_Delay(1);
        EMMV5_Pos_Control(
            CHASSIS_MOTOR_LB, CHASSIS_DIR_CW, CHASSIS_MOTOR_SPEED, CHASSIS_ACC_DEFAULT, distance, false, true);
        HAL_Delay(1);
        EMMV5_Pos_Control(
            CHASSIS_MOTOR_LF, CHASSIS_DIR_CW, CHASSIS_MOTOR_SPEED, CHASSIS_ACC_DEFAULT, distance, false, true);
        HAL_Delay(1);

        // 触发同步运动
        syncMotion();

        _motorBusy = true;
        Vofa_FireWater("开始前进 %lu 单位\r\n", distance);
        return false;
    }

    // 检查完成状态
    return checkMotionStatus();
}

/**
 * @brief 后退指定距离
 * @param distance 距离值，正值
 * @return 是否完成动作，true完成，false未完成
 */
bool Chassis::moveBackward(uint32_t distance)
{
    if (!_initialized) return false;

    // 新动作初始化
    if (!_motorBusy)
    {
        // 重置电机响应标志
        resetMotorResponse();

        // 重置位置，配置电机
        // resetPosition();
        // 右侧电机(1,2)顺时针，左侧电机(3,4)逆时针
        EMMV5_Pos_Control(
            CHASSIS_MOTOR_RF, CHASSIS_DIR_CW, CHASSIS_MOTOR_SPEED, CHASSIS_ACC_DEFAULT, distance, false, true);
        HAL_Delay(1);
        EMMV5_Pos_Control(
            CHASSIS_MOTOR_RB, CHASSIS_DIR_CW, CHASSIS_MOTOR_SPEED, CHASSIS_ACC_DEFAULT, distance, false, true);
        HAL_Delay(1);
        EMMV5_Pos_Control(
            CHASSIS_MOTOR_LB, CHASSIS_DIR_CCW, CHASSIS_MOTOR_SPEED, CHASSIS_ACC_DEFAULT, distance, false, true);
        HAL_Delay(1);
        EMMV5_Pos_Control(
            CHASSIS_MOTOR_LF, CHASSIS_DIR_CCW, CHASSIS_MOTOR_SPEED, CHASSIS_ACC_DEFAULT, distance, false, true);
        HAL_Delay(1);


        // 触发同步运动
        syncMotion();

        _motorBusy = true;
        Vofa_FireWater("开始后退 %lu 单位\r\n", distance);
        return false;
    }
    // 检查完成状态
    return checkMotionStatus();
}

/**
 * @brief 向右横移指定距离
 * @param distance 距离值，正值
 * @return 是否完成动作，true完成，false未完成
 */
bool Chassis::moveRight(uint32_t distance)
{
    if (!_initialized) return false;

    // 新动作初始化
    if (!_motorBusy)
    {
        // 重置电机响应标志
        resetMotorResponse();

        // 重置位置，配置电机
        // resetPosition();
        // 控制四个电机同步运动 - 右移模式 (麦克纳姆轮)
        // RF和LB顺时针, RB和LF逆时针
        EMMV5_Pos_Control(
            CHASSIS_MOTOR_RF, CHASSIS_DIR_CW, CHASSIS_MOTOR_SPEED, CHASSIS_ACC_DEFAULT, distance, false, true);
        HAL_Delay(1);
        EMMV5_Pos_Control(
            CHASSIS_MOTOR_RB, CHASSIS_DIR_CCW, CHASSIS_MOTOR_SPEED, CHASSIS_ACC_DEFAULT, distance, false, true);
        HAL_Delay(1);
        EMMV5_Pos_Control(
            CHASSIS_MOTOR_LB, CHASSIS_DIR_CCW, CHASSIS_MOTOR_SPEED, CHASSIS_ACC_DEFAULT, distance, false, true);
        HAL_Delay(1);
        EMMV5_Pos_Control(
            CHASSIS_MOTOR_LF, CHASSIS_DIR_CW, CHASSIS_MOTOR_SPEED, CHASSIS_ACC_DEFAULT, distance, false, true);
        HAL_Delay(1);

        // 触发同步运动
        syncMotion();

        _motorBusy = true;
        Vofa_FireWater("开始右移 %lu 单位\r\n", distance);
        return false;
    }

    // 检查完成状态
    return checkMotionStatus();
}

/**
 * @brief 向左横移指定距离
 * @param distance 距离值，正值
 * @return 是否完成动作，true完成，false未完成
 */
bool Chassis::moveLeft(uint32_t distance)
{
    if (!_initialized) return false;

    // 新动作初始化
    if (!_motorBusy)
    {
        // 重置电机响应标志
        resetMotorResponse();

        // 重置位置，配置电机
        // resetPosition();
        // 控制四个电机同步运动 - 左移模式 (麦克纳姆轮)
        // RF和LB逆时针, RB和LF顺时针
        EMMV5_Pos_Control(
            CHASSIS_MOTOR_RF, CHASSIS_DIR_CCW, CHASSIS_MOTOR_SPEED, CHASSIS_ACC_DEFAULT, distance, false, true);
        HAL_Delay(1);
        EMMV5_Pos_Control(
            CHASSIS_MOTOR_RB, CHASSIS_DIR_CW, CHASSIS_MOTOR_SPEED, CHASSIS_ACC_DEFAULT, distance, false, true);
        HAL_Delay(1);
        EMMV5_Pos_Control(
            CHASSIS_MOTOR_LB, CHASSIS_DIR_CW, CHASSIS_MOTOR_SPEED, CHASSIS_ACC_DEFAULT, distance, false, true);
        HAL_Delay(1);
        EMMV5_Pos_Control(
            CHASSIS_MOTOR_LF, CHASSIS_DIR_CCW, CHASSIS_MOTOR_SPEED, CHASSIS_ACC_DEFAULT, distance, false, true);
        HAL_Delay(1);

        // 触发同步运动
        syncMotion();

        _motorBusy = true;
        Vofa_FireWater("开始左移 %lu 单位\r\n", distance);
        return false;
    }

    // 检查完成状态
    return checkMotionStatus();
}

/**
 * @brief 左转指定角度
 * @param angle 角度值，正值
 * @return 是否完成动作，true完成，false未完成
 */
bool Chassis::turnLeft(uint32_t angle)
{
    if (!_initialized) return false;

    // 新动作初始化
    if (!_motorBusy)
    {
        // 重置电机响应标志
        resetMotorResponse();

        // 重置位置，配置电机
        // resetPosition();
        // 控制四个电机同步运动 - 左转模式
        // 右侧电机(1,2)逆时针，左侧电机(3,4)逆时针
        EMMV5_Pos_Control(
            CHASSIS_MOTOR_RF, CHASSIS_DIR_CCW, CHASSIS_MOTOR_SPEED, CHASSIS_ACC_DEFAULT, angle, false, true);
        HAL_Delay(1);
        EMMV5_Pos_Control(
            CHASSIS_MOTOR_RB, CHASSIS_DIR_CCW, CHASSIS_MOTOR_SPEED, CHASSIS_ACC_DEFAULT, angle, false, true);
        HAL_Delay(1);
        EMMV5_Pos_Control(
            CHASSIS_MOTOR_LB, CHASSIS_DIR_CCW, CHASSIS_MOTOR_SPEED, CHASSIS_ACC_DEFAULT, angle, false, true);
        HAL_Delay(1);
        EMMV5_Pos_Control(
            CHASSIS_MOTOR_LF, CHASSIS_DIR_CCW, CHASSIS_MOTOR_SPEED, CHASSIS_ACC_DEFAULT, angle, false, true);
        HAL_Delay(1);

        // 触发同步运动
        syncMotion();

        _motorBusy = true;
        Vofa_FireWater("开始左转 %lu 单位\r\n", angle);
        return false;
    }

    // 检查完成状态
    return checkMotionStatus();
}

/**
 * @brief 右转指定角度
 * @param angle 角度值，正值
 * @return 是否完成动作，true完成，false未完成
 */
bool Chassis::turnRight(uint32_t angle)
{
    if (!_initialized) return false;

    // 新动作初始化
    if (!_motorBusy)
    {
        // 重置电机响应标志
        resetMotorResponse();

        // 重置位置，配置电机
        // resetPosition();
        // 控制四个电机同步运动 - 右转模式
        // 右侧电机(1,2)顺时针，左侧电机(3,4)顺时针
        EMMV5_Pos_Control(
            CHASSIS_MOTOR_RF, CHASSIS_DIR_CW, CHASSIS_MOTOR_SPEED, CHASSIS_ACC_DEFAULT, angle, false, true);
        HAL_Delay(1);
        EMMV5_Pos_Control(
            CHASSIS_MOTOR_RB, CHASSIS_DIR_CW, CHASSIS_MOTOR_SPEED, CHASSIS_ACC_DEFAULT, angle, false, true);
        HAL_Delay(1);
        EMMV5_Pos_Control(
            CHASSIS_MOTOR_LB, CHASSIS_DIR_CW, CHASSIS_MOTOR_SPEED, CHASSIS_ACC_DEFAULT, angle, false, true);
        HAL_Delay(1);
        EMMV5_Pos_Control(
            CHASSIS_MOTOR_LF, CHASSIS_DIR_CW, CHASSIS_MOTOR_SPEED, CHASSIS_ACC_DEFAULT, angle, false, true);
        HAL_Delay(1);

        // 触发同步运动
        syncMotion();

        _motorBusy = true;
        Vofa_FireWater("开始右转 %lu 单位\r\n", angle);
        return false;
    }

    // 检查完成状态
    return checkMotionStatus();
}

/**
 * @brief 旋转到绝对角度（使用imu）
 * @param targetAngle 目标角度，范围0-360度
 * @return 是否完成旋转，true完成，false未完成
 */
bool Chassis::rotateToAngle(float targetAngle)
{
    if (!_initialized) return false;

    float currentAngle = g_hwt101->getYawAngle();
    float angleError   = calculateAngleError(currentAngle, targetAngle);

    // 如果误差很小，认为已到达目标角度
    if (fabs(angleError) < 0.4f)
    {
        if (_motorBusy)
        {
            Vofa_FireWater("旋转完成，当前角度:%.1f，目标角度:%.1f\r\n", currentAngle, targetAngle);
        }
        else
        {
            Vofa_FireWater("已在目标角度附近，无需旋转\r\n");
        }
        _motorBusy = false;
        return true;
    }

    // 如果电机繁忙，等待当前动作完成
    if (_motorBusy && !checkMotionStatus())
    {
        return false;   // 电机仍在执行动作
    }

    // 到这里说明：要么电机不忙，要么电机已完成但需要继续调整

    // 计算旋转角度并应用补偿
    float rotationAngle = fabs(angleError);

    // 适当调整旋转系数
    if (rotationAngle > 90.0f)
        rotationAngle *= 1.2f;
    else if (rotationAngle > 45.0f)
        rotationAngle *= 10.4f;
    else if (rotationAngle > 8.0f)
        rotationAngle *= 6.5f;
    else
        rotationAngle *= 1.5f;


    // 限制最大旋转角度
    if (rotationAngle > 1500.0f) rotationAngle = 1500.0f;

    // 如果是继续调整，记录日志
    if (_motorBusy)
    {
        Vofa_FireWater("需要继续调整，当前:%.1f 目标:%.1f 误差:%.1f\r\n", currentAngle, targetAngle, angleError);
    }
    else
    {
        // 记录角度误差并打印开始旋转日志
        _angleError = angleError;
        Vofa_FireWater("开始旋转到 %.1f 度，当前角度 %.1f 度，误差 %.1f 度\r\n", targetAngle, currentAngle, angleError);
    }

    // 将角度转换为脉冲数 (假设100脉冲=1度，需要根据实际情况调整)
    const float PULSE_PER_DEGREE = 50.0f;   // 每度对应的脉冲数
    uint32_t    rotationPulse    = (uint32_t)(rotationAngle * PULSE_PER_DEGREE);

    // 确保至少有一定数量的脉冲
    if (rotationPulse < 10) rotationPulse = 10;

    // 执行旋转
    if (angleError > 0)
        turnRight(rotationPulse);
    else
        turnLeft(rotationPulse);

    Vofa_FireWater("旋转 %.1f 度 (对应 %lu 脉冲)\r\n", rotationAngle, rotationPulse);

    return false;
}

/**
 * @brief 停止动作
 */
void Chassis::stop()
{
    if (!_initialized) return;

    // 所有电机立即停止
    EMMV5_Stop_Now(CHASSIS_MOTOR_RF, true);
    EMMV5_Stop_Now(CHASSIS_MOTOR_RB, true);
    EMMV5_Stop_Now(CHASSIS_MOTOR_LB, true);
    EMMV5_Stop_Now(CHASSIS_MOTOR_LF, true);

    // 触发同步运动
    syncMotion();

    // 重置状态
    _motorBusy    = false;
    _waitingDelay = false;

    Vofa_FireWater("底盘停止\r\n");
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
    EMMV5_Synchronous_motion(CHASSIS_MOTOR_ALL);
}

/**
 * @brief 检查电机响应状态
 * @return 是否收到响应，true已响应，false未响应
 */
bool Chassis::checkMotorResponse() const
{
    static uint32_t lastDebugTime = 0;

    // 如果已经收到响应，直接返回true
    if (g_motorResponse)
    {
        // 定期打印状态
        if (HAL_GetTick() - lastDebugTime > 500)
        {
            lastDebugTime = HAL_GetTick();
            Vofa_FireWater("电机状态: 就绪\r\n");
        }
        return true;
    }

    // 未收到响应，检查是否有新响应
    uint8_t status = EMMV5_Receive_Data_Right();
    if (status == 1)
    {
        g_motorResponse = true;
        Vofa_FireWater("收到电机到位响应!\r\n");
    }

    // 定期打印状态
    if (HAL_GetTick() - lastDebugTime > 500)
    {
        lastDebugTime = HAL_GetTick();
        Vofa_FireWater("电机状态: %s\r\n", g_motorResponse ? "就绪" : "等待响应");
    }

    return g_motorResponse;
}
/**
 * @brief 移动到指定坐标（麦克纳姆轮矢量解算方式）
 * @param targetX_mm 目标X坐标(毫米)
 * @param targetY_mm 目标Y坐标(毫米)
 * @param speed 速度参数
 * @param acc 加速度参数
 * @return 是否完成移动，true完成，false未完成
 */
bool Chassis::moveToCoordinates(int32_t targetX_mm, int32_t targetY_mm, int16_t speed, uint16_t acc)
{
    if (!_initialized) return false;

    // 新动作初始化
    if (!_motorBusy)
    {
        // 重置电机响应标志
        resetMotorResponse();

        // 麦克纳姆轮解算：从mm到脉冲数的转换
        // 根据您的示例代码，脉冲数 = 距离(mm) * 16.15
        const float PULSE_FACTOR = 16.15f;
        int32_t     tarX         = (int32_t)(targetX_mm * PULSE_FACTOR);
        int32_t     tarY         = (int32_t)(targetY_mm * PULSE_FACTOR);

        // 计算速度分量
        float distance = sqrtf(tarX * tarX + tarY * tarY);
        if (distance < 1.0f)
        {
            Vofa_FireWater("目标距离太近，无需移动\r\n");
            return true;
        }

        // 速度解算
        int32_t v2 = (int32_t)(speed * fabsf(tarY - tarX) / (1.4f * distance));
        int32_t v1 = (int32_t)(speed * fabsf(tarY + tarX) / (1.4f * distance));

        // 防止速度为0
        if (v1 < 10) v1 = 10;
        if (v2 < 10) v2 = 10;

        Vofa_FireWater("移动到坐标: (%ld, %ld)mm, 脉冲数: (%ld, %ld), 速度: V1=%ld, V2=%ld\r\n",
                       targetX_mm,
                       targetY_mm,
                       tarX,
                       tarY,
                       v1,
                       v2);

        // RF=1, RB=2, LF=4, LB=3 (根据您的代码对应关系)

        // Y-X分量控制(电机2和4)
        if (tarY - tarX >= 0)
        {
            // 正向旋转
            EMMV5_Pos_Control(CHASSIS_MOTOR_RB, CHASSIS_DIR_CW, v2, acc, tarY - tarX, false, true);
            HAL_Delay(1);
            EMMV5_Pos_Control(CHASSIS_MOTOR_LF, CHASSIS_DIR_CCW, v2, acc, tarY - tarX, false, true);
            HAL_Delay(1);
        }
        else
        {
            // 反向旋转
            EMMV5_Pos_Control(CHASSIS_MOTOR_RB, CHASSIS_DIR_CCW, v2, acc, tarX - tarY, false, true);
            HAL_Delay(1);
            EMMV5_Pos_Control(CHASSIS_MOTOR_LF, CHASSIS_DIR_CW, v2, acc, tarX - tarY, false, true);
            HAL_Delay(1);
        }

        // X+Y分量控制(电机1和3)
        if (tarX + tarY >= 0)
        {
            // 正向旋转
            EMMV5_Pos_Control(CHASSIS_MOTOR_RF, CHASSIS_DIR_CCW, v1, acc, tarX + tarY, false, true);
            HAL_Delay(1);
            EMMV5_Pos_Control(CHASSIS_MOTOR_LB, CHASSIS_DIR_CW, v1, acc, tarX + tarY, false, true);
            HAL_Delay(1);
        }
        else
        {
            // 反向旋转
            EMMV5_Pos_Control(CHASSIS_MOTOR_RF, CHASSIS_DIR_CW, v1, acc, -(tarX + tarY), false, true);
            HAL_Delay(1);
            EMMV5_Pos_Control(CHASSIS_MOTOR_LB, CHASSIS_DIR_CCW, v1, acc, -(tarX + tarY), false, true);
            HAL_Delay(1);
        }

        // 触发同步运动
        syncMotion();

        _motorBusy = true;
        return false;
    }

    // 检查完成状态
    return checkMotionStatus();
}
/**
 * @brief 检查电机运动状态(包含超时保护)
 * @return 是否完成，true完成，false未完成
 */
bool Chassis::checkMotionStatus()
{
    static uint32_t operationStartTime = 0;

    // 初始化计时器
    if (operationStartTime == 0)
    {
        operationStartTime = HAL_GetTick();
    }

    // 超时保护 (2.5秒)
    if (HAL_GetTick() - operationStartTime > 2500)
    {
        Vofa_FireWater("警告: 操作超时(2.5秒)，强制结束\r\n");
        _motorBusy         = false;
        operationStartTime = 0;
        return true;
    }

    // 检查电机是否就绪
    if (checkMotorResponse())
    {
        if (!_waitingDelay)
        {
            _motorReadyTime = HAL_GetTick();
            _waitingDelay   = true;
        }

        // 额外延时确保稳定
        if (HAL_GetTick() - _motorReadyTime >= CHASSIS_MOTOR_DELAY)
        {
            _motorBusy         = false;
            _waitingDelay      = false;
            operationStartTime = 0;
            Vofa_FireWater("动作完成\r\n");
            return true;
        }
    }

    return false;
}