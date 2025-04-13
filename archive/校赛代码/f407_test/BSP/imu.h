#ifndef __IMU_H__
#define __IMU_H__

#include "main.h"
#include "stdio.h"
#include "string.h"

#define MAXSIZE 30

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

extern uint8_t End_flag; 
extern uint8_t DMA_buf[30];
extern float YawAngle;

void IMU_Data_Init(void);
void IMU_Data_Proc(void);



#endif
