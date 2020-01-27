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
static void runLoopbackIO( void ) __attribute__((used));

uint32_t nextSecond = 0;

int main(void)
{
	bspInit();

	//Run a test or allow the default loop
	//runLoopback();
	runLoopbackIO();

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

void runLoopbackIO( void )
{
	bool button1Save = false;
	bool button2Save = false;
	while (1)
	{
		bool button1Value = bspIOPinRead(D2);
		bool button2Value = bspIOPinRead(D3);
		if( button1Save != button1Value )
		{
			button1Save = button1Value;
			bspSerialConsolePrintf("Button 1: %d\n", button1Save);
		}
		if( button2Save != button2Value )
		{
			button2Save = button2Value;
			bspSerialConsolePrintf("Button 2: %d\n", button2Save);
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
