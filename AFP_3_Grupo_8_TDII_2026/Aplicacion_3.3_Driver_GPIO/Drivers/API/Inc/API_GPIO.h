/*
 * API_GPIO.h
 *
 *  Created on: Jul 22, 2026
 *      Author: Lenovo
 *      Funtion of driver: driver que contiene funciones para manejo de puertos GPIO que utiliza funciones de la HAL de STM32
 */


#ifndef API_INC_API_GPIO_H_
#define API_INC_API_GPIO_H_

/* Includes ****************************************************************/
#include <stdint.h>   // para poder usar uint16_t
#include <stdbool.h>  // para poder usar bool

/* Exported types *************************************************************************************************************/
typedef uint16_t led_t;        // tipo para representar un pin de LED (ej: LD1_Pin)
typedef bool buttonStatus_t;   // tipo para representar el estado del botón (true/false)

/* Exported functions prototypes *********************************************************************************************/
void MX_GPIO_Init(void);              // inicializa los pines de LEDs y botón
void writeLedOn_GPIO(led_t LDx);      // enciende el LED indicado
void writeLedOff_GPIO(led_t LDx);     // apaga el LED indicado
void toggleLed_GPIO(led_t LDx);       // invierte el estado actual del LED indicado
buttonStatus_t readButton_GPIO(void); // lee y devuelve el estado del botón (presionado o no


#endif /* API_INC_API_GPIO_H_ */
