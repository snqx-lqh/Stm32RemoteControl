/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "nrf24l01.h"
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

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#define RX_BUFF_LEN  32  
uint8_t rxBuff[RX_BUFF_LEN]={0};//定义一个全局变量

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)  
{  
    if(huart->Instance == USART1)//假如有串口1触发接收中断  
    {  
	   HAL_UART_Receive_IT(&huart1,rxBuff,32);  
       HAL_UART_Transmit(&huart1,rxBuff,sizeof(rxBuff),100);
    }  
        
}


int16_t check_nrf = 0;
uint8_t check_rx = 0;
uint8_t nrf_test[32] = {0};

typedef struct{
	uint16_t lx_value;
	uint16_t ly_value;
	uint16_t rx_value;
	uint16_t ry_value;
	
	uint16_t gyrox;
	uint16_t gyroy;
	uint16_t gyroz;
	
	uint16_t accx;
	uint16_t accy;
	uint16_t accz;
	
	uint8_t key_value;
	
	float anglex;
	float angley;
	float anglez;
	
}RC_Ctrl_t;

RC_Ctrl_t RC_Ctrl;

typedef union {
	uint8_t uchar_data[4];
	float   float_data;
}uchar_to_float_t;

uchar_to_float_t uchar_to_float;

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	uint8_t reLen = 0;						// 接收到的数据长度
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
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
	__HAL_UART_ENABLE_IT(&huart1,UART_IT_RXNE);
	HAL_UART_Receive_IT(&huart1,rxBuff,32); 
	NRF24L01_Gpio_Init();
	NRF24L01_Init();
	check_nrf = NRF24L01_Check();
	NRF24L01_Set_Mode( MODE_RX );		
//	NRF24L01_RX_Mode();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	reLen = NRF24L01_RxPacket( nrf_test );		// 接收字节
	if( 0 != reLen )
	{
		//printf("rxData is: %d%s \r\n", nrf24l01RxBuffer[0], &nrf24l01RxBuffer[1]);
	}
	if(nrf_test[0] == 0xA3 && nrf_test[28] == 0X3A)
	{
		RC_Ctrl.lx_value = (nrf_test[1]<<8) + nrf_test[2];
		RC_Ctrl.ly_value = (nrf_test[3]<<8) + nrf_test[4];
		RC_Ctrl.rx_value = (nrf_test[5]<<8) + nrf_test[6];
		RC_Ctrl.ry_value = (nrf_test[7]<<8) + nrf_test[8];
		
		RC_Ctrl.gyrox = (nrf_test[9]<<8) + nrf_test[10];
		RC_Ctrl.gyroy = (nrf_test[11]<<8) + nrf_test[12];
		RC_Ctrl.gyroz = (nrf_test[13]<<8) + nrf_test[14];
		
//			RC_Ctrl.accx = (nrf_test[15]<<8) + nrf_test[16];
//			RC_Ctrl.accy = (nrf_test[17]<<8) + nrf_test[18];
//			RC_Ctrl.accz = (nrf_test[19]<<8) + nrf_test[20];
		
		RC_Ctrl.key_value = nrf_test[15];
		
		uchar_to_float.uchar_data[0] = nrf_test[16];
		uchar_to_float.uchar_data[1] = nrf_test[17];
		uchar_to_float.uchar_data[2] = nrf_test[18];
		uchar_to_float.uchar_data[3] = nrf_test[19];
		RC_Ctrl.anglex = uchar_to_float.float_data;
		
		uchar_to_float.uchar_data[0] = nrf_test[20];
		uchar_to_float.uchar_data[1] = nrf_test[21];
		uchar_to_float.uchar_data[2] = nrf_test[22];
		uchar_to_float.uchar_data[3] = nrf_test[23];
		RC_Ctrl.angley = uchar_to_float.float_data;
		
		uchar_to_float.uchar_data[0] = nrf_test[24];
		uchar_to_float.uchar_data[1] = nrf_test[25];
		uchar_to_float.uchar_data[2] = nrf_test[26];
		uchar_to_float.uchar_data[3] = nrf_test[27];
		RC_Ctrl.anglez = uchar_to_float.float_data;
		
	}
//	PAout(1) = 0;
//	HAL_Delay(1000);
//	PAout(1) = 1;
//	HAL_Delay(1000);
	//HAL_Delay(5);
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
