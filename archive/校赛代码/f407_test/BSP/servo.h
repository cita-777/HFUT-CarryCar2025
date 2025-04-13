#ifndef __SERVO_H__
#define __SERVO_H__

#include "fashion_star_uart_servo.h"

// 变量定义
extern Usart_DataTypeDef *servo_usart;

//某个舵机转动多少角度
void Servo_Control(uint8_t servo_id,float angle);

void Crawl_Close(void);
void Crawl_Open(void);
void Forward(void);
void Backward(void);
void Location_First(void);
void Location_Second(void);
void Location_Third(void);
void Servo_Init(void);
void test_servo(void);

//******************************以下为例程函数**************************************

/* 舵机通讯检测 */
void FSUSExample_PingServo(void);

/* 控制单个舵机的角度 */
void FSUSExample_SetServoAngle(void);

/* 控制多个舵机的角度 */
void FSUSExample_SetNServoAngle(void);

/* 设置舵机角度(多圈模式) */
void FSUSExample_SetServoAngleMTurn(void);

/* 舵机阻尼模式与角度回传 */
void FSUSExample_SetServoDamping(void);

/* 轮式模式控制(一直旋转) */
void FSUSExample_WheelKeepMove(void);

/* 估计旋转圈数所需要花费的时间 */
uint16_t FSUSExample_EstimateTimeMs(uint16_t n_circle, uint16_t speed);

/* 轮式模式控制(定圈) */
void FSUSExample_WheelMoveNCircle(void);

/* 轮式控制模式(定时) */
void FSUSExample_WheelMoveNTime(void);

/* 用户自定义数据-重置 */
void FSUSExample_ResetData(void);

/* 用户自定义数据-读取 */
void FSUSExample_ReadData(void);

/* 用户自定义数据-写入 */
void FSUSExample_WriteData(void);



#endif 
