#ifndef __USR__TIM_H__
#define __USR__TIM_H__

#include "Delay.h"
#include "main.h"
#include "tim.h"
extern double Yaw_Compensate;
extern uint8_t Counter_Enable;
extern uint16_t Counter;
extern uint16_t Counter_Times;

extern uint8_t Car_Counter_Enable;
extern uint16_t Car_Counter_Times;
extern uint16_t Car_Counter;

extern uint8_t Action_Counter_Enable ;
extern uint16_t Action_Counter_Times ;
extern uint16_t Action_Counter ;

extern uint8_t flag;
extern uint16_t u10ms;
uint8_t Get_Time_Interval(uint32_t Current_Time, uint32_t Past_Time, uint32_t Delay_Time);
#endif
