/* Includes -- STD -----------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>

/* Includes -- HAL -----------------------------------------------------------*/
#include "gpio.h"

/* Includes -- BSP -----------------------------------------------------------*/
#include "bsp.h"

/* References ----------------------------------------------------------------*/
static bool ledState = false;

/* Functions -----------------------------------------------------------------*/
void bspIOToggleLED(void)
{
	ledState =! ledState;
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, ledState);
}
