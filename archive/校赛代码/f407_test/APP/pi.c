//#include "pi.h"
//#include "imu.h"
//#include <math.h>
//extern MSG_EULER_ORIEN MEO_Struct;
//void PI_Controller_Init(PI_Controller *controller, float Kp, float Ki)
//{
//    controller->Kp = Kp;
//    controller->Ki = Ki;
//    controller->integral = 0.0f;
//}

//float PI_Controller_Update(PI_Controller *controller, float error)
//{
//    controller->integral += error;
//    return controller->Kp * error + controller->Ki * controller->integral;
//}

//void StraightLine_Control(uint16_t PassTick)
//{
//    static uint8_t flag = 0;
//    static PI_Controller anglePID;
//    static float targetAngle = 0.0f;

//    if (!flag)  // 以IMU初始化角度作为小车初始角度
//    {
//        flag = 1;
//        targetAngle = 0.0f;  // 目标角度设为0
//        PI_Controller_Init(&anglePID, 1.0f, 0.1f);  // 初始化PI控制器，Kp和Ki根据需要调整
//    }

//    float currentAngle =MEO_Struct.Heading;  // 获取当前角度
//    float error = targetAngle - currentAngle;

//    if (error >= 180) error -= 360;
//    else if (error <= -180) error += 360;

//    float output = PI_Controller_Update(&anglePID, error);

//    // 限幅
//    if (output > 100) output = 100;
//    else if (output < -100) output = -100;

//    // 调用函数控制小车的运动
//    SetMotorSpeed(output);
//}
