#include "ui.h"

#define RX_BUFFER_SIZE 1
#define START_BYTE 0x55
#define END_BYTE 0xFF

uint8_t uiValue;
uint8_t uiRx[1] = {0x00};
uint8_t uiRxCNT = 0;

void UI_Init(void)
{
    if (HAL_UART_Receive_IT(&huart1, &uiValue, 1) != HAL_OK)
    {
        // 错误处理
    }

    Delay_ms(100); // 检查是否真的需要
}

uint8_t UI_Receive_Data(UART_HandleTypeDef *huart)
{
    if (uiRxCNT < RX_BUFFER_SIZE)
    {
        uiRx[uiRxCNT] = uiValue;
        uiRxCNT++;
        HAL_UART_Receive_IT(&huart1, &uiValue, 1);
    }
    else
    {
        // 错误处理，缓冲区溢出

        uiRxCNT = 0;
        return 0;
    }

    if (uiRxCNT == RX_BUFFER_SIZE)
    {
        if (uiRx[0] == START_BYTE)
        {
            uiRxCNT = 0; // 重置计数器
            //printf("receive start!/r/n");
            return 1;

        }

        // 清空数组
        memset(uiRx, 0, sizeof(uiRx));
        uiRxCNT = 0;
    }

    //printf("no receive start!\r\n");
    return 0;
}
