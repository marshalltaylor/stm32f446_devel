/* Includes -- STD -----------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
__attribute__((used)) static int cmdParser( int argc, char *argv[] );
__attribute__((used)) void taskConsolePrintHelp(void);

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

	localPrintf("%d\n", ulTotalRunTime);
	/* For percentage calculations. */
	ulTotalRunTime /= 100UL;
	//ulTotalRunTime = 1; //fake it
	localPrintf("%d\n", ulTotalRunTime);
	
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
			if((c >= 0x20)&&(c < 0x80))
			{
				localPrintf("%c", c);
				cmdBuffer[cmdBufferPtr] = c;
				if(cmdBufferPtr < CMDBUFFERSIZE - 1)
				{
					cmdBufferPtr++;
				}
			}
			else if(c == 0x08) // Backspace
			{
				cmdBuffer[cmdBufferPtr] = 0x00;
				if(cmdBufferPtr > 0)
				{
					localPrintf("%c", 0x08);
					localPrintf("%c", ' ');
					localPrintf("%c", 0x08);
					cmdBufferPtr--;
				}
			}
			else if(c == '\n')
			{
				localPrintf("\n");
				// Parse buffer
				cmdBuffer[cmdBufferPtr] = 0x00;
				//  here, cmdBufferPtr is length of string
				//localPrintf("%s\n", cmdBuffer);  // <-- USE TO DEBUG INPUT STRING
				//  Identify first real char
				int firstCharIndex = 0;
				for(firstCharIndex = 0; (cmdBuffer[firstCharIndex] == ' ') && (firstCharIndex < cmdBufferPtr); firstCharIndex++)
				{
				}
				int argc = 0;
				if(firstCharIndex < cmdBufferPtr)
				{
					argc = 1;
				}
				
				if(argc == 1)
				{
					//  Count instances of " <char>" in string and build args
					int i;
					for(i = firstCharIndex; i < (cmdBufferPtr - 1); i++)
					{
						if((cmdBuffer[i] == ' ')&&(cmdBuffer[i + 1] != ' '))
						{
							argc++;
						}
						if(cmdBuffer[i] == ' ')
						{
							cmdBuffer[i] = 0x00;
						}
					}
					//localPrintf("arg count:%d\n", argc);
					
					char *argv[argc];
					argv[0] = &cmdBuffer[firstCharIndex];
					int index = 1;
					
					for(i = firstCharIndex; i < (cmdBufferPtr - 1); i++)
					{
						if((cmdBuffer[i] == 0x00)&&(cmdBuffer[i + 1] != 0x00))
						{
							argv[index] = &cmdBuffer[i + 1];
							index++;
						}
					}
					//  Call command handler
					cmdParser(argc, argv);
					localPrintf("\n");
				}
				// Reset
				localPrintf(">");
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

void taskConsolePrintHelp(void)
{
	localPrintf("Generic Console.\n");
	localPrintf(" 'help' -- This list\n");
	localPrintf(" 'stat' -- CPU usage\n");
	localPrintf(" 'xq' -- \n");
	localPrintf(" 'hello' -- \n");
	localPrintf(" 'delay [task|load]' -- \n");
	localPrintf(" 'bit [set|clr] <bit num>' -- \n");
	localPrintf(" 'log [auto|default]' -- \n");
}

int cmdParser( int argc, char *argv[] )
{
	//if( argc == 2 )
	//{
	//	localPrintf("The argument supplied is %s\n", argv[1]);
	//}
	//else if( argc > 2 )
	//{
	//	localPrintf("Too many arguments supplied.\n");
	//}
	//else
	//{
	//	localPrintf("One argument expected.\n");
	//}
	if( argc > 0 )
	{
		if(0 == strcmp((const char*)argv[0], "help"))
		{
			taskConsolePrintHelp();
		}
		else if(0 == strcmp((const char*)argv[0], "stat"))
		{
			taskConsolePrintStats();
		}
		else if(0 == strcmp((const char*)argv[0], "xq"))
		{
			localPrintf("3");
			delay(300);
			localPrintf("2");
			delay(300);
			localPrintf("1");
			delay(300);

			strMsg_t * msg = new strMsg_t();

			msg->id = -1;
			msg->data[0] = '\0';
			if(pdPASS != xQueueSend( logQueue, &msg, 0 ))
			{
				localPrintf(".dud");
				delete msg;
			}
		}
		else if(0 == strcmp((const char*)argv[0], "hello"))
		{
			strMsg_t * msg = new strMsg_t();

			msg->id = 0;
			sprintf( msg->data, "Hello world!\n");
			if(pdPASS != xQueueSend( logQueue, &msg, 0 ))
			{
				//TODO: error on send
				delete msg;
			}
		}
		else if(0 == strcmp((const char*)argv[0], "delay"))
		{
			if(0 == strcmp((const char*)argv[1], "task"))
			{
				//Test delay times
				localPrintf("\n");
				localPrintf("Start time  %dms\n", millis());
				vTaskDelay( 1000 );
				localPrintf("Stop time   %dms\n", millis());
				//localPrintf("tick count     %dms\n", xTaskGetTickCount());
			}
			else if(0 == strcmp((const char*)argv[1], "load"))
			{
				//Test delay times
				localPrintf("L");
				delay( 333 );
				localPrintf("O");
				delay( 333 );
				localPrintf("A");
				delay( 333 );
				localPrintf("D");
			}
			else
			{
				localPrintf("Needs 'task' or 'load'\n");
			}
		}
		else if(0 == strcmp((const char*)argv[0], "bit"))
		{
			EventBits_t uxBits = xEventGroupGetBits(xTestEventGroup);
			if(argc < 3)
			{
				localPrintf("needs arg and value\n");
			}
			int intArg = atoi(argv[2]);
			if((intArg < 0)||(intArg > 7))
			{
				localPrintf("value 0 - 7\n");
			}
			else
			{
				uint16_t testKeyMask = 0;
				testKeyMask = 0x0001 << intArg;
				if(0 == strcmp((const char*)argv[1], "set"))
				{
					uxBits |= testKeyMask;
					xEventGroupSetBits(xTestEventGroup, testKeyMask );
				}
				else if(0 == strcmp((const char*)argv[1], "clr"))
				{
					uxBits &= ~testKeyMask;
					xEventGroupClearBits(xTestEventGroup, testKeyMask );
				}
				else
				{
					localPrintf("Needs 'set' or 'clr'\n");
				}
			}
			for(int i = 7; i >= 0; i--)
			{
				localPrintf("%d", (uxBits >> i)&0x0001);
			}
		}
		else if(0 == strcmp((const char*)argv[0], "log"))
		{
			if(0 == strcmp((const char*)argv[1], "auto"))
			{
				consoleDebug.setMode(LOG_MODE_AUTO);
			}
			else if(0 == strcmp((const char*)argv[1], "default"))
			{
				consoleDebug.setMode(LOG_MODE_DEFAULT);
			}
			else
			{
				localPrintf("Needs 'auto' or 'default'\n");
			}
		}
		else
		{
			localPrintf("Command not supported.  try 'help'\n");
		}
	}
	return 0;
}
