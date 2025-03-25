#ifndef MAIN_PROC_H
#define MAIN_PROC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

// 状态枚举定义
typedef enum
{
    PROC_STATE_TASK1 = 0,
    PROC_STATE_TASK2,
    PROC_STATE_TASK3,
    PROC_STATE_DONE,
    PROC_STATE_MAX
} ProcState_e;

// 修改为返回bool的函数指针类型
typedef bool (*ProcHandler_t)(void);

// 对外接口
void main_proc_run(void);
void Proc_SetState(uint8_t state);

#ifdef __cplusplus
}
#endif

#endif /* MAIN_PROC_H */