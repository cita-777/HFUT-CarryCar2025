/**
 * *****************************************************************************
 * @file        main_proc.h
 * @brief
 * @author      ciat-777 (juricek.chen@gmail.com)
 * @date        2025-03-07
 * @copyright   cita
 * *****************************************************************************
 */
#ifndef __MAIN_PROC_H__
#define __MAIN_PROC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

// 状态数目
#define PROC_STATE_MAX 4

// 定义状态机处理函数类型
typedef void (*ProcHandler_t)(void);

// 状态机处理函数，每次调用执行当前状态函数并自动跳转到下一状态
void main_proc_run(void);

// 调试接口：手动设置当前状态（状态索引范围 0~PROC_STATE_MAX-1）
void Proc_SetState(uint8_t state);

#ifdef __cplusplus
}
#endif

#endif   // __MAIN_PROC_H__