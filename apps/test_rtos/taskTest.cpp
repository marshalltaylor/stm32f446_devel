//#include "FreeRTOS.h"
//#include "task.h"
////#include "main_bsp.h"
//
///* Private includes ----------------------------------------------------------*/
///* USER CODE BEGIN Includes */     
//#include "sketch.h"
//#include "bsp.h"
//#include "osTasks.h"
//
//#include "timers.h"
//#include "queue.h"
//#include "semphr.h"
//#include "event_groups.h"

#include "Arduino.h"
#include "interface.h"
#include <MIDI.h>
#include "adc_ext.h"

#include "timerModule32.h"

#define Serial Serial6

extern "C" void taskTestStart(void * argument)
{
	while(1)
	{
		udelay(400);
	}
}
