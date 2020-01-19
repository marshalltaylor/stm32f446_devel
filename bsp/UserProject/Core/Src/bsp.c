/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include "stm32f4xx_it.h"
#include "usart.h"
#include "gpio.h"
#include "main_bsp.h"
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

void bsp_printf(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	char buffer[MAX_PRINTF_LEN];
	vsprintf(buffer, fmt, args);
	int i;
	for(i = 0; (i < MAX_PRINTF_LEN)&&buffer[i]!='\0'; i++)
	{
		halUartWrite(buffer[i], &VCP_UART);
	}
	va_end(args);
}
