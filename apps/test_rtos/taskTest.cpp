//FreeRTOS system
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

//FreeRTOS app
#include "taskLog.h"

//Arduino
#include "Arduino.h"
#include "interface.h"

//BSP
//#include "main_bsp.h"

#define Serial Serial6

//state inputs, not threadsafe
bool genTestLog = false;

extern "C" void taskTestStart(void * argument)
{
	uint16_t stamp_ms = 0;
	uint8_t stamp_s = 0;
	while(1)
	{
		// Don't smash it!
		if(genTestLog)
		{
			
			stamp_ms += 10;
			if(stamp_ms > 999)
			{
				stamp_ms = 0;
				stamp_s++;
				if(stamp_s > 9)
				{
					stamp_s = 0;
				}
			}
			strMsg_t * msg = new strMsg_t();
			
			msg->id = 0;
			sprintf( msg->data, "Test: %02d:%03d\n", stamp_s, stamp_ms );
			if(pdPASS != xQueueSend( logQueue, &msg, 1 ))
			{
				//TODO: error on send
				delete msg;
			}
			vTaskDelay( 10 );
			
		}
		else
		{
			vTaskDelay( 1000 );
		}
	}
}
