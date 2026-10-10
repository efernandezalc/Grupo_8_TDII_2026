/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "API_GPIO.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
//static void MX_GPIO_Init(void);
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
   uint16_t leds[3] = {LD1_Pin, LD2_Pin, LD3_Pin}; //vector de leds
   buttonStatus_t Boton;       // estado actual del boton en cada vuelta ( driver)
   buttonStatus_t Boton_Anterior = false; // guarda cómo estaba el botón en la vuelta anterior, arranca en false (no apretado)
   uint8_t secuencia=0; // 0=Sec1, 1=Sec2, 2=Sec3, 3=Sec4
   uint32_t tiempoAnteriorLD1 = 0;   // instante en el que cada led cambia de estado para la secuencia 3
   uint32_t tiempoAnteriorLD2 = 0;
   uint32_t tiempoAnteriorLD3 = 0;
   uint32_t tiempoAnteriorSec4 = 0;
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  uint32_t ahora = HAL_GetTick();   // se lee el tiempo del sistema

	  Boton = readButton_GPIO(); //lectura del boton

		   if (Boton ==1 && Boton_Anterior == 0) //comparacion del estado
		   {
		    secuencia= secuencia+1;
		    if (secuencia>3)
		    {
			   secuencia=0;
			 }

		   // Cada vez que cambia de secuencia, apagamos los 3 leds y reseteamos los relojes, para que la secuencia nueva arranque siempre desde el mismo punto de partida
		    writeLedOff_GPIO(LD1_Pin);
		    writeLedOff_GPIO(LD2_Pin);
		    writeLedOff_GPIO(LD3_Pin);
		    tiempoAnteriorLD1 = ahora;
		    tiempoAnteriorLD2 = ahora;
		    tiempoAnteriorLD3 = ahora;
		    tiempoAnteriorSec4 = ahora;

		    if (secuencia == 3)// Si la secuencia nueva es la 4, prendemos LD2 así arranca en fase contraria a LD1/LD3
		    {
		     writeLedOn_GPIO(LD2_Pin);
		    }
		   }

		   Boton_Anterior= Boton;

		   if (secuencia == 0)
		   {
		     for (uint8_t i = 0; i < 3; i++)
		     {
		    	 writeLedOn_GPIO(leds[i]);
		    	 HAL_Delay(150);
   		         writeLedOff_GPIO(leds[i]);
		         HAL_Delay(150);
		     }
		   }
		   else if (secuencia == 1)
		   {
			   toggleLed_GPIO(LD1_Pin);
			   toggleLed_GPIO(LD2_Pin);
		       toggleLed_GPIO(LD3_Pin);
   	           HAL_Delay(300);
		  		   }
		   else if (secuencia == 2)
		   {
			    if (ahora - tiempoAnteriorLD1 >= 100)   // pasaron 100ms desde el último cambio de LD1
			    {
			      toggleLed_GPIO(LD1_Pin);
			      tiempoAnteriorLD1 = ahora;            // guarda el momento de este cambio
			    }

			    if (ahora - tiempoAnteriorLD2 >= 300)   // pasaron 300ms desde el último cambio de LD2
			    {
			      toggleLed_GPIO(LD2_Pin);
			      tiempoAnteriorLD2 = ahora;
			    }

			    if (ahora - tiempoAnteriorLD3 >= 600)   // pasaron 600ms desde el último cambio de LD3
			    {
			      toggleLed_GPIO(LD3_Pin);
			      tiempoAnteriorLD3 = ahora;
			    }
		   }

		    else
		    {
		    	if (ahora - tiempoAnteriorSec4 >= 150) // cada 150ms, invierte el estado de los 3
			    {
		    	 toggleLed_GPIO(LD1_Pin);
		    	 toggleLed_GPIO(LD3_Pin);
		         toggleLed_GPIO(LD2_Pin);
			      tiempoAnteriorSec4 = ahora;
			    }
		   }
  }
}
  /* USER CODE END 3 */

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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
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
