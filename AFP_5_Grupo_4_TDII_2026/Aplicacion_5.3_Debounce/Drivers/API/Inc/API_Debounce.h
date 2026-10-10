/*
 * API_Debounce.h
 *
 *  Created on:
 *      Author: Lenovo
 */

#ifndef API_INC_API_DEBOUNCE_H_
#define API_INC_API_DEBOUNCE_H_


/*Includes ***********************************************************************/
#include "stdint.h"
#include "stdbool.h"
#include "API_Delay.h"


/*Prototipos de funciones publicas ***************************************************/

/* Carga el estado inicial de la MEF, el delay de 40 ms y apaga los LEDs */
void debounceFSM_init(void);
// Lee el pulsador, resuelve las transiciones de la MEF y actualiza salidas

void debounceFSM_update(void);

// Devuelve true si se detecta una tecla presionada (flanco descendente)

bool_t readKey(void);



#endif /* API_INC_API_DEBOUNCE_H_ */
