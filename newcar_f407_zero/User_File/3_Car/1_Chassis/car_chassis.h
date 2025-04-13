/**
 * *****************************************************************************
 * @file        car_chassis.h
 * @brief       底盘控制类声明
 * @author      ciat-777 (juricek.chen@gmail.com)
 * @date        2025-03-28
 * @copyright   cita
 * *****************************************************************************
 */

#ifndef __CAR_CHASSIS_H
#define __CAR_CHASSIS_H

/* Includes ------------------------------------------------------------------*/
#include "2_Device/HWT101/dvc_hwt101.h"
#include "2_Device/Motor/Motor_ZDT42/dvc_zdt_emmv5.h"

/* Defines -------------------------------------------------------------------*/
// 底盘电机编号定义
#define CHASSIS_MOTOR_RF 1    // 右前轮
#define CHASSIS_MOTOR_RB 2    // 右后轮
#define CHASSIS_MOTOR_LB 3    // 左后轮
#define CHASSIS_MOTOR_LF 4    // 左前轮
#define CHASSIS_MOTOR_ALL 0   // 所有电机

// 底盘电机方向定义
#define CHASSIS_DIR_CW 0    // 顺时针
#define CHASSIS_DIR_CCW 1   // 逆时针

// 底盘参数定义
#define CHASSIS_MOTOR_SPEED 200   // 电机基础速度
#define CHASSIS_ACC_DEFAULT 200   // 默认加速度
#define CHASSIS_MOTOR_DELAY 50    // 电机到位后额外等待时间(ms)

/**
 * @brief 底盘控制类
 */
class Chassis
{
public:
    // 构造和析构
    Chassis();
    ~Chassis();

    // 基本功能
    bool init();                  // 初始化底盘
    void setEnable(bool state);   // 设置电机使能状态
    void resetPosition();         // 重置电机位置为零
    void stop();                  // 停止动作

    // 移动功能
    bool moveForward(uint32_t distance);    // 前进
    bool moveBackward(uint32_t distance);   // 后退
    bool moveLeft(uint32_t distance);       // 左移
    bool moveRight(uint32_t distance);      // 右移
    bool moveToCoordinates(int32_t targetX_mm, int32_t targetY_mm, int16_t speed, uint16_t acc);
    // 转向功能
    bool turnLeft(uint32_t angle);           // 左转
    bool turnRight(uint32_t angle);          // 右转
    bool rotateToAngle(float targetAngle);   // 旋转到绝对角度

    // 状态查询
    float getYawAngle() const;   // 获取当前航向角

private:
    // 内部辅助函数
    void  syncMotion();                                       // 触发电机同步运动
    bool  isMotorReady() const;                               // 检查电机是否到位
    float calculateAngleError(float current, float target);   // 计算角度误差
    bool  checkMotionStatus();                                // 检查运动状态(带超时保护)
    // 重置电机响应标志
    void resetMotorResponse();

    // 检查电机响应状态
    bool checkMotorResponse() const;
    // 成员变量
    bool     _initialized;      // 初始化状态
    bool     _motorBusy;        // 电机繁忙状态
    float    _angleError;       // 角度误差
    uint32_t _motorReadyTime;   // 电机就绪时间
    bool     _waitingDelay;     // 等待延时状态
};

// 全局实例声明
extern Chassis* g_chassis;

#endif /* __CAR_CHASSIS_H */