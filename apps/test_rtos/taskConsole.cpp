#include "Arduino.h"
#include "interface.h"
#include <MIDI.h>
#include "adc_ext.h"

#include "timerModule32.h"

#define Serial Serial6

#include "FreeRTOS.h"
#include "task.h"
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

//#if defined(__arm__)
//extern "C" char* sbrk(int incr);
//static int FreeStack() {
//  char top = 't';
//  return &top - reinterpret_cast<char*>(sbrk(0));
//}
//#endif

///* This example demonstrates how a human readable table of run time stats
//information is generated from raw data provided by uxTaskGetSystemState().
//The human readable table is written to pcWriteBuffer.  (see the vTaskList()
//API function which actually does just this). */
//void vTaskGetRunTimeStats( signed char *pcWriteBuffer )
//{
//TaskStatus_t *pxTaskStatusArray;
//volatile UBaseType_t uxArraySize, x;
//unsigned long ulTotalRunTime, ulStatsAsPercentage;
//
//   /* Make sure the write buffer does not contain a string. */
//   *pcWriteBuffer = 0x00;
//
//   /* Take a snapshot of the number of tasks in case it changes while this
//   function is executing. */
//   uxArraySize = uxTaskGetNumberOfTasks();
//
//   /* Allocate a TaskStatus_t structure for each task.  An array could be
//   allocated statically at compile time. */
//   pxTaskStatusArray = pvPortMalloc( uxArraySize * sizeof( TaskStatus_t ) );
//
//   if( pxTaskStatusArray != NULL )
//   {
//      /* Generate raw status information about each task. */
//      uxArraySize = uxTaskGetSystemState( pxTaskStatusArray,
//                                 uxArraySize,
//                                 &ulTotalRunTime );
//
//      /* For percentage calculations. */
//      ulTotalRunTime /= 100UL;
//
//      /* Avoid divide by zero errors. */
//      if( ulTotalRunTime > 0 )
//      {
//         /* For each populated position in the pxTaskStatusArray array,
//         format the raw data as human readable ASCII data. */
//         for( x = 0; x < uxArraySize; x++ )
//         {
//            /* What percentage of the total run time has the task used?
//            This will always be rounded down to the nearest integer.
//            ulTotalRunTimeDiv100 has already been divided by 100. */
//            ulStatsAsPercentage =
//                  pxTaskStatusArray[ x ].ulRunTimeCounter / ulTotalRunTime;
//
//            if( ulStatsAsPercentage > 0UL )
//            {
//               sprintf( pcWriteBuffer, "%stt%lutt%lu%%rn",
//                                 pxTaskStatusArray[ x ].pcTaskName,
//                                 pxTaskStatusArray[ x ].ulRunTimeCounter,
//                                 ulStatsAsPercentage );
//            }
//            else
//            {
//               /* If the percentage is zero here then the task has
//               consumed less than 1% of the total run time. */
//               sprintf( pcWriteBuffer, "%stt%lutt<1%%rn",
//                                 pxTaskStatusArray[ x ].pcTaskName,
//                                 pxTaskStatusArray[ x ].ulRunTimeCounter );
//            }
//
//            pcWriteBuffer += strlen( ( char * ) pcWriteBuffer );
//         }
//      }
//
//      /* The array is no longer needed, free the memory it consumes. */
//      vPortFree( pxTaskStatusArray );
//   }
//}

//TODO: Write proper print function -- probably put in os? maybe
//Also not sure if this function works?  Needed to enabled some extra stuff
TaskStatus_t pxTaskStatusArray[3];

void taskConsolePrint(void)
{
	volatile UBaseType_t uxArraySize, x;
	unsigned long ulTotalRunTime, ulStatsAsPercentage;
	char pcWriteBuffer[128];
	
	/* get num of tasks */
	uxArraySize = uxTaskGetNumberOfTasks();
    /* Generate raw status information about each task. */
    uxTaskGetSystemState( pxTaskStatusArray,
                               uxArraySize,
                               &ulTotalRunTime );

    /* For percentage calculations. */
    ulTotalRunTime /= 100UL;
	ulTotalRunTime = 1; //fake it
	
    /* Avoid divide by zero errors. */
    if( ulTotalRunTime > 0 )
    {
       /* For each populated position in the pxTaskStatusArray array,
       format the raw data as human readable ASCII data. */
       for( x = 0; x < uxArraySize; x++ )
       {
          /* What percentage of the total run time has the task used?
          This will always be rounded down to the nearest integer.
          ulTotalRunTimeDiv100 has already been divided by 100. */
          ulStatsAsPercentage =
                pxTaskStatusArray[ x ].ulRunTimeCounter / ulTotalRunTime;

          if( ulStatsAsPercentage > 0UL )
          {
             sprintf( pcWriteBuffer, "%stt%lutt%lu%%rn",
                               pxTaskStatusArray[ x ].pcTaskName,
                               pxTaskStatusArray[ x ].ulRunTimeCounter,
                               ulStatsAsPercentage );
          }
          else
          {
             /* If the percentage is zero here then the task has
             consumed less than 1% of the total run time. */
             sprintf( pcWriteBuffer, "%stt%lutt<1%%rn",
                               pxTaskStatusArray[ x ].pcTaskName,
                               pxTaskStatusArray[ x ].ulRunTimeCounter );
          }
		Serial6.println(pcWriteBuffer);
       }
    }



}

extern "C" void taskConsoleStart(void * argument)
{
	uint32_t nextUpdate = 0;
	
	while(1)
	{
		if(Serial6.available())
		{
			char c = (char)Serial6.read();
			Serial6.println(c);
			if(c == '\r')
			{
				Serial6.println("Hello");
			}
			if(c == 'r')
			{
				//UBaseType_t uxTaskGetSystemState( TaskStatus_t * const pxTaskStatusArray,
				//	const UBaseType_t uxArraySize,
				//	unsigned long * const pulTotalRunTime );
				
				Serial6.println("Report:");
				taskConsolePrint();
			}
		}
		//if( usTicksLocked == 0 )
		//{
		//	mainPanelTimer.update(usTicks);
		//	debugTimer.update(usTicks);
		//	statusPanelTimer.update(usTicks);
		//	segmentVideoTimer.update(usTicks);
		//	//Done?  Lock it back up
		//	usTicksLocked = 1;
		//}  //The ISR will unlock.
		//
		uint32_t now = xTaskGetTickCount();
		if(now > nextUpdate)
		{
			nextUpdate = nextUpdate + 5000;
			//User code
			char buffer[200] = {0};
			//sprintf(buffer, "__DEBUG______\nintPlayState = %d, extPlayState = %d\nbeatLedState = %d, playLedState = %d\n\n", intMidiClock.getState(), extMidiClock.getState(), statusPanel.getBeatLedState(), statusPanel.getPlayLedState());
			//sprintf(buffer, "__DEBUG__\n");
			sprintf(buffer, "__DEBUG__ [%d]\n", (int16_t)millis());
			Serial6.print(buffer);
			sprintf(buffer, "__DEBUG__ [%d]\n", xTaskGetTickCount());
			Serial6.print(buffer);
			//Serial6.println(mainPanel.getState());
			//Serial6.print("Playing: ");
		}
	}
	
}
