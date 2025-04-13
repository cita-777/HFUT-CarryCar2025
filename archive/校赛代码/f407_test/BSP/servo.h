#ifndef __SERVO_H__
#define __SERVO_H__

#include "fashion_star_uart_servo.h"

// ��������
extern Usart_DataTypeDef *servo_usart;

//ĳ�����ת�����ٽǶ�
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

//******************************����Ϊ���̺���**************************************

/* ���ͨѶ��� */
void FSUSExample_PingServo(void);

/* ���Ƶ�������ĽǶ� */
void FSUSExample_SetServoAngle(void);

/* ���ƶ������ĽǶ� */
void FSUSExample_SetNServoAngle(void);

/* ���ö���Ƕ�(��Ȧģʽ) */
void FSUSExample_SetServoAngleMTurn(void);

/* �������ģʽ��ǶȻش� */
void FSUSExample_SetServoDamping(void);

/* ��ʽģʽ����(һֱ��ת) */
void FSUSExample_WheelKeepMove(void);

/* ������תȦ������Ҫ���ѵ�ʱ�� */
uint16_t FSUSExample_EstimateTimeMs(uint16_t n_circle, uint16_t speed);

/* ��ʽģʽ����(��Ȧ) */
void FSUSExample_WheelMoveNCircle(void);

/* ��ʽ����ģʽ(��ʱ) */
void FSUSExample_WheelMoveNTime(void);

/* �û��Զ�������-���� */
void FSUSExample_ResetData(void);

/* �û��Զ�������-��ȡ */
void FSUSExample_ReadData(void);

/* �û��Զ�������-д�� */
void FSUSExample_WriteData(void);



#endif 
