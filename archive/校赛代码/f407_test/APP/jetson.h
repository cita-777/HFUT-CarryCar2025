#ifndef __JETSON_H__
#define __JETSON_H__

#include "main.h"
#include "Delay.h"
#include <string.h>

extern uint8_t value;   // main.cҪ����AngelaDecode.h��value�Ķ��������.h���涨����extern
extern uint8_t JetsonRx[9];
extern uint8_t JetsonRxCNT;
extern char QRCodeString[8];  // �޸�Ϊ8������"123+321\0"
extern int16_t coord_array[2] ; // [0]��X����, [1]��Y����

void Jetson_Init(void);
void Jetson_Send(uint8_t data);
void Jetson_Receive_Data(UART_HandleTypeDef *huart);

#endif // __JETSON_H__
