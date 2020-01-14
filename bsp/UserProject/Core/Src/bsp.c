#include <stdbool.h>
#include "main_bsp.h"
#include "stm32f4xx_it.h"
#include "gpio.h"
#include "bsp.h"

static bool ledState = false;

void bspInit(void)
{
	// Without this garbage sysTick_Handler won't be linked?
	volatile void * trash = SysTick_Handler;
	UNUSED(trash);

	main_bsp();
}

void bspToggleLED(void)
{
	ledState =! ledState;
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, ledState);
}
