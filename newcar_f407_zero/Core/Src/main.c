/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "can.h"
#include "dma.h"
#include "iwdg.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
// #include "2_Device/Motor/Motor_ZDT42/Emm_V5.h"
#include "2_Device/Motor/Motor_ZDT42/ZDT_X42_V2.h"
#include "arm_math.h"
#include "2_Device/Vofa/vofa.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
bool  init_finished;
float a = 0.0f;
float b = 0.0f;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_CAN1_Init();
  MX_UART4_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_IWDG_Init();
  MX_TIM4_Init();
  MX_TIM5_Init();
  MX_UART5_Init();
  /* USER CODE BEGIN 2 */
    init_finished = true;
    USER_CAN1_Filter_Init();   // 初始化CAN滤波器
    if (HAL_CAN_Start(&hcan1) != HAL_OK)
    {
        Error_Handler();
    }   // 启动CAN控制器
    if (HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
    {
        Error_Handler();
    }   // 使能CAN控制器接收中断

    // /**********************************************************
    //  ***	上电延时2秒等待Emm_V5.0闭环初始化完毕
    //  **********************************************************/
    // HAL_Delay(2000);

    // /**********************************************************
    // ***
    // *地址1电机：位置模式，方向CW，速度1000RPM，加速度0（不使用加减速直接启动），脉冲数3200（16细分下发送3200个脉冲电机转一圈），相对运动
    // **********************************************************/
    // Emm_V5_Pos_Control(1, 0, 1000, 200, 3200 * 16, 0, 1);   // 多机同步标志位置1
    // HAL_Delay(10);                                          // 每条命令后面延时10毫秒，防止粘包

    // /**********************************************************
    // ***
    // *地址2电机：位置模式，方向CW，速度1000RPM，加速度0（不使用加减速直接启动），脉冲数3200（16细分下发送3200个脉冲电机转一圈），相对运动
    // **********************************************************/
    // Emm_V5_Pos_Control(2, 0, 1000, 0, 3200, 0, 1);   // 多机同步标志位置1
    // HAL_Delay(10);                                   // 每条命令后面延时10毫秒，防止粘包

    // /**********************************************************
    // ***
    // *地址3电机：位置模式，方向CW，速度1000RPM，加速度0（不使用加减速直接启动），脉冲数3200（16细分下发送3200个脉冲电机转一圈），相对运动
    // **********************************************************/
    // Emm_V5_Pos_Control(3, 0, 1000, 0, 3200, 0, 1);   // 多机同步标志位置1
    // HAL_Delay(10);                                   // 每条命令后面延时10毫秒，防止粘包

    // /**********************************************************
    // ***
    // *地址4电机：位置模式，方向CW，速度1000RPM，加速度0（不使用加减速直接启动），脉冲数3200（16细分下发送3200个脉冲电机转一圈），相对运动
    // **********************************************************/
    // Emm_V5_Pos_Control(4, 0, 1000, 0, 3200, 0, 1);   // 多机同步标志位置1
    // HAL_Delay(10);                                   // 每条命令后面延时10毫秒，防止粘包

    // /**********************************************************
    // ***	触发多机同步开始运动
    // **********************************************************/
    // Emm_V5_Synchronous_motion(0);   // 广播地址0触发
    // HAL_Delay(10);                  // 每条命令后面延时10毫秒，防止粘包

    // /**********************************************************
    //  *	可以在电机小屏幕Response菜单中开启到位返回命令（Reached）
    //  *	开启后，电机到位后会返回到位命令：0xFD + 0x9F + 0x6B（帧ID为电机地址左移8位）
    //  *	注意，不要开启多个电机到位返回，不然可能会导致总线冲突
    //  **********************************************************/

    // /**********************************************************
    //  *	等待返回到位命令
    //  ***	返回数据缓存在can.rxData数组上，长度为can.CAN_RxMsg.DLC,
    //  ***	电机地址为帧ID右移8位：can.CAN_RxMsg.ExtId >> 8
    //  **********************************************************/
    // while (can.rxData[0] != 0xFD || can.rxData[1] != 0x9F);
    // can.rxFrameFlag = false;

    /**********************************************************
    ***  假设有地址1和地址2两个电机，假设需要：
    * 地址1电机：直通限速位置模式，加速度为1000RPM/S，速度1500RPM，相对运动-3600.0°
    * 地址2电机：直通限速位置模式，加速度为1500RPM/S，速度1000RPM，绝对运动 7200.0°
    * 地址1和地址2电机要同时开始运动
    **********************************************************/

    // 地址1电机：直通限速位置模式，加速度为1000RPM/S，速度1500RPM，相对运动-3600.0°
    // ZDT_X42_V2_Traj_Position_Control(1, 1, 200, 200, 500, 360,1,0); // 多机同步标志置1
    // // 等待返回命令，命令数据缓存在数组rxCmd上，长度为rxCount
    // if (ZDT_X42_V2_Receive_Data_Right())
    // {
    //     //HAL_Delay(10);
    //     ZDT_X42_V2_Traj_Position_Control(1, 1, 100, 100, 500, 360*3,0,1);
    //     HAL_Delay(1);
    //     ZDT_X42_V2_Synchronous_motion(0); // 注意：用广播地址0发送命令，只有地址为1的电机会返回命令，其他地址不返回
    // }
    // ZDT_X42_V2_Traj_Position_Control(1, 1, 200, 200, 500, 360,1,0); // 多机同步标志置1

    // 用广播地址0发送命令触发多机同步开始运动，则地址1和地址2电机要同时开始运动，其余电机不会动


    // 等待返回命令，命令数据缓存在数组rxCmd上，长度为rxCount
    // if (ZDT_X42_V2_Receive_Data_Right()){  HAL_Delay(10);    }

    ZDT_X42_V2_Traj_Position_Control(1, 1, 1000, 1000, 2000, 0, 1, 0);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    while (1)
    {
        // ZDT_X42_V2_Traj_Position_Control(1, 1, 500, 500, 2000, 360000,1,0); // 多机同步标志置1
        //a += 0.1;
        //b = arm_sin_f32(a);
        HAL_IWDG_Refresh(&hiwdg);
        float f1=0.5,f2=114.514;
        Vofa_FireWater("%f,%f\r\n", f1, f2);
        //Vofa_FireWater("%f,%f\r\n", a, b);
        //HAL_Delay(100);
        // if (ZDT_X42_V2_Receive_Data_Right())
        // {
        //     ZDT_X42_V2_Traj_Position_Control(1, 1, 500, 500, 2000, b,0,1);
        //     HAL_Delay(1);
        //     ZDT_X42_V2_Synchronous_motion(0); //
        //     注意：用广播地址0发送命令，只有地址为1的电机会返回命令，其他地址不返回
        // }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1)
    {}
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
