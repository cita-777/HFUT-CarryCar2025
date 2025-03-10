/**
 * *****************************************************************************
 * @file        jetson.cpp
 * @brief
 * @author      ciat-777 (juricek.chen@gmail.com)
 * @date        2025-03-09
 * @copyright   cita
 * *****************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "jetson.h"
#include "2_Device/tjc_screen/tjc.h"   // 导入TJC屏幕头文件，用于显示二维码
//#include "staMachine.h"                // 导入状态机头文件，用于访问is_wait_jetson_grab

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
 * @brief 发送数据到Jetson
 */
void JetsonCommunicator::send(uint8_t data)
{
    // 组装发送帧
    memset(_txBuffer, 0, sizeof(_txBuffer));
    _txBuffer[0] = JETSON_START_BYTE;
    _txBuffer[1] = JETSON_CMD_SEND;

    // 填充数据字段
    for (int i = 2; i <= 7; i++)
    {
        _txBuffer[i] = data;
    }

    _txBuffer[8] = JETSON_END_BYTE;

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
    // 如果是起始字节且当前计数不为0，重置计数
    if (byte == JETSON_START_BYTE && _rxCount != 0)
    {
        _rxCount = 0;
    }

    // 存储字节
    if (_rxCount < JETSON_FRAME_SIZE)
    {
        _rxBuffer[_rxCount++] = byte;
    }

    // 检查是否接收完整一帧
    if (_rxCount == JETSON_FRAME_SIZE)
    {
        // 验证帧格式
        if (_rxBuffer[0] == JETSON_START_BYTE && _rxBuffer[JETSON_FRAME_SIZE - 1] == JETSON_END_BYTE)
        {
            // 处理接收到的数据帧
            processDataFrame();
        }

        // 重置计数
        _rxCount = 0;
    }
}

/**
 * @brief 处理完整数据帧
 */
void JetsonCommunicator::processDataFrame()
{
    // 根据命令类型分发处理
    switch (_rxBuffer[1])
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

    // 前三个数字
    for (int i = 0; i < 3; i++)
    {
        sprintf(temp, "%d", _rxBuffer[i + 2]);
        _qrCodeString[i] = temp[0];
    }

    // 添加加号
    _qrCodeString[3] = '+';

    // 后三个数字
    for (int i = 0; i < 3; i++)
    {
        sprintf(temp, "%d", _rxBuffer[i + 5]);
        _qrCodeString[i + 4] = temp[0];
    }

    _qrCodeString[7] = '\0';   // 字符串结束符

    // 发送到串口显示
    printf("t10.txt=\"%s\"\xff\xff\xff", _qrCodeString);
    printf("t10.txt=\"%s\"\xff\xff\xff", _qrCodeString);
    printf("t10.txt=\"%s\"\xff\xff\xff", _qrCodeString);
}

/**
 * @brief 处理坐标数据
 */
void JetsonCommunicator::processCoordinates()
{
    // 解析X坐标 - 与Y坐标使用相同的十六进制解析方式
    int16_t x_value = (_rxBuffer[3] << 4) | _rxBuffer[4];
    _coordinates[0] = (_rxBuffer[2] == 0x0A) ? x_value : -x_value;

    // 解析Y坐标
    int16_t y_value = (_rxBuffer[6] << 4) | _rxBuffer[7];
    _coordinates[1] = (_rxBuffer[5] == 0x0A) ? y_value : -y_value;
}

/**
 * @brief 处理抓取命令
 */
void JetsonCommunicator::processGrabCommand()
{
    // 处理抓取命令
    if (_rxBuffer[2] == 0x02)
    {
        // 可以抓取，更新状态
        _canGrab = true;

        // 更新外部状态变量（与原代码兼容）
        //extern bool is_wait_jetson_grab;
        //is_wait_jetson_grab = 0;
    }
    else
    {
        // 其他情况，检测异常
        _canGrab = false;
    }
}