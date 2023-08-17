/* USER CODE BEGIN Header */

/*
 * VERSÃO DE FIRMWARE v0.2
 */

/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "crc.h"
#include "dma.h"
#include "i2c.h"
#include "sai.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#define ARM_MATH_CM4  // Use ARM Cortex M4
#define __FPU_PRESENT 1
#define ARM_MATH_NEON
#include "adc.h"
#include "stdio.h"
#include "string.h"
#include "usbd_cdc_if.h"
#include <stdlib.h>
#include <stdbool.h>
#include"low_pass_filter.h"
#include"low_pass_filter2.h"

#include <arm_math.h>
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

#define NumAmostras 512
/*
 * Uma linha contem NumAmostras amostras
 * Cada amostra é um inteiro de 16 bits (2 bytes)
 * Para representar cada amostra com chars usa-se 12 caracteres (1 sinal, 10 digitos e o espaço ' ')
 * no começo de cada linha há o "\r\n" (2 caracteres)
 * há um "\0" para marcar o fim da string (1 caracter)
 */
#define N 15*NumAmostras
int32_t sai_data[NumAmostras];
float int_sai_data_filter[NumAmostras];
float sai_data_filter[NumAmostras];

uint32_t start_get_time = 0;
uint32_t stop_get_time = 0;

// filtro de ordem 6
//#define N_filter 6
//float a[]= {1,-5.514635515723137,12.689567164247162,-15.594458394508003,10.794044128201001,-3.989699331813231,0.615185058832997};
//float b[]={4.858182486754450e-08,2.914909492052670e-07,7.287273730131675e-07,9.716364973508900e-07,7.287273730131675e-07,2.914909492052670e-07,4.858182486754450e-08};

// filtro de ordem 4
#define N_filter 4
// coeficientes do filtro iir
float a[]=	{1, -3.77704525591331, 5.35565449342871, -3.37862309614059, 0.800061402744814};
float b[]=	{2.97150747683261e-06, 1.18860299073305e-05, 1.78290448609957e-05, 1.18860299073305e-05, 2.97150747683261e-06};

//float  sai_data_f[NumAmostras];
bool cdcTransmitionCplt = 1;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

// função para usar filtro na forma direta
void filterDirectSignal(float *input, float *output, int signalLength) {
    for (int i = N_filter; i < signalLength; i++) {
        output[i] = 0;
		  HAL_GPIO_WritePin(GPIOC, TIME_GET_Pin, GPIO_PIN_SET);

        for (int j = 0; j <= N_filter; j++) {
            output[i] += b[j] * input[i - j];
            if (j > 0) {
                output[i] -= a[j] * output[i - j];
            }
        }
		  HAL_GPIO_WritePin(GPIOC, TIME_GET_Pin, GPIO_PIN_RESET);

    }
}
/*
const int8_t LP_a_bytes[24]={
	0x55,0xd2,0xc5,0xbf,
	0x9a,0x99,0x19,0x3f,
	0xec,0x5,0xd1,0xbf,
	0x17,0xcb,0x30,0x3f,
	0xa6,0xc0,0xe7,0xbf,
	0x13,0xdb,0x5f,0x3f};
const float* LP_a = (float*) LP_a_bytes;

const int8_t LP_b_bytes[36]={0x3a,0x55,0x52,0x36,
	0x5e,0xf4,0xd2,0x36,
	0x24,0x94,0x53,0x36,
	0x0,0x0,0x80,0x3f,
	0xfe,0xfe,0xff,0x3f,
	0xbf,0xfe,0x7f,0x3f,
	0x0,0x0,0x80,0x3f,
	0x4f,0x3f,0xff,0x3f,
	0x5f,0x7f,0x7e,0x3f};
const float* LP_b = (float*) LP_b_bytes;

//Delay Variables
float LP_w[6]={0,0,0,0,0,0};

float LP_Update6(float x)
{
	float y;
	float w;
	//Section 0
	w = x-LP_w[0]*LP_a[0]-LP_w[1]*LP_a[1];
	y = w*LP_b[0]+LP_w[0]*LP_b[1]+LP_w[1]*LP_b[2];
	LP_w[1]=LP_w[0];
	LP_w[0]=w;
	//Section 1
	w = y-LP_w[2]*LP_a[2]-LP_w[3]*LP_a[3];
	y = w*LP_b[3]+LP_w[2]*LP_b[4]+LP_w[3]*LP_b[5];
	LP_w[3]=LP_w[2];
	LP_w[2]=w;
	//Section 2
	w = y-LP_w[4]*LP_a[4]-LP_w[5]*LP_a[5];
	y = w*LP_b[6]+LP_w[4]*LP_b[7]+LP_w[5]*LP_b[8];
	LP_w[5]=LP_w[4];
	LP_w[4]=w;
	return y;
}*/

// função para usar filtro na forma cascata (transfomrmada z dividia em sessões de ordem 2)
//Filter Coefficients
const int8_t LP_a_bytes[16]={
	0xc0,0x38,0x2c,0xbf,
	0x9,0x1,0x14,0x3e,
	0xe9,0xcc,0x65,0xbf,
	0xb9,0xf5,0x6,0x3f};
const float* LP_a = (float*) LP_a_bytes;

const int8_t LP_b_bytes[24]={0x74,0x11,0x98,0x3c,
	0x74,0x11,0x18,0x3d,
	0x75,0x11,0x98,0x3c,
	0x0,0x0,0x80,0x3f,
	0x0,0x0,0x0,0x40,
	0x0,0x0,0x80,0x3f};
const float* LP_b = (float*) LP_b_bytes;

//Delay Variables
float LP_w[4]={0,0,0,0};

float filterCascatetSignal(float x)
{
	float y;
	float w;
	//Section 0
	w = x-LP_w[0]*LP_a[0]-LP_w[1]*LP_a[1];
	y = w*LP_b[0]+LP_w[0]*LP_b[1]+LP_w[1]*LP_b[2];
	LP_w[1]=LP_w[0];
	LP_w[0]=w;
	//Section 1
	w = y-LP_w[2]*LP_a[2]-LP_w[3]*LP_a[3];
	y = w*LP_b[3]+LP_w[2]*LP_b[4]+LP_w[3]*LP_b[5];
	LP_w[3]=LP_w[2];
	LP_w[2]=w;
	return y;
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

/* Configure the peripherals common clocks */
  PeriphCommonClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_CRC_Init();
  MX_SAI1_Init();
  MX_I2C1_Init();
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN 2 */

  adc_mic_config3();

  HAL_SAI_Receive_DMA(&hsai_BlockA1, (uint8_t *)sai_data, NumAmostras);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  int contadorDeAmostras = 1;
  uint8_t last_CDC_return = USBD_OK;

  filter1Type *filtro_low_pass;
  filtro_low_pass = filter1_create();
  filter1_init(filtro_low_pass);//filter1Type_M


  filter2Type *filtro_low_pass2;
  filtro_low_pass2 = filter2_create();
  filter2_init(filtro_low_pass2);



  //DEBUG
//  int contadorDeLinhas = 1;
  while (1)
  {

	  if ( (!HAL_GPIO_ReadPin(BOTAO_GPIO_Port, BOTAO_Pin)) && cdcTransmitionCplt ){

		  char msg[N*10] = {0};
		  char dado_formatado[100] = {0};

		  //DEBUG
//		  sprintf(msg, "\r\n%d %ld ", contadorDeLinhas, sai_data[0]);

		  sprintf(msg, "\r\n%f ", 0.0f);

		  while (contadorDeAmostras < NumAmostras){

			 int_sai_data_filter[contadorDeAmostras]= (float) sai_data[contadorDeAmostras];
			 start_get_time = HAL_GetTick();

// filtro com biblioteca CMSIS
			 filterCMSIS_filterBlock(filtro_low_pass, int_sai_data_filter, sai_data_filter, contadorDeAmostras);
//// filtro com implementação C com ponteiro
//			 filterC_filterBlock(filtro_low_pass2, int_sai_data_filter, sai_data_filter, contadorDeAmostras);
//  filtro com implementação C vetores
//			 sai_data_filter[contadorDeAmostras]= filterCascatetSignal(int_sai_data_filter[contadorDeAmostras]);


			  stop_get_time = start_get_time - HAL_GetTick();
			 //stop_get_time = start_get_time - stop_get_time;

			  sprintf(dado_formatado, "%f ", sai_data_filter[contadorDeAmostras]);
			  strcat(msg, dado_formatado);

			  contadorDeAmostras++;
		  }

		  if (contadorDeAmostras >= NumAmostras){

//// filtro com vetores forma direta
//			  filterDirectSignal(int_sai_data_filter, sai_data_filter, contadorDeAmostras);
//
//			  for(int x = 0 ; x<contadorDeAmostras;x++){
//				  sprintf(dado_formatado, "%f ", sai_data_filter[x]);
//				  strcat(msg, dado_formatado);
//			  }

			  last_CDC_return = CDC_Transmit_FS((uint8_t*)msg, strlen(msg)); // essa funcao nao pode ser chamada muitas vezes seguidas mto rapido
			  // Isso causa falhas e quebras nas linhas e estruturas da mensagem enviada pela USB

			  if(last_CDC_return == USBD_OK){

				  HAL_SAI_Receive_DMA(&hsai_BlockA1, (uint8_t *)sai_data, NumAmostras);

				  contadorDeAmostras = 1;

				  //DEBUG
//				  contadorDeLinhas++;

			  }

		  }

	  }

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
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 20;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief Peripherals Common Clock Configuration
  * @retval None
  */
void PeriphCommonClock_Config(void)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the peripherals clock
  */
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_SAI1|RCC_PERIPHCLK_USB;
  PeriphClkInit.Sai1ClockSelection = RCC_SAI1CLKSOURCE_PLLSAI1;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLLSAI1;
  PeriphClkInit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_HSE;
  PeriphClkInit.PLLSAI1.PLLSAI1M = 1;
  PeriphClkInit.PLLSAI1.PLLSAI1N = 12;
  PeriphClkInit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_SAI1CLK|RCC_PLLSAI1_48M2CLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void HAL_SAI_RxCpltCallback(SAI_HandleTypeDef *hsai) {
//
//	HAL_SAI_Receive_DMA(&hsai_BlockA1, (uint8_t *)sai_data, NumAmostras);

}

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

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
