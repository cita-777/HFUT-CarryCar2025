/**
 * *****************************************************************************
 * @file        car_chassis.h
 * @brief       底盘控制类
 * @author      ciat-777 (juricek.chen@gmail.com)
 * @date        2025-03-28
 * @copyright   cita
 * *****************************************************************************
 */

#ifndef CAR_CHASSIS_H
#    define CAR_CHASSIS_H

/* Includes ------------------------------------------------------------------*/
#    include "2_Device/HWT101/dvc_hwt101.h"
#    include "2_Device/Motor/Motor_ZDT42/dvc_zdt_x42.h"
#    include "2_Device/Vofa/dvc_vofa.h"
#    include <stdbool.h>
#    include <stdint.h>

#    ifdef __cplusplus
extern "C" {
#    endif

/* 定义常量 ------------------------------------------------------------------*/
#    define CHASSIS_ACC_DEFAULT 100      // 默认加速度
#    define CHASSIS_DEC_DEFAULT 100      // 默认减速度
#    define CHASSIS_MOTOR_SPEED 200.0f   // 默认电机速度
#    define CHASSIS_MOTOR_DELAY 30       // 电机到位后的等待时间(ms)

#    define CHASSIS_MOTOR_ALL 0     // 用于同步所有电机
#    define CHASSIS_MOTOR_RF 0x01   // 右前轮
#    define CHASSIS_MOTOR_RB 0x02   // 右后轮
#    define CHASSIS_MOTOR_LB 0x03   // 左后轮
#    define CHASSIS_MOTOR_LF 0x04   // 左前轮

#    define CHASSIS_DIR_CW 0    // 顺时针方向
#    define CHASSIS_DIR_CCW 1   // 逆时针方向

/**
 * @brief 底盘控制类，封装四轮车底盘的基本控制功能
 */
class Chassis
{
public:
    /**
     * @brief 构造函数
     */
    Chassis();

    /**
     * @brief 析构函数
     */
    ~Chassis();

    /**
     * @brief 初始化底盘
     * @return 初始化结果，true成功，false失败
     */
    bool init();

    /**
     * @brief 前进指定距离
     * @param distance 距离值，正值
     * @return 是否完成动作，true完成，false未完成
     */
    bool moveForward(float distance);

    /**
     * @brief 后退指定距离
     * @param distance 距离值，正值
     * @return 是否完成动作，true完成，false未完成
     */
    bool moveBackward(float distance);
    bool moveRight(float distance);
    bool moveLeft(float distance);
    /**
     * @brief 左转指定角度
     * @param angle 角度值，正值
     * @return 是否完成动作，true完成，false未完成
     */
    bool turnLeft(float angle);

    /**
     * @brief 右转指定角度
     * @param angle 角度值，正值
     * @return 是否完成动作，true完成，false未完成
     */
    bool turnRight(float angle);

    /**
     * @brief 停止动作
     */
    void stop();

    /**
     * @brief 旋转到绝对角度
     * @param targetAngle 目标角度，范围0-360度
     * @return 是否完成旋转，true完成，false未完成
     */
    bool rotateToAngle(float targetAngle);

    /**
     * @brief 设置电机使能状态
     * @param state 使能状态，true使能，false禁用
     */
    void setEnable(bool state);

    /**
     * @brief 重置电机位置为零
     */
    void resetPosition();

    /**
     * @brief 获取IMU航向角
     * @return 当前航向角，范围0-360度
     */
    float getYawAngle() const;

private:
    // 电机参数和状态
    bool _initialized;   // 初始化状态
    bool _motorBusy;     // 电机繁忙状态

    // 角度控制相关
    float _angleError;   // 角度误差

    // 等待电机到位的额外延时
    uint32_t _motorReadyTime;   // 记录电机到位的时间戳
    bool     _waitingDelay;     // 电机到位后的额外等待标志

    /**
     * @brief 触发电机同步运动
     */
    void syncMotion();

    /**
     * @brief 计算角度误差
     * @param current 当前角度
     * @param target 目标角度
     * @return 最短路径的角度误差
     */
    float calculateAngleError(float current, float target);

    /**
     * @brief 检查电机是否到位
     * @return 是否到位，true已到位，false未到位
     */
    bool isMotorReady() const;
};

// 全局实例声明
extern Chassis* g_chassis;

#    ifdef __cplusplus
}
#    endif

#endif   // CAR_CHASSIS_H

/************************ COPYRIGHT(C) CITA **************************/