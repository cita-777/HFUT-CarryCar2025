#include "jetson.h"
#include "usart.h"
#include "motor_ctrl.h"
#include "staMachine.h"
uint8_t value; // main.c要包含AngelaDecode.h，value的定义在这里，.h里面定义了extern
uint8_t JetsonRx[9] = {0x00};
uint8_t JetsonRxCNT = 0;
char QRCodeString[8] = {0}; // 增加大小为8

// 添加坐标数组
int16_t coord_array[2] = {0}; // [0]存X坐标, [1]存Y坐标

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
    memset(Send_Data_Jetson, 0, sizeof(Send_Data_Jetson)); // 清空数组
    Send_Data_Jetson[0] = 0x00;
    Send_Data_Jetson[1] = 0x0B;

    for (int i = 2; i <= 7; i++)
    {
        Send_Data_Jetson[i] = data; // 使用传入的参数设置数据
    }

    Send_Data_Jetson[8] = 0xEF;
    HAL_UART_Transmit(&huart5, Send_Data_Jetson, 9, 1000);
    Delay_ms(20);
}

void Jetson_Receive_Data(UART_HandleTypeDef *huart)
{
    JetsonRx[JetsonRxCNT] = value;
    JetsonRxCNT++;
    HAL_UART_Receive_IT(&huart5, &value, 1); // 确保在接收到数据后立即调用

    if (JetsonRxCNT == 9)
    {
        if (JetsonRx[0] == 0x00 && JetsonRx[8] == 0xEF)
        {
            char temp[4]; // 临时存储转换后的字符串
            switch (JetsonRx[1])
            {
            case 0x0A: // 任务A
                // 前三个数字

                for (int i = 0; i < 3; i++)
                {
                    sprintf(temp, "%d", JetsonRx[i + 2]);
                    QRCodeString[i] = temp[0];
                }
                // 添加加号
                QRCodeString[3] = '+';
                // 后三个数字
                for (int i = 0; i < 3; i++)
                {
                    sprintf(temp, "%d", JetsonRx[i + 5]);
                    QRCodeString[i + 4] = temp[0];
                }
                QRCodeString[7] = '\0'; // 字符串结束符

                // 发送到串口显示
                printf("t10.txt=\"%s\"\xff\xff\xff", QRCodeString);
								printf("t10.txt=\"%s\"\xff\xff\xff", QRCodeString);
								printf("t10.txt=\"%s\"\xff\xff\xff", QRCodeString);
                break;

            case 0x0C: // 任务C
            {
                // 解析X坐标 - 与Y坐标使用相同的十六进制解析方式
                int16_t x_value = (JetsonRx[3] << 4) | JetsonRx[4];
                coord_array[0] = (JetsonRx[2] == 0x0A) ? x_value : -x_value;

                // 解析Y坐标
                int16_t y_value = (JetsonRx[6] << 4) | JetsonRx[7];
                coord_array[1] = (JetsonRx[5] == 0x0A) ? y_value : -y_value;
            }
            break;

            case 0x0D: // 任务D
            {
                // 处理抓取命令
                if (JetsonRx[2] == 0x02)
                {
                    // 可以抓取，提示可以抓取
                    is_wait_jetson_grab = 0;
                }
                else
                {
                    // 其他情况，检测异常
                }
            }
            break;

            default:
                printf("t10.txt=\"error\"\xff\xff\xff");
                // 无效命令
                break;
            }
        }

        // 清空数组
        memset(JetsonRx, 0, sizeof(JetsonRx));
        // HAL_UART_Transmit(&huart5, (uint8_t *)"_RECEIVE_COMPLETE!\r\n", 20, 0xffff);
        JetsonRxCNT = 0;
    }
}
