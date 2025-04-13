### 结构

Core，Drivers，MDK-ARM为CubeMX生成代码

App,BSP,Middleware,System为用户代码

keilkill.bat清空编译文件

f407_test.ioc为CubeMX工程

### 功能

USART1--PA9，PA10，Debug串口和串口屏。

USART2--PA2，PA3，开启DMA（normal），Idle中断回调接受imu传输MSG_SYS_STATE和MSG_EULER_ORIEN两个数组并且解析出Roll，Pitch和Heading(Yaw)。

USART3--PB10，PB11，步进电机控制

UART4--PA0，PA1 ,总线舵机

UART5--PC12 ,PD2上位机通讯

USART6--PC6 ,PC7，备用



