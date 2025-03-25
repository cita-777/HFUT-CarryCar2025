/**
 * *****************************************************************************
 * @file        dvc_hwt101.cpp
 * @brief       HWT101惯性测量单元设备驱动
 * @author      ciat-777 (juricek.chen@gmail.com)
 * @date        2025-03-10
 * @copyright   cita
 * *****************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "dvc_hwt101.h"
#include "2_Device/Vofa/dvc_vofa.h"
/* 静态成员初始化 ----------------------------------------------------------*/
HWT101Communicator* HWT101Communicator::_instance = nullptr;
HWT101Communicator* g_hwt101                      = nullptr;

/**
 * @brief 构造函数
 */
HWT101Communicator::HWT101Communicator(UART_HandleTypeDef* huart)
    : _huart(huart)
    , _dataReadyFlag(0)
    , _yawAngle(0)
    , _rawYawAngle(0)
    , _lastYawAngle(0)
{
    // 初始化缓冲区
    memset(_rxBuffer, 0, sizeof(_rxBuffer));

    // 设置单例指针
    _instance = this;
    g_hwt101  = this;
}

/**
 * @brief 初始化HWT101设备
 * @return uint8_t 0表示成功，非0表示错误代码
 */
uint8_t HWT101Communicator::init(UART_HandleTypeDef* huart)
{
    // 保存串口句柄
    _huart = huart;

    // 初始化状态
    _dataReadyFlag = 0;
    memset(_rxBuffer, 0, sizeof(_rxBuffer));

    // 确保DMA通道已正确配置
    if (huart->hdmarx == NULL)
    {
        // DMA未配置，返回错误
        return 1;
    }

    // 初始化串口，设置回调函数
    UART_Init(huart, receiveCallback, HWT101_RX_BUFFER_SIZE);

    // 等待设备初始化
    HAL_Delay(10);

    // 校准Z轴角度
    return calibrateYaw();
}

/**
 * @brief HWT101数据接收回调函数
 * @param buffer 接收到的数据缓冲区
 * @param length 数据长度
 */
void HWT101Communicator::receiveCallback(uint8_t* buffer, uint16_t length)
{
    // 复制数据到设备缓冲区
    memcpy(_instance->_rxBuffer, buffer, length);
    _instance->_dataReadyFlag = 1;
    // Vofa_FireWater("数据帧有效, 已设置标志\r\n");
}

/**
 * @brief 处理HWT101接收到的数据
 * @return uint8_t 0表示成功，非0表示数据无效或校验失败
 */
uint8_t HWT101Communicator::processData()
{
    if (_dataReadyFlag)
    {
        // Vofa_FireWater("接收数据: ");
        //  for (int i = 0; i < HWT101_DATA_LENGTH; i++)
        //  {
        //      Vofa_FireWater("%02X ", _rxBuffer[i]);
        //  }
        // Vofa_FireWater("\r\n");
        //  计算校验和
        uint8_t sum = checkSum(_rxBuffer);
        // Vofa_FireWater("计算校验和: %02X, 接收校验和: %02X\r\n", sum, _rxBuffer[21]);
        // 校验数据有效性
        if (_rxBuffer[21] == sum)
        {
            // 提取原始偏航角数据
            _rawYawAngle = (float)((_rxBuffer[18] << 8) | _rxBuffer[17]) / 32768.0f * 180.0f;

            // 转换偏航角范围(从顺时针0-360转换为逆时针)
            _yawAngle = 360.0f - _rawYawAngle;

            // 处理边界情况
            if (_yawAngle >= 360.0f)
            {
                _yawAngle = 0.0f;
            }

            // 更新上一次角度值
            _lastYawAngle = _yawAngle;

            // 清除数据标志
            _dataReadyFlag = 0;

            return 0;
        }
        else
        {
            // 校验和错误，清除数据标志
            _dataReadyFlag = 0;
            return 1;
        }
    }

    return 2;   // 无数据可处理
}

/**
 * @brief 校准HWT101的偏航角(Z轴角度)归零
 * @return uint8_t 0表示成功，非0表示错误代码
 */
uint8_t HWT101Communicator::calibrateYaw()
{
    // Z轴校准命令
    uint8_t calibrate_cmd[] = {0xFF, 0xAA, 0x76, 0x00, 0x00};

    // 发送校准命令
    return UART_Send_Data(_huart, calibrate_cmd, sizeof(calibrate_cmd));
}

/**
 * @brief 获取当前偏航角(航向角)
 * @return float 当前偏航角，范围0-360度
 */
float HWT101Communicator::getYawAngle() const
{
    return _yawAngle;
}

/**
 * @brief 设置传感器输出模式
 * @param mode 模式值
 * @return uint8_t 0表示成功，非0表示错误代码
 */
uint8_t HWT101Communicator::setOutputMode(uint8_t mode)
{
    // 设置输出模式命令
    uint8_t set_mode_cmd[] = {0xFF, 0xAA, 0x02, mode, 0x00};

    // 发送命令
    return UART_Send_Data(_huart, set_mode_cmd, sizeof(set_mode_cmd));
}

/**
 * @brief 计算校验和
 * @param data 数据数组
 * @return uint8_t 校验和结果,0成功,1不成功
 */
uint8_t HWT101Communicator::checkSum(uint8_t* data) const
{
    // 根据HWT101协议计算校验和
    return (uint8_t)(0x55 + 0x53 + data[17] + data[18] + data[19] + data[20]);
}

/************************ COPYRIGHT(C) CITA **************************/