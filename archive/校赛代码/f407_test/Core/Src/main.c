/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
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
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "debug_mode.h"
#include "motor_ctrl.h"
#include "imu.h"
#include "staMachine.h"
#include "jetson.h"
#include "ui.h"
#include "servo.h"
#include "user_uart.h"
#include <stdio.h>
#include "usr_tim.h"
#include "action.h"
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

// //解码数据结构体
// MSG_SYS_STATE MSSDecodedData;

// //接收数据数组
// uint8_t MSG_SYS_STATE_Buf[MSG_SYS_STATE_LEN];
// uint8_t MSG_EULER_ORIEN_Buf[DATA_LEN(MSG_EULER_ORIEN_LEN)];

// //储存数据数组
// uint8_t MEO_Data[MSG_EULER_ORIEN_LEN];
// extern MSG_EULER_ORIEN MEO_Struct;

uint8_t stop = 0;
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
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_TIM6_Init();
  MX_USART3_UART_Init();
  MX_UART5_Init();
  MX_UART4_Init();
  /* USER CODE BEGIN 2 */
  // HAL_UARTEx_ReceiveToIdle_DMA(&huart2, (uint8_t *)MSG_EULER_ORIEN_Buf, sizeof(MSG_EULER_ORIEN_Buf));
  //  初始化
  IMU_Data_Init();
  Motor_Init();
  User_Uart_Init(&huart4);
  Motor_Enable_All();
  Jetson_Init();
  UI_Init();

  // Car_Turn(90,200,250);
  // Car_Go(90,1000,10000,250);
  // Car_Go_Target(10000,0,200,150);
  // Delay_ms(13000);
  // Car_Go_Target(-75, 75, 200, 150);

  StateMachine_Init();
  // HuaGui_JiaoZhun();
  //  启动TIM6定时器
  HAL_TIM_Base_Start_IT(&htim6);
  // FSUSExample_SetServoAngle();
  // Servo_Control(0,15,200);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  // Motor_SetPosition_A(5, 0, 1000, 200);
  // Jetson_Send();
  // HuaGui_Get_From_Map(1000,230);
  // HuaGui_Fang_To_Map_2(1000,230);
  // HuaGui_ZhuanPan(1000,230);
  // Action_Put_Obj_To_Map(1);
  // Delay_ms(3000);
  // Action_Get_Obj_From_Map(1);
  // Forward();
  // Car_Go_Target(0, -200, 100, 150);
  // Car_Turn_NoUse_IMU(90,400,220);
  // Delay_ms(3000);
  // IMU_Data_Proc();
  // Delay_ms(500);
//Action_Catch_Obj_To_ZhuanPan(1);
  while (1)
  { // Car_Turn(90,400,220);
    //		Car_Go_Target(0, 80, 200, 150);
    // Delay_ms(10000);
    //		Car_Go_Target(0, -80, 200, 150);
    //		 Delay_ms(5000);
    //
    //  Car_Go_Target(150, 0, 50, 50);
    // Delay_ms(7000);
    //  Car_Go_Target(-150, 0, 50, 50);
    //   Delay_ms(7000);
    // Car_Go_Target(0, -1700, 400, 130);
    // Delay_ms(100000);

    // Car_Calibration();

    // Jetson_Send();
    // HAL_UART_Transmit(&huart5, (uint8_t *)"hello 1!\r\n", 16, 0xffff);
    // printf("turn right \r\n");
    // printf("Yaw Angle: %f\n", MEO_Struct.Heading);
    // Car_Turn(90,200,250);

    // Delay_ms(5);
    // Action_Catch_And_Put_First_Obj();

    //		    HuaGui_UP(1000, 230);
    //				Delay_ms(9000);
    //		   HuaGui_ZhuanPan(1000, 230);
    //				Delay_ms(9000);
    
//    if (End_flag == 1)
//    {
//			
//      if (stop == 0 && Car_Turn_Use_IMU(90,200,150))
//      {
//        stop = 1;
//      }
//      if (stop == 1)
//      {
//				//Car_Calibration(200,150);
//        Car_Go_Target(0, 80, 10, 150);
//      }
//      IMU_Data_Proc();
//    }

    // HAL_UART_Transmit(&huart1, (uint8_t *)"hello 1!\r\n", 16, 0xffff);
    if (End_flag == 1)
    {
      StateMachine_Update();
      IMU_Data_Proc();
    }
  }

  /* USER CODE END WHILE */
	
  /* USER CODE BEGIN 3 */
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV4;

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
  {
  }

  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
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
