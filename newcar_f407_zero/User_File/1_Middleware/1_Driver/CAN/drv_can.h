/**
 * *****************************************************************************
 * @file        drv_can.h
 * @brief       Header file for CAN driver interface
 * @author      ciat-777 (juricek.chen@gmail.com)
 * @date        2025-02-28
 * @copyright   cita
 * *****************************************************************************
 */

#ifndef DRV_CAN_H
#define DRV_CAN_H

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------include-----------------------------------*/
#include "can.h"
#include "main.h"
#include "stdbool.h"
/*-----------------------------------macro------------------------------------*/

/*----------------------------------typedef-----------------------------------*/
typedef struct
{
    __IO CAN_RxHeaderTypeDef CAN_RxMsg;
    __IO uint8_t             rxData[32];

    __IO CAN_TxHeaderTypeDef CAN_TxMsg;
    __IO uint8_t             txData[32];

    __IO bool rxFrameFlag;
} CAN_t;
/*----------------------------------variable----------------------------------*/
extern __IO CAN_t can;
/*-------------------------------------os-------------------------------------*/

/*----------------------------------function----------------------------------*/
void USER_CAN1_Filter_Init(void);
void can_SendCmd(__IO uint8_t* cmd, uint8_t len);
/*------------------------------------test------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* DRV_CAN_H */
