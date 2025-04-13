#include "usr_tim.h"
#include "imu.h"
uint16_t u10ms;
double Yaw_Compensate = 0;

uint8_t Counter_Enable = 0;
uint16_t Counter = 0;
uint16_t Counter_Times = 0;

uint8_t Car_Counter_Enable = 0;
uint16_t Car_Counter_Times = 0;
uint16_t Car_Counter = 0;

uint8_t Action_Counter_Enable = 0;
uint16_t Action_Counter_Times = 0;
uint16_t Action_Counter = 0;

uint8_t flag = 0;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim == (&htim6)) // 0.01s触发一次中断
    {

        if (Counter_Enable == 1)
        {
            Counter++;
            // printf("Counter:%d\n",Counter);
						//printf("t7.txt=\"%d\"\xff\xff\xff", Counter);
            if (Counter >= Counter_Times)
            {
                Counter = 0;
                Counter_Enable = 0;
            }
        }
        else if (Counter_Enable == 0)
        {
            Counter = 0;
        }

        if (Car_Counter_Enable == 1)
        {
            Car_Counter++;

            if (Car_Counter >= Car_Counter_Times)
            {
                Car_Counter = 0;
                Car_Counter_Enable = 0;
            }
        }
        else if (Car_Counter_Enable == 0)
        {
            Car_Counter = 0;
        }

        u10ms++;

        if (u10ms >= 1000)
        {
            u10ms = 0;
            flag = 1;
            // HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
            Yaw_Compensate -= 0.007;
        }

        // Servo_CallBack();
    }

    
}
