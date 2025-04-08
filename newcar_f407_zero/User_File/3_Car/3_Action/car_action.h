/**
 * *****************************************************************************
 * @file        car_action.h
 * @brief       车辆机械动作控制类
 * @author      ciat-777 (juricek.chen@gmail.com)
 * @date        2025-03-28
 * @copyright   cita
 * *****************************************************************************
 */

#ifndef CAR_ACTION_H
#    define CAR_ACTION_H

/* Includes ------------------------------------------------------------------*/
#    include "2_Device/Motor/Motor_CyberGear/dvc_cybergear.h"
#    include "2_Device/Motor/Motor_ZDT42/dvc_zdt_emmv5.h"
#    include "2_Device/Servo/dvc_fsuServo.h"
#    include <stdint.h>

#    ifdef __cplusplus
extern "C" {
#    endif

/* 前置声明 ------------------------------------------------------------------*/
extern fsuservo::FSUS_Protocol* g_servoProtocol;
extern fsuservo::FSUS_Servo*    g_servo1;   // 云台舵机
extern fsuservo::FSUS_Servo*    g_servo3;   // 爪子舵机

/* 定义常量 ------------------------------------------------------------------*/
// 转盘舵机2位置对应的角度值
#    define SERVO2_A_ANGLE -45.0f   // A位置角度
#    define SERVO2_B_ANGLE 0.0f     // B位置角度
#    define SERVO2_C_ANGLE 45.0f    // C位置角度

// 夹爪舵机3状态对应的角度值
#    define SERVO3_OPEN_ANGLE 0.0f      // 爪子打开角度
#    define SERVO3_CLOSE_ANGLE -47.0f   // 爪子关闭角度

// Cybergear云台位置对应的角度值
#    define CYBERGEAR_FRONT_ANGLE 0.1f   // 前方位置角度
#    define CYBERGEAR_BACK_ANGLE 1.0f    // 后方位置角度

// ZDT_X42滑轨高度对应的位置值
// #    define SLIDER_PICK_FROM_TURNTABLE 6000.0f   // 从转盘上拿取物料的高度
// #    define SLIDER_PLACE_ON_TEMP 2000.0f         // 放到车上转盘临时存放的高度
// #    define SLIDER_PLACE_ON_GROUND 10000.0f      // 放到地上的高度
#    define SLIDER_PICK_FROM_TURNTABLE 6000.0f   // 从转盘上拿取物料的高度
#    define SLIDER_PLACE_ON_TEMP 0.0f            // 放到车上转盘临时存放的高度
#    define SLIDER_PLACE_ON_GROUND 2000.0f       // 放到地上的高度
#    define SLIDER_STACKING 7000.0f              // 码垛的高度

// ZDT_X42电机地址
#    define SLIDER_MOTOR_ADDR 5   // 滑轨电机地址

/* 枚举类型定义 --------------------------------------------------------------*/
/**
 * @brief 舵机2位置枚举(云台舵机)
 */
enum class ServoPosition
{
    A = 1,   // 左侧位置
    B = 2,   // 中间位置
    C = 3    // 右侧位置
};

/**
 * @brief 舵机3状态枚举(爪子舵机)
 */
enum class GripperState
{
    OPEN,   // 爪子打开
    CLOSE   // 爪子关闭
};

/**
 * @brief Cybergear云台位置枚举
 */
enum class CybergearPosition
{
    FRONT,   // 前方(抓取物料)
    BACK     // 后方(摆放物料)
};

/**
 * @brief ZDT_X42滑轨高度枚举
 */
enum class SliderHeight
{
    PICK_FROM_TURNTABLE,   // 从转盘上拿取物料的高度
    PLACE_ON_TEMP,         // 放到车上转盘临时存放的高度
    PLACE_ON_GROUND,       // 放到地上的高度
    STACKING               // 码垛的高度
};

/**
 * @brief 车辆机械动作控制类
 */
class CarAction
{
public:
    /**
     * @brief 构造函数
     */
    CarAction();

    /**
     * @brief 初始化
     * @return bool 初始化结果，true成功，false失败
     */
    bool init();

    /**
     * @brief 云台舵机控制
     * @param position 舵机位置
     * @param interval 运动时间间隔(ms)
     * @return bool 操作结果，true成功，false失败
     */
    bool setServo2Position(ServoPosition position, uint16_t interval = 200);

    /**
     * @brief 爪子舵机控制
     * @param state 爪子状态
     * @param interval 运动时间间隔(ms)
     * @return bool 操作结果，true成功，false失败
     */
    bool setGripperState(GripperState state, uint16_t interval = 200);

    /**
     * @brief Cybergear电机控制(抓取云台)
     * @param position 云台位置
     * @return bool 操作结果，true成功，false失败
     */
    bool setCybergearPosition(CybergearPosition position);

    /**
     * @brief ZDT_X42电机控制(滑轨高度)
     * @param height 滑轨高度
     * @return bool 操作结果，true成功，false失败
     */
    bool setSliderHeight(SliderHeight height);

    /**
     * @brief 等待舵机2完成动作
     */
    void waitForServo2();

    /**
     * @brief 等待舵机3完成动作
     */
    void waitForServo3();

    /**
     * @brief 等待Cybergear电机完成动作
     */
    void waitForCybergear();

    /**
     * @brief 等待滑轨电机完成动作
     */
    void waitForSlider();

    /**
     * @brief 等待所有执行器完成动作
     */
    void waitForAllActuators();

    /**
     * @brief 抓取物料组合动作
     * @return bool 操作结果，true成功，false失败
     */
    bool pickMaterial();

    /**
     * @brief 放置物料组合动作
     * @return bool 操作结果，true成功，false失败
     */
    bool placeMaterial();

private:
    /**
     * @brief 检查舵机通信状态
     * @return bool 检查结果，true正常，false异常
     */
    bool checkServoConnection();

    // 初始化标志
    bool _isInitialized;
};

// 全局实例声明
extern CarAction* g_carAction;

#    ifdef __cplusplus
}
#    endif

#endif   // CAR_ACTION_H

/************************ COPYRIGHT(C) CITA **************************/