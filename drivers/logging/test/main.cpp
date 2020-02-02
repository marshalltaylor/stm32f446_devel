/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include "bsp.h"
#include "main.h"
#include "logging.h"

/* Private variables ---------------------------------------------------------*/
//static const char name[] = "Dummy";

class DummyModule
{
public:
	DummyModule(void);
	void logData(void);
private:
	int data;
	Logging dbg;
};

DummyModule::DummyModule(void) : dbg("Dummy")
{
	data = 0;
}

void DummyModule::logData(void)
{
	//Write data
	dbg.printf("data = 0x%08X\n", data);
	
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

	Logging dbg("dbg");

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
