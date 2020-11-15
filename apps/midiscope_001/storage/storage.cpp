#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "storage.h"

//memsets not necessary except for debugging
StorageController::StorageController(void)
{
    data = (char *)malloc(DATA_ARRAY_SIZE_BYTES);
    memset(data, 0x00, DATA_ARRAY_SIZE_BYTES);
    times = (struct SC_byteTimeData_t *)malloc(TIME_ARRAY_SIZE_ITEMS * sizeof(SC_byteTimeData_t));
    memset(times, 0x00, TIME_ARRAY_SIZE_ITEMS * sizeof(SC_byteTimeData_t));
    circularModeEnabled = false;
    recordingEnabled = false;
    time = 0;
    clear();
}

// Out of order to keep inits on top
void StorageController::clear(void)
{
    dataWriteStartIndex = 0;
    dataWriteEndIndex = 0;
    timesWriteStartIndex = 0;
    timesWriteEndIndex = 0;
    readIndex = 0;
}

bool StorageController::write(char c)
{
    //Prep for the new byte
    if(!recordingEnabled) //dump
    {
        return false;
    }
    if(!circularModeEnabled)
    {
        if(dataArrayFull() || timesArrayFull())
        {
            return false;
        }
    }
    
    //Write the index
    uint32_t writeIndex = dataWriteEndIndex;
    data[writeIndex] = c;
    
    //Advance the end position
    dataWriteEndIndex++;
    if(dataWriteEndIndex >= DATA_ARRAY_SIZE_BYTES) dataWriteEndIndex -= DATA_ARRAY_SIZE_BYTES;
    //nudge the start position
    if(dataWriteStartIndex == dataWriteEndIndex)
    {
        dataWriteStartIndex++;
        if(dataWriteStartIndex >= DATA_ARRAY_SIZE_BYTES) dataWriteStartIndex -= DATA_ARRAY_SIZE_BYTES;
    }

    //Save the time
    //  If there's no times saved generate a fresh one
    SC_byteTimeData_t * pTimeData;
    if(timesWriteStartIndex == timesWriteEndIndex)
    {
        //no current stamps, create one
        pTimeData = &times[timesWriteEndIndex];
        pTimeData->index = writeIndex;
        pTimeData->time = time;
        timesWriteEndIndex++;
        if(timesWriteEndIndex > TIME_ARRAY_SIZE_ITEMS) timesWriteEndIndex = 0;
        return true;
    }
    //index to the previous one
    uint32_t prevIndex = timesWriteEndIndex - 1;
    if(prevIndex < 0)prevIndex = TIME_ARRAY_SIZE_ITEMS;
    pTimeData = &times[prevIndex];
    //  If current time is sooner than the stamp time, make a new one
    
    if(time == pTimeData->time)
    {
        //Current timestamp still applies
        return true;
    }
    pTimeData = &times[timesWriteEndIndex];
    pTimeData->index = writeIndex;
    pTimeData->time = time;
    timesWriteEndIndex++;
    if(timesWriteEndIndex >= TIME_ARRAY_SIZE_ITEMS) timesWriteEndIndex -= TIME_ARRAY_SIZE_ITEMS;
    //nudge the start position
    if(timesWriteStartIndex == timesWriteEndIndex)
    {
        timesWriteStartIndex++;
        if(timesWriteStartIndex >= TIME_ARRAY_SIZE_ITEMS) timesWriteStartIndex -= TIME_ARRAY_SIZE_ITEMS;
    }

    return true;
}

void StorageController::updateUSTime(uint32_t uSTicks)
{
    time = uSTicks;
    
    //Todo: overflow?
}

void StorageController::circularMode(bool isCircular)
{
    circularModeEnabled = isCircular;
}

void StorageController::record(bool isRecording)
{
    recordingEnabled = isRecording;
}

//Move to oldest
bool StorageController::moveReadHead(void)
{
    return false;
}

//Move to first byte on or after given time
bool StorageController::moveReadHead(uint32_t uSTime)
{
    return false;
}

SC_readStatus_t StorageController::read(char* pC, uint32_t* time)
{
    //TODO H)OW TO REEED?
    
    return SC_READ_OK;
}

//  in, out,  (sub), delta
//   3,   5,      2,     2
//   299, 2,   -297,     3
//     9, 8,     -1,   299
// and on a (DATA_ARRAY_SIZE_BYTES - 1) we are full.
bool StorageController::dataArrayFull(void)
{
    int32_t indexDelta = dataWriteEndIndex - dataWriteStartIndex;
    if(indexDelta < 0)
    {
        //rolled
        indexDelta += DATA_ARRAY_SIZE_BYTES;
    }
    if(indexDelta == (DATA_ARRAY_SIZE_BYTES - 1))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool StorageController::timesArrayFull(void)
{
    int32_t indexDelta = timesWriteEndIndex - timesWriteStartIndex;
    if(indexDelta < 0) indexDelta += TIME_ARRAY_SIZE_ITEMS;
    if(indexDelta == (TIME_ARRAY_SIZE_ITEMS - 1)) return true;
    return false;
}
