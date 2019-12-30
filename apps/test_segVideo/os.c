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


#include "osTasks.h"


extern void xPortSysTickHandler(void);

//void taskHardwareStart(void const * argument);
//void StartDefaultTask(void const * argument);
//void configShellTaskStart(void * argument);
//void midiTaskStart(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

void MX_FREERTOS_Init(void)
{
  //TODO: Should we check for scheduler running first?
  bspRegisterSysTickCallback(xPortSysTickHandler);

  
//vStartLEDFlashTasks( mainFLASH_TASK_PRIORITY );
//	xTaskCreate( vCompeteingIntMathTask1, "IntMath1", intgSTACK_SIZE, ( void * ) &( usTaskCheck[ 0 ] ), uxPriority, NULL );
  xTaskCreate( configShellTaskStart, "config", 2048, (void*) 1, tskIDLE_PRIORITY, NULL);
//
//  /* definition and creation of midiTask */
//  osThreadDef(midiTask, midiTaskStart, osPriorityIdle, 0, 128);
//  midiTaskHandle = osThreadCreate(osThread(midiTask), NULL);
//
//  /* USER CODE BEGIN RTOS_THREADS */
//  /* definition and creation of hardwareTask */
//  osThreadDef(hardwareTask, taskHardwareStart, osPriorityNormal, 0, 128);
//  hardwareTaskHandle = osThreadCreate(osThread(hardwareTask), NULL);
//
//  
//    if (defaultTaskHandle == NULL)
//	{
//		while(1);
//	}
//    if (configShellTaskHandle == NULL)
//	{
//		while(1);
//	}
//    if (midiTaskHandle == NULL)
//	{
//		while(1);
//	}
//	if (hardwareTaskHandle == NULL)
//	{
//		while(1);
//	}
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */
}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{

  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
	for(;;)
	{
		taskPanel();
		//osDelay(1);
	}
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_configShellTaskStart */
/**
* @brief Function implementing the configShellTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_configShellTaskStart */
void configShellTaskStart(void * argument)
{
  /* USER CODE BEGIN configShellTaskStart */
  /* Infinite loop */
	for(;;)
	{
		taskConsole();
		//osDelay(1);
	}
  /* USER CODE END configShellTaskStart */
}

/* USER CODE BEGIN Header_midiTaskStart */
/**
* @brief Function implementing the midiTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_midiTaskStart */
void midiTaskStart(void const * argument)
{
  /* USER CODE BEGIN midiTaskStart */
  /* Infinite loop */
	for(;;)
	{
		taskMidi();
	}
  /* USER CODE END midiTaskStart */
}

void osInit(void)
{
	// Run arduino like setup lop
	setup();
	
	MX_FREERTOS_Init();
	
	// Init other os objects
	
	
	// Start os and don't come back
	vTaskStartScheduler();
	
	//Old entry point:	AppEntry();
}
