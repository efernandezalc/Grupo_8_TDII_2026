/*
 * API_GPIO.c
 *
 *  Created on: Jul 22, 2026
 *      Author: Lenovo
 */

/* Includes *********************************************************************************************************************/
#include "main.h"
#include "API_GPIO.h"
/*Defines ***********************************************************************************************************************/

/* Declaration of variables *****************************************************************************************************/

/*** Function definition ********************************************************************************************************/
/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
 void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LD1_Pin|LD3_Pin|LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : Boton_Pin */
  GPIO_InitStruct.Pin = Boton_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(Boton_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD1_Pin LD3_Pin LD2_Pin */
  GPIO_InitStruct.Pin = LD1_Pin|LD3_Pin|LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

 /**
   * @brief Enciende un LED
   * @param LDx: pin del LED a encender (ej: LD1_Pin)
   * @retval None
   */
 void writeLedOn_GPIO(led_t LDx)
 {
     HAL_GPIO_WritePin(GPIOB, LDx, GPIO_PIN_SET);
 }

 /**
   * @brief Apaga un LED
   * @param LDx: pin del LED a apagar
   * @retval None
   */
 void writeLedOff_GPIO(led_t LDx)
 {
     HAL_GPIO_WritePin(GPIOB, LDx, GPIO_PIN_RESET);
 }

 /**
   * @brief Invierte el estado actual de un LED
   * @param LDx: pin del LED a invertir
   * @retval None
   */
 void toggleLed_GPIO(led_t LDx)
 {
     HAL_GPIO_TogglePin(GPIOB, LDx);
 }

 /**
   * @brief Lee el estado del pulsador de la placa
   * @param none
   * @retval true si está presionado, false si no
   */
  buttonStatus_t readButton_GPIO(void)
 {
     return HAL_GPIO_ReadPin(GPIOC, Boton_Pin);
 }
