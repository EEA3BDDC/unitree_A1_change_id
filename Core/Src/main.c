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
#include "adc.h"
#include "can.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "BMI088driver.h"
#include <unitree_A1_change_ID.h>
#include "bsp_can.h"
#include "dm_motor_drv.h"
#include "dm_motor_ctrl.h"

#include "odrive_can.h"
#include "ros_main.h"
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
uint8_t rebuf[12];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{	
        HAL_UART_Receive_DMA(&huart1,(uint8_t *)rebuf,12); // ????DMA??  
}

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
  MX_CAN2_Init();
  MX_SPI1_Init();
  MX_UART4_Init();
  MX_UART5_Init();
  MX_ADC1_Init();
  MX_TIM4_Init();
  MX_TIM5_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_USART6_UART_Init();
  MX_USB_DEVICE_Init();
  MX_TIM12_Init();
  MX_TIM6_Init();
  /* USER CODE BEGIN 2 */
	setup();//ros communicate sys
	
	while(BMI088_init()){}
//	startup_sing();	

//	dm_motor_init();
//	write_motor_data(motor[Motor1].id, 10, 1, 0, 0, 0);// hack ,i feel nosense
//	HAL_Delay(1);
//	save_motor_data(motor[Motor1].id, 10);
//	HAL_Delay(1);

		
	HAL_GPIO_WritePin(dir1_485_GPIO_Port,dir1_485_Pin,1);//TX mode
	HAL_GPIO_WritePin(dir2_485_GPIO_Port,dir2_485_Pin,1);	
		
	HAL_GPIO_WritePin(led_r_GPIO_Port,led_r_Pin,1); //turn off red led;
	HAL_GPIO_WritePin(led_g_GPIO_Port,led_g_Pin,1);
	HAL_GPIO_WritePin(led_b_GPIO_Port,led_b_Pin,1);		
	while(1){
		HAL_Delay(200);
		HAL_GPIO_TogglePin(led_r_GPIO_Port,led_r_Pin);
	
		if(HAL_GPIO_ReadPin(key_c_GPIO_Port,key_c_Pin) == GPIO_PIN_SET){ //if all the motor has power supply
			HAL_GPIO_WritePin(led_r_GPIO_Port,led_r_Pin,1); //turn off red led;
			break;
		}
	}
	
	while(1){ //waiting person start mission
		HAL_Delay(200);
		HAL_GPIO_TogglePin(led_r_GPIO_Port,led_r_Pin);
		HAL_GPIO_TogglePin(led_g_GPIO_Port,led_g_Pin);
		
		if(HAL_GPIO_ReadPin(key_d_GPIO_Port,key_d_Pin) == GPIO_PIN_SET){
			HAL_GPIO_WritePin(led_r_GPIO_Port,led_r_Pin,1); //turn off red led;
//		bsp_can_init();
//			dm_motor_enable(&hcan1, &motor[Motor1]);
//			dm_motor_enable(&hcan2, &motor[Motor1]);
			change_A1_id();
			
			HAL_GPIO_WritePin(dir1_485_GPIO_Port,dir1_485_Pin,0);//RX mode
			HAL_GPIO_WritePin(dir2_485_GPIO_Port,dir2_485_Pin,0);	
			//HAL_TIM_Base_Start_IT(&htim6);// 50hz event
			
			//HAL_UART_Receive_DMA(&huart5,(uint8_t *)rebuf,12);
			break;
		}
	}
	
		

	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		loop();
		
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
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
  {
  }
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