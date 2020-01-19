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
		if( bspSerialConsoleBytesAvailable() )
		{
			crapDelay(50);
			while( bspSerialConsoleBytesAvailable() )
			{
				bspSerialConsoleWrite(bspSerialConsoleRead());
			}
		}
		if( millis() > nextSecond )
		{
			nextSecond = nextSecond + 1000;
			//bspToggleLED();
			bspSerialConsolePrintf("number = %d\n", nextSecond); 
		}
		crapDelay(10);
	}
}

void runLoopback( void )
{
	while (1)
	{
		if( bspSerialConsoleBytesAvailable() )
		{
			crapDelay(50);
			while( bspSerialConsoleBytesAvailable() )
			{
				bspSerialConsoleWrite(bspSerialConsoleRead());
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
