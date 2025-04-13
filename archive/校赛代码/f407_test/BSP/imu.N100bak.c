#include "imu.h"
float Degree;
float Last_Degree;
//�ṹ�嶨��
MSG_EULER_ORIEN MEO_Struct;

//���ڿ����жϻص�����
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if (huart->Instance == USART2)
    {
        if (Size == MSG_EULER_ORIEN_LEN) //�ӳ����ж��Ƿ�ΪĿ�����ݰ�
        {
            for (char i = 0; i < MSG_EULER_ORIEN_LEN; i++)
            {
                //printf("write origin imu data! \r\n");
                MEO_Data[i] = MSG_EULER_ORIEN_Buf[i];//д��ԭʼ���ݴ�������
            }

            if (Data_Check(MEO_Data, Msg_Euler_Orien, MSG_EULER_ORIEN_LEN))
            {
                //printf(" imu data process successful!\r\n");
                MEO_PacketDec(MEO_Data, &MEO_Struct);//���봢��
                //printf("Yaw Angle: %f\n", MEO_Struct.Heading);
            }
        }
    }

    HAL_UARTEx_ReceiveToIdle_DMA(&huart2, (uint8_t *)MSG_EULER_ORIEN_Buf, sizeof(MSG_EULER_ORIEN_Buf)); //����ʹ�ܴ���DMA���տ����ж�
}

//ͨ�����ݰ�ͷ��β��ID��������Ƿ���ȷ
//����ֵ����ȷ1������0
int8_t Data_Check(uint8_t * Rx_Buffer, uint8_t ID, uint8_t Length)
{
    if (Rx_Buffer[0] == 0xFC&&Rx_Buffer[Length -1] == 0xFD&&Rx_Buffer[1] == ID)
        return 1;
    else
        return 0;
}

/**********************************
ʵ��16���Ƶ�����ת���ɸ���������
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

    sign = (transition_32 & 0x80000000) ? -1 : 1;//����λ

    //�����Ʋ������ٰ�λ����㣬���������30��23λ��Ӧ��e
    exponent = ((transition_32 >> 23) & 0xff) - 127;

    //��22~0ת��Ϊ10���ƣ��õ���Ӧ��xϵ��
    mantissa = 1 + ((float)(transition_32 & 0x7fffff) / 0x7fffff);

    tmp=sign * mantissa * pow(2, exponent);

    return tmp;
}//�˺�����ȫ��ֲ������

//����ת�Ƕ�
float RadtoDegree(float rads)
{
    // ������ת��Ϊ����
    float Degree = rads * 180.0f / Pi;

//    // �����������
//    if (Degree - Last_Degree > 180)
//    {
//        Degree -= 360;
//    }
//    else if (Degree - Last_Degree < -180)
//    {
//        Degree += 360;
//    }

//    // ������һ�ζ���
//    Last_Degree = Degree;

    // ���ƫ������
    //Degree += Yaw_Compensate;

    // ���ش����Ķ���
    return Degree;
}

//���ݽ���
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
