/**
 * *****************************************************************************
 * @file        dvc_hwt101.h
 * @brief       HWT101惯性测量单元设备驱动
 * @author      ciat-777 (juricek.chen@gmail.com)
 * @date        2025-03-10
 * @copyright   cita
 * *****************************************************************************
 */

#ifndef DVC_HWT101_H
#    define DVC_HWT101_H

/* Includes ------------------------------------------------------------------*/
#    include "1_Middleware/1_Driver/UART/drv_uart.h"
#    include "stm32f4xx_hal.h"
#    include <stdio.h>
#    include <string.h>

#    ifdef __cplusplus
extern "C" {
#    endif

/* 定义常量 ------------------------------------------------------------------*/
#    define HWT101_RX_BUFFER_SIZE 30   // 接收缓冲区大小
#    define HWT101_DATA_LENGTH 22      // 标准数据帧长度
#    define HWT101_HEADER1 0x55        // 数据帧头字节1
#    define HWT101_HEADER2 0x52        // 数据帧头字节2(角度数据包)

/**
 * @brief HWT101惯性测量单元通信类
 */
class HWT101Communicator
{
public:
    /**
     * @brief 构造函数
     * @param huart 使用的UART句柄
     */
    HWT101Communicator(UART_HandleTypeDef* huart);

    /**
     * @brief 初始化HWT101设备
     * @return uint8_t 0表示成功，非0表示错误代码
     */
    uint8_t init(UART_HandleTypeDef* huart);

    /**
     * @brief 处理HWT101接收到的数据
     * @return uint8_t 0表示成功，非0表示数据无效或校验失败
     */
    uint8_t processData();

    /**
     * @brief 校准HWT101的偏航角(Z轴角度)归零
     * @return uint8_t 0表示成功，非0表示错误代码
     */
    uint8_t calibrateYaw();

    /**
     * @brief 获取当前偏航角(航向角)
     * @return float 当前偏航角，范围0-360度
     */
    float getYawAngle() const;

    /**
     * @brief 设置传感器输出模式
     * @param mode 模式值
     * @return uint8_t 0表示成功，非0表示错误代码
     */
    uint8_t setOutputMode(uint8_t mode);

    // 静态回调函数，用于接收UART数据
    static void receiveCallback(uint8_t* buffer, uint16_t length);

private:
    // 单例指针，用于静态回调
    static HWT101Communicator* _instance;

    // UART句柄
    UART_HandleTypeDef* _huart;

    // 接收缓冲区
    uint8_t _rxBuffer[HWT101_RX_BUFFER_SIZE];
    uint8_t _dataReadyFlag;

    // 角度数据
    float _yawAngle;       // 偏航角 0-360°
    float _rawYawAngle;    // 原始偏航角数据
    float _lastYawAngle;   // 上一次角度值

    // 计算校验和
    uint8_t checkSum(uint8_t* data) const;
};

// 全局实例声明
extern HWT101Communicator* g_hwt101;

#    ifdef __cplusplus
}
#    endif

#endif /* DVC_HWT101_H */

/************************ COPYRIGHT(C) CITA **************************/