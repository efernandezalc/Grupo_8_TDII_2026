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
#include "API_Delay.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SEC1_TIEMPO   150   /* sec 1: paso de LED a LED          */
#define SEC2_TIEMPO   300   /* sec 2: los 3 LEDs juntos          */
#define SEC3_LD1      100   /* sec 3: periodo propio de LD1      */
#define SEC3_LD2      300   /* sec 3: periodo propio de LD2      */
#define SEC3_LD3      600   /* sec 3: periodo propio de LD3      */
#define SEC4_TIEMPO   150   /* sec 4: LD1/LD3 inversos a LD2     */
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
   buttonStatus_t Boton_Anterior = false; // guarda cómo estaba el boton en la vuelta anterior, arranca en false (no apretado)
   uint8_t secuencia=0; // 0=Sec1, 1=Sec2, 2=Sec3, 3=Sec4
   uint8_t paso = 0;       // paso actual de la Sec1 (0 a 5): par = prende, impar = apaga

   //para cada temporizacion que necesito
    delay_t delaySec1;      // Sec1: 150 ms entre pasos
    delay_t delaySec2;      // Sec2: 300 ms para los 3 juntos
    delay_t delayLD1;       // Sec3: 100 ms para LD1
    delay_t delayLD2;       // Sec3: 300 ms para LD2
    delay_t delayLD3;       // Sec3: 600 ms para LD3
    delay_t delaySec4;      // Sec4: 150 ms

    //solo carga la duracion, no arranca el conteo
      delayInit(&delaySec1, SEC1_TIEMPO);
      delayInit(&delaySec2, SEC2_TIEMPO);
      delayInit(&delayLD1, SEC3_LD1);
      delayInit(&delayLD2, SEC3_LD2);
      delayInit(&delayLD3, SEC3_LD3);
      delayInit(&delaySec4, SEC4_TIEMPO);
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

	  Boton = readButton_GPIO(); //lectura del boton

		   if (Boton ==1 && Boton_Anterior == 0) //comparacion del estado
		   {
		    secuencia++;
		    if (secuencia>3)
		    {
			   secuencia=0;
			 }

		   // apaga los 3 leds para que la secuencia nueva arranque limpia
		    writeLedOff_GPIO(LD1_Pin);
		    writeLedOff_GPIO(LD2_Pin);
		    writeLedOff_GPIO(LD3_Pin);
		    // Reinicio el paso y todos los retardos (el conteo empieza de cero)
		         paso = 0;
		         delayInit(&delaySec1, SEC1_TIEMPO);
		         delayInit(&delaySec2, SEC2_TIEMPO);
		         delayInit(&delayLD1, SEC3_LD1);
		         delayInit(&delayLD2, SEC3_LD2);
		         delayInit(&delayLD3, SEC3_LD3);
		         delayInit(&delaySec4, SEC4_TIEMPO);

		    if (secuencia == 3)// Si la secuencia nueva es la 4, prendemos LD2 asi arranca en fase contraria a LD1/LD3
		    {
		     writeLedOn_GPIO(LD2_Pin);
		    }
		   }

		   Boton_Anterior= Boton; //guardo el estado para comparar en la proxima vuelta


		      if (secuencia == 0)
		      {
		        // Sec 1: cada LED prende y apaga (150 ms por paso).
		        if (delayRead(&delaySec1))           // true cuando pasaron los 150 ms
		        {
		          if (paso % 2 == 0)
		          {
		            writeLedOn_GPIO(leds[paso / 2]);   // paso par: prende el LED correspondiente
		          }
		          else
		          {
		            writeLedOff_GPIO(leds[paso / 2]);  // paso impar: apaga ese mismo LED
		          }

		          paso++;                              // avanzo al paso siguiente
		          if (paso >= 6)
		          {
		            paso = 0;                          // 3 LEDs x (prender + apagar) = 6 pasos
		          }
		        }
		      }


		   else if (secuencia == 1)
		   {
			   // Sec 2: los 3 LEDs parpadean juntos cada 300 ms
			        if (delayRead(&delaySec2))
			        {
			          toggleLed_GPIO(LD1_Pin);
			          toggleLed_GPIO(LD2_Pin);
			          toggleLed_GPIO(LD3_Pin);
			        }
			      }
		   else if (secuencia == 2)
		   {
			   // Sec 3: cada LED alterna con su tiempo (100 / 300 / 600 ms).
			         if (delayRead(&delayLD1)) { toggleLed_GPIO(LD1_Pin); }
			         if (delayRead(&delayLD2)) { toggleLed_GPIO(LD2_Pin); }
			         if (delayRead(&delayLD3)) { toggleLed_GPIO(LD3_Pin); }
		   }

		    else
		    {
		    	// Sec 4: LD1 y LD3 juntos inversos a LD2, alternando cada 150 ms.
		    	     if (delayRead(&delaySec4))
		    	     {
		    	       toggleLed_GPIO(LD1_Pin);
		    	       toggleLed_GPIO(LD3_Pin);
		    	       toggleLed_GPIO(LD2_Pin);
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
