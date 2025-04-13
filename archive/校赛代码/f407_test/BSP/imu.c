#include "imu.h"



uint8_t End_flag = 0;
uint8_t DMA_buf[30] = {0};
float raw_YawAngle = 0;
float temp_YawAngle = 0;
float YawAngle=0;
void IMU_Data_Init(void)
{
  //z��Ƕȹ���
  uint8_t CALIYAW[] = {0xFF,0xAA,0x76,0x00,0x00};
  //ʹ��IDLE�ж�
  __HAL_UART_ENABLE_IT(&huart2,UART_IT_IDLE);
  //����DMA����
  HAL_UART_Receive_DMA(&huart2,DMA_buf,MAXSIZE);
  //Z�����
	//HAL_Delay(10);
  HAL_UART_Transmit_DMA(&huart2,CALIYAW,0xFF);
}

/**
 * @brief ��������
*/
void IMU_Data_Proc(void)
{
    if (End_flag == 1)
    {

				
//				if((temp_YawAngle-last_YawAngle)<10||(temp_YawAngle-last_YawAngle)>350)
//				{
//					YawAngle=temp_YawAngle;
//				}
//				else if((temp_YawAngle-last_YawAngle)>-10||(temp_YawAngle-last_YawAngle)<-350)
//				{
//					YawAngle=temp_YawAngle;
//				}
//				
//				
//				last_YawAngle=YawAngle;
        // У���
        if (DMA_buf[21] == (uint8_t)(0x55 + 0x53 + DMA_buf[17] + DMA_buf[18] + DMA_buf[19] + DMA_buf[20]))
        {
            printf("ERROR!\r\n");
					        // ��ȡԭʼ˳ʱ��Ƕ�
        raw_YawAngle = (float)((DMA_buf[18] << 8) | DMA_buf[17]) / 32768 * 180;

        // ˳ʱ�� 0-360 ת��Ϊ��ʱ�� 0-30-180-360-0
        YawAngle = 360.0f - raw_YawAngle;

        // ���⴦���� 360 ת��Ϊ 0
        if (YawAngle >= 360.0f)
        {
            YawAngle = 0.0f;
        }
        }

        //printf("t2.txt=\"%f\"\xff\xff\xff", YawAngle);
//printf("%f\n", YawAngle);
        End_flag = 0;

        // ��ս��ջ�����
        memset(DMA_buf, 0, MAXSIZE);

        // ���¿���DMA����
        HAL_UART_Receive_DMA(&huart2, DMA_buf, MAXSIZE);
    }
}


