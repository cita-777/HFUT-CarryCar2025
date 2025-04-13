#ifndef __ACTION_H__
#define __ACTION_H__

#include "main.h"
#include "usr_tim.h"
#include "servo.h"
#include "motor_ctrl.h"
#include "Delay.h"
#include "staMachine.h"
#include "jetson.h"
void Action_Catch_Obj_To_ZhuanPan(uint8_t position);
void Action_Put_Obj_To_Map(uint8_t position);
void Action_Put_Obj_To_Map_MaDuo(uint8_t position);
void Action_Get_Obj_From_Map(uint8_t position);

uint8_t Action_Put_Obj_To_Map_321(void);
uint8_t Action_Put_Obj_To_Map_312(void);
uint8_t Action_Put_Obj_To_Map_123(void);
uint8_t Action_Put_Obj_To_Map_132(void);
uint8_t Action_Put_Obj_To_Map_213(void);
uint8_t Action_Put_Obj_To_Map_231(void);

uint8_t Action_Put_Obj_To_Map_123_Tempstore(void);
uint8_t Action_Put_Obj_To_Map_321_Tempstore_Again(void);
uint8_t Action_Put_Obj_To_Map_321_Tempstore(void);
uint8_t Action_Put_Obj_To_Map_123_Tempstore_Again(void);
#endif
