#include "imu.h"
float Degree;
float Last_Degree;
//结构体定义
MSG_EULER_ORIEN MEO_Struct;

//串口空闲中断回调函数
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if (huart->Instance == USART2)
    {
        if (Size == MSG_EULER_ORIEN_LEN) //从长度判断是否为目标数据包
        {
            for (char i = 0; i < MSG_EULER_ORIEN_LEN; i++)
            {
                //printf("write origin imu data! \r\n");
                MEO_Data[i] = MSG_EULER_ORIEN_Buf[i];//写入原始数据储存数组
            }

            if (Data_Check(MEO_Data, Msg_Euler_Orien, MSG_EULER_ORIEN_LEN))
            {
                //printf(" imu data process successful!\r\n");
                MEO_PacketDec(MEO_Data, &MEO_Struct);//解码储存
                //printf("Yaw Angle: %f\n", MEO_Struct.Heading);
            }
        }
    }

    HAL_UARTEx_ReceiveToIdle_DMA(&huart2, (uint8_t *)MSG_EULER_ORIEN_Buf, sizeof(MSG_EULER_ORIEN_Buf)); //重新使能串口DMA接收空闲中断
}

//通过数据包头、尾、ID检查数据是否正确
//返回值：正确1，错误0
int8_t Data_Check(uint8_t * Rx_Buffer, uint8_t ID, uint8_t Length)
{
    if (Rx_Buffer[0] == 0xFC&&Rx_Buffer[Length -1] == 0xFD&&Rx_Buffer[1] == ID)
        return 1;
    else
        return 0;
}

/**********************************
实现16进制的数据转换成浮点型数据
**********************************/
float DATA_Trans(uint8_t Data_1, uint8_t Data_2, uint8_t Data_3, uint8_t Data_4)
{
    long long transition_32;
    float tmp=0;
    int sign=0;
    int exponent=0;
    float mantissa=0;

    transition_32 = 0;
    transition_32 |=  Data_4<<24;
    transition_32 |=  Data_3<<16;
    transition_32 |=  Data_2<<8;
    transition_32 |=  Data_1;

    sign = (transition_32 & 0x80000000) ? -1 : 1;//符号位

    //先右移操作，再按位与计算，出来结果是30到23位对应的e
    exponent = ((transition_32 >> 23) & 0xff) - 127;

    //将22~0转化为10进制，得到对应的x系数
    mantissa = 1 + ((float)(transition_32 & 0x7fffff) / 0x7fffff);

    tmp=sign * mantissa * pow(2, exponent);

    return tmp;
}//此函数完全移植于例程

//弧度转角度
float RadtoDegree(float rads)
{
    // 将弧度转换为度数
    float Degree = rads * 180.0f / Pi;

//    // 处理度数跳变
//    if (Degree - Last_Degree > 180)
//    {
//        Degree -= 360;
//    }
//    else if (Degree - Last_Degree < -180)
//    {
//        Degree += 360;
//    }

//    // 更新上一次度数
//    Last_Degree = Degree;

    // 添加偏航补偿
    //Degree += Yaw_Compensate;

    // 返回处理后的度数
    return Degree;
}

//数据解码
void MSS_PacketDec(uint8_t * Rx_Buffer, MSG_SYS_STATE * DecodedStruct)
{

}

void MEO_PacketDec(uint8_t * Rx_Buffer, MSG_EULER_ORIEN * DecodedStruct)
{
    DecodedStruct->Roll     = RadtoDegree(DATA_Trans(Rx_Buffer[7],  Rx_Buffer[8],  Rx_Buffer[9],  Rx_Buffer[10]));
    DecodedStruct->Pitch    = RadtoDegree(DATA_Trans(Rx_Buffer[11], Rx_Buffer[12], Rx_Buffer[13], Rx_Buffer[14]));
    DecodedStruct->Heading  = RadtoDegree(DATA_Trans(Rx_Buffer[15], Rx_Buffer[16], Rx_Buffer[17], Rx_Buffer[18]));
}

// float GetYawAngle(void)
// {
//     return MEO_Struct.Heading;
// }
