#ifndef __IMU_H
#define __IMU_H
#include "math.h"
#include "main.h"
#include "usart.h"
#include "usr_tim.h"
extern UART_HandleTypeDef huart2;
//数据包ID
enum
{
    Msg_Sys_State = 0x50,
    Msg_Euler_Orien = 0x63,
};

//解码后各数据
typedef struct
{
    uint16_t System_status;               //系统状态
    uint16_t Filter_status;               //滤波器状态
    uint32_t Unix_time;                   //UTC时间，秒数部分
    uint32_t Microseconds;                //UTC时间，微秒数部分
    double   Latitude;                    //WGS84参考系纬度
    double   Longitude;                   //WGS84参考系经度
    double   Height;                      //海拔
    float    Velocity_north;              //北向速度
    float    Velocity_east;               //东向速度
    float    Velocity_down;               //地向速度
    float    Body_acceleration_X;         //机体系X轴加速度
    float    Body_acceleration_Y;         //机体系Y轴加速度
    float    Body_acceleration_Z;         //机体系Z轴加速度
    float    G_force;                     //估计的重力加速度
    float    Roll;                        //横滚角，范围-180°~180°
    float    Pitch;                       //俯仰角，范围-90°~90°
    float    Heading;                     //偏航角，范围0 ~ 360°
    float    Angular_velocity_X;          //机体系X轴角速度
    float    Angular_velocity_Y;          //机体系Y轴角速度
    float    Angular_velocity_Z;          //机体系Z轴角速度
    float    Latitude_standard_deviation; //纬度标准差
    float    Longitude_standard_deviation;//经度标准差
    float    Height_standard_deviation;   //高度标准差
} MSG_SYS_STATE;

typedef struct
{
    float Roll;     //横滚角
    float Pitch;    //俯仰角
    float Heading;  //偏航角
} MSG_EULER_ORIEN;

//数据数组声明
extern uint8_t MSG_EULER_ORIEN_Buf[DATA_LEN(MSG_EULER_ORIEN_LEN)];

extern uint8_t MEO_Data[MSG_EULER_ORIEN_LEN];

//用户定义函数声明
float RadtoDegree(float rads);

float DATA_Trans(uint8_t Data_1, uint8_t Data_2, uint8_t Data_3, uint8_t Data_4);
int8_t Data_Check(uint8_t * Rx_Buffer, uint8_t ID, uint8_t Length);

void MEO_PacketDec(uint8_t * Rx_Buffer, MSG_EULER_ORIEN * DecodedStruct);

float GetYawAngle(void);

#endif

