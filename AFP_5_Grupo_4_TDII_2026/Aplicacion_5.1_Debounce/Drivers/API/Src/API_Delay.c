/*
 * API_Delay.c
 *
 *  Created on:
 *      Author: Lenovo
 */

/* Includes ****************************************************************/
#include "main.h"
#include "API_Delay.h"

/* Declaration of variables **************************************************/

/*** Function definition ******************************************************/

/**
  * @brief Inicializa una estructura delay_t con una duración dada.
  * @param delay: puntero a la estructura del retardo
  * @param duration: duración del retardo en milisegundos
  * @retval None
  */
void delayInit(delay_t * delay, tick_t duration)
{
    delay->duration = duration;
    delay->running = false;
}

/**
  * @brief Chequea (sin bloquear) si el tiempo del retardo se cumplió.
  * @param delay: puntero a la estructura del retardo
  * @retval true si el tiempo se cumplió, false si no
  */
bool_t delayRead(delay_t * delay)
{
    if (!delay->running) {
        delay->startTime = HAL_GetTick();
        delay->running = true;
    }

    if (delay->running) {
        if ((HAL_GetTick() - delay->startTime) >= delay->duration) {
            delay->running = false;
            return true;
        }
    }

    return false;
}

/**
  * @brief Cambia la duración de un delay ya existente.
  * @param
  * @param duration: nueva duración en milisegundos
  * @retval None
  */
void delayWrite(delay_t * delay, tick_t duration)
{
    delay->duration = duration;
}
