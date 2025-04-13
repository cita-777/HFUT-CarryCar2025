#ifndef __JETSON_H__
#define __JETSON_H__

#include "main.h"
#include "Delay.h"
#include <string.h>

extern uint8_t value;   // main.c要包含AngelaDecode.h，value的定义在这里，.h里面定义了extern
extern uint8_t JetsonRx[9];
extern uint8_t JetsonRxCNT;
extern char QRCodeString[8];  // 修改为8以容纳"123+321\0"
extern int16_t coord_array[2] ; // [0]存X坐标, [1]存Y坐标

void Jetson_Init(void);
void Jetson_Send(uint8_t data);
void Jetson_Receive_Data(UART_HandleTypeDef *huart);

#endif // __JETSON_H__
