#include "servo.h"

/*
 * 总线伺服舵机示例代码
 * 使用API函数时，不需要循环发送同一个控制命令，新的命令会覆盖旧的命令。
 * 如果不能控制舵机，请使用ping指令检查通信是否正常。如正常，使用FSUSExample_ReadData()例程获取舵机当前状态。通常是供电电压不符导致判断电压异常，重新上电或更换电源即可。
 * ---------------------------------------------------
 */

// 包含一些必需的头文件
#include "servo.h"
#include "user_uart.h"
#include "math.h"
#include "Delay.h"

Usart_DataTypeDef *servo_usart = &FSUS_Usart; // 串口总线舵机对应的USART

// 舵机控制函数
// 控制舵机转动一定角度
	//@id 舵机编号
	//@angle 舵机角度
	//@interval 时间间隔
void Servo_Control(uint8_t servo_id,float angle){
	
	//// 舵机控制相关的参数
	// 时间间隔ms
	// 可以尝试修改设置更小的时间间隔，例如500ms
	uint16_t interval = 2000;
	// 舵机执行功率 mV 默认为0
	uint16_t power = 0;
	// 设置舵机角度的时候, 是否为阻塞式
	// 0:不等待 1:等待舵机旋转到特定的位置;
	uint8_t wait = 0;
	// 是否为多圈模式
	// 0: 单圈模式; 1: 多圈模式;
	//uint8_t is_mturn = 0;
	
	float anglea = angle;
	uint8_t servo_ida = servo_id;

	FSUS_SetServoAngle(servo_usart, servo_ida, anglea, interval, power, wait);


	// 阻塞式等待, 等待旋转到目标角度
	//FSUS_Wait(servo_usart, servo_ida, anglea, is_mturn);
	
	HAL_Delay(1);
}


// 执行抓取动作
void Crawl_Close(){
	//// 舵机控制相关的参数
	// 时间间隔ms
	// 可以尝试修改设置更小的时间间隔，例如500ms
	uint16_t interval = 200;
	// 舵机执行功率 mV 默认为0
	uint16_t power = 0;
	// 设置舵机角度的时候, 是否为阻塞式
	// 0:不等待 1:等待舵机旋转到特定的位置;
	uint8_t wait = 0;
	// 是否为多圈模式
	// 0: 单圈模式; 1: 多圈模式;
	//uint8_t is_mturn = 0;
	
	
	// 控制舵机旋转到另外一个角度
	//FSUS_SetServoAngle(servo_usart, 1, -90.0, interval, power, wait);
	//FSUS_SetServoAngle(servo_usart, 2, -45.0, interval, power, wait);
	FSUS_SetServoAngle(servo_usart, 3, -61, interval, power, wait);
	
	// 阻塞式等待, 等待旋转到目标角度
	// 注意要跟设定值相同
	//FSUS_Wait(servo_usart, 1, -90.0, is_mturn);
	//FSUS_Wait(servo_usart, 2, -45.0, is_mturn);
	//FSUS_Wait(servo_usart, 3, -60.0, is_mturn);

	// 等待2s
	//HAL_Delay(500);
}


// 执行打开动作
void Crawl_Open(){
		//// 舵机控制相关的参数
	// 时间间隔ms
	// 可以尝试修改设置更小的时间间隔，例如500ms
	uint16_t interval = 200;
	// 舵机执行功率 mV 默认为0
	uint16_t power = 0;
	// 设置舵机角度的时候, 是否为阻塞式
	// 0:不等待 1:等待舵机旋转到特定的位置;
	uint8_t wait = 0;
	// 是否为多圈模式
	// 0: 单圈模式; 1: 多圈模式;
	//uint8_t is_mturn = 0;
	
	
	// 控制舵机旋转到另外一个角度
	//FSUS_SetServoAngle(servo_usart, 1, 0, interval, power, wait);
	//FSUS_SetServoAngle(servo_usart, 2, 0, interval, power, wait);
	FSUS_SetServoAngle(servo_usart, 3, 5, interval, power, wait);
	
	// 阻塞式等待, 等待旋转到目标角度
	// 注意要跟设定值相同
	//FSUS_Wait(servo_usart, 1, 0, is_mturn);
	//FSUS_Wait(servo_usart, 2, 0, is_mturn);
	//FSUS_Wait(servo_usart, 3, 0, is_mturn);
}

// 向前动作
void Forward(){
		//// 舵机控制相关的参数
	// 时间间隔ms
	// 可以尝试修改设置更小的时间间隔，例如500ms
	uint16_t interval = 200;
	// 舵机执行功率 mV 默认为0
	uint16_t power = 0;
	// 设置舵机角度的时候, 是否为阻塞式
	// 0:不等待 1:等待舵机旋转到特定的位置;
	uint8_t wait = 0;
	// 是否为多圈模式
	// 0: 单圈模式; 1: 多圈模式;
	//uint8_t is_mturn = 0;
	
	
	// 控制舵机旋转到另外一个角度
	//FSUS_SetServoAngle(servo_usart, 1, 24, interval, power, wait);
	FSUS_SetServoAngle(servo_usart, 2, -87, interval, power, wait);
}

// 向后动作
void Backward(){
	//// 舵机控制相关的参数
	// 时间间隔ms
	// 可以尝试修改设置更小的时间间隔，例如500ms
	uint16_t interval = 200;
	// 舵机执行功率 mV 默认为0
	uint16_t power = 0;
	// 设置舵机角度的时候, 是否为阻塞式
	// 0:不等待 1:等待舵机旋转到特定的位置;
	uint8_t wait = 0;
	// 是否为多圈模式
	// 0: 单圈模式; 1: 多圈模式;
	//uint8_t is_mturn = 0;
	
	
	// 控制舵机旋转到另外一个角度
	//FSUS_SetServoAngle(servo_usart, 1, 24, interval, power, wait);
	FSUS_SetServoAngle(servo_usart, 2, 74, interval, power, wait);
	//FSUS_SetServoAngle(servo_usart, 3, 0, interval, power, wait);
	
	// 阻塞式等待, 等待旋转到目标角度
	// 注意要跟设定值相同
	//FSUS_Wait(servo_usart, 1, 24, is_mturn);
	//FSUS_Wait(servo_usart, 2, 55, is_mturn);
	//FSUS_Wait(servo_usart, 3, 0, is_mturn);
}

// 位置1
void Location_First(){
	//Servo_Control(1,24);
	//// 舵机控制相关的参数
	// 时间间隔ms
	// 可以尝试修改设置更小的时间间隔，例如500ms
	uint16_t interval = 200;
	// 舵机执行功率 mV 默认为0
	uint16_t power = 0;
	// 设置舵机角度的时候, 是否为阻塞式
	// 0:不等待 1:等待舵机旋转到特定的位置;
	uint8_t wait = 0;
	// 是否为多圈模式
	// 0: 单圈模式; 1: 多圈模式;
	//uint8_t is_mturn = 0;
	
	
	// 控制舵机旋转到另外一个角度
	FSUS_SetServoAngle(servo_usart, 1, 82, interval, power, wait);
	//FSUS_SetServoAngle(servo_usart, 2, 55, interval, power, wait);
	//FSUS_SetServoAngle(servo_usart, 3, 0, interval, power, wait);
}

// 位置2
void Location_Second(){
		// 可以尝试修改设置更小的时间间隔，例如500ms
	uint16_t interval = 200;
	// 舵机执行功率 mV 默认为0
	uint16_t power = 0;
	// 设置舵机角度的时候, 是否为阻塞式
	// 0:不等待 1:等待舵机旋转到特定的位置;
	uint8_t wait = 0;
	// 是否为多圈模式
	// 0: 单圈模式; 1: 多圈模式;
	//uint8_t is_mturn = 0;
	
	
	// 控制舵机旋转到另外一个角度
	FSUS_SetServoAngle(servo_usart, 1, -33, interval, power, wait);
}

// 位置3
void Location_Third(){
			// 可以尝试修改设置更小的时间间隔，例如500ms
	uint16_t interval = 200;
	// 舵机执行功率 mV 默认为0
	uint16_t power = 0;
	// 设置舵机角度的时候, 是否为阻塞式
	// 0:不等待 1:等待舵机旋转到特定的位置;
	uint8_t wait = 0;
	// 是否为多圈模式
	// 0: 单圈模式; 1: 多圈模式;
	//uint8_t is_mturn = 0;
	
	
	// 控制舵机旋转到另外一个角度
	FSUS_SetServoAngle(servo_usart, 1, -163, interval, power, wait);
}




//******************************以下为例程函数**************************************

//int fputc(int ch, FILE *f)
//{
//	HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xffff);
//	return ch;
//}

/* 舵机通讯检测 */
void FSUSExample_PingServo(void)
{

	FSUS_STATUS status_code; // 状态码
	uint8_t servo_id = 0;	 // 舵机ID = 0

	printf("===Test Uart Servo Ping===r\n");
	while (1)
	{
		// 舵机通信检测
		status_code = FSUS_Ping(servo_usart, servo_id);
		if (status_code == FSUS_STATUS_SUCCESS)
		{
			// 舵机在线， LED1闪烁(绿灯)
			printf("Servo Online \r\n");
			HAL_GPIO_WritePin(GPIOF, GPIO_PIN_9, GPIO_PIN_SET);
			HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_10);
		}
		else
		{
			// 舵机离线， LED0闪烁(红灯)
			printf("Servo Offline,Error Code=%d \r\n", status_code);
			HAL_GPIO_WritePin(GPIOF, GPIO_PIN_10, GPIO_PIN_SET);
			HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_9);
		}
		// 延时等待1s
		HAL_Delay(1000);
	}
}

/* 控制单个舵机的角度 */
void FSUSExample_SetServoAngle(void)
{
	// 舵机控制相关的参数
	// 舵机的ID号
	uint8_t servo_id = 0;
	// 舵机的目标角度
	// 舵机角度在-180度到180度之间, 最小单位0.1
	float angle = 0;
	// 时间间隔ms
	// 可以尝试修改设置更小的时间间隔，例如500ms
	uint16_t interval;
	// 目标转速
	float velocity;
	// 加速时间
	uint16_t t_acc;
	// 减速时间
	uint16_t t_dec;
	// 舵机执行功率 mV 默认为0
	uint16_t power = 0;
	// 设置舵机角度的时候, 是否为阻塞式
	// 0:不等待 1:等待舵机旋转到特定的位置;
	uint8_t wait = 0;
	// 读取的角度
	float angle_read;

	while (1)
	{
		printf("GOTO: 90.0f\r\n");
		// 控制舵机角度
		angle = 45.0;
		interval = 2000;
		FSUS_SetServoAngle(servo_usart, servo_id, angle, interval, power, wait);
		FSUS_QueryServoAngle(servo_usart, servo_id, &angle_read);
		printf("Cur Angle: %.1f\r\n", angle_read);

		// 等待2s
		HAL_Delay(10000);

		// 控制舵机角度 + 指定时间
		printf("GOTO+Interval: 0.0f\r\n");
		angle = 0.0f;
		interval = 1000;
		t_acc = 100;
		t_dec = 150;
		FSUS_SetServoAngleByInterval(servo_usart, servo_id, angle, interval, t_acc, t_dec, power, wait);
		FSUS_QueryServoAngle(servo_usart, servo_id, &angle_read);
		printf("Cur Angle: %.1f\r\n", angle_read);

		// 等待2s
		HAL_Delay(2000);

		// 控制舵机角度 + 指定转速
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

/* 控制多个舵机的角度 */
void FSUSExample_SetNServoAngle(void)
{
	//// 舵机控制相关的参数
	// 时间间隔ms
	// 可以尝试修改设置更小的时间间隔，例如500ms
	uint16_t interval = 2000;
	// 舵机执行功率 mV 默认为0
	uint16_t power = 0;
	// 设置舵机角度的时候, 是否为阻塞式
	// 0:不等待 1:等待舵机旋转到特定的位置;
	uint8_t wait = 0;
	// 是否为多圈模式
	// 0: 单圈模式; 1: 多圈模式;
	uint8_t is_mturn = 0;
	while (1)
	{
		// 控制舵机云台角度
		FSUS_SetServoAngle(servo_usart, 0, 90.0, interval, power, wait);
		FSUS_SetServoAngle(servo_usart, 1, 45.0, interval, power, wait);
		// 阻塞式等待, 等待旋转到目标角度
		// 注意要跟设定值相同
		FSUS_Wait(servo_usart, 0, 90.0, is_mturn);
		FSUS_Wait(servo_usart, 1, 45.0, is_mturn);

		// 等待2s
		HAL_Delay(2000);

		// 控制舵机旋转到另外一个角度
		FSUS_SetServoAngle(servo_usart, 0, -90.0, interval, power, wait);
		FSUS_SetServoAngle(servo_usart, 1, -45.0, interval, power, wait);
		// 阻塞式等待, 等待旋转到目标角度
		// 注意要跟设定值相同
		FSUS_Wait(servo_usart, 0, -90.0, is_mturn);
		FSUS_Wait(servo_usart, 1, -45.0, is_mturn);

		// 等待2s
		HAL_Delay(2000);
	}
}

/* 设置舵机角度(多圈模式) */
void FSUSExample_SetServoAngleMTurn(void)
{
	//// 舵机控制相关的参数
	// 舵机的ID号
	uint8_t servo_id = 0;
	// 舵机的目标角度
	// 舵机角度在-180度到180度之间, 最小单位0.1
	float angle;
	uint32_t interval; // 时间间隔ms
	float velocity;	   // 电机转速, 单位dps,°/s
	// 舵机执行功率 mV 默认为0
	uint16_t power = 0;
	// 设置舵机角度的时候, 是否为阻塞式
	// 0:不等待 1:等待舵机旋转到特定的位置;
	uint8_t wait = 1;
	// 加速时间(单位ms)
	uint16_t t_acc;
	// 减速时间
	uint16_t t_dec;
	// 读取的角度
	float angle_read;

	while (1)
	{
		printf("MTurn GOTO: 720.0f\r\n");
		// 控制舵机角度(多圈)
		angle = 720.0f;
		interval = 2000;
		FSUS_SetServoAngleMTurn(servo_usart, servo_id, angle, interval, power, wait);
		FSUS_QueryServoAngleMTurn(servo_usart, servo_id, &angle_read);
		printf("Cur Angle: %.1f\r\n", angle_read);

		// 等待2s
		HAL_Delay(2000);

		// 控制舵机旋转到另外一个角度(多圈)
		printf("MTurn GOTO: 0.0f\r\n");
		angle = 0.0;
		FSUS_SetServoAngleMTurn(servo_usart, servo_id, angle, interval, power, wait);
		FSUS_QueryServoAngleMTurn(servo_usart, servo_id, &angle_read);
		printf("Cur Angle: %.1f\r\n", angle_read);

		// 等待2s
		HAL_Delay(2000);

		// 控制舵机角度(多圈+指定周期)
		printf("MTurn+Interval GOTO: -180.0f\r\n");
		angle = 180.0f;
		interval = 1000;
		t_acc = 100;
		t_dec = 200;
		FSUS_SetServoAngleMTurnByInterval(servo_usart, servo_id, angle, interval, t_acc, t_dec, power, wait);
		FSUS_QueryServoAngleMTurn(servo_usart, servo_id, &angle_read);
		printf("Cur Angle: %.1f\r\n", angle_read);

		// 等待2s
		HAL_Delay(2000);

		// 控制舵机角度(多圈+指定转速)
		printf("MTurn+Velocity GOTO: -180.0f\r\n");
		angle = -180.0f;
		velocity = 100.0f;
		t_acc = 100;
		t_dec = 200;
		FSUS_SetServoAngleMTurnByVelocity(servo_usart, servo_id, angle, velocity, t_acc, t_dec, power, wait);
		FSUS_QueryServoAngleMTurn(servo_usart, servo_id, &angle_read);
		printf("Cur Angle: %.1f\r\n", angle_read);

		// 等待2s
		HAL_Delay(2000);
	}
}

/* 舵机阻尼模式与角度回传 */
void FSUSExample_SetServoDamping(void)
{
	FSUS_STATUS status_code; // 请求包的状态码
	uint8_t servo_id = 0;	 // 连接在转接板上的总线伺服舵机ID号
	uint16_t power = 500;	 // 阻尼模式下的功率，功率越大阻力越大
	float angle = 0;		 // 舵机的角度

	// 设置舵机为阻尼模式
	FSUS_DampingMode(servo_usart, servo_id, power);
	while (1)
	{
		// 读取一下舵机的角度
		status_code = FSUS_QueryServoAngle(servo_usart, servo_id, &angle);

		if (status_code == FSUS_STATUS_SUCCESS)
		{
			// 成功的读取到了舵机的角度
			printf("[INFO] servo id= %d ; angle = %f\r\n", servo_id, angle);
		}
		else
		{
			// 没有正确的读取到舵机的角度
			printf("\r\n[INFO] read servo %d angle, status code: %d \r\n", servo_id, status_code);
			printf("[ERROR]failed to read servo angle\r\n");
		}
		// 等待1000ms
		HAL_Delay(500);
	}
}

/* 轮式模式控制(一直旋转) */
void FSUSExample_WheelKeepMove(void)
{
	uint8_t servo_id = 0; // 连接在转接板上的总线伺服舵机ID号
	uint16_t speed = 20;  // 舵机的旋转速度 °/s
	uint8_t is_cw = 0;	  // 舵机的旋转方向

	while (1)
	{
		// 舵机轮式模式定速控制 顺时针旋转3s
		is_cw = 1;
		FSUS_WheelKeepMove(servo_usart, servo_id, is_cw, speed);
		HAL_Delay(3000);

		// 舵机刹车 停顿1s
		FSUS_WheelStop(servo_usart, servo_id);
		HAL_Delay(1000);

		// 舵机轮式模式定速控制 逆时针旋转3s
		is_cw = 0;
		FSUS_WheelKeepMove(servo_usart, servo_id, is_cw, speed);
		HAL_Delay(3000);

		// 舵机刹车 停顿1s
		FSUS_WheelStop(servo_usart, servo_id);
		HAL_Delay(1000);
	}
}

// 估计旋转圈数所需要花费的时间
uint16_t FSUSExample_EstimateTimeMs(uint16_t n_circle, uint16_t speed)
{
	return (uint16_t)((float)n_circle * 360.0f / (float)speed * 1000.0f);
}

/* 轮式模式控制(定圈) */
void FSUSExample_WheelMoveNCircle(void)
{
	uint8_t servo_id = 0;  // 连接在转接板上的总线伺服舵机ID号
	uint16_t speed = 200;  // 舵机的旋转速度 °/s
	uint8_t is_cw = 0;	   // 舵机的旋转方向
	uint16_t n_circle = 1; // 舵机旋转的圈数

	while (1)
	{
		// 舵机轮式模式定速控制 顺时针旋转1圈
		is_cw = 1;
		FSUS_WheelMoveNCircle(servo_usart, servo_id, is_cw, speed, n_circle);
		// FSUS_WheelMoveNCircle是非阻塞的,因为有时候需要控制多个舵机同时旋转
		// 延时估算所需时间
		HAL_Delay(FSUSExample_EstimateTimeMs(n_circle, speed));

		// 停顿1s
		HAL_Delay(1000);

		// 舵机轮式模式定速控制 逆时针旋转1圈
		is_cw = 0;
		FSUS_WheelMoveNCircle(servo_usart, servo_id, is_cw, speed, n_circle);
		// 注意: FSUS_WheelMoveNCircle是非阻塞的,因为有时候需要控制多个舵机同时旋转
		// 延时估算所需时间
		HAL_Delay(FSUSExample_EstimateTimeMs(n_circle, speed));

		// 停顿1s
		HAL_Delay(1000);
	}
}

/* 轮式控制模式(定时) */
void FSUSExample_WheelMoveNTime(void)
{
	uint8_t servo_id = 0;	// 连接在转接板上的总线伺服舵机ID号
	uint16_t speed = 20;	// 舵机的旋转速度 °/s
	uint8_t is_cw = 0;		// 舵机的旋转方向
	uint16_t n_time = 3000; // 延时时间

	while (1)
	{
		// 舵机轮式模式定速控制 顺时针旋转3s
		is_cw = 1;
		FSUS_WheelMoveTime(servo_usart, servo_id, is_cw, speed, n_time);
		// FSUS_WheelMoveTime是非阻塞的,因为有时候需要控制多个舵机同时旋转
		// 所以在后面要手动加延迟
		HAL_Delay(n_time);

		// 停顿1s
		HAL_Delay(1000);

		// 舵机轮式模式定速控制 逆时针旋转3s
		is_cw = 0;
		FSUS_WheelMoveTime(servo_usart, servo_id, is_cw, speed, n_time);
		HAL_Delay(n_time);

		// 停顿1s
		HAL_Delay(1000);
	}
}

/*读取舵机状态*/
void FSUSExample_ReadData(void)
{
	uint8_t servo_id = 0;	// 连接在转接板上的总线伺服舵机ID号
	FSUS_STATUS statusCode; // 状态码

	// 数据表里面的数据字节长度一般为1个字节/2个字节/4个字节
	// 查阅通信协议可知,舵机角度上限的数据类型是有符号短整型(UShort, 对应STM32里面的int16_t),长度为2个字节
	// 所以这里设置value的数据类型为int16_t
	int16_t value;
	uint8_t dataSize;
	// 传参数的时候, 要将value的指针强行转换为uint8_t

	// 读取电压
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

	// 读取电流
	statusCode = FSUS_ReadData(servo_usart, servo_id, FSUS_PARAM_CURRENT, (uint8_t *)&value, &dataSize);
	if (statusCode == FSUS_STATUS_SUCCESS)
	{
		printf("read sucess, current: %d mA\r\n", value);
	}
	else
	{
		printf("fail\r\n");
	}

	// 读取功率
	statusCode = FSUS_ReadData(servo_usart, servo_id, FSUS_PARAM_POWER, (uint8_t *)&value, &dataSize);
	if (statusCode == FSUS_STATUS_SUCCESS)
	{
		printf("read sucess, power: %d mW\r\n", value);
	}
	else
	{
		printf("fail\r\n");
	}
	// 读取温度
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
	// 读取工作状态
	statusCode = FSUS_ReadData(servo_usart, servo_id, FSUS_PARAM_SERVO_STATUS, (uint8_t *)&value, &dataSize);
	if (statusCode == FSUS_STATUS_SUCCESS)
	{
		// 舵机工作状态标志位
		// BIT[0] - 执行指令置1，执行完成后清零。
		// BIT[1] - 执行指令错误置1，在下次正确执行后清零。
		// BIT[2] - 堵转错误置1，解除堵转后清零。
		// BIT[3] - 电压过高置1，电压恢复正常后清零。
		// BIT[4] - 电压过低置1，电压恢复正常后清零。
		// BIT[5] - 电流错误置1，电流恢复正常后清零。
		// BIT[6] - 功率错误置1，功率恢复正常后清零。
		// BIT[7] - 温度错误置1，温度恢复正常后清零。

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

	// 死循环
	while (1)
	{
	}
}

/* 用户自定义数据-写入 */
void FSUSExample_WriteData(void)
{
	uint8_t servo_id = 0;	 // 连接在转接板上的总线伺服舵机ID号
	FSUS_STATUS status_code; // 状态码
	// 限位角度
	float angle_limit_high = 90.0;	   // 舵机角度上限 (默认值 180)
	float angle_limit_low = -90.0;	   // 舵机角度下限 (默认值-180)
	uint8_t angle_limit_switch = 0x01; // 0x01: 开启限位; 0x00: 关闭限位

	uint16_t value;

	// 写入舵机角度上限
	value = (int16_t)(angle_limit_high * 10); // 舵机角度上限 转换单位为0.1度
	status_code = FSUS_WriteData(servo_usart, servo_id, FSUS_PARAM_ANGLE_LIMIT_HIGH, (uint8_t *)&value, 2);
	printf("write angle limit high = %f, status code: %d\r\n", angle_limit_high, status_code);

	// 写入舵机角度下限制
	value = (int16_t)(angle_limit_low * 10); // 舵机角度下限 转换单位为0.1度
	status_code = FSUS_WriteData(servo_usart, servo_id, FSUS_PARAM_ANGLE_LIMIT_LOW, (uint8_t *)&value, 2);
	printf("write angle limit low = %f, status code: %d\r\n", angle_limit_low, status_code);

	// 打开舵机角度限位开关, 配置生效
	status_code = FSUS_WriteData(servo_usart, servo_id, FSUS_PARAM_ANGLE_LIMIT_LOW, &angle_limit_switch, 1);
	printf("enable angle limit mode, status code: %d\r\n", status_code);

	while (1)
	{
		// 控制舵机角度
		FSUS_SetServoAngle(servo_usart, servo_id, 90.0, 2000, 0, 1);
		FSUS_SetServoAngle(servo_usart, servo_id, -90.0, 2000, 0, 1);
	}
}

/* 用户自定义数据-重置 */
void FSUSExample_ResetData(void)
{

	// 连接在转接板上的总线伺服舵机ID号
	uint8_t servo_id = 0;
	// 状态码
	FSUS_STATUS status_code;

	// 发送重置用户数据指令
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

	// 死循环
	while (1)
	{
	}
}
