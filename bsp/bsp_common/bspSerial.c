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

UartInstance_t * comPorts[] =
{
	&VCP_UART,
	&D01_UART,
	NULL
};

/* Functions -----------------------------------------------------------------*/
// bspPrintf
//   It is assumed that only one user will ever be operating the console,
// so bspPrintf's port is set in the bsp.  Re-implement if needed.
void bspPrintf(const char* fmt, ...)
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

// Peek, Write, and BytesAvailable are called with a port name.
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

void bspGetSerialConsoleObj(comPortInterface_t * interface)
{
	if( interface == NULL ) return;
	
	interface->peek = bspSerialConsolePeek;
	interface->write = bspSerialConsoleWrite;
	interface->read = bspSerialConsoleRead;
	interface->bytesAvailable = bspSerialConsoleBytesAvailable;

}
