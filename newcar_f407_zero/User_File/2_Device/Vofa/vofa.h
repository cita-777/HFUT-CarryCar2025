#ifndef _VOFA_H
#define _VOFA_H
#ifdef __cplusplus
extern "C" {
#endif
//....在此替换你的串口函数路径........
#include "usart.h"
//...................................
#include "stdint.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

void Vofa_FireWater(const char* format, ...);
void Vofa_JustFloat(float* _data, uint8_t _num);
#ifdef __cplusplus
}
#endif
#endif
