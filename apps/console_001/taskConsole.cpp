/* Includes -- STD -----------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>

/* Includes -- BSP -----------------------------------------------------------*/
#include "bsp.h"

/* Includes -- FreeRTOS system -----------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "event_groups.h"

/* Includes -- FreeRTOS app --------------------------------------------------*/
#include "taskLog.h"
#include "taskCommon.h"
//#include "MidiClockDisplay.h"

/* Includes -- modules -------------------------------------------------------*/
#include "logging.h"

/* References ----------------------------------------------------------------*/
#define USE_LOGGING
#ifdef USE_LOGGING
// Create logging object and macro for local printf
#define localPrintf consoleDebug.printf
Logging consoleDebug;

#else
// Connect directly to bsp.
#define localPrintf bspPrintf

#endif

static comPortInterface_t console;
TaskStatus_t pxTaskStatusArray[5];


void delay(uint32_t delayInput);

/* Functions -----------------------------------------------------------------*/
void delay(uint32_t delayInput)
{
	uint32_t targetTicks = millis() + delayInput; //OK for this to roll
	while(millis() > targetTicks)
	{
		//if we rolled, wait until millis rolls
	}
	while(millis() < targetTicks)
	{
		//nop
	}
}

void taskConsolePrintStats(void)
{
	volatile UBaseType_t uxArraySize, x;
	unsigned long ulTotalRunTime, ulStatsAsPercentage;

	/* get num of tasks */
	uxArraySize = uxTaskGetNumberOfTasks();
	/* Generate raw status information about each task. */
	uxTaskGetSystemState( pxTaskStatusArray,
								uxArraySize,
								&ulTotalRunTime );

	/* For percentage calculations. */
	ulTotalRunTime /= 100UL;
	//ulTotalRunTime = 1; //fake it
	
	/* Avoid divide by zero errors. */
	if( ulTotalRunTime > 0 )
	{
		localPrintf(" %-7s%10s%7s\n", "name", "epoch", "Load");
		localPrintf("--------------------------\n");
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
				localPrintf(" %-7s%10lu%6lu%%\n",
							pxTaskStatusArray[ x ].pcTaskName,
							pxTaskStatusArray[ x ].ulRunTimeCounter,
							ulStatsAsPercentage
							);
			}
			else
			{
			 /* If the percentage is zero here then the task has
			 consumed less than 1% of the total run time. */
			 localPrintf(" %-7s%10lu%7s\n",
						pxTaskStatusArray[ x ].pcTaskName,
						pxTaskStatusArray[ x ].ulRunTimeCounter,
						"<1%"
						);
			}
		}
	}
}

void taskConsolePrintHelp(void)
{
	localPrintf("Segment Video Test Console.\n");
	localPrintf(" h: help\n");
	localPrintf(" i: increment value\n");
	localPrintf(" p: start peeking\n");
	localPrintf(" 0 - 9: graphical tests\n");
}

#define CMDBUFFERSIZE 128

//strMsg_t globoMsg = {0};
extern "C" void taskConsoleStart(void * argument)
{
	uint32_t nextUpdate = 0;
#ifdef USE_LOGGING
	consoleDebug.setStamp("Console", 7);
	consoleDebug.setMode(LOG_MODE_DEFAULT);
#endif

	bspGetSerialFunctions(COM0, &console);

	taskConsolePrintHelp();
	
	char cmdBuffer[CMDBUFFERSIZE];
	uint16_t cmdBufferPtr = 0;
	
	while(1)
	{
		if(console.bytesAvailable())
		{
			char c = (char)console.read();
			localPrintf("%c", c);
			// buffer data unle
			if((c >= 0x20)&&(c < 0x80))
			{
				cmdBuffer[cmdBufferPtr] = c;
				if(cmdBufferPtr < CMDBUFFERSIZE - 1)
				{
					cmdBufferPtr++;
				}
			}
			else if(c == '\n')
			{
				// Parse buffer
				cmdBuffer[cmdBufferPtr] = 0x00;
				//  here, cmdBufferPtr is length of string
				localPrintf("%s\n", cmdBuffer);
				//  call command handler
				// Reset
				cmdBufferPtr = 0;
			}
			
			

		}

		uint16_t now = xTaskGetTickCount();
		if(now > nextUpdate)
		{
			nextUpdate = nextUpdate + 33;
			while(nextUpdate > 0xFFFF)
			{
				nextUpdate -= 0xFFFF;
			}
		}
		// Clock segment driver
		vTaskDelay( 5 );
//		Segments.tickValueStateMachine(millis());

//		Segments.processEffects();
//		Segments.writeNextFrame();
		
	}
	
}
