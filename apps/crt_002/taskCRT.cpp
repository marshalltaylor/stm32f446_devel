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
#include "videoData.h"
#include "bitmaps.h"

extern game_obj ufo;

//state inputs, not threadsafe
bool genTestLog = false;

//Inherits CRTVideo
graphics_obj crt;

//Ext variables
extern EventGroupHandle_t xTestEventGroup;

EventBits_t uxBitSave;

extern "C" void taskCRTStart(void * argument)
{

	//uint16_t stamp_ms = 0;
	//uint8_t stamp_s = 0;
	//bool testValueStateIdle = true;
	
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
		
		////Pass on set flag
		//uxBits = xEventGroupWaitBits(
        //    xTestEventGroup,
        //    TEST_EVENT_0, // logical OR macros
        //    pdFALSE,        /* BIT_0 & BIT_4 should be cleared before returning. */
        //    pdFALSE,       /* Don't wait for both bits, either bit will do. */
        //    100 );/* Wait a maximum of 100ms for either bit to be set. */
		//	
		//if( ( uxBits & TEST_EVENT_1 ) == TEST_EVENT_1 )
		//{
		//	if( testValueStateIdle )
		//	{
		//		testValueStateIdle = false;
		//		
		//		//Print through logger
		//		strMsg_t * msg = new strMsg_t();
		//		msg->id = 0;
		//		sprintf( msg->data, "Test flagged" );
		//		if(pdPASS != xQueueSend( logQueue, &msg, 1 ))
		//		{
		//			//TODO: error on send
		//			delete msg;
		//		}
		//	}
		//}
		//else
		//{
		//	//Clear
		//	testValueStateIdle = true;
		//}
		//if( ( uxBits & TEST_EVENT_0 ) == TEST_EVENT_0 )
		//{
		//	// Event has triggered, run it
		//	stamp_ms += 50;
		//	if(stamp_ms > 999)
		//	{
		//		stamp_ms = 0;
		//		stamp_s++;
		//		if(stamp_s > 9)
		//		{
		//			stamp_s = 0;
		//		}
		//	}
		//	strMsg_t * msg = new strMsg_t();
		//	
		//	msg->id = 0;
		//	sprintf( msg->data, "Test: %02d:%03d", stamp_s, stamp_ms );
		//	if(pdPASS != xQueueSend( logQueue, &msg, 1 ))
		//	{
		//		//TODO: error on send
		//		delete msg;
		//	}
		//	
		//	vTaskDelay( 50 );
		//}
		uxBits = xEventGroupGetBits( xTestEventGroup );
		
		if((uxBits & 0x80)!=(uxBitSave & 0x80))
		{
			uxBitSave = uxBits;
			if(uxBits & 0x80)
			{
				crt.interlace(true);
			}
			else
			{
				crt.interlace(false);
			}
		}
		
		if(uxBits & 0x20)
		{
			ufo.tick(&crt);
		}
		else if(uxBits & 0x40)
		{
			uint8_t * buf = NULL;
			if(crt.getBlank(&buf, 0xFF))
			{
				basic_bitmap_type_t testPage;
				
				testPage.type = BITMAP_1X1;
				testPage.srcFile = &testPattern;
				testPage.data = &testPage.tileTable;
				testPage.tileTable = 0;
				
				Sprite testSprite;
				testSprite.bitmap = &testPage;
				
				crt.drawBitmap(buf, testSprite.bitmap, 0, 0);
				
				crt.swap();

			}
		}
		else
		{
			//Generic method:  crt.drawFrame();
			//Here, we get our own buffer and swap ourselves
			uint8_t * buf = NULL;
			if(crt.getBlank(&buf, 0x00))
			{
				crt.console(buf);

				for(int p = 0; p < 192; p++)
				{
					crt.pixel(buf, p, 0, 0xFF);
					crt.pixel(buf, p, 143, 0xFF);
				}
				for(int p = 0; p < 144; p++)
				{
					crt.pixel(buf, 0, p, 0xFF);
					crt.pixel(buf, 191, p, 0xFF);
				}
				
				for(int p = 1; p < 191; p++)
				{
					crt.pixel(buf, p, 1, 0xC8);
					crt.pixel(buf, p, 142, 0xC8);
				}
				for(int p = 1; p < 143; p++)
				{
					crt.pixel(buf, 1, p, 0xC8);
					crt.pixel(buf, 190, p, 0xC8);
				}
				
				for(int p = 2; p < 190; p++)
				{
					crt.pixel(buf, p, 2, 0xA0);
					crt.pixel(buf, p, 141, 0xA0);
				}
				for(int p = 2; p < 142; p++)
				{
					crt.pixel(buf, 2, p, 0xA0);
					crt.pixel(buf, 189, p, 0xA0);
				}

				crt.swap();
			}
		}
		vTaskDelay( 33 );
	}
}
