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
static void runLoopbackIO( void ) __attribute__((used));
static void runObjLoopback( void ) __attribute__((used));
uint32_t nextSecond = 0;

static comPortInterface_t console;
static comPortInterface_t midiSerial;

static uint8_t testDACBuffer [] = {
	255, 247, 239, 231, 223, 215, 207, 199, 191, 183, 175, 167, 159, 151, 143, 135,
	127, 119, 111, 103, 95, 87, 79, 71, 63, 55, 47, 255, 255, 255, 0, 0,
	0, 255, 255, 255, 47, 55, 63, 71, 79, 87, 95, 103, 111, 119, 127, 135,
	143, 151, 159, 167, 175, 183, 191, 199, 207, 215, 223, 231, 239, 247, 255, 0 
};

int main(void)
{
	bspInit();
	
	bspGetSerialFunctions(COM0, &console);
	bspGetSerialFunctions(COM0, &midiSerial);
	
	bspPrintf("BSP Test on board: %s\n", boardName); 
	
	//Run a test or allow the default loop

	//runLoopbackIO();
	//runObjLoopback();

	nextSecond = millis() + 1000;
	while (1)
	{
		//Choose a routine
		
		#if 0
		if( console.bytesAvailable() )
		{
			crapDelay(50);
			while( console.bytesAvailable() )
			{
				console.write(console.read());
			}
		}
		#endif
		#if 1
		if( midiSerial.bytesAvailable() )
		{
			crapDelay(50);
			while( midiSerial.bytesAvailable() )
			{
				bspPrintf("read: 0x%02X\n", midiSerial.read());
			}
		}
		#endif
		
		
		
		
		//Do some periodic stuff too
		
		if( millis() > nextSecond )
		{
			nextSecond = nextSecond + 100;
			//bspToggleLED();
			console.write('N');
			bspPrintf("umber = %d\n", nextSecond); 
			bspPrintf("Sending DAC buffer\n"); 
			bspIOPinWrite(D31, 0);
			bspIOPinWrite(D31, 1);
			bspDACSendBuffer((uint32_t*)testDACBuffer, 64);
		}
		crapDelay(10);
	}
}

void runObjLoopback( void )
{
	comPortInterface_t comObj;
	bspGetSerialFunctions(COM0, &comObj);
	
	while (1)
	{
		if( comObj.bytesAvailable() )
		{
			crapDelay(50);
			while( comObj.bytesAvailable() )
			{
				comObj.write(comObj.read());
			}
		}
	}
}

void runLoopbackIO( void )
{
	bspPrintf_t printf = bspGetSerialConsolePrintf();
	bool button1Save = false;
	bool button2Save = false;
	while (1)
	{
		bool button1Value = bspIOPinRead(D24);
		bool button2Value = bspIOPinRead(D25);
		bool anyDetected = false;
		if( button1Save != button1Value )
		{
			button1Save = button1Value;
			bspIOPinWrite(D31, button1Save);
			printf("Button 1: %d\n", button1Save);
			anyDetected = true;
		}
		if( button2Save != button2Value )
		{
			button2Save = button2Value;
			bspIOPinWrite(D32, button2Save);
			printf("Button 2: %d\n", button2Save);
			anyDetected = true;
		}
		if(anyDetected)
		{
			printf("ADC 0: %d\n", bspIOPinReadAnalog(A0));
			printf("ADC 1: %d\n", bspIOPinReadAnalog(A1));
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
