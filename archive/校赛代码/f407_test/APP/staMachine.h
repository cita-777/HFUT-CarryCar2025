#ifndef __STAMACHINE_H__
#define __STAMACHINE_H__

#include "pi.h"
#include "stm32f4xx_hal.h"

extern uint8_t ready_flag;
extern uint8_t is_wait_jetson_grab ;
extern uint8_t running_flag ;  
typedef enum
{
    STATE_IDLE,
    MOVE_TO_RAW,
    MOVE_TO_ROUGH,
    MOVE_TO_TEMPSTORE,
    MOVE_TO_RAW_AGAIN,
    MOVE_TO_ROUGH_AGAIN,
    MOVE_TO_TEMPSTORE_AGAIN,
    MOVE_TO_HOME,
    STATE_CUSTOM
} State;

void StateMachine_Init(void);
void StateMachine_SetState(State newState);
void StateMachine_Update(void);

#endif // __STAMACHINE_H
