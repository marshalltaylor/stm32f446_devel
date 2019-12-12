#include "FreeRTOS.h"
#include "task.h"
//#include "main_bsp.h"

#include "Arduino.h"
#include "interface.h"

#define Serial Serial6

extern "C" void taskTestStart(void * argument)
{
	while(1)
	{
		// Don't smash it!
		vTaskDelay( 100 );
	}
}
