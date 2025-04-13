#include "servo.h"

/*
 * �����ŷ����ʾ������
 * ʹ��API����ʱ������Ҫѭ������ͬһ����������µ�����Ḳ�Ǿɵ����
 * ������ܿ��ƶ������ʹ��pingָ����ͨ���Ƿ���������������ʹ��FSUSExample_ReadData()���̻�ȡ�����ǰ״̬��ͨ���ǹ����ѹ���������жϵ�ѹ�쳣�������ϵ�������Դ���ɡ�
 * ---------------------------------------------------
 */

// ����һЩ�����ͷ�ļ�
#include "servo.h"
#include "user_uart.h"
#include "math.h"
#include "Delay.h"

Usart_DataTypeDef *servo_usart = &FSUS_Usart; // �������߶����Ӧ��USART

// ������ƺ���
// ���ƶ��ת��һ���Ƕ�
	//@id ������
	//@angle ����Ƕ�
	//@interval ʱ����
void Servo_Control(uint8_t servo_id,float angle){
	
	//// ���������صĲ���
	// ʱ����ms
	// ���Գ����޸����ø�С��ʱ����������500ms
	uint16_t interval = 2000;
	// ���ִ�й��� mV Ĭ��Ϊ0
	uint16_t power = 0;
	// ���ö���Ƕȵ�ʱ��, �Ƿ�Ϊ����ʽ
	// 0:���ȴ� 1:�ȴ������ת���ض���λ��;
	uint8_t wait = 0;
	// �Ƿ�Ϊ��Ȧģʽ
	// 0: ��Ȧģʽ; 1: ��Ȧģʽ;
	//uint8_t is_mturn = 0;
	
	float anglea = angle;
	uint8_t servo_ida = servo_id;

	FSUS_SetServoAngle(servo_usart, servo_ida, anglea, interval, power, wait);


	// ����ʽ�ȴ�, �ȴ���ת��Ŀ��Ƕ�
	//FSUS_Wait(servo_usart, servo_ida, anglea, is_mturn);
	
	HAL_Delay(1);
}


// ִ��ץȡ����
void Crawl_Close(){
	//// ���������صĲ���
	// ʱ����ms
	// ���Գ����޸����ø�С��ʱ����������500ms
	uint16_t interval = 200;
	// ���ִ�й��� mV Ĭ��Ϊ0
	uint16_t power = 0;
	// ���ö���Ƕȵ�ʱ��, �Ƿ�Ϊ����ʽ
	// 0:���ȴ� 1:�ȴ������ת���ض���λ��;
	uint8_t wait = 0;
	// �Ƿ�Ϊ��Ȧģʽ
	// 0: ��Ȧģʽ; 1: ��Ȧģʽ;
	//uint8_t is_mturn = 0;
	
	
	// ���ƶ����ת������һ���Ƕ�
	//FSUS_SetServoAngle(servo_usart, 1, -90.0, interval, power, wait);
	//FSUS_SetServoAngle(servo_usart, 2, -45.0, interval, power, wait);
	FSUS_SetServoAngle(servo_usart, 3, -61, interval, power, wait);
	
	// ����ʽ�ȴ�, �ȴ���ת��Ŀ��Ƕ�
	// ע��Ҫ���趨ֵ��ͬ
	//FSUS_Wait(servo_usart, 1, -90.0, is_mturn);
	//FSUS_Wait(servo_usart, 2, -45.0, is_mturn);
	//FSUS_Wait(servo_usart, 3, -60.0, is_mturn);

	// �ȴ�2s
	//HAL_Delay(500);
}


// ִ�д򿪶���
void Crawl_Open(){
		//// ���������صĲ���
	// ʱ����ms
	// ���Գ����޸����ø�С��ʱ����������500ms
	uint16_t interval = 200;
	// ���ִ�й��� mV Ĭ��Ϊ0
	uint16_t power = 0;
	// ���ö���Ƕȵ�ʱ��, �Ƿ�Ϊ����ʽ
	// 0:���ȴ� 1:�ȴ������ת���ض���λ��;
	uint8_t wait = 0;
	// �Ƿ�Ϊ��Ȧģʽ
	// 0: ��Ȧģʽ; 1: ��Ȧģʽ;
	//uint8_t is_mturn = 0;
	
	
	// ���ƶ����ת������һ���Ƕ�
	//FSUS_SetServoAngle(servo_usart, 1, 0, interval, power, wait);
	//FSUS_SetServoAngle(servo_usart, 2, 0, interval, power, wait);
	FSUS_SetServoAngle(servo_usart, 3, 5, interval, power, wait);
	
	// ����ʽ�ȴ�, �ȴ���ת��Ŀ��Ƕ�
	// ע��Ҫ���趨ֵ��ͬ
	//FSUS_Wait(servo_usart, 1, 0, is_mturn);
	//FSUS_Wait(servo_usart, 2, 0, is_mturn);
	//FSUS_Wait(servo_usart, 3, 0, is_mturn);
}

// ��ǰ����
void Forward(){
		//// ���������صĲ���
	// ʱ����ms
	// ���Գ����޸����ø�С��ʱ����������500ms
	uint16_t interval = 200;
	// ���ִ�й��� mV Ĭ��Ϊ0
	uint16_t power = 0;
	// ���ö���Ƕȵ�ʱ��, �Ƿ�Ϊ����ʽ
	// 0:���ȴ� 1:�ȴ������ת���ض���λ��;
	uint8_t wait = 0;
	// �Ƿ�Ϊ��Ȧģʽ
	// 0: ��Ȧģʽ; 1: ��Ȧģʽ;
	//uint8_t is_mturn = 0;
	
	
	// ���ƶ����ת������һ���Ƕ�
	//FSUS_SetServoAngle(servo_usart, 1, 24, interval, power, wait);
	FSUS_SetServoAngle(servo_usart, 2, -87, interval, power, wait);
}

// �����
void Backward(){
	//// ���������صĲ���
	// ʱ����ms
	// ���Գ����޸����ø�С��ʱ����������500ms
	uint16_t interval = 200;
	// ���ִ�й��� mV Ĭ��Ϊ0
	uint16_t power = 0;
	// ���ö���Ƕȵ�ʱ��, �Ƿ�Ϊ����ʽ
	// 0:���ȴ� 1:�ȴ������ת���ض���λ��;
	uint8_t wait = 0;
	// �Ƿ�Ϊ��Ȧģʽ
	// 0: ��Ȧģʽ; 1: ��Ȧģʽ;
	//uint8_t is_mturn = 0;
	
	
	// ���ƶ����ת������һ���Ƕ�
	//FSUS_SetServoAngle(servo_usart, 1, 24, interval, power, wait);
	FSUS_SetServoAngle(servo_usart, 2, 74, interval, power, wait);
	//FSUS_SetServoAngle(servo_usart, 3, 0, interval, power, wait);
	
	// ����ʽ�ȴ�, �ȴ���ת��Ŀ��Ƕ�
	// ע��Ҫ���趨ֵ��ͬ
	//FSUS_Wait(servo_usart, 1, 24, is_mturn);
	//FSUS_Wait(servo_usart, 2, 55, is_mturn);
	//FSUS_Wait(servo_usart, 3, 0, is_mturn);
}

// λ��1
void Location_First(){
	//Servo_Control(1,24);
	//// ���������صĲ���
	// ʱ����ms
	// ���Գ����޸����ø�С��ʱ����������500ms
	uint16_t interval = 200;
	// ���ִ�й��� mV Ĭ��Ϊ0
	uint16_t power = 0;
	// ���ö���Ƕȵ�ʱ��, �Ƿ�Ϊ����ʽ
	// 0:���ȴ� 1:�ȴ������ת���ض���λ��;
	uint8_t wait = 0;
	// �Ƿ�Ϊ��Ȧģʽ
	// 0: ��Ȧģʽ; 1: ��Ȧģʽ;
	//uint8_t is_mturn = 0;
	
	
	// ���ƶ����ת������һ���Ƕ�
	FSUS_SetServoAngle(servo_usart, 1, 82, interval, power, wait);
	//FSUS_SetServoAngle(servo_usart, 2, 55, interval, power, wait);
	//FSUS_SetServoAngle(servo_usart, 3, 0, interval, power, wait);
}

// λ��2
void Location_Second(){
		// ���Գ����޸����ø�С��ʱ����������500ms
	uint16_t interval = 200;
	// ���ִ�й��� mV Ĭ��Ϊ0
	uint16_t power = 0;
	// ���ö���Ƕȵ�ʱ��, �Ƿ�Ϊ����ʽ
	// 0:���ȴ� 1:�ȴ������ת���ض���λ��;
	uint8_t wait = 0;
	// �Ƿ�Ϊ��Ȧģʽ
	// 0: ��Ȧģʽ; 1: ��Ȧģʽ;
	//uint8_t is_mturn = 0;
	
	
	// ���ƶ����ת������һ���Ƕ�
	FSUS_SetServoAngle(servo_usart, 1, -33, interval, power, wait);
}

// λ��3
void Location_Third(){
			// ���Գ����޸����ø�С��ʱ����������500ms
	uint16_t interval = 200;
	// ���ִ�й��� mV Ĭ��Ϊ0
	uint16_t power = 0;
	// ���ö���Ƕȵ�ʱ��, �Ƿ�Ϊ����ʽ
	// 0:���ȴ� 1:�ȴ������ת���ض���λ��;
	uint8_t wait = 0;
	// �Ƿ�Ϊ��Ȧģʽ
	// 0: ��Ȧģʽ; 1: ��Ȧģʽ;
	//uint8_t is_mturn = 0;
	
	
	// ���ƶ����ת������һ���Ƕ�
	FSUS_SetServoAngle(servo_usart, 1, -163, interval, power, wait);
}




//******************************����Ϊ���̺���**************************************

//int fputc(int ch, FILE *f)
//{
//	HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xffff);
//	return ch;
//}

/* ���ͨѶ��� */
void FSUSExample_PingServo(void)
{

	FSUS_STATUS status_code; // ״̬��
	uint8_t servo_id = 0;	 // ���ID = 0

	printf("===Test Uart Servo Ping===r\n");
	while (1)
	{
		// ���ͨ�ż��
		status_code = FSUS_Ping(servo_usart, servo_id);
		if (status_code == FSUS_STATUS_SUCCESS)
		{
			// ������ߣ� LED1��˸(�̵�)
			printf("Servo Online \r\n");
			HAL_GPIO_WritePin(GPIOF, GPIO_PIN_9, GPIO_PIN_SET);
			HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_10);
		}
		else
		{
			// ������ߣ� LED0��˸(���)
			printf("Servo Offline,Error Code=%d \r\n", status_code);
			HAL_GPIO_WritePin(GPIOF, GPIO_PIN_10, GPIO_PIN_SET);
			HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_9);
		}
		// ��ʱ�ȴ�1s
		HAL_Delay(1000);
	}
}

/* ���Ƶ�������ĽǶ� */
void FSUSExample_SetServoAngle(void)
{
	// ���������صĲ���
	// �����ID��
	uint8_t servo_id = 0;
	// �����Ŀ��Ƕ�
	// ����Ƕ���-180�ȵ�180��֮��, ��С��λ0.1
	float angle = 0;
	// ʱ����ms
	// ���Գ����޸����ø�С��ʱ����������500ms
	uint16_t interval;
	// Ŀ��ת��
	float velocity;
	// ����ʱ��
	uint16_t t_acc;
	// ����ʱ��
	uint16_t t_dec;
	// ���ִ�й��� mV Ĭ��Ϊ0
	uint16_t power = 0;
	// ���ö���Ƕȵ�ʱ��, �Ƿ�Ϊ����ʽ
	// 0:���ȴ� 1:�ȴ������ת���ض���λ��;
	uint8_t wait = 0;
	// ��ȡ�ĽǶ�
	float angle_read;

	while (1)
	{
		printf("GOTO: 90.0f\r\n");
		// ���ƶ���Ƕ�
		angle = 45.0;
		interval = 2000;
		FSUS_SetServoAngle(servo_usart, servo_id, angle, interval, power, wait);
		FSUS_QueryServoAngle(servo_usart, servo_id, &angle_read);
		printf("Cur Angle: %.1f\r\n", angle_read);

		// �ȴ�2s
		HAL_Delay(10000);

		// ���ƶ���Ƕ� + ָ��ʱ��
		printf("GOTO+Interval: 0.0f\r\n");
		angle = 0.0f;
		interval = 1000;
		t_acc = 100;
		t_dec = 150;
		FSUS_SetServoAngleByInterval(servo_usart, servo_id, angle, interval, t_acc, t_dec, power, wait);
		FSUS_QueryServoAngle(servo_usart, servo_id, &angle_read);
		printf("Cur Angle: %.1f\r\n", angle_read);

		// �ȴ�2s
		HAL_Delay(2000);

		// ���ƶ���Ƕ� + ָ��ת��
		printf("GOTO+Velocity: -9.0f\r\n");
		angle = -90.0f;
		velocity = 200.0f;
		t_acc = 100;
		t_dec = 150;
		FSUS_SetServoAngleByVelocity(servo_usart, servo_id, angle, velocity, t_acc, t_dec, power, wait);
		FSUS_QueryServoAngle(servo_usart, servo_id, &angle_read);
		printf("Cur Angle: %.1f\r\n", angle_read);
	}
}

/* ���ƶ������ĽǶ� */
void FSUSExample_SetNServoAngle(void)
{
	//// ���������صĲ���
	// ʱ����ms
	// ���Գ����޸����ø�С��ʱ����������500ms
	uint16_t interval = 2000;
	// ���ִ�й��� mV Ĭ��Ϊ0
	uint16_t power = 0;
	// ���ö���Ƕȵ�ʱ��, �Ƿ�Ϊ����ʽ
	// 0:���ȴ� 1:�ȴ������ת���ض���λ��;
	uint8_t wait = 0;
	// �Ƿ�Ϊ��Ȧģʽ
	// 0: ��Ȧģʽ; 1: ��Ȧģʽ;
	uint8_t is_mturn = 0;
	while (1)
	{
		// ���ƶ����̨�Ƕ�
		FSUS_SetServoAngle(servo_usart, 0, 90.0, interval, power, wait);
		FSUS_SetServoAngle(servo_usart, 1, 45.0, interval, power, wait);
		// ����ʽ�ȴ�, �ȴ���ת��Ŀ��Ƕ�
		// ע��Ҫ���趨ֵ��ͬ
		FSUS_Wait(servo_usart, 0, 90.0, is_mturn);
		FSUS_Wait(servo_usart, 1, 45.0, is_mturn);

		// �ȴ�2s
		HAL_Delay(2000);

		// ���ƶ����ת������һ���Ƕ�
		FSUS_SetServoAngle(servo_usart, 0, -90.0, interval, power, wait);
		FSUS_SetServoAngle(servo_usart, 1, -45.0, interval, power, wait);
		// ����ʽ�ȴ�, �ȴ���ת��Ŀ��Ƕ�
		// ע��Ҫ���趨ֵ��ͬ
		FSUS_Wait(servo_usart, 0, -90.0, is_mturn);
		FSUS_Wait(servo_usart, 1, -45.0, is_mturn);

		// �ȴ�2s
		HAL_Delay(2000);
	}
}

/* ���ö���Ƕ�(��Ȧģʽ) */
void FSUSExample_SetServoAngleMTurn(void)
{
	//// ���������صĲ���
	// �����ID��
	uint8_t servo_id = 0;
	// �����Ŀ��Ƕ�
	// ����Ƕ���-180�ȵ�180��֮��, ��С��λ0.1
	float angle;
	uint32_t interval; // ʱ����ms
	float velocity;	   // ���ת��, ��λdps,��/s
	// ���ִ�й��� mV Ĭ��Ϊ0
	uint16_t power = 0;
	// ���ö���Ƕȵ�ʱ��, �Ƿ�Ϊ����ʽ
	// 0:���ȴ� 1:�ȴ������ת���ض���λ��;
	uint8_t wait = 1;
	// ����ʱ��(��λms)
	uint16_t t_acc;
	// ����ʱ��
	uint16_t t_dec;
	// ��ȡ�ĽǶ�
	float angle_read;

	while (1)
	{
		printf("MTurn GOTO: 720.0f\r\n");
		// ���ƶ���Ƕ�(��Ȧ)
		angle = 720.0f;
		interval = 2000;
		FSUS_SetServoAngleMTurn(servo_usart, servo_id, angle, interval, power, wait);
		FSUS_QueryServoAngleMTurn(servo_usart, servo_id, &angle_read);
		printf("Cur Angle: %.1f\r\n", angle_read);

		// �ȴ�2s
		HAL_Delay(2000);

		// ���ƶ����ת������һ���Ƕ�(��Ȧ)
		printf("MTurn GOTO: 0.0f\r\n");
		angle = 0.0;
		FSUS_SetServoAngleMTurn(servo_usart, servo_id, angle, interval, power, wait);
		FSUS_QueryServoAngleMTurn(servo_usart, servo_id, &angle_read);
		printf("Cur Angle: %.1f\r\n", angle_read);

		// �ȴ�2s
		HAL_Delay(2000);

		// ���ƶ���Ƕ�(��Ȧ+ָ������)
		printf("MTurn+Interval GOTO: -180.0f\r\n");
		angle = 180.0f;
		interval = 1000;
		t_acc = 100;
		t_dec = 200;
		FSUS_SetServoAngleMTurnByInterval(servo_usart, servo_id, angle, interval, t_acc, t_dec, power, wait);
		FSUS_QueryServoAngleMTurn(servo_usart, servo_id, &angle_read);
		printf("Cur Angle: %.1f\r\n", angle_read);

		// �ȴ�2s
		HAL_Delay(2000);

		// ���ƶ���Ƕ�(��Ȧ+ָ��ת��)
		printf("MTurn+Velocity GOTO: -180.0f\r\n");
		angle = -180.0f;
		velocity = 100.0f;
		t_acc = 100;
		t_dec = 200;
		FSUS_SetServoAngleMTurnByVelocity(servo_usart, servo_id, angle, velocity, t_acc, t_dec, power, wait);
		FSUS_QueryServoAngleMTurn(servo_usart, servo_id, &angle_read);
		printf("Cur Angle: %.1f\r\n", angle_read);

		// �ȴ�2s
		HAL_Delay(2000);
	}
}

/* �������ģʽ��ǶȻش� */
void FSUSExample_SetServoDamping(void)
{
	FSUS_STATUS status_code; // �������״̬��
	uint8_t servo_id = 0;	 // ������ת�Ӱ��ϵ������ŷ����ID��
	uint16_t power = 500;	 // ����ģʽ�µĹ��ʣ�����Խ������Խ��
	float angle = 0;		 // ����ĽǶ�

	// ���ö��Ϊ����ģʽ
	FSUS_DampingMode(servo_usart, servo_id, power);
	while (1)
	{
		// ��ȡһ�¶���ĽǶ�
		status_code = FSUS_QueryServoAngle(servo_usart, servo_id, &angle);

		if (status_code == FSUS_STATUS_SUCCESS)
		{
			// �ɹ��Ķ�ȡ���˶���ĽǶ�
			printf("[INFO] servo id= %d ; angle = %f\r\n", servo_id, angle);
		}
		else
		{
			// û����ȷ�Ķ�ȡ������ĽǶ�
			printf("\r\n[INFO] read servo %d angle, status code: %d \r\n", servo_id, status_code);
			printf("[ERROR]failed to read servo angle\r\n");
		}
		// �ȴ�1000ms
		HAL_Delay(500);
	}
}

/* ��ʽģʽ����(һֱ��ת) */
void FSUSExample_WheelKeepMove(void)
{
	uint8_t servo_id = 0; // ������ת�Ӱ��ϵ������ŷ����ID��
	uint16_t speed = 20;  // �������ת�ٶ� ��/s
	uint8_t is_cw = 0;	  // �������ת����

	while (1)
	{
		// �����ʽģʽ���ٿ��� ˳ʱ����ת3s
		is_cw = 1;
		FSUS_WheelKeepMove(servo_usart, servo_id, is_cw, speed);
		HAL_Delay(3000);

		// ���ɲ�� ͣ��1s
		FSUS_WheelStop(servo_usart, servo_id);
		HAL_Delay(1000);

		// �����ʽģʽ���ٿ��� ��ʱ����ת3s
		is_cw = 0;
		FSUS_WheelKeepMove(servo_usart, servo_id, is_cw, speed);
		HAL_Delay(3000);

		// ���ɲ�� ͣ��1s
		FSUS_WheelStop(servo_usart, servo_id);
		HAL_Delay(1000);
	}
}

// ������תȦ������Ҫ���ѵ�ʱ��
uint16_t FSUSExample_EstimateTimeMs(uint16_t n_circle, uint16_t speed)
{
	return (uint16_t)((float)n_circle * 360.0f / (float)speed * 1000.0f);
}

/* ��ʽģʽ����(��Ȧ) */
void FSUSExample_WheelMoveNCircle(void)
{
	uint8_t servo_id = 0;  // ������ת�Ӱ��ϵ������ŷ����ID��
	uint16_t speed = 200;  // �������ת�ٶ� ��/s
	uint8_t is_cw = 0;	   // �������ת����
	uint16_t n_circle = 1; // �����ת��Ȧ��

	while (1)
	{
		// �����ʽģʽ���ٿ��� ˳ʱ����ת1Ȧ
		is_cw = 1;
		FSUS_WheelMoveNCircle(servo_usart, servo_id, is_cw, speed, n_circle);
		// FSUS_WheelMoveNCircle�Ƿ�������,��Ϊ��ʱ����Ҫ���ƶ�����ͬʱ��ת
		// ��ʱ��������ʱ��
		HAL_Delay(FSUSExample_EstimateTimeMs(n_circle, speed));

		// ͣ��1s
		HAL_Delay(1000);

		// �����ʽģʽ���ٿ��� ��ʱ����ת1Ȧ
		is_cw = 0;
		FSUS_WheelMoveNCircle(servo_usart, servo_id, is_cw, speed, n_circle);
		// ע��: FSUS_WheelMoveNCircle�Ƿ�������,��Ϊ��ʱ����Ҫ���ƶ�����ͬʱ��ת
		// ��ʱ��������ʱ��
		HAL_Delay(FSUSExample_EstimateTimeMs(n_circle, speed));

		// ͣ��1s
		HAL_Delay(1000);
	}
}

/* ��ʽ����ģʽ(��ʱ) */
void FSUSExample_WheelMoveNTime(void)
{
	uint8_t servo_id = 0;	// ������ת�Ӱ��ϵ������ŷ����ID��
	uint16_t speed = 20;	// �������ת�ٶ� ��/s
	uint8_t is_cw = 0;		// �������ת����
	uint16_t n_time = 3000; // ��ʱʱ��

	while (1)
	{
		// �����ʽģʽ���ٿ��� ˳ʱ����ת3s
		is_cw = 1;
		FSUS_WheelMoveTime(servo_usart, servo_id, is_cw, speed, n_time);
		// FSUS_WheelMoveTime�Ƿ�������,��Ϊ��ʱ����Ҫ���ƶ�����ͬʱ��ת
		// �����ں���Ҫ�ֶ����ӳ�
		HAL_Delay(n_time);

		// ͣ��1s
		HAL_Delay(1000);

		// �����ʽģʽ���ٿ��� ��ʱ����ת3s
		is_cw = 0;
		FSUS_WheelMoveTime(servo_usart, servo_id, is_cw, speed, n_time);
		HAL_Delay(n_time);

		// ͣ��1s
		HAL_Delay(1000);
	}
}

/*��ȡ���״̬*/
void FSUSExample_ReadData(void)
{
	uint8_t servo_id = 0;	// ������ת�Ӱ��ϵ������ŷ����ID��
	FSUS_STATUS statusCode; // ״̬��

	// ���ݱ�����������ֽڳ���һ��Ϊ1���ֽ�/2���ֽ�/4���ֽ�
	// ����ͨ��Э���֪,����Ƕ����޵������������з��Ŷ�����(UShort, ��ӦSTM32�����int16_t),����Ϊ2���ֽ�
	// ������������value����������Ϊint16_t
	int16_t value;
	uint8_t dataSize;
	// ��������ʱ��, Ҫ��value��ָ��ǿ��ת��Ϊuint8_t

	// ��ȡ��ѹ
	statusCode = FSUS_ReadData(servo_usart, servo_id, FSUS_PARAM_VOLTAGE, (uint8_t *)&value, &dataSize);

	printf("read ID: %d\r\n", servo_id);

	if (statusCode == FSUS_STATUS_SUCCESS)
	{
		printf("read sucess, voltage: %d mV\r\n", value);
	}
	else
	{
		printf("fail\r\n");
	}

	// ��ȡ����
	statusCode = FSUS_ReadData(servo_usart, servo_id, FSUS_PARAM_CURRENT, (uint8_t *)&value, &dataSize);
	if (statusCode == FSUS_STATUS_SUCCESS)
	{
		printf("read sucess, current: %d mA\r\n", value);
	}
	else
	{
		printf("fail\r\n");
	}

	// ��ȡ����
	statusCode = FSUS_ReadData(servo_usart, servo_id, FSUS_PARAM_POWER, (uint8_t *)&value, &dataSize);
	if (statusCode == FSUS_STATUS_SUCCESS)
	{
		printf("read sucess, power: %d mW\r\n", value);
	}
	else
	{
		printf("fail\r\n");
	}
	// ��ȡ�¶�
	statusCode = FSUS_ReadData(servo_usart, servo_id, FSUS_PARAM_TEMPRATURE, (uint8_t *)&value, &dataSize);
	if (statusCode == FSUS_STATUS_SUCCESS)
	{
		double temperature, temp;
		temp = (double)value;
		temperature = 1 / (log(temp / (4096.0f - temp)) / 3435.0f + 1 / (273.15 + 25)) - 273.15;
		printf("read sucess, temperature: %f\r\n", temperature);
	}
	else
	{
		printf("fail\r\n");
	}
	// ��ȡ����״̬
	statusCode = FSUS_ReadData(servo_usart, servo_id, FSUS_PARAM_SERVO_STATUS, (uint8_t *)&value, &dataSize);
	if (statusCode == FSUS_STATUS_SUCCESS)
	{
		// �������״̬��־λ
		// BIT[0] - ִ��ָ����1��ִ����ɺ����㡣
		// BIT[1] - ִ��ָ�������1�����´���ȷִ�к����㡣
		// BIT[2] - ��ת������1�������ת�����㡣
		// BIT[3] - ��ѹ������1����ѹ�ָ����������㡣
		// BIT[4] - ��ѹ������1����ѹ�ָ����������㡣
		// BIT[5] - ����������1�������ָ����������㡣
		// BIT[6] - ���ʴ�����1�����ʻָ����������㡣
		// BIT[7] - �¶ȴ�����1���¶Ȼָ����������㡣

		if ((value >> 3) & 0x01)
			printf("read sucess, voltage too high\r\n");
		if ((value >> 4) & 0x01)
			printf("read sucess, voltage too low\r\n");
	}
	else
	{
		printf("fail\r\n");
	}
	printf("================================= \r\n");

	// ��ѭ��
	while (1)
	{
	}
}

/* �û��Զ�������-д�� */
void FSUSExample_WriteData(void)
{
	uint8_t servo_id = 0;	 // ������ת�Ӱ��ϵ������ŷ����ID��
	FSUS_STATUS status_code; // ״̬��
	// ��λ�Ƕ�
	float angle_limit_high = 90.0;	   // ����Ƕ����� (Ĭ��ֵ 180)
	float angle_limit_low = -90.0;	   // ����Ƕ����� (Ĭ��ֵ-180)
	uint8_t angle_limit_switch = 0x01; // 0x01: ������λ; 0x00: �ر���λ

	uint16_t value;

	// д�����Ƕ�����
	value = (int16_t)(angle_limit_high * 10); // ����Ƕ����� ת����λΪ0.1��
	status_code = FSUS_WriteData(servo_usart, servo_id, FSUS_PARAM_ANGLE_LIMIT_HIGH, (uint8_t *)&value, 2);
	printf("write angle limit high = %f, status code: %d\r\n", angle_limit_high, status_code);

	// д�����Ƕ�������
	value = (int16_t)(angle_limit_low * 10); // ����Ƕ����� ת����λΪ0.1��
	status_code = FSUS_WriteData(servo_usart, servo_id, FSUS_PARAM_ANGLE_LIMIT_LOW, (uint8_t *)&value, 2);
	printf("write angle limit low = %f, status code: %d\r\n", angle_limit_low, status_code);

	// �򿪶���Ƕ���λ����, ������Ч
	status_code = FSUS_WriteData(servo_usart, servo_id, FSUS_PARAM_ANGLE_LIMIT_LOW, &angle_limit_switch, 1);
	printf("enable angle limit mode, status code: %d\r\n", status_code);

	while (1)
	{
		// ���ƶ���Ƕ�
		FSUS_SetServoAngle(servo_usart, servo_id, 90.0, 2000, 0, 1);
		FSUS_SetServoAngle(servo_usart, servo_id, -90.0, 2000, 0, 1);
	}
}

/* �û��Զ�������-���� */
void FSUSExample_ResetData(void)
{

	// ������ת�Ӱ��ϵ������ŷ����ID��
	uint8_t servo_id = 0;
	// ״̬��
	FSUS_STATUS status_code;

	// ���������û�����ָ��
	status_code = FSUS_ResetUserData(servo_usart, servo_id);
	printf("=====reset user data======\r\n status code: %d\r\n", status_code);
	if (status_code == FSUS_STATUS_SUCCESS)
	{
		printf("sucess\r\n");
	}
	else
	{
		printf("fail\r\n");
	}

	// ��ѭ��
	while (1)
	{
	}
}
