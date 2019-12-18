/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
//#include "main_bsp.h"

#include "sketch.h"
#include "bsp.h"

#include "timers.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"

#include "taskCommon.h"
#include "taskLog.h"

extern void xPortSysTickHandler(void);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

void MX_FREERTOS_Init(void)
{
	//TODO: Should we check for scheduler running first?
	bspRegisterSysTickCallback(xPortSysTickHandler);

	// Example:
	//vStartLEDFlashTasks( mainFLASH_TASK_PRIORITY );
	//	xTaskCreate( vCompeteingIntMathTask1, "IntMath1", intgSTACK_SIZE, ( void * ) &( usTaskCheck[ 0 ] ), uxPriority, NULL );

	BaseType_t retVal;

	logQueue = xQueueCreate( 10, sizeof( strMsg_t* ) );

	retVal = xTaskCreate( taskLogStart, "log", 1024, (void*) 1, tskIDLE_PRIORITY, NULL);
    if (retVal != pdPASS)
	{
		while(1);
	}

	retVal = xTaskCreate( taskConsoleStart, "config", 512, (void*) 1, tskIDLE_PRIORITY, NULL);
    if (retVal != pdPASS)
	{
		while(1);
	}

	retVal = xTaskCreate( taskTestStart, "test", 128, (void*) 1, tskIDLE_PRIORITY, NULL);
    if (retVal != pdPASS)
	{
		while(1);
	}

}

void osInit(void)
{
	// Run arduino like setup lop
	setup();
	
	MX_FREERTOS_Init();
	
	// Init other os objects
	
	
	// Start os and don't come back
	vTaskStartScheduler();
}

/* Defined in main.c. */
void vConfigureTimerForRunTimeStats( void )
{
	//This is called once, usually for HW timer config (done at bsp init)

}
