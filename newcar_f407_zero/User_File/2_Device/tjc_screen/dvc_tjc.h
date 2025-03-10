/**
 * *****************************************************************************
 * @file        dvc_tjc.h
 * @brief       TJC串口屏驱动头文件
 * @author      ciat-777 (juricek.chen@gmail.com)
 * @date        2025-03-08
 * @copyright   cita
 * *****************************************************************************
 */

#ifndef DVC_TJC_H
#define DVC_TJC_H

#include "stm32f4xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief TJC屏幕初始化
 *
 * @param huart 使用的串口句柄
 * @return uint8_t 初始化结果，0成功，非0失败
 */
uint8_t TJC_Init(UART_HandleTypeDef* huart);

/**
 * @brief 发送数据到TJC屏幕
 *
 * @param data 数据指针
 * @param length 数据长度
 * @return uint8_t 发送结果，0成功，非0失败
 */
uint8_t TJC_Send_Data(uint8_t* data, uint16_t length);

/**
 * @brief 检查是否接收到起始字节
 *
 * @return uint8_t 1表示接收到起始字节，0表示未接收到
 */
uint8_t TJC_Check_Receive(void);

/**
 * @brief 发送命令到TJC屏幕（自动添加结束符）
 *
 * @param cmd 命令字符串
 * @return uint8_t 发送结果，0成功，非0失败
 */
uint8_t TJC_Send_Command(const char* cmd);

/**
 * @brief 发送格式化命令到TJC屏幕（支持变量，自动添加结束符）
 *
 * @param format 格式化字符串，如"t0.txt=\"%d\""
 * @param ... 可变参数
 * @return uint8_t 发送结果，0成功，非0失败
 */
uint8_t TJC_Send_Format(const char* format, ...);

/**
 * @brief TJC屏幕测试函数
 */
void TJC_Test(void);

#ifdef __cplusplus
}
#endif

#endif /* DVC_TJC_H */