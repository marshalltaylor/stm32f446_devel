/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
//#include "main_cubemx.h"

#include "bsp.h"

#include "timers.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"

#include "os.h"
#include "taskLog.h"

/* Declare a variable to hold the created event group. */
EventGroupHandle_t xTestEventGroup;

//Tasks
extern void taskCRTStart(void * argument);
extern void taskConsoleStart(void * argument);

extern void xPortSysTickHandler(void);

extern EventGroupHandle_t xTestEventGroup;

QueueHandle_t controlQueue = NULL;

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

void MX_FREERTOS_Init(void)
{
	//TODO: Should we check for scheduler running first?
	bspRegisterSysTickCallback(xPortSysTickHandler);

	// Example:
	//vStartLEDFlashTasks( mainFLASH_TASK_PRIORITY );
	//	xTaskCreate( vCompeteingIntMathTask1, "IntMath1", intgSTACK_SIZE, ( void * ) &( usTaskCheck[ 0 ] ), uxPriority, NULL );

	BaseType_t retVal;

	logQueue = xQueueCreate( 100, sizeof( strMsg_t* ) );
	controlQueue = xQueueCreate( 100, sizeof( gameControlInput_t* ) );

    /* Attempt to create the event group. */
    xTestEventGroup = xEventGroupCreate();

    /* Was the event group created successfully? */
    if( xTestEventGroup == NULL )
    {
        /* The event group was not created because there was insufficient
        FreeRTOS heap available. */
		while(1);
    }

	retVal = xTaskCreate( taskLogStart, "log", 1024, (void*) 1, tskIDLE_PRIORITY, NULL);
    if (retVal != pdPASS)
	{
		while(1);
	}

	retVal = xTaskCreate( taskConsoleStart, "console", 512, (void*) 1, tskIDLE_PRIORITY, NULL);
    if (retVal != pdPASS)
	{
		while(1);
	}

	retVal = xTaskCreate( taskCRTStart, "crt", 128, (void*) 1, tskIDLE_PRIORITY, NULL);
    if (retVal != pdPASS)
	{
		while(1);
	}

}

void osInit(void)
{
	// Possibly setup activities here, might be architected elsewhere
	
	MX_FREERTOS_Init();
	
	// Init other os objects
	
	
	// Start os and don't come back
	vTaskStartScheduler();
}