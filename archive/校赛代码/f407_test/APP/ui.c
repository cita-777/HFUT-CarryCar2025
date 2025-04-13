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
        // ������
    }

    Delay_ms(100); // ����Ƿ������Ҫ
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
        // ���������������

        uiRxCNT = 0;
        return 0;
    }

    if (uiRxCNT == RX_BUFFER_SIZE)
    {
        if (uiRx[0] == START_BYTE)
        {
            uiRxCNT = 0; // ���ü�����
            //printf("receive start!/r/n");
            return 1;

        }

        // �������
        memset(uiRx, 0, sizeof(uiRx));
        uiRxCNT = 0;
    }

    //printf("no receive start!\r\n");
    return 0;
}
