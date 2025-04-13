/**
 * *****************************************************************************
 * @file        dvc_jetson.h
 * @brief
 * @author      ciat-777 (juricek.chen@gmail.com)
 * @date        2025-03-09
 * @copyright   cita
 * *****************************************************************************
 */
#ifndef DVC_JETSON_H
#define DVC_JETSON_H

/* Includes ------------------------------------------------------------------*/
#include "1_Middleware/1_Driver/UART/drv_uart.h"
#include <stdio.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/* 定义常量 ------------------------------------------------------------------*/
#define JETSON_FRAME_SIZE 10         // Jetson通信帧长度 (原9字节+1字节额外帧头)
#define JETSON_START_BYTE 0xFF       // 起始字节
#define JETSON_START_BYTES_COUNT 2   // 帧头由两个连续的0xFF组成
#define JETSON_END_BYTE 0xEF         // 结束字节

// 命令标识
#define JETSON_CMD_QR 0x0A      // 二维码任务
#define JETSON_CMD_COORD 0x0C   // 坐标任务
#define JETSON_CMD_GRAB 0x0D    // 抓取任务
#define JETSON_CMD_SEND 0x0B    // 发送命令
// 区域标识定义
#define ZONE_RAW_MATERIAL 0x04        // 原料区标识
#define ZONE_ROUGH_PROCESSING 0x05    // 粗加工区标识
#define ZONE_TEMPORARY_STORAGE 0x06   // 暂存区标识
/**
 * @brief Jetson通信类
 */
class JetsonCommunicator
{
public:
    /**
     * @brief 构造函数
     * @param huart 使用的UART句柄
     */
    JetsonCommunicator(UART_HandleTypeDef* huart);

    /**
     * @brief 初始化Jetson通信
     */
    void init();


    /**
     * @brief 获取二维码字符串
     * @return 二维码字符串指针
     */
    const char* getQRCodeString() const;

    /**
     * @brief 获取坐标数据
     * @param x X坐标引用
     * @param y Y坐标引用
     */
    void getCoordinates(int16_t& x, int16_t& y) const;

    /**
     * @brief 获取抓取状态
     * @return 是否可以抓取
     */
    bool canGrab() const;
    void updateDisplay();
    /**
     * @brief 更新抓取等待状态
     * @param isWaiting 是否等待抓取
     */
    void setWaitGrab(bool isWaiting);
    void sendZoneReached(uint8_t zone);
    // 静态回调函数，用于接收UART数据
    static void receiveCallback(uint8_t* buffer, uint16_t size);

private:
    // 单例指针，用于静态回调
    static JetsonCommunicator* _instance;

    // UART句柄
    UART_HandleTypeDef* _huart;

    // 接收缓冲区
    uint8_t _rxBuffer[JETSON_FRAME_SIZE];
    uint8_t _rxCount;

    // 发送缓冲区
    uint8_t _txBuffer[JETSON_FRAME_SIZE];

    // 解析后的数据
    char    _qrCodeString[8];
    int16_t _coordinates[2];
    bool    _canGrab;
    bool    _qrCodeUpdated = false;
    // 是否等待抓取
    bool _isWaitingGrab;

    // 用于检测双字节帧头
    bool _lastByteWasFF = false;

    // 处理接收到的数据
    void processReceivedData(uint8_t byte);

    // 处理完整数据帧
    void processDataFrame();

    // 处理二维码数据
    void processQRCode();

    // 处理坐标数据
    void processCoordinates();

    // 处理抓取命令
    void processGrabCommand();
};

// 全局实例声明
extern JetsonCommunicator* g_jetson;
#ifdef __cplusplus
}
#endif

#endif /* DVC_JETSON_H */