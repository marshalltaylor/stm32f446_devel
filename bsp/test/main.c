/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include "bsp.h"
/* Private variables ---------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

static void crapDelay( int16_t msInput ) __attribute__((used));
static void runLoopback( void ) __attribute__((used));

uint32_t nextSecond = 0;

int main(void)
{
	bspInit();
	

	//Run the loopback test for python interaction
	//runLoopback();

	//Alternately, do random stuff here
	nextSecond = millis() + 1000;
	while (1)
	{
		if( halUartReadBytesAvailable(&VCP_UART) )
		{
			crapDelay(50);
			while( halUartReadBytesAvailable(&VCP_UART) )
			{
				halUartWrite(halUartRead(&VCP_UART), &VCP_UART);
			}
		}
		if( millis() > nextSecond )
		{
			nextSecond = millis() + 1000;
			//bspToggleLED();
		}
		bsp_printf("number = %d\n", 42); 
		//halUartWrite('X', &VCP_UART);
		crapDelay(1000);
	}
}

void runLoopback( void )
{
	while (1)
	{
		if( halUartReadBytesAvailable(&VCP_UART) )
		{
			crapDelay(50);
			while( halUartReadBytesAvailable(&VCP_UART) )
			{
				halUartWrite(halUartRead(&VCP_UART), &VCP_UART);
			}
		}
	}
}

void crapDelay( int16_t msInput )
{
	volatile int32_t startTime;
	startTime = millis();
	while(millis() < startTime + msInput)
	{
	}
	//int32_t i;
	//int32_t j;
	//for(i = 0; i < 3000; i++)
	//{
	//	for(j = 0; j < 1000; j++)
	//	{
	//		startTime++;
	//	}
	//}
	
}
