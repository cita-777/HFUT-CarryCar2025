/**
 * *****************************************************************************
 * @file        task_config.h
 * @brief
 * @author      ciat-777 (juricek.chen@gmail.com)
 * @date        2025-03-07
 * @copyright   cita
 * *****************************************************************************
 */
#ifndef __TASK_CONFIG_H__
#define __TASK_CONFIG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdio.h>

#define TASK_MAX_NUM 10   // 最大任务数量

// 定义任务函数类型
typedef void (*TaskFunction_t)(void);

// 定义任务配置结构体，增加enabled字段用于使能控制（1: 使能；0: 禁用）
typedef struct
{
    TaskFunction_t taskFunction;   // 任务函数指针
    const char*    taskName;       // 任务名称
    uint8_t        enabled;        // 任务使能标志，默认应设为1
} TaskConfig_t;

// 任务配置表声明
extern TaskConfig_t taskConfigTable[TASK_MAX_NUM];

// 任务初始化(若有需要，可在初始化中设置各任务默认状态)
void Task_InitAll(void);

// 运行所有任务, 依次调用任务表中处于使能状态的任务函数
void Task_RunAll(void);

// 开启或禁用指定任务（通过名称方式）
// 如果匹配多个任务则只处理第一个匹配项
void Task_EnableHandle(const char* taskName);
void Task_DisableHandle(const char* taskName);

#ifdef __cplusplus
}
#endif

#endif   // __TASK_CONFIG_H__