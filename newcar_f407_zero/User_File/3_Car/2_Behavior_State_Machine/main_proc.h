/**
 * *****************************************************************************
 * @file        main_proc.h
 * @brief       主处理逻辑状态机头文件
 * @author      ciat-777
 * @date        2025-04-08
 * *****************************************************************************
 */

#ifndef __MAIN_PROC_H__
#define __MAIN_PROC_H__

#ifdef __cplusplus
extern "C" {
#endif
#include <stdbool.h>
#include <stdint.h>
// 状态处理函数类型
typedef bool (*ProcHandler_t)(void);

// 运行状态机
void main_proc_run(void);

// 手动设置状态
void Proc_SetState(uint8_t state);

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_PROC_H__ */