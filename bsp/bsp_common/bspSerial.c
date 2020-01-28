/* Includes -- STD -----------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>

/* Includes -- HAL -----------------------------------------------------------*/
#include "usart.h"

/* Includes -- BSP -----------------------------------------------------------*/
#include "bsp.h"

/* References ----------------------------------------------------------------*/
extern UartInstance_t VCP_UART;
extern UartInstance_t D01_UART;

/* Functions -----------------------------------------------------------------*/
void bspSerialConsolePrintf(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	char buffer[MAX_PRINTF_LEN];
	vsprintf(buffer, fmt, args);
	int i;
	for(i = 0; (i < MAX_PRINTF_LEN)&&buffer[i]!='\0'; i++)
	{
		bspSerialConsoleWrite(buffer[i]);
	}
	va_end(args);
}

uint8_t bspSerialConsolePeek(void)
{
	return halUartPeek(&VCP_UART);
}

void bspSerialConsoleWrite(uint8_t data)
{
	halUartWrite(data, &VCP_UART);
}

uint8_t bspSerialConsoleRead(void)
{
	return halUartRead(&VCP_UART);
}

uint16_t bspSerialConsoleBytesAvailable(void)
{
	return halUartReadBytesAvailable(&VCP_UART);
}
