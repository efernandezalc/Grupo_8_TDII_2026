/*
 * API_GPIO.h
 *
 *  Created on:
 *      Author: Lenovo
 */

#ifndef API_INC_API_GPIO_H_
#define API_INC_API_GPIO_H_

/* Includes ****************************************************************/
#include <stdint.h>    // para usar uint16_t
#include <stdbool.h>   // para usar bool

/* Exported types ***********************************************************/
typedef uint16_t led_t;        // tipo para representar un pin de LED (ej: LD1_Pin)
typedef bool buttonStatus_t;   // tipo para representar el estado del botón (true/false)

/* Exported functions prototypes ********************************************/
void MX_GPIO_Init(void);              // inicializa los pines de LEDs y botón
void writeLedOn_GPIO(led_t LDx);      // enciende el LED indicado
void writeLedOff_GPIO(led_t LDx);     // apaga el LED indicado
void toggleLed_GPIO(led_t LDx);       // invierte el estado del LED indicado
buttonStatus_t readButton_GPIO(void); // lee y devuelve el estado del botón


#endif /* API_INC_API_GPIO_H_ */
