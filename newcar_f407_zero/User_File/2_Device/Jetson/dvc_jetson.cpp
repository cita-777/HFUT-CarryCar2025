/**
 * *****************************************************************************
 * @file        dvc_jetson.cpp
 * @brief
 * @author      ciat-777 (juricek.chen@gmail.com)
 * @date        2025-03-09
 * @copyright   cita
 * *****************************************************************************
 */

/*------------------------------------------ Includes
 * ------------------------------------------------------------------*/
#include "dvc_jetson.h"
#include "2_Device/tjc_screen/dvc_tjc.h"   // 导入TJC屏幕头文件，用于显示二维码
// #include "staMachine.h"                // 导入状态机头文件，用于访问is_wait_jetson_grab
#include "2_Device/Vofa/dvc_vofa.h"   // 导入UART头文件，用于串口通信
/* 静态成员初始化 ----------------------------------------------------------*/
JetsonCommunicator* JetsonCommunicator::_instance = nullptr;
JetsonCommunicator* g_jetson                      = nullptr;

/**
 * @brief 构造函数
 */
JetsonCommunicator::JetsonCommunicator(UART_HandleTypeDef* huart)
    : _huart(huart)
    , _rxCount(0)
    , _canGrab(false)
    , _isWaitingGrab(false)
    , _lastByteWasFF(false)
{
    // 初始化缓冲区
    memset(_rxBuffer, 0, sizeof(_rxBuffer));
    memset(_txBuffer, 0, sizeof(_txBuffer));
    memset(_qrCodeString, 0, sizeof(_qrCodeString));
    _coordinates[0] = 0;
    _coordinates[1] = 0;

    // 设置单例指针
    _instance = this;
    g_jetson  = this;
}

/**
 * @brief 初始化Jetson通信
 */
void JetsonCommunicator::init()
{
    // 使用UART驱动初始化
    UART_Init(_huart, receiveCallback, JETSON_FRAME_SIZE);

    // 等待初始化完成
    HAL_Delay(10);
}

/**
 * @brief 发送区域到达通知
 * @param zone 区域标识 (参见宏定义)
 */
void JetsonCommunicator::sendZoneReached(uint8_t zone)
{
    // 组装发送帧
    memset(_txBuffer, 0, sizeof(_txBuffer));

    // 设置两个连续的0xFF作为帧头
    _txBuffer[0] = JETSON_START_BYTE;   // 第一个帧头字节
    _txBuffer[1] = JETSON_START_BYTE;   // 第二个帧头字节
    _txBuffer[2] = JETSON_CMD_SEND;     // 任务B命令

    // 填充区域标识 (注意偏移量变化)
    for (int i = 3; i <= 8; i++)
    {
        _txBuffer[i] = zone;
    }

    _txBuffer[9] = JETSON_END_BYTE;   // 帧尾

    // 发送数据
    UART_Send_Data(_huart, _txBuffer, JETSON_FRAME_SIZE);

    // 等待发送完成
    HAL_Delay(1);
}

/**
 * @brief 获取二维码字符串
 */
const char* JetsonCommunicator::getQRCodeString() const
{
    return _qrCodeString;
}

/**
 * @brief 获取坐标数据
 */
void JetsonCommunicator::getCoordinates(int16_t& x, int16_t& y) const
{
    x = _coordinates[0];
    y = _coordinates[1];
}

/**
 * @brief 获取抓取状态
 */
bool JetsonCommunicator::canGrab() const
{
    return _canGrab;
}

/**
 * @brief 更新抓取等待状态
 */
void JetsonCommunicator::setWaitGrab(bool isWaiting)
{
    _isWaitingGrab = isWaiting;
    _canGrab       = false;   //
    Vofa_FireWater("反转抓取状态\r\n");
}

/**
 * @brief 接收回调处理函数
 */
void JetsonCommunicator::receiveCallback(uint8_t* buffer, uint16_t size)
{
    // 确保实例存在
    if (_instance == nullptr)
    {
        return;
    }

    // 逐字节处理接收到的数据
    for (uint16_t i = 0; i < size; i++)
    {
        _instance->processReceivedData(buffer[i]);
    }
}

/**
 * @brief 处理接收到的数据字节
 */
void JetsonCommunicator::processReceivedData(uint8_t byte)
{
    // 检测双字节帧头
    if (byte == JETSON_START_BYTE)
    {
        if (_lastByteWasFF)
        {
            // 检测到两个连续的0xFF，说明这是一个新帧的开始
            _rxCount       = 2;   // 已经接收了两个字节的帧头
            _rxBuffer[0]   = JETSON_START_BYTE;
            _rxBuffer[1]   = JETSON_START_BYTE;
            _lastByteWasFF = false;   // 重置标志位
            return;
        }
        else
        {
            // 检测到第一个0xFF
            _lastByteWasFF = true;
            return;
        }
    }
    else
    {
        // 不是0xFF，重置连续0xFF检测
        _lastByteWasFF = false;
    }

    // 如果已经开始接收帧，则保存字节到缓冲区
    if (_rxCount >= 2 && _rxCount < JETSON_FRAME_SIZE)
    {
        _rxBuffer[_rxCount++] = byte;
    }

    // 当接收完整个帧时进行验证和处理
    if (_rxCount == JETSON_FRAME_SIZE)
    {
        // 验证帧格式(双字节帧头和结束字节)
        if (_rxBuffer[0] == JETSON_START_BYTE && _rxBuffer[1] == JETSON_START_BYTE &&
            _rxBuffer[JETSON_FRAME_SIZE - 1] == JETSON_END_BYTE)
        {
            // 帧有效，标记为已更新 - 在主循环中处理
            _qrCodeUpdated = true;
        }

        // 无论帧是否有效，都重置计数器准备接收下一帧
        _rxCount = 0;
    }
}
/**
 * @brief 处理完整数据帧
 */
void JetsonCommunicator::processDataFrame()
{
    // 打印接收到的完整数据包
    Vofa_FireWater("接收数据包: [");
    for (int i = 0; i < JETSON_FRAME_SIZE; i++)
    {
        Vofa_FireWater("0x%02X ", _rxBuffer[i]);
    }
    Vofa_FireWater("]\r\n");

    // 继续处理命令
    Vofa_FireWater("接收命令: [0x%02X]\r\n", _rxBuffer[2]);

    // 根据命令类型分发处理 (注意: 命令字节现在位于索引2)
    switch (_rxBuffer[2])
    {
    case JETSON_CMD_QR:   // 二维码任务
        processQRCode();
        break;

    case JETSON_CMD_COORD:   // 坐标任务
        processCoordinates();
        break;

    case JETSON_CMD_GRAB:   // 抓取任务
        processGrabCommand();
        break;

    default:
        // 无效命令，向显示器输出错误信息
        TJC_Send_Format("t10.txt=\"Jserror\"\xff\xff\xff");
        break;
    }
}

/**
 * @brief 处理二维码数据
 */
void JetsonCommunicator::processQRCode()
{
    char temp[4];   // 临时存储转换后的字符串

    // 前三个数字 (注意索引偏移+1)
    for (int i = 0; i < 3; i++)
    {
        sprintf(temp, "%d", _rxBuffer[i + 3]);
        _qrCodeString[i] = temp[0];
    }

    // 添加加号
    _qrCodeString[3] = '+';

    // 后三个数字 (注意索引偏移+1)
    for (int i = 0; i < 3; i++)
    {
        sprintf(temp, "%d", _rxBuffer[i + 6]);
        _qrCodeString[i + 4] = temp[0];
    }

    _qrCodeString[7] = '\0';   // 字符串结束符

    // 发送到串口显示
    TJC_Send_Format("t10.txt=\"%s\"\xff\xff\xff", _qrCodeString);
}

void JetsonCommunicator::updateDisplay()
{
    if (_qrCodeUpdated)
    {
        // 处理接收到的数据帧
        processDataFrame();
        _qrCodeUpdated = false;
    }
}

/**
 * @brief 处理坐标数据
 */
void JetsonCommunicator::processCoordinates()
{
    // 修改为十进制解析 - X坐标 (注意索引偏移+1)
    int16_t x_value = (_rxBuffer[4] * 10) + _rxBuffer[5];
    _coordinates[0] = (_rxBuffer[3] == 0x0A) ? x_value : -x_value;

    // 修改为十进制解析 - Y坐标 (注意索引偏移+1)
    int16_t y_value = (_rxBuffer[7] * 10) + _rxBuffer[8];
    _coordinates[1] = (_rxBuffer[6] == 0x0A) ? y_value : -y_value;
}

/**
 * @brief 处理抓取命令
 */
void JetsonCommunicator::processGrabCommand()
{
    // 处理抓取命令 (注意索引偏移+1)
    if (_rxBuffer[3] == 0x02)
    {
        // 可以抓取，更新状态
        _canGrab = true;
    }
    else
    {
        // 其他情况，检测异常
        _canGrab = false;
    }
}