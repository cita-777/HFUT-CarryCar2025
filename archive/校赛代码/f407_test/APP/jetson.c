#include "jetson.h"
#include "usart.h"
#include "motor_ctrl.h"
#include "staMachine.h"
uint8_t value; // main.cҪ����AngelaDecode.h��value�Ķ��������.h���涨����extern
uint8_t JetsonRx[9] = {0x00};
uint8_t JetsonRxCNT = 0;
char QRCodeString[8] = {0}; // ���Ӵ�СΪ8

// �����������
int16_t coord_array[2] = {0}; // [0]��X����, [1]��Y����

uint8_t Send_Data_Jetson[9];

void Jetson_Init(void)
{
    //__HAL_UART_ENABLE_IT(&huart5, UART_IT_RXNE);
    HAL_UART_Receive_IT(&huart5, &value, 1);
    Delay_ms(100);
    // Delay_ms(10);
}

void Jetson_Send(uint8_t data)
{
    memset(Send_Data_Jetson, 0, sizeof(Send_Data_Jetson)); // �������
    Send_Data_Jetson[0] = 0x00;
    Send_Data_Jetson[1] = 0x0B;

    for (int i = 2; i <= 7; i++)
    {
        Send_Data_Jetson[i] = data; // ʹ�ô���Ĳ�����������
    }

    Send_Data_Jetson[8] = 0xEF;
    HAL_UART_Transmit(&huart5, Send_Data_Jetson, 9, 1000);
    Delay_ms(20);
}

void Jetson_Receive_Data(UART_HandleTypeDef *huart)
{
    JetsonRx[JetsonRxCNT] = value;
    JetsonRxCNT++;
    HAL_UART_Receive_IT(&huart5, &value, 1); // ȷ���ڽ��յ����ݺ���������

    if (JetsonRxCNT == 9)
    {
        if (JetsonRx[0] == 0x00 && JetsonRx[8] == 0xEF)
        {
            char temp[4]; // ��ʱ�洢ת������ַ���
            switch (JetsonRx[1])
            {
            case 0x0A: // ����A
                // ǰ��������

                for (int i = 0; i < 3; i++)
                {
                    sprintf(temp, "%d", JetsonRx[i + 2]);
                    QRCodeString[i] = temp[0];
                }
                // ��ӼӺ�
                QRCodeString[3] = '+';
                // ����������
                for (int i = 0; i < 3; i++)
                {
                    sprintf(temp, "%d", JetsonRx[i + 5]);
                    QRCodeString[i + 4] = temp[0];
                }
                QRCodeString[7] = '\0'; // �ַ���������

                // ���͵�������ʾ
                printf("t10.txt=\"%s\"\xff\xff\xff", QRCodeString);
								printf("t10.txt=\"%s\"\xff\xff\xff", QRCodeString);
								printf("t10.txt=\"%s\"\xff\xff\xff", QRCodeString);
                break;

            case 0x0C: // ����C
            {
                // ����X���� - ��Y����ʹ����ͬ��ʮ�����ƽ�����ʽ
                int16_t x_value = (JetsonRx[3] << 4) | JetsonRx[4];
                coord_array[0] = (JetsonRx[2] == 0x0A) ? x_value : -x_value;

                // ����Y����
                int16_t y_value = (JetsonRx[6] << 4) | JetsonRx[7];
                coord_array[1] = (JetsonRx[5] == 0x0A) ? y_value : -y_value;
            }
            break;

            case 0x0D: // ����D
            {
                // ����ץȡ����
                if (JetsonRx[2] == 0x02)
                {
                    // ����ץȡ����ʾ����ץȡ
                    is_wait_jetson_grab = 0;
                }
                else
                {
                    // �������������쳣
                }
            }
            break;

            default:
                printf("t10.txt=\"error\"\xff\xff\xff");
                // ��Ч����
                break;
            }
        }

        // �������
        memset(JetsonRx, 0, sizeof(JetsonRx));
        // HAL_UART_Transmit(&huart5, (uint8_t *)"_RECEIVE_COMPLETE!\r\n", 20, 0xffff);
        JetsonRxCNT = 0;
    }
}
