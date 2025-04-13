#ifndef __IMU_H
#define __IMU_H
#include "math.h"
#include "main.h"
#include "usart.h"
#include "usr_tim.h"
extern UART_HandleTypeDef huart2;
//���ݰ�ID
enum
{
    Msg_Sys_State = 0x50,
    Msg_Euler_Orien = 0x63,
};

//����������
typedef struct
{
    uint16_t System_status;               //ϵͳ״̬
    uint16_t Filter_status;               //�˲���״̬
    uint32_t Unix_time;                   //UTCʱ�䣬��������
    uint32_t Microseconds;                //UTCʱ�䣬΢��������
    double   Latitude;                    //WGS84�ο�ϵγ��
    double   Longitude;                   //WGS84�ο�ϵ����
    double   Height;                      //����
    float    Velocity_north;              //�����ٶ�
    float    Velocity_east;               //�����ٶ�
    float    Velocity_down;               //�����ٶ�
    float    Body_acceleration_X;         //����ϵX����ٶ�
    float    Body_acceleration_Y;         //����ϵY����ٶ�
    float    Body_acceleration_Z;         //����ϵZ����ٶ�
    float    G_force;                     //���Ƶ��������ٶ�
    float    Roll;                        //����ǣ���Χ-180��~180��
    float    Pitch;                       //�����ǣ���Χ-90��~90��
    float    Heading;                     //ƫ���ǣ���Χ0 ~ 360��
    float    Angular_velocity_X;          //����ϵX����ٶ�
    float    Angular_velocity_Y;          //����ϵY����ٶ�
    float    Angular_velocity_Z;          //����ϵZ����ٶ�
    float    Latitude_standard_deviation; //γ�ȱ�׼��
    float    Longitude_standard_deviation;//���ȱ�׼��
    float    Height_standard_deviation;   //�߶ȱ�׼��
} MSG_SYS_STATE;

typedef struct
{
    float Roll;     //�����
    float Pitch;    //������
    float Heading;  //ƫ����
} MSG_EULER_ORIEN;

//������������
extern uint8_t MSG_EULER_ORIEN_Buf[DATA_LEN(MSG_EULER_ORIEN_LEN)];

extern uint8_t MEO_Data[MSG_EULER_ORIEN_LEN];

//�û����庯������
float RadtoDegree(float rads);

float DATA_Trans(uint8_t Data_1, uint8_t Data_2, uint8_t Data_3, uint8_t Data_4);
int8_t Data_Check(uint8_t * Rx_Buffer, uint8_t ID, uint8_t Length);

void MEO_PacketDec(uint8_t * Rx_Buffer, MSG_EULER_ORIEN * DecodedStruct);

float GetYawAngle(void);

#endif

