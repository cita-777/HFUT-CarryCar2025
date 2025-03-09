/**
 * *****************************************************************************
 * @file        tjc.cpp
 * @brief       TJC串口屏驱动实现
 * @author      ciat-777 (juricek.chen@gmail.com)
 * @date        2025-03-08
 * @copyright   cita
 * *****************************************************************************
 */

/*----------------------------------include-----------------------------------*/
#include "tjc.h"
#include "1_Middleware/1_Driver/UART/drv_uart.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
/*-----------------------------------macro------------------------------------*/
#define TJC_RX_BUFFER_SIZE 64
#define TJC_START_BYTE 0x55
#define TJC_END_BYTE 0xFF

/*----------------------------------typedef-----------------------------------*/
typedef struct
{
    uint8_t             rx_data[TJC_RX_BUFFER_SIZE];
    uint8_t             rx_count;
    uint8_t             receive_flag;
    UART_HandleTypeDef* huart;
} TJC_Handle_t;

/*----------------------------------variable----------------------------------*/
static TJC_Handle_t tjc_handle = {0};

/*----------------------------------function----------------------------------*/

/**
 * @brief TJC屏幕串口接收回调函数
 *
 * @param Buffer 接收缓冲区
 * @param Length 数据长度
 */
void TJC_UART_Callback(uint8_t* Buffer, uint16_t Length)
{
    if (Length == 0 || Buffer == nullptr)
    {
        return;
    }

    // 扫描整个缓冲区寻找起始字节
    for (uint16_t i = 0; i < Length; i++)
    {
        if (Buffer[i] == TJC_START_BYTE)
        {
            tjc_handle.receive_flag = 1;
            break;
        }
    }

    // 不需要维护自己的计数器，DMA会自动管理
}

/**
 * @brief TJC屏幕初始化
 *
 * @param huart 使用的串口句柄
 * @return uint8_t 初始化结果，0成功，非0失败
 */
uint8_t TJC_Init(UART_HandleTypeDef* huart)
{
    // 保存串口句柄
    tjc_handle.huart = huart;

    // 初始化状态
    tjc_handle.rx_count     = 0;
    tjc_handle.receive_flag = 0;
    memset(tjc_handle.rx_data, 0, sizeof(tjc_handle.rx_data));

    // 确保DMA通道已正确配置
    if (huart->hdmarx == NULL)
    {
        // DMA未配置，尝试初始化DMA
        return 1;   // 返回错误
    }
    // 初始化串口，设置回调函数
    UART_Init(huart, TJC_UART_Callback, TJC_RX_BUFFER_SIZE);

    // 等待屏幕初始化
    // HAL_Delay(100);
    TJC_Send_Command("t10.txt=\"tjc_init_right\"");
    // HAL_Delay(1);
    return 0;   // 返回成功
}

/**
 * @brief 发送数据到TJC屏幕
 *
 * @param data 数据指针
 * @param length 数据长度
 * @return uint8_t 发送结果，0成功，非0失败
 */
uint8_t TJC_Send_Data(uint8_t* data, uint16_t length)
{
    if (tjc_handle.huart == nullptr || data == nullptr)
    {
        return 1;
    }

    return UART_Send_Data(tjc_handle.huart, data, length);
}

/**
 * @brief 检查是否接收到起始字节
 *
 * @return uint8_t 1表示接收到起始字节，0表示未接收到
 */
uint8_t TJC_Check_Receive(void)
{
    uint8_t flag            = tjc_handle.receive_flag;
    tjc_handle.receive_flag = 0;   // 清除标志
    HAL_Delay(1);
    return flag;
}
/**
 * @brief 发送格式化命令到TJC屏幕（支持变量，自动添加结束符）
 *
 * @param format 格式化字符串，如"t0.txt=\"%d\""
 * @param ... 可变参数
 * @return uint8_t 发送结果，0成功，非0失败
 */
// // 发送浮点数到文本显示
// float YawAngle = 123.456f;
// TJC_Send_Format("t2.txt=\"%.2f\"", YawAngle);

// // 发送整数
// int counter = 42;
// TJC_Send_Format("n0.val=%d", counter);

// // 发送多个变量
// TJC_Send_Format("t0.txt=\"X:%.1f Y:%.1f\"", posX, posY);
uint8_t TJC_Send_Format(const char* format, ...)
{
    if (format == nullptr || tjc_handle.huart == nullptr)
    {
        return 1;
    }

    // 临时缓冲区
    char    temp_buffer[256];
    va_list args;

    // 处理格式化字符串
    va_start(args, format);
    vsnprintf(temp_buffer, sizeof(temp_buffer) - 3, format, args);
    va_end(args);

    // 获取格式化后的字符串长度
    uint16_t length = strlen(temp_buffer);

    // 添加结束符 0xFF 0xFF 0xFF
    temp_buffer[length]     = 0xFF;
    temp_buffer[length + 1] = 0xFF;
    temp_buffer[length + 2] = 0xFF;

    // 发送数据
    return UART_Send_Data(tjc_handle.huart, (uint8_t*)temp_buffer, length + 3);
}
/**
 * @brief 发送命令到TJC屏幕（自动添加结束符）
 *
 * @param cmd 命令字符串
 * @return uint8_t 发送结果，0成功，非0失败
 */
uint8_t TJC_Send_Command(const char* cmd)
{
    if (cmd == nullptr || tjc_handle.huart == nullptr) return 1;

    // 使用静态缓冲区代替动态内存分配
    static uint8_t buffer[256];
    uint16_t       length = strlen(cmd);

    if (length + 3 > sizeof(buffer)) return 2;   // 命令过长

    memcpy(buffer, cmd, length);
    buffer[length]     = 0xFF;
    buffer[length + 1] = 0xFF;
    buffer[length + 2] = 0xFF;

    return UART_Send_Data(tjc_handle.huart, buffer, length + 3);
}

/*------------------------------------test------------------------------------*/
/**
 * @brief TJC屏幕测试函数
 */
void TJC_Test(void)
{
    static uint32_t last_tick    = 0;
    static uint8_t  flag         = 0;
    uint32_t        current_tick = HAL_GetTick();

    // 每隔2秒发送一次测试命令
    if (current_tick - last_tick > 100)
    {
        last_tick = current_tick;

        // 发送一个应该会触发屏幕回复的命令
        // printf("发送测试命令: get t0.txt\r\n");
        // TJC_Send_Command("t10.txt=\"000+000\"");

        // 显示当前接收状态
        if (TJC_Check_Receive())
        {
            // printf("接收状态: 已收到起始字节\r\n");
            TJC_Send_Command("t10.txt=\"111+222\"");
            // HAL_Delay(1);
            flag = 1;
        }
        else
        {
            if (flag == 0)
            {
                TJC_Send_Command("t10.txt=\"333+333\"");
                // HAL_Delay(1);
            }
        }
    }
}