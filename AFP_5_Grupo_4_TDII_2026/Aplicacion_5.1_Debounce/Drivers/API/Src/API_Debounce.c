/*
 * API_Debounce.c
 *
 *  Created on:
 *      Author: Lenovo
 */


#include "API_debounce.h"
#include "API_Delay.h"
#include "main.h"          /* HAL y los labels del .ioc: LD1_Pin, B1_Pin, etc. */

/* ------------------------------ Defines ------------------------------- */

/* Tiempo de anti-rebote pedido por la consigna */
#define DEBOUNCE_TIME_MS   40

/* Nivel lógico que lee el pin cuando el pulsador está PRESIONADO.
 * En la NUCLEO-144 (F413ZH) el pulsador B1 lee 1 al presionarlo.
 * Si en otra placa funcionara al revés, solo hay que cambiar a GPIO_PIN_RESET. */
#define BOTON_ACTIVO       GPIO_PIN_SET

/* ------------------------ Declaraciones privadas ---------------------- */

/* Los 4 estados de la MEF (privados: solo se usan dentro de este archivo):
 *  BUTTON_UP      -> pulsador suelto (estado de reposo)
 *  BUTTON_FALLING -> se detectó que se apretó; esperando 40 ms para confirmar
 *  BUTTON_DOWN    -> pulsador apretado (ya confirmado)
 *  BUTTON_RISING  -> se detectó que se soltó; esperando 40 ms para confirmar */
typedef enum {
    BUTTON_UP,
    BUTTON_FALLING,
    BUTTON_DOWN,
    BUTTON_RISING
} debounceState_t;

/* "static" = la variable existe solo en este archivo; el main.c no la ve. */
static debounceState_t estadoActual;       /* en qué estado está la MEF ahora */
static delay_t         delayAntirrebote;   /* retardo no bloqueante de 40 ms */
static bool_t          teclaPresionada;    /* true = hubo una pulsación sin informar */

/* ------------------------- Prototipos privados ------------------------ */
static bool_t leerPulsador(void);   /* true si el pulsador está apretado */
static void   buttonPressed(void);  /* acción al confirmarse la pulsación */
static void   buttonReleased(void); /* acción al confirmarse que se soltó */

/* ------------------------- Funciones públicas ------------------------- */

void debounceFSM_init(void)
{
    /* Arranca en reposo: pulsador suelto */
    estadoActual = BUTTON_UP;

    /* Configura el retardo de 40 ms (todavía no empieza a contar) */
    delayInit(&delayAntirrebote, DEBOUNCE_TIME_MS);

    /* Todavía no hubo ninguna pulsación */
    teclaPresionada = false;

    /* Estado inicial de las salidas: los 3 LEDs apagados */
    HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
}

void debounceFSM_update(void)
{
    /* En cada llamada se mira en qué estado está la MEF y se evalúa
     * solo la condición de salida de ESE estado. */
    switch (estadoActual) {

    case BUTTON_UP:
        /* Pulsador suelto. Si aparece un nivel "apretado", puede ser una
         * pulsación real o puro rebote: todavía no se sabe. */
        if (leerPulsador()) {
            estadoActual = BUTTON_FALLING;
            /* Primera llamada a delayRead: arranca la cuenta de 40 ms */
            delayRead(&delayAntirrebote);
        }
        break;

    case BUTTON_FALLING:
        /* Se espera a que pasen los 40 ms. Mientras no pasen, delayRead
         * devuelve false y no se hace nada (no se bloquea el programa). */
        if (delayRead(&delayAntirrebote)) {
            if (leerPulsador()) {
                /* Sigue apretado tras 40 ms: la pulsación es real */
                estadoActual = BUTTON_DOWN;
                buttonPressed();
            } else {
                /* Ya no está apretado: era rebote o ruido, se descarta */
                estadoActual = BUTTON_UP;
            }
        }
        break;

    case BUTTON_DOWN:
        /* Pulsador apretado. Si aparece un nivel "suelto", puede ser que
         * lo hayan soltado o un rebote: hay que confirmar. */
        if (!leerPulsador()) {
            estadoActual = BUTTON_RISING;
            delayRead(&delayAntirrebote);   /* arranca la cuenta de 40 ms */
        }
        break;

    case BUTTON_RISING:
        /* Se espera a que pasen los 40 ms */
        if (delayRead(&delayAntirrebote)) {
            if (!leerPulsador()) {
                /* Sigue suelto tras 40 ms: se soltó de verdad */
                estadoActual = BUTTON_UP;
                buttonReleased();
            } else {
                /* Volvió a estar apretado: era rebote, sigue apretado */
                estadoActual = BUTTON_DOWN;
            }
        }
        break;

    default:
        /* Nunca debería pasar. Si la variable de estado se corrompe,
         * se reinicia la MEF para volver a un estado seguro. */
        debounceFSM_init();
        break;
    }
}

bool_t readKey(void)
{
    /* Si hay una pulsación sin informar, se informa y se borra la bandera,
     * así la misma pulsación no se devuelve dos veces. */
    if (teclaPresionada) {
        teclaPresionada = false;
        return true;
    }
    return false;
}

/* ------------------------- Funciones privadas ------------------------- */

/* Lee el pin del pulsador B1. Devuelve true si está apretado. */
static bool_t leerPulsador(void)
{
    return (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == BOTON_ACTIVO);
}

/* Se ejecuta una vez por pulsación confirmada:
 * invierte LED1 (consigna) y avisa a readKey() que hubo una pulsación. */
static void buttonPressed(void)
{
    HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);
    teclaPresionada = true;
}

/* Se ejecuta una vez cuando se confirma que se soltó: invierte LED3. */
static void buttonReleased(void)
{
    HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
}
