#ifndef __UI_H__
#define __UI_H__

#include "main.h"
#include "usart.h"
#include "Delay.h"
#include <string.h>

extern uint8_t uiValue;
extern uint8_t uiRx[1];
extern uint8_t uiRxCNT;

void UI_Init(void);
uint8_t UI_Receive_Data(UART_HandleTypeDef *huart);

#endif /* __UI_H */
