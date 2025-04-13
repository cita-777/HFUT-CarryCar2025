#include "staMachine.h"
#include "motor_ctrl.h"
#include "imu.h"
#include "usart.h"
#include "Delay.h"
#include "usr_tim.h"
#include "servo.h"
#include "action.h"
#include "jetson.h"

uint8_t ready_flag = 0;          // �Ƿ�׼����һ������
uint8_t running_flag = 0;        // ���ӵ���Ƿ�������
uint8_t is_turning = 0;          // �Ƿ���ת��
uint8_t is_wait_jetson_grab = 1; // �Ƿ�ץȡ

#define turn_speed 300
#define turn_acc 167
#define turn_time 300
static State currentState = MOVE_TO_RAW;

void StateMachine_SetState(State newState)
{
    currentState = newState;
}

void StateMachine_Init(void)
{
    StateMachine_SetState(MOVE_TO_RAW);
}

void StateMachine_Update(void)
{
    switch (currentState)
    {
    case STATE_IDLE:
        Car_Go_Target(0, 0, 0, 0);
        printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
        printf("t5.txt=\"%d\"\xff\xff\xff", currentState);

        if (ready_flag == 1)
        {
            Delay_ms(2000);
            StateMachine_SetState(MOVE_TO_RAW);
            ready_flag = 0;
        }

        // printf("%f\r\n",YawAngle);
        // printf("t5.txt=\"%d\"\xff\xff\xff", currentState); // ����״̬��Ϣ
        break;
case STATE_CUSTOM:
    if (Counter_Enable == 0 || is_turning == 1)
    {
        switch (running_flag)
        {
        case 0: // ����
            Counter_Times = 200;
            Counter_Enable = 1;
            Car_Go_Target(-150, 0, 50, 50);
            running_flag++;
            break;

        case 1: // y����ֱ��1450mm
            Counter_Times = 600;
            Counter_Enable = 1;
            Car_Go_Target(0, 1450, 160, 150);
            running_flag++;
            break;

        case 2: // x�᷽����50mm��������
            Counter_Times = 80;
            Counter_Enable = 1;
            Car_Go_Target(50, 0, 200, 150);
            running_flag++;
            break;

        case 3: // x�����50mm
            Counter_Times = 80;
            Counter_Enable = 1;
            Car_Go_Target(-50, 0, 200, 150);
            running_flag++;
            break;

        case 4: // y�������400mm
            Counter_Times = 200;
            Counter_Enable = 1;
            Car_Go_Target(0, -380, 300, 150);
            running_flag++;
            break;

        case 5: // ������ת90��
            Counter_Times = turn_time;
            Counter_Enable = 1;
            Car_Turn_NoUse_IMU(90, turn_speed, turn_acc);
            running_flag++;
            break;

        case 6: // У��
            is_turning = 1;
            if (is_turning == 1 && Car_Calibration(200, 150))
            {
                is_turning = 0;
            }
            if (!is_turning)
            {
                running_flag++;
            }
            break;

        case 7: // y�᷽����-1720mm
            Counter_Times = 800;
            Counter_Enable = 1;
            Car_Go_Target(0, -1685, 200, 130);
            running_flag++;
            break;

        case 8: // ������ת90��
            Counter_Times = turn_time;
            Counter_Enable = 1;
            Car_Turn_NoUse_IMU(90, turn_speed, turn_acc);
            running_flag++;
            break;

        case 9: // У��
            is_turning = 1;
            if (is_turning == 1 && Car_Calibration(200, 150))
            {
                is_turning = 0;
            }
            if (!is_turning)
            {
                running_flag++;
            }
            break;

        case 10: // x��ǰ��50mm
            Counter_Times = 80;
            Counter_Enable = 1;
            Car_Go_Target(50, 0, 200, 150);
            running_flag++;
            break;

        case 11: // y�������825mm
            Counter_Times = 350;
            Counter_Enable = 1;
            Car_Go_Target(0, -875 + 0, 300, 150);
            running_flag++;
            break;

        case 12: // ������ת90��
            Counter_Times = turn_time;
            Counter_Enable = 1;
            Car_Turn_NoUse_IMU(90, turn_speed, turn_acc);
            running_flag++;
            break;

        case 13: // У��
            is_turning = 1;
            if (is_turning == 1 && Car_Calibration(200, 150))
            {
                is_turning = 0;
            }
            if (!is_turning)
            {
                running_flag++;
            }
            break;

        case 14: // y�᷽����-860mm
            Counter_Times = 350;
            Counter_Enable = 1;
            Car_Go_Target(0, -860, 300, 150);
            running_flag++;
            break;

        case 15: // y�᷽����-425mm
            Counter_Times = 280;
            Counter_Enable = 1;
            Car_Go_Target(0, -425, 250, 150);
            running_flag++;
            break;

        case 16: // x�᷽����50mm��������
            Counter_Times = 80;
            Counter_Enable = 1;
            Car_Go_Target(50, 0, 200, 150);
            running_flag++;
            break;

        case 17: // x�����50mm
            Counter_Times = 80;
            Counter_Enable = 1;
            Car_Go_Target(-50, 0, 200, 150);
            running_flag++;
            break;

        case 18: // y�������400mm
            Counter_Times = 200;
            Counter_Enable = 1;
            Car_Go_Target(0, -400, 300, 150);
            running_flag++;
            break;

        case 19: // ������ת90��
            Counter_Times = turn_time;
            Counter_Enable = 1;
            Car_Turn_NoUse_IMU(90, turn_speed, turn_acc);
            running_flag++;
            break;

        case 20: // У��
            is_turning = 1;
            if (is_turning == 1 && Car_Calibration(200, 150))
            {
                is_turning = 0;
            }
            if (!is_turning)
            {
                running_flag++;
            }
            break;

        case 21: // y�᷽����-1720mm
            Counter_Times = 700;
            Counter_Enable = 1;
            Car_Go_Target(0, -1720, 400, 130);
            running_flag++;
            break;

        case 22: // ������ת90��
            Counter_Times = turn_time;
            Counter_Enable = 1;
            Car_Turn_NoUse_IMU(90, turn_speed, turn_acc);
            running_flag++;
            break;

        case 23: // У��
            is_turning = 1;
            if (is_turning == 1 && Car_Calibration(200, 150))
            {
                is_turning = 0;
            }
            if (!is_turning)
            {
                running_flag++;
            }
            break;

        case 24: // x��ǰ��50mm
            Counter_Times = 80;
            Counter_Enable = 1;
            Car_Go_Target(50, 0, 200, 150);
            running_flag++;
            break;

        case 25: // y�������825mm
            Counter_Times = 350;
            Counter_Enable = 1;
            Car_Go_Target(0, -875 + 0, 300, 150);
            running_flag++;
            break;

        case 26: // y�᷽����-1475mm
            printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
            Counter_Times = 280;
            Counter_Enable = 1;
            Car_Go_Target(0, -1475, 400, 150);
            running_flag++;
            break;

        case 27: // x�᷽����75mm
            printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
            Counter_Times = 280;
            Counter_Enable = 1;
            Car_Go_Target(75, 0, 400, 150);
            running_flag++;
            break;

        case 28:
            running_flag = 0;
            StateMachine_SetState(STATE_IDLE);
            break;
        }
    }
    break;


    case MOVE_TO_RAW:
        if (Counter_Enable == 0 || is_turning == 1)
        {
            switch (running_flag)
            {
            case 0: // ����
                // printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                // printf("t5.txt=\"%d\"\xff\xff\xff", currentState);
                Counter_Times = 200;
                Counter_Enable = 1;
                Car_Go_Target(-150, 0, 50, 50);
                running_flag++;
                // running_flag+=7;
                break;

            case 1: // y����ֱ��1450mm
                // printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                Counter_Times = 600;
                Counter_Enable = 1;
                Car_Go_Target(0, 1450, 160, 150);
                running_flag++;
                break;

            case 2: // x�᷽����50mm��������
                // printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                Counter_Times = 80;
                Counter_Enable = 1;
                Car_Go_Target(30, 0, 200, 150);
                running_flag++;
                break;
            case 3: // ��jetson���͵���ԭ�����ź�
                Jetson_Send(0x04);
                // printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                running_flag++;
                break;
            case 4: // �ȴ�jetson��������
                if (is_wait_jetson_grab == 0)
                {
                    is_wait_jetson_grab = 1;
                    Action_Catch_Obj_To_ZhuanPan(1);
                }
                // printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                break;
            case 5: // �ȴ�jetson��������
                if (is_wait_jetson_grab == 0)
                {
                    // running_flag++;
                    is_wait_jetson_grab = 1;
                    Action_Catch_Obj_To_ZhuanPan(2);
                }
                // printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                break;
            case 6: // �ȴ�jetson��������
                if (is_wait_jetson_grab == 0)
                {
                    // running_flag++;
                    is_wait_jetson_grab = 1;
                    Action_Catch_Obj_To_ZhuanPan(3);
                }
                // printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                break;

            case 7: // �������ж���ɣ�������һ��״̬
                running_flag = 0;
                StateMachine_SetState(MOVE_TO_ROUGH);
                break;
            }
        }

        break;

    case MOVE_TO_ROUGH:
        if (Counter_Enable == 0 || is_turning == 1)
        {
            switch (running_flag)
            {
            case 0: // x�����50mm
                // printf("t5.txt=\"%d\"\xff\xff\xff", currentState);
                // printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                Counter_Times = 80;
                Counter_Enable = 1;
                Car_Go_Target(-50, 0, 200, 150);
                // running_flag+=5;
                running_flag++;
                break;

            case 1: // y�������400mm
                // printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                Counter_Times = 200;
                Counter_Enable = 1;
                Car_Go_Target(0, -430, 300, 150);
                running_flag++;
                break;

            case 2: // ������ת90��
                // printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                Counter_Times = turn_time;
                Counter_Enable = 1;
                Car_Turn_NoUse_IMU(90, turn_speed, turn_acc);
                running_flag++;
                break;
            case 3: // У��
                // printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                is_turning = 1;

                if (is_turning == 1 && Car_Calibration(200, 150))
                {
                    is_turning = 0;
                }

                if (!is_turning)
                {
                    running_flag++;
                }

                break;

            case 4: // y�᷽����-1720mm
                printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                Counter_Times = 1200;
                Counter_Enable = 1;
                Car_Go_Target(0, -1700, 200, 130);
                running_flag++;
                break;
            case 5: // ������ת90��
                // printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                Counter_Times = turn_time;
                Counter_Enable = 1;
                Car_Turn_NoUse_IMU(90, turn_speed, turn_acc);
                running_flag++;
                break;
            case 6: // У��
                // printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                is_turning = 1;

                if (is_turning == 1 && Car_Calibration(200, 150))
                {
                    is_turning = 0;
                }

                if (!is_turning)
                {
                    running_flag++;
                }

                break;
            case 7: // x��ǰ��50mm
                // printf("t5.txt=\"%d\"\xff\xff\xff", currentState);
                // printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                Counter_Times = 80;
                Counter_Enable = 1;
                Car_Go_Target(50, 0, 200, 150);
                running_flag++;
                break;
            case 8:
                // printf("t5.txt=\"%d\"\xff\xff\xff", currentState);

                if (QRCodeString[0] == '2' && QRCodeString[1] == '1' && QRCodeString[2] == '3')
                {
                    if (Action_Put_Obj_To_Map_213())
                    {
                        running_flag++;
                    }
                }
                else if (QRCodeString[0] == '2' && QRCodeString[1] == '3' && QRCodeString[2] == '1')
                {
                    if (Action_Put_Obj_To_Map_231())
                    {
                        running_flag++;
                    }
                }
                else if (QRCodeString[0] == '1' && QRCodeString[1] == '2' && QRCodeString[2] == '3')
                {
                    if (Action_Put_Obj_To_Map_123())
                    {
                        running_flag++;
                    }
                }
                else if (QRCodeString[0] == '1' && QRCodeString[1] == '3' && QRCodeString[2] == '2')
                {
                    if (Action_Put_Obj_To_Map_132())
                    {
                        running_flag++;
                    }
                }
                else if (QRCodeString[0] == '3' && QRCodeString[1] == '1' && QRCodeString[2] == '2')
                {
                    if (Action_Put_Obj_To_Map_312())
                    {
                        running_flag++;
                    }
                }
                else if (QRCodeString[0] == '3' && QRCodeString[1] == '2' && QRCodeString[2] == '1')
                {
                    if (Action_Put_Obj_To_Map_321())
                    {
                        running_flag++;
                    }
                }
                else
                {
                    running_flag++;
                    printf("t8.txt=\"Error: Invalid QR Code\"\xff\xff\xff");
                }
                break;
            case 9: // x����80mm
                // printf("t5.txt=\"%d\"\xff\xff\xff", currentState);
                // printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                Counter_Times = 80;
                Counter_Enable = 1;
                Car_Go_Target(-80, 0, 200, 150);
                running_flag++;
                break;
            case 10:

                // printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                is_turning = 1;

                if (is_turning == 1 && Car_Calibration(200, 150))
                {
                    is_turning = 0;
                }

                if (!is_turning)
                {
                    running_flag = 0;
                    StateMachine_SetState(MOVE_TO_TEMPSTORE);
                }

                break;
            }
        }

        break;

    case MOVE_TO_TEMPSTORE:
        if (Counter_Enable == 0 || is_turning == 1)
        {
            switch (running_flag)
            {
            case 0: // y�������825mm
                 printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                 printf("t5.txt=\"%d\"\xff\xff\xff", currentState);
                Counter_Times = 500;
                Counter_Enable = 1;
                if (QRCodeString[2] == '1')
                {
                    Car_Go_Target(0, -800 + 100, 300, 150);
                }
                else if (QRCodeString[2] == '2')
                {
                    Car_Go_Target(0, -800 + 0, 300, 150);
                }
                else if (QRCodeString[2] == '3')
                {
                    Car_Go_Target(0, -800 + 200, 300, 150);
                }
								else{
								}

                running_flag++;
                break;

            case 1: // ������ת90��
                 printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                is_turning = 1;

                if (is_turning == 1 && Car_Turn_Use_IMU(-90,200, 150))
                {
                    is_turning = 0;
                }

                if (!is_turning)
                {
                    running_flag=running_flag+2;
                }
                break;
            case 2: // У��
                 printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                is_turning = 1;

                if (is_turning == 1 && Car_Calibration(200, 150))
                {
                    is_turning = 0;
                }

                if (!is_turning)
                {
                    running_flag++;
                }

                break;
            case 3: // y�᷽����-860mm
                 printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                Counter_Times = 350;
                Counter_Enable = 1;
                Car_Go_Target(0, -860, 300, 150);
                running_flag++;
                break;
            case 4: // x��ǰ��50mm
                // printf("t5.txt=\"%d\"\xff\xff\xff", currentState);
                // printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                Counter_Times = 80;
                Counter_Enable = 1;
                Car_Go_Target(50, 0, 200, 150);
                running_flag++;
                break;
            case 5:
                 printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);

                if (QRCodeString[0] == '2' && QRCodeString[1] == '1' && QRCodeString[2] == '3')
                {
                    if (Action_Put_Obj_To_Map_213())
                    {
                        running_flag++;
                    }
                }
                else if (QRCodeString[0] == '2' && QRCodeString[1] == '3' && QRCodeString[2] == '1')
                {
                    if (Action_Put_Obj_To_Map_231())
                    {
                        running_flag++;
                    }
                }
                else if (QRCodeString[0] == '1' && QRCodeString[1] == '2' && QRCodeString[2] == '3')
                {
                    if (Action_Put_Obj_To_Map_123_Tempstore())
                    {
                        running_flag++;
                    }
                }
                else if (QRCodeString[0] == '1' && QRCodeString[1] == '3' && QRCodeString[2] == '2')
                {
                    if (Action_Put_Obj_To_Map_132())
                    {
                        running_flag++;
                    }
                }
                else if (QRCodeString[0] == '3' && QRCodeString[1] == '1' && QRCodeString[2] == '2')
                {
                    if (Action_Put_Obj_To_Map_312())
                    {
                        running_flag++;
                    }
                }
                else if (QRCodeString[0] == '3' && QRCodeString[1] == '2' && QRCodeString[2] == '1')
                {
                    if (Action_Put_Obj_To_Map_321_Tempstore())
                    {
                        running_flag++;
                    }
                }
                else
                {
                    printf("t8.txt=\"Error: Invalid QR Code\"\xff\xff\xff");
                }
                break;

            case 6:

                // printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                is_turning = 1;

                if (is_turning == 1 && Car_Calibration(200, 150))
                {
                    is_turning = 0;
                }

                if (!is_turning)
                {
                    running_flag = 0;
                    StateMachine_SetState(MOVE_TO_RAW_AGAIN);
                }

                break;
            }
        }

        break;

    case MOVE_TO_RAW_AGAIN:
        if (Counter_Enable == 0 || is_turning == 1)
        {
            switch (running_flag)
            {
            case 0: // y�������860mm
                // printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                // printf("t5.txt=\"%d\"\xff\xff\xff", currentState);
                Counter_Times = 350;
                Counter_Enable = 1;
                if (QRCodeString[2] == '1')
                {
                    Car_Go_Target(0, -800 + 100, 300, 150);
                }
                else if (QRCodeString[2] == '2')
                {
                    Car_Go_Target(0, -800 + 0, 300, 150);
                }
                else if (QRCodeString[2] == '3')
                {
                    Car_Go_Target(0, -800 + 200, 300, 150);
                }
                running_flag++;
                break;

            case 1: // ������ת90��
                // printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                Counter_Times = turn_time;
                Counter_Enable = 1;
                Car_Turn_NoUse_IMU(90, turn_speed, turn_acc);
                running_flag++;
                break;
            case 2: // У��
                // printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                is_turning = 1;

                if (is_turning == 1 && Car_Calibration(200, 150))
                {
                    is_turning = 0;
                }

                if (!is_turning)
                {
                    running_flag++;
                }

                break;

            case 3: // y�᷽����-425mm
                // printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                Counter_Times = 280;
                Counter_Enable = 1;
                Car_Go_Target(0, -425, 250, 150);
                running_flag++;
                break;
            case 4: // x�᷽����50mm��������
                // printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                Counter_Times = 80;
                Counter_Enable = 1;
                Car_Go_Target(50, 0, 200, 150);
                running_flag++;
                break;
            case 5: // ��jetson���͵���ԭ�����ź�
                Jetson_Send(0x04);
                // printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                running_flag++;
                break;
            case 6: // �ȴ�jetson��������
                if (is_wait_jetson_grab == 0)
                {
                    is_wait_jetson_grab = 1;
                    Action_Catch_Obj_To_ZhuanPan(1);
                }
                // printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                break;
            case 7: // �ȴ�jetson��������
                if (is_wait_jetson_grab == 0)
                {
                    // running_flag++;
                    is_wait_jetson_grab = 1;
                    Action_Catch_Obj_To_ZhuanPan(2);
                }
                // printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                break;
            case 8: // �ȴ�jetson��������
                if (is_wait_jetson_grab == 0)
                {
                    // running_flag++;
                    is_wait_jetson_grab = 1;
                    Action_Catch_Obj_To_ZhuanPan(3);
                }
                // printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                break;
            case 9: // �������ж���ɣ�������һ��״̬
                // printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                is_turning = 1;

                if (is_turning == 1 && Car_Calibration(200, 200))
                {
                    is_turning = 0;
                }

                if (!is_turning)
                {
                    running_flag = 0;
                    StateMachine_SetState(MOVE_TO_ROUGH_AGAIN);
                }

                break;
            }
        }

        break;
    case MOVE_TO_ROUGH_AGAIN:
        if (Counter_Enable == 0 || is_turning == 1)
        {
            switch (running_flag)
            {
            case 0: // x�����50mm
                // printf("t5.txt=\"%d\"\xff\xff\xff", currentState);
                // printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                Counter_Times = 80;
                Counter_Enable = 1;
                Car_Go_Target(-50, 0, 200, 150);
                // running_flag+=5;
                running_flag++;
                break;

            case 1: // y�������400mm
                // printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                Counter_Times = 200;
                Counter_Enable = 1;
                Car_Go_Target(0, -400, 300, 150);
                running_flag++;
                break;

            case 2: // ������ת90��
                // printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                Counter_Times = turn_time;
                Counter_Enable = 1;
                Car_Turn_NoUse_IMU(90, turn_speed, turn_acc);
                running_flag++;
                break;
            case 3: // У��
                // printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                is_turning = 1;

                if (is_turning == 1 && Car_Calibration(200, 150))
                {
                    is_turning = 0;
                }

                if (!is_turning)
                {
                    running_flag++;
                }

                break;

            case 4: // y�᷽����-1720mm
                // printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                Counter_Times = 700;
                Counter_Enable = 1;
                Car_Go_Target(0, -1720, 400, 130);
                running_flag++;
                break;

            case 5: // ������ת90��
                // printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                Counter_Times = turn_time;
                Counter_Enable = 1;
                Car_Turn_NoUse_IMU(90, turn_speed, turn_acc);
                running_flag++;
                break;
            case 6: // У��
                // printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                is_turning = 1;

                if (is_turning == 1 && Car_Calibration(200, 150))
                {
                    is_turning = 0;
                }

                if (!is_turning)
                {
                    running_flag++;
                }

                break;
            case 7: // x��ǰ��50mm
                // printf("t5.txt=\"%d\"\xff\xff\xff", currentState);
                // printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                Counter_Times = 80;
                Counter_Enable = 1;
                Car_Go_Target(50, 0, 200, 150);
                running_flag++;
                break;
            case 8:
                // printf("t5.txt=\"%d\"\xff\xff\xff", currentState);

                if (QRCodeString[4] == '2' && QRCodeString[5] == '1' && QRCodeString[6] == '3')
                {
                    if (Action_Put_Obj_To_Map_213())
                    {
                        running_flag++;
                    }
                }
                else if (QRCodeString[4] == '2' && QRCodeString[5] == '3' && QRCodeString[6] == '1')
                {
                    if (Action_Put_Obj_To_Map_231())
                    {
                        running_flag++;
                    }
                }
                else if (QRCodeString[4] == '1' && QRCodeString[5] == '2' && QRCodeString[6] == '3')
                {
                    if (Action_Put_Obj_To_Map_123())
                    {
                        running_flag++;
                    }
                }
                else if (QRCodeString[4] == '1' && QRCodeString[5] == '3' && QRCodeString[6] == '2')
                {
                    if (Action_Put_Obj_To_Map_132())
                    {
                        running_flag++;
                    }
                }
                else if (QRCodeString[4] == '3' && QRCodeString[5] == '1' && QRCodeString[6] == '2')
                {
                    if (Action_Put_Obj_To_Map_312())
                    {
                        running_flag++;
                    }
                }
                else if (QRCodeString[4] == '3' && QRCodeString[5] == '2' && QRCodeString[6] == '1')
                {
                    if (Action_Put_Obj_To_Map_321())
                    {
                        running_flag++;
                    }
                }
                else
                {
                    printf("t8.txt=\"Error: Invalid QR Code\"\xff\xff\xff");
                }
                break;
            case 9: // �������ж���ɣ�������һ��״̬
                // printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                is_turning = 1;

                if (is_turning == 1 && Car_Calibration(200, 200))
                {
                    is_turning = 0;
                }

                if (!is_turning)
                {
                    running_flag = 0;
                    StateMachine_SetState(MOVE_TO_ROUGH_AGAIN);
                }

                break;
            }
        }

        break;
    case MOVE_TO_TEMPSTORE_AGAIN:
        if (Counter_Enable == 0 || is_turning == 1)
        {
            switch (running_flag)
            {
            case 0: // y�������825mm
                // printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                // printf("t5.txt=\"%d\"\xff\xff\xff", currentState);
                Counter_Times = 350;
                Counter_Enable = 1;
                if (QRCodeString[6] == '1')
                {
                    Car_Go_Target(0, -875 + 100, 300, 150);
                }
                else if (QRCodeString[6] == '2')
                {
                    Car_Go_Target(0, -875 + 0, 300, 150);
                }
                else if (QRCodeString[6] == '3')
                {
                    Car_Go_Target(0, -875 + 200, 300, 150);
                }

                running_flag++;
                break;

            case 1: // ������ת90��
                // printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                Counter_Times = turn_time;
                Counter_Enable = 1;
                Car_Turn_NoUse_IMU(90, turn_speed, turn_acc);
                running_flag++;
                break;
            case 2: // У��
                // printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                is_turning = 1;

                if (is_turning == 1 && Car_Calibration(200, 150))
                {
                    is_turning = 0;
                }

                if (!is_turning)
                {
                    running_flag++;
                }

                break;

            case 3: // y�᷽����-860mm
                // printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                Counter_Times = 350;
                Counter_Enable = 1;
                Car_Go_Target(0, -860, 300, 150);
                running_flag++;
                break;
            case 4:
                // printf("t5.txt=\"%d\"\xff\xff\xff", currentState);

                if (QRCodeString[4] == '2' && QRCodeString[5] == '1' && QRCodeString[6] == '3')
                {
                    if (Action_Put_Obj_To_Map_213())
                    {
                        running_flag++;
                    }
                }
                else if (QRCodeString[4] == '2' && QRCodeString[5] == '3' && QRCodeString[6] == '1')
                {
                    if (Action_Put_Obj_To_Map_231())
                    {
                        running_flag++;
                    }
                }
                else if (QRCodeString[4] == '1' && QRCodeString[5] == '2' && QRCodeString[6] == '3')
                {
                    if (Action_Put_Obj_To_Map_123_Tempstore_Again())
                    {
                        running_flag++;
                    }
                }
                else if (QRCodeString[4] == '1' && QRCodeString[5] == '3' && QRCodeString[6] == '2')
                {
                    if (Action_Put_Obj_To_Map_132())
                    {
                        running_flag++;
                    }
                }
                else if (QRCodeString[4] == '3' && QRCodeString[5] == '1' && QRCodeString[6] == '2')
                {
                    if (Action_Put_Obj_To_Map_312())
                    {
                        running_flag++;
                    }
                }
                else if (QRCodeString[4] == '3' && QRCodeString[5] == '2' && QRCodeString[6] == '1')
                {
                    if (Action_Put_Obj_To_Map_321_Tempstore_Again())
                    {
                        running_flag++;
                    }
                }
                else
                {
                    printf("t8.txt=\"Error: Invalid QR Code\"\xff\xff\xff");
                }
                break;

            case 5: // �������ж���ɣ�������һ��״̬
                // printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                is_turning = 1;

                if (is_turning == 1 && Car_Calibration(200, 200))
                {
                    is_turning = 0;
                }

                if (!is_turning)
                {
                    running_flag = 0;
                    StateMachine_SetState(MOVE_TO_HOME);
                }

                break;
            }
        }

        break;
    case MOVE_TO_HOME:
        if (Counter_Enable == 0 || is_turning == 1)
        {
            switch (running_flag)
            {
            case 0: // y�������860mm
                printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                printf("t5.txt=\"%d\"\xff\xff\xff", currentState);
                Counter_Times = 350;
                Counter_Enable = 1;
                if (QRCodeString[6] == '1')
                {
                    Car_Go_Target(0, -860 + 100, 300, 150);
                }
                else if (QRCodeString[6] == '2')
                {
                    Car_Go_Target(0, -860 + 0, 300, 150);
                }
                else if (QRCodeString[6] == '3')
                {
                    Car_Go_Target(0, -860 + 200, 300, 150);
                }
                running_flag++;
                break;

            case 1: // ������ת90��
                // printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                Counter_Times = turn_time;
                Counter_Enable = 1;
                Car_Turn_NoUse_IMU(90, turn_speed, turn_acc);
                running_flag++;
                break;
            case 2: // У��
                // printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                is_turning = 1;

                if (is_turning == 1 && Car_Calibration(200, 150))
                {
                    is_turning = 0;
                }

                if (!is_turning)
                {
                    running_flag++;
                }

                break;

            case 3: // y�᷽����-1475mm
                printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                Counter_Times = 280;
                Counter_Enable = 1;
                Car_Go_Target(0, -1475, 400, 150);
                running_flag++;
                break;
            case 4: // x�᷽����75mm
                printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                Counter_Times = 280;
                Counter_Enable = 1;
                Car_Go_Target(75, 0, 400, 150);
                running_flag++;
                break;
            case 5:
                running_flag = 0;
                StateMachine_SetState(STATE_IDLE);
                break;
            }
        }

        break;

    default:
        break;
    }
}
