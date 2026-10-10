/*
 * API_Debounce.c
 *
 *  Created on:
 *      Author: Lenovo
 */

/*
 * API_Debounce.c
 *
 *  Created on:
 *      Author: Lenovo
 */
/*Includes************************************************************************/
#include "main.h"
#include "API_Debounce.h"
#include "API_GPIO.h"


/*Defines **********************************************************************/
#define DEBOUNCE_DELAY 40  // tiempo de anti-rebote en ms

/* Declaraciones privadas **************************************************/

// Estados de la MEF
typedef enum {
	BUTTON_UP,        // pulsador suelto
	BUTTON_FALLING,   // esperando confirmar a los 40 ms (pulso)
	BUTTON_DOWN,      // pulsador presionado
	BUTTON_RISING     // esperando confirmar a los 40 ms (liberacion)
} debounceState_t;

/*Declaracion de variables (globales)******************************************************/

static debounceState_t actualState;   // estado actual de la MEF
static bool_t keyPressed = false;     // true cuando se confirma una pulsacion
static bool_t fallingEdge = false;    // true cuando ocurre el flanco descendente
static delay_t debounceDelay;         // delay no bloqueante de 40 ms
static buttonStatus_t buttonRead = false; // ultima lectura del pulsador (true = presionado)

/*Prototipos de funciones privadas ***********************************************************/
void buttonPressed(void);    // evento: tecla presionada
void buttonReleased(void);   // evento: tecla liberada

/*Define functions ***************************************************************/

bool_t readKey(void)
{
	bool_t keyPress = false;
	fallingEdge = false;       // limpia la marca de flanco descendente

	if(keyPressed)             // si hubo una pulsación confirmada
	{
		keyPress = true;       // la informa al que llama
		keyPressed = false;    //  resetea la variable
	}
	return keyPress;
}

void debounceFSM_init(void)		// debe cargar el estado inicial
{
	actualState = BUTTON_UP;                    // estado inicial de la MEF
	delayInit(&debounceDelay, DEBOUNCE_DELAY);  // carga el delay de 40 ms
	// LEDs off
	writeLedOff_GPIO(LD1_Pin);
	writeLedOff_GPIO(LD2_Pin);
	writeLedOff_GPIO(LD3_Pin);
	buttonRead = false;                         // pulsador no activado
	keyPressed = false;                         // ninguna tecla presionada
	fallingEdge = false;                        // ningún flanco detectado
}

void debounceFSM_update(void)
{
	// Lee las entradas
	buttonRead = readButton_GPIO();

	switch (actualState)
	{
	case BUTTON_UP: //estado inicial
		//Chequea condicion de transicion
		if(buttonRead == true) //se presiona el pulsador
		{
			actualState = BUTTON_FALLING; //pasa al estado siguiente
			delayRead(&debounceDelay);    //arranca cuenta de delay
		}
	break;

	case BUTTON_FALLING:
		//Chequea si pasa el tiempo de 40 ms
		if(delayRead(&debounceDelay))
		{
			//Chequea condicion de transicion
			if(buttonRead == true) //sigue presionado: no fue rebote
			{
				buttonPressed();
				keyPressed = true;         //Indica tecla presionada luego de 2 lecturas en 40 ms
				fallingEdge = true;        //En este estado hubo un flanco descendente
				actualState = BUTTON_DOWN; //pasa al estado siguiente
			}
			else
			{
				actualState = BUTTON_UP;   // si fue ruido, regresa al estado anterior
			}
		}
	break;

	case BUTTON_DOWN:
		//Chequea condicion de transicion
		if(buttonRead == false) //se soltó el pulsador
		{
			actualState = BUTTON_RISING; //pasa al estado siguiente
			delayRead(&debounceDelay);
		}
	break;

	case BUTTON_RISING:
		//Chequea si pasa el tiempo de 40 ms
		if(delayRead(&debounceDelay))
		{
			//Chequea condicion de transicion
			if(buttonRead == false) //el pulsador regresó a estado inactivo
			{
				buttonReleased();
				actualState = BUTTON_UP;  //pasa al estado siguiente, el inicial
			}
			else
			{
				actualState = BUTTON_DOWN; // si fue ruido, regresa al estado anterior
			}
		}
	break;

	default:
		Error_Handler();
	break;
	}
}


// Evento: tecla presionada (confirmada tras 40 ms).
void buttonPressed(void)
{
	// readKey()
}

// Evento: tecla liberada (confirmada tras 40 ms).
void buttonReleased(void)
{
}

