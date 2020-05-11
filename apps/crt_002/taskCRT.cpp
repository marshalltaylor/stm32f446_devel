/* Includes -- STD -----------------------------------------------------------*/
#include <stdio.h>

/* Includes -- BSP -----------------------------------------------------------*/

/* Includes -- FreeRTOS system -----------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "event_groups.h"
#include "os.h"

/* Includes -- FreeRTOS app --------------------------------------------------*/
#include "taskLog.h"
#include "taskCRT.h"

/* Includes -- modules -------------------------------------------------------*/

/* References ----------------------------------------------------------------*/
#include "game.h"
extern game_obj ufo;

//state inputs, not threadsafe
bool genTestLog = false;

CRTVideo crt;

//Ext variables
extern EventGroupHandle_t xTestEventGroup;

extern "C" void taskCRTStart(void * argument)
{

	uint16_t stamp_ms = 0;
	uint8_t stamp_s = 0;
	bool testValueStateIdle = true;
	
	//Start CRT
	crt.init();
	char str[] = "CRT Init.\n\r          ";
	for(int i = 0; i < 10; i++)
	{
		crt.writeChar(str[i]);
	}

	while(1)
	{
		// Wait for event group
		EventBits_t uxBits;
		
		//Pass on set flag
		uxBits = xEventGroupWaitBits(
            xTestEventGroup,
            TEST_EVENT_0, // logical OR macros
            pdFALSE,        /* BIT_0 & BIT_4 should be cleared before returning. */
            pdFALSE,       /* Don't wait for both bits, either bit will do. */
            100 );/* Wait a maximum of 100ms for either bit to be set. */
			
		if( ( uxBits & TEST_EVENT_1 ) == TEST_EVENT_1 )
		{
			if( testValueStateIdle )
			{
				testValueStateIdle = false;
				
				//Print through logger
				strMsg_t * msg = new strMsg_t();
				msg->id = 0;
				sprintf( msg->data, "Test flagged" );
				if(pdPASS != xQueueSend( logQueue, &msg, 1 ))
				{
					//TODO: error on send
					delete msg;
				}
			}
		}
		else
		{
			//Clear
			testValueStateIdle = true;
		}
		if( ( uxBits & TEST_EVENT_0 ) == TEST_EVENT_0 )
		{
			// Event has triggered, run it
			stamp_ms += 50;
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
			sprintf( msg->data, "Test: %02d:%03d", stamp_s, stamp_ms );
			if(pdPASS != xQueueSend( logQueue, &msg, 1 ))
			{
				//TODO: error on send
				delete msg;
			}
			
			vTaskDelay( 50 );
		}
		uxBits = xEventGroupGetBits( xTestEventGroup );
		uxBits &= (0x01 << 5);
		if(uxBits)
		{
			ufo.tick(&crt);
		}
		else
		{
			crt.drawFrame();
		}
		vTaskDelay( 33 );
	}
}
