/*
 * 用来处理舵机的底层通信协议
 *--------------------------
 * 作者: 深圳市华馨京科技有限公司
 * 网站：https://fashionrobo.com/
 * 更新时间: 2024/08/05
 * STM32适配: cita
 */
#include "FashionStar_UartServoProtocol.h"
#include "2_Device/Vofa/dvc_vofa.h"

// 全局变量代替静态成员
FSUS_Protocol* g_activeProtocolInstance = nullptr;

extern "C" {
// UART接收数据回调函数
void FSUS_UART_Callback(uint8_t* Buffer, uint16_t Length)
{
    static int callback_count = 0;
    callback_count++;
    // Vofa_FireWater("UART收到数据: %d字节, 共%d次\r\n", Length, callback_count);

    if (g_activeProtocolInstance != nullptr)
    {
        g_activeProtocolInstance->onReceiveData(Buffer, Length);
    }
}
}

FSUS_Protocol::FSUS_Protocol(UART_HandleTypeDef* huart, uint32_t baudrate)
{
    // 增加基本调试输出，确认构造函数被调用
    Vofa_FireWater("舵机构造函数被调用\r\n");

    // 记录参数
    this->huart    = huart;
    this->baudrate = baudrate;

    // 将当前实例设置为活动实例
    g_activeProtocolInstance = this;

    // 初始化串口
    UART_Init(huart, FSUS_UART_Callback, UART_BUFFER_SIZE);
}

// 获取当前的时间戳，单位ms
uint32_t FSUS_Protocol::millis()
{
    return HAL_GetTick();
}

void FSUS_Protocol::delay_ms(uint32_t microsecond)
{
    HAL_Delay(microsecond);
}

// 通过串口发送一个字节
void FSUS_Protocol::write(uint8_t value)
{
    tmp_buffer[0] = value;
    UART_Send_Data(huart, tmp_buffer, 1);
}

// 读取单个字节
bool FSUS_Protocol::read(uint8_t* value)
{
    if (recv_queue.empty())
    {
        return false;
    }

    *value = recv_queue.front();
    recv_queue.pop();
    return true;
}

// 读入所有
int FSUS_Protocol::readAll(uint8_t* buffer)
{
    int count = 0;
    while (!recv_queue.empty() && count < FSUS_PACK_RESPONSE_MAX_SIZE)
    {
        buffer[count++] = recv_queue.front();
        recv_queue.pop();
    }
    return count;
}

// 串口当前是否有数据读入
int FSUS_Protocol::available()
{
    return recv_queue.size();
}

// 接收来自UART的数据
void FSUS_Protocol::onReceiveData(uint8_t* data, uint16_t length)
{
    for (uint16_t i = 0; i < length; i++)
    {
        recv_queue.push(data[i]);
    }
}

// 加工并发送请求数据包
void FSUS_Protocol::sendPack()
{
    // 数据加工
    requestPack.header = FSUS_PACK_REQUEST_HEADER;   // 数据头
    // 计算校验码
    requestPack.checksum = calcPackChecksum(&requestPack);

    // 通过串口发送出去
    uint8_t buffer[FSUS_PACK_RESPONSE_MAX_SIZE + 5];   // 头(2) + ID(1) + 长度(1) + 内容 + 校验(1)
    int     idx = 0;

    buffer[idx++] = requestPack.header & 0xFF;
    buffer[idx++] = requestPack.header >> 8;
    buffer[idx++] = requestPack.cmdId;
    buffer[idx++] = requestPack.content_size;

    for (uint16_t i = 0; i < requestPack.content_size; i++)
    {
        buffer[idx++] = requestPack.content[i];
    }

    buffer[idx++] = requestPack.checksum;

    // 使用drv_uart发送完整数据包
    UART_Send_Data(huart, buffer, idx);
}

// 清空缓冲区
void FSUS_Protocol::emptyCache()
{
    // 清空队列(通过置换)
    std::queue<uint8_t> empty;
    swap(empty, recv_queue);
}

// 初始化响应数据包的数据结构
void FSUS_Protocol::initResponsePack()
{
    // 初始化响应包的数据
    responsePack.header       = 0;
    responsePack.cmdId        = 0;
    responsePack.content_size = 0;
    responsePack.checksum     = 0;
    responsePack.recv_cnt     = 0;
}

// 接收响应包
FSUS_STATUS FSUS_Protocol::recvPack()
{
    uint32_t start_time    = millis();
    uint32_t last_log_time = start_time;

    responsePack.recv_cnt = 0;   // 数据帧接收标志位

    // Vofa_FireWater("开始接收数据包...\r\n");

    while (true)
    {
        uint32_t current_time = millis();

        // // 每500ms输出一次等待状态
        // if (current_time - last_log_time > 1000)
        // {
        //     Vofa_FireWater("等待数据中... 已等待%dms, 接收队列大小:%d\r\n", current_time - start_time, available());
        //     last_log_time = current_time;
        // }

        // // 超时判断
        // if ((current_time - start_time) > FSUS_TIMEOUT_MS)
        // {
        //     Vofa_FireWater("接收超时! 已等待%dms\r\n", FSUS_TIMEOUT_MS);
        //     return FSUS_STATUS_TIMEOUT;
        // }

        // 超时判断
        if ((current_time - start_time) > FSUS_TIMEOUT_MS)
        {
            Vofa_FireWater("接收超时! 已等待%dms\r\n", FSUS_TIMEOUT_MS);
            return FSUS_STATUS_TIMEOUT;
        }
        uint8_t curByte;
        read(&curByte);

        // 打印接收到的每个字节
        // Vofa_FireWater("接收字节: 0x%02X, 位置:%d\r\n", curByte, responsePack.recv_cnt);

        uint8_t curIdx                   = responsePack.recv_cnt;
        responsePack.recv_buffer[curIdx] = curByte;   // 接收一个字节
        responsePack.recv_cnt += 1;                   // 计数自增

        // 数据接收是否完成
        if (curIdx == 1)
        {
            // 校验帧头
            responsePack.header = responsePack.recv_buffer[curIdx - 1] | curByte << 8;
            if (responsePack.header != FSUS_PACK_RESPONSE_HEADER)
            {
                return FSUS_STATUS_WRONG_RESPONSE_HEADER;
            }
        }
        else if (curIdx == 2)
        {
            // 载入cmdId
            responsePack.cmdId = curByte;
            // 检查cmdId是否满足指令范围
            if (responsePack.cmdId > FSUS_CMD_NUM)
            {
                return FSUS_STATUS_UNKOWN_CMD_ID;
            }
        }
        else if (curIdx == 3)
        {
            // 载入Size
            responsePack.content_size = curByte;
            // 判断size是否合法
            if (responsePack.content_size > FSUS_PACK_RESPONSE_MAX_SIZE)
            {
                return FSUS_STATUS_SIZE_TOO_BIG;
            }
        }
        else if (curIdx < 4 + responsePack.content_size)
        {
            // 填充内容
            responsePack.content[curIdx - 4] = curByte;
        }
        else
        {
            // 接收校验合
            responsePack.checksum = curByte;
            // 检查校验和是否匹配
            FSUS_CHECKSUM_T checksum = calcPackChecksum(&responsePack);
            // if (responsePack.cmdId == FSUS_CMD_QUERY_ANGLE){
            //     checksum -= 0x03;// TODO Delete 不知道为什么要这样
            // }

            if (checksum != responsePack.checksum)
            {
                return FSUS_STATUS_CHECKSUM_ERROR;
            }
            else
            {
                return FSUS_STATUS_SUCCESS;
            }
        }
    }
}

// 计算CRC校验码
FSUS_CHECKSUM_T FSUS_Protocol::calcPackChecksum(const FSUS_PACKAGE_T* package)
{
    // uint16_t checksum = 0;
    uint16_t checksum = 0;
    checksum += (package->header & 0xFF);
    checksum += (package->header >> 8);
    checksum += package->cmdId;
    checksum += package->content_size;
    for (uint16_t i = 0; i < package->content_size; i++)
    {
        checksum += package->content[i];
    }

    return (FSUS_CHECKSUM_T)(checksum % 256);
}
// 获取请求内容的尺寸
FSUS_PACKAGE_SIZE_T FSUS_Protocol::getPackSize(const FSUS_PACKAGE_T* package)
{
    // 包头(2 uint8_t) + 指令ID(1byte) + 长度(1byte) + 内容 + 校验码(1byte)
    return package->content_size + 5;
}

// 发送PING的请求包
void FSUS_Protocol::sendPing(FSUS_SERVO_ID_T servoId)
{
    // 清空接收缓冲区
    emptyCache();

    // Vofa_FireWater("发送ping到舵机ID:%d\r\n", servoId);
    requestPack.cmdId        = FSUS_CMD_PING;
    requestPack.content_size = 1;
    requestPack.content[0]   = servoId;
    sendPack();   // 发送包
}

// 接收PING的响应包
FSUS_STATUS FSUS_Protocol::recvPing(FSUS_SERVO_ID_T* servoId, bool* isOnline)
{
    // 接收数据帧
    FSUS_STATUS status = recvPack();

    *servoId                 = responsePack.content[0];   // 提取舵机ID
    *isOnline                = (status == FSUS_STATUS_SUCCESS);
    responsePack.recv_status = status;
    // Vofa_FireWater("ping响应解析: 状态=%d, 舵机ID=%d, 校验和=0x%02X\r\n", status, *servoId, responsePack.checksum);
    //  emptyCache();
    return status;
}

// 发送旋转的请求包
void FSUS_Protocol::sendSetAngle(FSUS_SERVO_ID_T servoId, FSUS_SERVO_ANGLE_T angle, FSUS_INTERVAL_T interval,
                                 FSUS_POWER_T power)
{
    requestPack.cmdId        = FSUS_CMD_SET_ANGLE;   // 指令ID
    requestPack.content_size = 7;                    // 内容长度
    requestPack.content[0]   = servoId;              // 舵机ID
    int16_t angle_int        = angle * 10;           // 舵机的角度
    requestPack.content[1]   = angle_int & 0xFF;
    requestPack.content[2]   = angle_int >> 8;
    requestPack.content[3]   = interval & 0xFF;   // 周期
    requestPack.content[4]   = interval >> 8;
    requestPack.content[5]   = power & 0xFF;   // 功率
    requestPack.content[6]   = power >> 8;
    sendPack();
}

// 发送旋转的请求包(指定周期)
void FSUS_Protocol::sendSetAngleByInterval(FSUS_SERVO_ID_T servoId, FSUS_SERVO_ANGLE_T angle, FSUS_INTERVAL_T interval,
                                           FSUS_INTERVAL_T t_acc, FSUS_INTERVAL_T t_dec, FSUS_POWER_T power)
{
    requestPack.cmdId        = FSUS_CMD_SET_ANGLE_BY_INTERVAL;   // 指令ID
    requestPack.content_size = 11;                               // 内容长度
    requestPack.content[0]   = servoId;                          // 舵机ID
    int16_t angle_int        = angle * 10;                       // 舵机的角度
    requestPack.content[1]   = angle_int & 0xFF;
    requestPack.content[2]   = angle_int >> 8;
    requestPack.content[3]   = interval & 0xFF;   // 周期
    requestPack.content[4]   = interval >> 8;
    requestPack.content[5]   = t_acc & 0xFF;
    requestPack.content[6]   = t_acc >> 8;
    requestPack.content[7]   = t_dec & 0xFF;
    requestPack.content[8]   = t_dec >> 8;
    requestPack.content[9]   = power & 0xFF;   // 功率
    requestPack.content[10]  = power >> 8;
    sendPack();
}

// 发送旋转的请求包(指定转速)
void FSUS_Protocol::sendSetAngleByVelocity(FSUS_SERVO_ID_T servoId, FSUS_SERVO_ANGLE_T angle,
                                           FSUS_SERVO_SPEED_T velocity, FSUS_INTERVAL_T t_acc, FSUS_INTERVAL_T t_dec,
                                           FSUS_POWER_T power)
{
    requestPack.cmdId        = FSUS_CMD_SET_ANGLE_BY_VELOCITY;   // 指令ID
    requestPack.content_size = 11;                               // 内容长度
    requestPack.content[0]   = servoId;                          // 舵机ID
    int16_t  angle_int       = angle * 10;                       // 舵机的角度
    uint16_t velocity_int    = velocity * 10;                    // 转速
    requestPack.content[1]   = angle_int & 0xFF;
    requestPack.content[2]   = angle_int >> 8;
    requestPack.content[3]   = velocity_int & 0xFF;   // 周期
    requestPack.content[4]   = velocity_int >> 8;
    requestPack.content[5]   = t_acc & 0xFF;
    requestPack.content[6]   = t_acc >> 8;
    requestPack.content[7]   = t_dec & 0xFF;
    requestPack.content[8]   = t_dec >> 8;
    requestPack.content[9]   = power & 0xFF;   // 功率
    requestPack.content[10]  = power >> 8;
    sendPack();
}

// 发送舵机角度查询指令
void FSUS_Protocol::sendQueryAngle(FSUS_SERVO_ID_T servoId)
{
    requestPack.cmdId        = FSUS_CMD_QUERY_ANGLE;
    requestPack.content_size = 1;
    requestPack.content[0]   = servoId;
    sendPack();
}

// 接收角度查询指令
FSUS_STATUS FSUS_Protocol::recvQueryAngle(FSUS_SERVO_ID_T* servoId, FSUS_SERVO_ANGLE_T* angle)
{
    FSUS_STATUS status = recvPack();

    // 初始化为默认值，避免使用未赋值变量
    *servoId = 0;
    *angle   = 0.0f;

    if (status == FSUS_STATUS_SUCCESS || status == FSUS_STATUS_CHECKSUM_ERROR)
    {
        // 提取舵机ID
        *servoId = responsePack.content[0];

        // 打印接收到的原始字节，用于调试
        Vofa_FireWater("角度查询: 接收字节 [0x%02X, 0x%02X]\r\n", responsePack.content[1], responsePack.content[2]);

        // 使用明确的位运算构建16位整数，避免指针操作带来的风险
        int16_t angleVal = (int16_t)(responsePack.content[1] | ((int16_t)responsePack.content[2] << 8));

        // 检查数值是否合理，避免异常值
        if (angleVal > -3600 && angleVal < 3600)
        {   // 合理范围 ±360度(放大10倍)
            *angle = 0.1f * (float)angleVal;
            Vofa_FireWater("角度查询: 原始值=%d, 角度=%.1f度\r\n", angleVal, *angle);
        }
        else
        {
            // 值超出范围，可能是通信错误
            Vofa_FireWater("角度查询: 接收到异常值 %d\r\n", angleVal);
            *angle = 0.0f;   // 设置默认值
        }
    }
    else
    {
        Vofa_FireWater("角度查询失败，状态=%d\r\n", status);
    }

    responsePack.recv_status = status;
    return status;
}

// 发送旋转的请求包(多圈)
void FSUS_Protocol::sendSetAngleMTurn(FSUS_SERVO_ID_T servoId, FSUS_SERVO_ANGLE_T angle, FSUS_INTERVAL_T_MTURN interval,
                                      FSUS_POWER_T power)
{
    requestPack.cmdId        = FSUS_CMD_SET_ANGLE_MTURN;   // 指令ID
    requestPack.content_size = 11;                         // 内容长度
    requestPack.content[0]   = servoId;                    // 舵机ID
    int32_t angle_long       = angle * 10;                 // 舵机的角度
    // 角度
    requestPack.content[1] = angle_long & 0xFF;
    requestPack.content[2] = (angle_long >> 8) & 0xFF;
    requestPack.content[3] = (angle_long >> 16) & 0xFF;
    requestPack.content[4] = (angle_long >> 24) & 0xFF;
    // 周期
    requestPack.content[5] = interval & 0xFF;
    requestPack.content[6] = (interval >> 8) & 0xFF;
    requestPack.content[7] = (interval >> 16) & 0xFF;
    requestPack.content[8] = (interval >> 24) & 0xFF;
    // 功率
    requestPack.content[9]  = power & 0xFF;
    requestPack.content[10] = (power >> 8) & 0xFF;
    sendPack();
}

// 发送旋转的请求包(多圈+指定周期)
void FSUS_Protocol::sendSetAngleMTurnByInterval(FSUS_SERVO_ID_T servoId, FSUS_SERVO_ANGLE_T angle,
                                                FSUS_INTERVAL_T_MTURN interval, FSUS_INTERVAL_T t_acc,
                                                FSUS_INTERVAL_T t_dec, FSUS_POWER_T power)
{

    requestPack.cmdId        = FSUS_CMD_SET_ANGLE_MTURN_BY_INTERVAL;   // 指令ID
    requestPack.content_size = 15;                                     // 内容长度
    requestPack.content[0]   = servoId;                                // 舵机ID
    int32_t angle_long       = angle * 10;                             // 舵机的角度
    // 角度
    requestPack.content[1] = angle_long & 0xFF;
    requestPack.content[2] = (angle_long >> 8) & 0xFF;
    requestPack.content[3] = (angle_long >> 16) & 0xFF;
    requestPack.content[4] = (angle_long >> 24) & 0xFF;
    // 周期
    requestPack.content[5] = interval & 0xFF;
    requestPack.content[6] = (interval >> 8) & 0xFF;
    requestPack.content[7] = (interval >> 16) & 0xFF;
    requestPack.content[8] = (interval >> 24) & 0xFF;
    // 加速时间
    requestPack.content[9]  = t_acc & 0xFF;
    requestPack.content[10] = (t_acc >> 8) & 0xFF;
    // 减速时间
    requestPack.content[11] = t_dec & 0xFF;
    requestPack.content[12] = (t_dec >> 8) & 0xFF;
    // 功率
    requestPack.content[13] = power & 0xFF;
    requestPack.content[14] = (power >> 8) & 0xFF;
    sendPack();
}

// 发送旋转的请求包(多圈+指定转速)
void FSUS_Protocol::sendSetAngleMTurnByVelocity(FSUS_SERVO_ID_T servoId, FSUS_SERVO_ANGLE_T angle,
                                                FSUS_SERVO_SPEED_T velocity, FSUS_INTERVAL_T t_acc,
                                                FSUS_INTERVAL_T t_dec, FSUS_POWER_T power)
{
    // 请求头
    requestPack.cmdId        = FSUS_CMD_SET_ANGLE_MTURN_BY_VELOCITY;   // 指令ID
    requestPack.content_size = 13;                                     // 内容长度
    requestPack.content[0]   = servoId;                                // 舵机ID
    int32_t  angle_long      = angle * 10;                             // 舵机的角度
    uint16_t velocity_int    = velocity * 10;                          // 舵机转速 单位0.1°/s
    // 角度
    requestPack.content[1] = angle_long & 0xFF;
    requestPack.content[2] = (angle_long >> 8) & 0xFF;
    requestPack.content[3] = (angle_long >> 16) & 0xFF;
    requestPack.content[4] = (angle_long >> 24) & 0xFF;
    // 周期
    requestPack.content[5] = velocity_int & 0xFF;
    requestPack.content[6] = (velocity_int >> 8) & 0xFF;
    // 加速时间
    requestPack.content[7] = t_acc & 0xFF;
    requestPack.content[8] = (t_acc >> 8) & 0xFF;
    // 减速时间
    requestPack.content[9]  = t_dec & 0xFF;
    requestPack.content[10] = (t_dec >> 8) & 0xFF;
    // 功率
    requestPack.content[11] = power & 0xFF;
    requestPack.content[12] = (power >> 8) & 0xFF;
    sendPack();
}

// 发送舵机角度查询指令
void FSUS_Protocol::sendQueryAngleMTurn(FSUS_SERVO_ID_T servoId)
{
    requestPack.cmdId        = FSUS_CMD_QUERY_ANGLE_MTURN;
    requestPack.content_size = 1;
    requestPack.content[0]   = servoId;
    sendPack();
}

// 接收角度查询指令(多圈模式)
FSUS_STATUS FSUS_Protocol::recvQueryAngleMTurn(FSUS_SERVO_ID_T* servoId, FSUS_SERVO_ANGLE_T* angle)
{
    FSUS_STATUS status = recvPack();
    int32_t     angleVal;
    uint8_t*    angleValPtr = (uint8_t*)&angleVal;

    // 偶尔会出现校验和错误的情况, 临时允许
    if (status == FSUS_STATUS_SUCCESS || status == FSUS_STATUS_CHECKSUM_ERROR)
    {
        (*servoId) = responsePack.content[0];

        angleValPtr[0] = responsePack.content[1];
        angleValPtr[1] = responsePack.content[2];
        angleValPtr[2] = responsePack.content[3];
        angleValPtr[3] = responsePack.content[4];

        (*angle) = 0.1 * angleVal;
    }
    responsePack.recv_status = status;
    return status;
}

// 发送阻尼模式
void FSUS_Protocol::sendDamping(FSUS_SERVO_ID_T servoId, FSUS_POWER_T power)
{
    requestPack.cmdId        = FSUS_CMD_DAMPING;
    requestPack.content_size = 3;
    requestPack.content[0]   = servoId;
    requestPack.content[1]   = power & 0xFF;
    requestPack.content[2]   = power >> 8;
    sendPack();
}

// 轮式控制模式
void FSUS_Protocol::sendWheelMove(FSUS_SERVO_ID_T servoId, uint8_t method, uint16_t speed, uint16_t value)
{
    requestPack.cmdId        = FSUS_CMD_WHEEL;
    requestPack.content_size = 6;
    requestPack.content[0]   = servoId;
    requestPack.content[1]   = method;
    requestPack.content[2]   = speed & 0xFF;
    requestPack.content[3]   = speed >> 8;
    requestPack.content[4]   = value & 0xFF;
    requestPack.content[5]   = value >> 8;
    sendPack();
}

// 轮子停止转动
void FSUS_Protocol::sendWheelStop(FSUS_SERVO_ID_T servoId)
{
    uint8_t  method = 0;
    uint16_t speed  = 0;
    uint16_t value  = 0;
    sendWheelMove(servoId, method, speed, value);
}

// 轮子持续旋转
void FSUS_Protocol::sendWheelKeepMove(FSUS_SERVO_ID_T servoId, uint8_t is_cw, uint16_t speed)
{
    uint8_t method = 0x01;   // 持续旋转
    if (is_cw)
    {
        // 顺时针旋转
        method = method | 0x80;
    }
    uint16_t value = 0;
    sendWheelMove(servoId, method, speed, value);
}

// 指定旋转的时间
void FSUS_Protocol::sendWheelMoveTime(FSUS_SERVO_ID_T servoId, uint8_t is_cw, uint16_t speed, uint16_t nTime)
{
    uint8_t method = 0x03;   // 旋转一段时间
    if (is_cw)
    {
        // 顺时针旋转
        method = method | 0x80;
    }
    sendWheelMove(servoId, method, speed, nTime);
}

// 轮式模式 旋转特定的圈数
void FSUS_Protocol::sendWheelMoveNCircle(FSUS_SERVO_ID_T servoId, uint8_t is_cw, uint16_t speed, uint16_t nCircle)
{
    uint8_t method = 0x02;   // 旋转特定的圈数
    if (is_cw)
    {
        // 顺时针旋转
        method = method | 0x80;
    }
    sendWheelMove(servoId, method, speed, nCircle);
}

// 发送重置用户数据
void FSUS_Protocol::sendResetUserData(FSUS_SERVO_ID_T servoId)
{
    requestPack.cmdId        = FSUS_CMD_RESET_USER_DATA;
    requestPack.content_size = 1;
    requestPack.content[0]   = servoId;
    sendPack();
}

// 接收重置用户数据
FSUS_STATUS FSUS_Protocol::recvResetUserData(FSUS_SERVO_ID_T* servoId, bool* result)
{
    FSUS_STATUS status = recvPack();
    if (status == FSUS_STATUS_SUCCESS)
    {
        *servoId = responsePack.content[0];
        *result  = responsePack.content[1];
    }

    return status;
}

// 发送数据读取指令
void FSUS_Protocol::sendReadData(FSUS_SERVO_ID_T servoId, uint8_t address)
{
    requestPack.cmdId        = FSUS_CMD_READ_DATA;
    requestPack.content_size = 2;
    requestPack.content[0]   = servoId;
    requestPack.content[1]   = address;
    sendPack();
}

// 接收数据读取指令
FSUS_STATUS FSUS_Protocol::recvReadData(FSUS_SERVO_ID_T* servoId, uint8_t* address, uint8_t* contentLen,
                                        uint8_t* content)
{
    FSUS_STATUS status = recvPack();
    if (status == FSUS_STATUS_SUCCESS)
    {
        *servoId    = responsePack.content[0];
        *address    = responsePack.content[1];         // 获取地址位
        *contentLen = responsePack.content_size - 2;   // 计算得到数据位的长度
        // 数据拷贝
        for (uint16_t i = 0; i < *contentLen; i++)
        {
            content[i] = responsePack.content[i + 2];
        }
    }
    return status;
}

// 发送数据写入指令
void FSUS_Protocol::sendWriteData(FSUS_SERVO_ID_T servoId, uint8_t address, uint8_t contentLen, uint8_t* content)
{
    requestPack.cmdId        = FSUS_CMD_WRITE_DATA;
    requestPack.content_size = 2 + contentLen;
    requestPack.content[0]   = servoId;
    requestPack.content[1]   = address;
    for (uint16_t i = 0; i < contentLen; i++)
    {
        requestPack.content[i + 2] = content[i];
    }
    sendPack();
}

// 接收数据写入指令
FSUS_STATUS FSUS_Protocol::recvWriteData(FSUS_SERVO_ID_T* servoId, uint8_t* address, bool* result)
{
    FSUS_STATUS status = recvPack();
    if (status == FSUS_STATUS_SUCCESS)
    {
        *servoId = responsePack.content[0];
        *address = responsePack.content[1];   // 获取地址位
        *result  = responsePack.content[2];
    }
    return status;
}

// 发送设置原点的请求包
void FSUS_Protocol::sendSetOriginPoint(FSUS_SERVO_ID_T servoId)
{
    requestPack.cmdId        = FSUS_CMD_SET_ORIGIN_POINT;
    requestPack.content_size = 2;
    requestPack.content[0]   = servoId;
    requestPack.content[1]   = 0;
    sendPack();   // 发送包
}

// 发送清除多圈圈数请求包
void FSUS_Protocol::sendResetMultiTurnAngle(FSUS_SERVO_ID_T servoId)
{
    requestPack.cmdId        = FSUS_CMD_RESET_MTURN_ANGLE;
    requestPack.content_size = 1;
    requestPack.content[0]   = servoId;
    sendPack();   // 发送包
}
