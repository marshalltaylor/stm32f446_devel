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
//#include "FreeRTOS.h"
//#include "task.h"
//#include "queue.h"
//#include "event_groups.h"
#include "os.h"
#include "storage.h"

/* Includes -- FreeRTOS app --------------------------------------------------*/
//#include "taskLog.h"
//#include "taskCRT.h"
//#include "MidiClockDisplay.h"
//#include "globals.h"

/* Includes -- modules -------------------------------------------------------*/
//#include "logging.h"
//#include "CRTVideo.h"

/* References ----------------------------------------------------------------*/

//#define USE_LOGGING
#ifdef USE_LOGGING
// Create logging object and macro for local printf
//#define localPrintf consoleDebug.printf
Logging consoleDebug;

#else
// Connect directly to bsp.
#define localPrintf bspPrintf

#endif
uint32_t timeo = 100;

void test_storage(void)
{
    StorageController unit;
    unit.updateUSTime(timeo++);
    unit.dumpVars();
    
    unit.write(0x12);
    unit.updateUSTime(timeo++);
    unit.dumpVars();
    
    unit.record(true);
    unit.dumpVars();
    
    unit.write(0x12);
    unit.updateUSTime(timeo++);
    unit.dumpVars();
    
    unit.write(0x12);
    unit.updateUSTime(timeo++);
    unit.dumpVars();

    unit.write(0x88);
    unit.write(0xAB);
    unit.write(0x34);
    unit.updateUSTime(timeo++);
    unit.dumpVars();

    unit.write(0x12);
    unit.updateUSTime(timeo++);
    unit.dumpVars();


}

void StorageController::dumpVars(void)
{
    localPrintf("\nData: ");
    for(int i = 0; i < DATA_ARRAY_SIZE_BYTES; i++)
    {
        localPrintf("%02X,", data[i]);
    }
    localPrintf("\n");
    localPrintf("Times [time,index]: ");
    for(int i = 0; i < TIME_ARRAY_SIZE_ITEMS; i++)
    {
        localPrintf("[%d,%d],", times[i].time, times[i].index);
    }
    localPrintf("\n");
    localPrintf("Data (full, start, end): %d, %d, %d\n", dataArrayFull(), dataWriteStartIndex, dataWriteEndIndex);
    localPrintf("Times (full, start, end): %d, %d, %d\n", timesArrayFull(), timesWriteStartIndex, timesWriteEndIndex);
    localPrintf("Control (time, cir, rec, read): %d, %d, %d, %d\n", time, circularModeEnabled, recordingEnabled, readIndex);
    localPrintf("\n");
}
