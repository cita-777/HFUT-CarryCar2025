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
#define CAR_ACTION_H

/* Includes ------------------------------------------------------------------*/
#include "2_Device/Motor/Motor_CyberGear/dvc_cybergear.h"
#include "2_Device/Motor/Motor_ZDT42/dvc_zdt_emmv5.h"
#include "2_Device/Servo/dvc_fsuServo.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* 前置声明 ------------------------------------------------------------------*/
extern fsuservo::FSUS_Protocol* g_servoProtocol;
extern fsuservo::FSUS_Servo*    g_servo1;   // 云台舵机
extern fsuservo::FSUS_Servo*    g_servo3;   // 爪子舵机

/* 定义与枚举 ----------------------------------------------------------------*/
// 舵机位置枚举
enum class ServoPosition
{
    A,   // 左侧位置
    B,   // 中间位置
    C    // 右侧位置
};

// 爪子状态枚举
enum class GripperState
{
    OPEN,   // 爪子打开
    CLOSE   // 爪子关闭
};

// 云台位置枚举
enum class CybergearPosition
{
    FRONT,   // 前方位置
    BACK     // 后方位置
};

// 滑轨高度枚举
enum class SliderHeight
{
    MOST_HIGH,             // 最高位置
    PICK_FROM_TURNTABLE,   // 从转盘上拿取物料的高度
    PLACE_ON_TEMP,         // 放到车上转盘临时存放的高度
    PLACE_ON_GROUND,       // 放到地上的高度
    STACKING               // 码垛的高度
};

// 物料顺序枚举
enum class MaterialSequence
{
    SEQ_321,   // 顺序3-2-1
    SEQ_312,   // 顺序3-1-2
    SEQ_123,   // 顺序1-2-3
    SEQ_132,   // 顺序1-3-2
    SEQ_213,   // 顺序2-1-3
    SEQ_231    // 顺序2-3-1
};

// 非阻塞动作状态机状态
enum class ActionState
{
    IDLE,   // 空闲状态

    // 抓取状态
    CATCH_OPEN_GRIPPER,
    CATCH_LOWER_TO_OBJECT,
    CATCH_CLOSE_GRIPPER,
    CATCH_RAISE_SLIDER,
    CATCH_POSITION_LOCATION,
    CATCH_ROTATE_TURNTABLE,
    CATCH_RELEASE_GRIPPER,
    CATCH_FINAL_RAISE,

    // 放置状态
    PUT_MOVE_TO_TARGET,
    PUT_POSITION_LOCATION,
    PUT_ROTATE_BACK,
    PUT_OPEN_GRIPPER_FIRST,
    PUT_LOWER_FIRST,
    PUT_CLOSE_GRIPPER,
    PUT_RAISE_SLIDER,
    PUT_ROTATE_FRONT,
    PUT_LOWER_TO_PLACE,
    PUT_OPEN_GRIPPER,
    PUT_FINAL_RAISE,

    // 获取状态
    GET_POSITION_LOCATION,
    GET_LOWER_TO_OBJECT,
    GET_CLOSE_GRIPPER,
    GET_RAISE_SLIDER,
    GET_MOVE_BACKWARD,
    GET_ROTATE_TURNTABLE,
    GET_OPEN_GRIPPER,
    GET_FINAL_RAISE,
    GET_MOVE_FORWARD,

    // 抓取物料状态
    PICK_SET_CYBERGEAR,
    PICK_SET_SERVO_POSITION,
    PICK_OPEN_GRIPPER,
    PICK_LOWER_SLIDER,
    PICK_CLOSE_GRIPPER,
    PICK_RAISE_SLIDER,

    // 放置物料状态
    PLACE_SET_CYBERGEAR,
    PLACE_SET_SERVO_POSITION,
    PLACE_LOWER_SLIDER,
    PLACE_OPEN_GRIPPER,
    PLACE_RAISE_SLIDER,
    PLACE_CLOSE_GRIPPER,

    COMPLETED   // 完成状态
};

// 动作序列状态机状态
enum class MapActionState
{
    IDLE,               // 空闲状态
    MOVE_TO_POSITION,   // 移动到位置
    PLACE_OBJECT,       // 放置物体
    MOVE_TO_NEXT,       // 移动到下一个
    COMPLETED           // 完成状态
};

// 设置角度参数
#define SERVO2_A_ANGLE -30.0f    // A位置角度
#define SERVO2_B_ANGLE 90.0f     // B位置角度
#define SERVO2_C_ANGLE -139.0f   // C位置角度

#define SERVO3_OPEN_ANGLE 0.0f      // 爪子打开角度
#define SERVO3_CLOSE_ANGLE -47.0f   // 爪子关闭角度

#define CYBERGEAR_FRONT_ANGLE -3.15f   // 前方位置角度
#define CYBERGEAR_BACK_ANGLE -0.45f    // 后方位置角度

#define SLIDER_MOSTHIGH 0.0f
#define SLIDER_PICK_FROM_TURNTABLE 100000.0f   // 从转盘上拿取物料的高度
#define SLIDER_PLACE_ON_TEMP 30000.0f          // 放到车上转盘临时存放的高度
#define SLIDER_PLACE_ON_GROUND 150000.0f       // 放到地上的高度
#define SLIDER_STACKING 130000.0f              // 码垛的高度

#define SLIDER_MOTOR_ADDR 5   // 滑轨电机地址

/**
 * @brief 车辆机械动作控制类
 */
class CarAction
{
public:
    CarAction();
    bool init();

    // 基本控制函数
    bool setServo2Position(ServoPosition position, uint16_t interval = 200);
    bool setGripperState(GripperState state, uint16_t interval = 200);
    bool setCybergearPosition(CybergearPosition position);
    bool setSliderHeight(SliderHeight height);

    // 非阻塞物体操作函数
    bool catchObjToTurntable(uint8_t position);
    bool putObjToMap(uint8_t position);
    bool putObjToMapMaDuo(uint8_t position);
    bool getObjFromMap(uint8_t position);

    // 非阻塞序列执行函数
    bool executeSequence(MaterialSequence sequence);
    bool placeOnlySequence(MaterialSequence sequence);

    // 非阻塞抓取和放置物料函数
    bool pickMaterial();
    bool placeMaterial();

    // 辅助函数
    void positionToLocation(uint8_t position);

private:
    // 私有方法
    bool checkServoConnection();

    // 成员变量
    bool             _isInitialized;
    ActionState      _actionState;
    MapActionState   _mapActionState;
    uint8_t          _currentPosition;
    uint8_t          _mapStateCounter;
    MaterialSequence _currentSequence;
    uint32_t         _actionTimer;
    bool             _isSendOk;

    // 常量
    static constexpr uint16_t DISTANCE_WITH_SEHUAN = 150;
};

// 全局实例声明
extern CarAction* g_carAction;

#ifdef __cplusplus
}
#endif

#endif   // CAR_ACTION_H