/*
 * API_GPIO.h
 *
 *
 *      Author: Lenovo
 */

#ifndef API_INC_API_GPIO_H_
#define API_INC_API_GPIO_H_

/* Includes ****************************************************************/
#include <stdint.h>   // para usar uint16_t

/* Exported types ***********************************************************/
typedef uint16_t led_t;   // tipo para representar un pin de LED (ej: LD1_Pin)

/* Exported functions prototypes ********************************************/
void MX_GPIO_Init(void);          // inicializa los pines de los LEDs
void writeLedOn_GPIO(led_t LDx);  // enciende el LED indicado
void writeLedOff_GPIO(led_t LDx); // apaga el LED indicado

#endif /* API_INC_API_GPIO_H_ */

