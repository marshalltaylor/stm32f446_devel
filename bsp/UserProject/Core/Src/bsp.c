#include <stdbool.h>
#include "main_bsp.h"
#include "stm32f4xx_it.h"
#include "gpio.h"

static bool ledState = false;

void bspInit(void)
{
	volatile void * trash = SysTick_Handler;
	trash++;
	main_bsp();
	SysTick_Handler();
}

void bspToggleLED(void)
{
	ledState =! ledState;
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, ledState);
}
