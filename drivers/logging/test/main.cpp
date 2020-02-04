/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include "bsp.h"
#include "main.h"
#include "logging.h"

/* Private variables ---------------------------------------------------------*/
#ifdef USE_LOGGING
// Create logging object and macro for local printf
#define localPrintf dummy.printf
Logging dummy;

#else
// Connect directly to bsp.
#define localPrintf bspPrintf

#endif

class DummyModule
{
public:
	DummyModule(void);
	void logData(void);
private:
	int data;
	Logging dbg;
};

DummyModule::DummyModule(void)
{
	data = 0;
}

void DummyModule::logData(void)
{
	//Write data
	localPrintf("data = 0x%08X\n", data);
	
	//Increment ticker
	data++;
}

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* Functions -----------------------------------------------------------------*/
int main(void)
{
	bspInit();

	bspPrintf("Logging test on board: %s\n", boardName); 

	Logging dbg;
	dbg.setStamp("dbg", 3);
	dbg.setMode(LOG_MODE_AUTO);

#ifdef USE_LOGGING
	dummy.setStamp("dummy", 5);
	//dummy.setMode(LOG_MODE_AUTO);
#endif

	DummyModule module;

	//Configure logger for..
	//  Create a reference for this task's serial output
	
	//  Register logger into used modules
	
	while (1)
	{
		module.logData();
		
		int seconds = millis() / 1000;
		if(seconds % 7 == 0)
		{
			dbg.printf("Seconds = %d\n", seconds);
		}
		uint32_t t = millis() + 1000;
		while(millis() < t);
	}
}
