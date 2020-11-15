#ifndef STORAGE_H
#define STORAGE_H

#include <stdint.h>

extern void test_storage(void);

#define DATA_ARRAY_SIZE_BYTES 25
#define TIME_ARRAY_SIZE_ITEMS 10

typedef struct SC_byteTimeData_t
{
    uint32_t index;
    uint32_t time;
} SC_byteTimeData_t;

typedef enum SC_readStatus_t
{
    SC_READ_OK = 0,
    SC_READ_NO_DATA,
    SC_READ_ERROR_UNKNOWN
} SC_readStatus_t;

// Controller API operates with index 0 = beginning of history
// If the controller is given data to overlap, 0 is moved.
// Read operations handle bad data having to do with overlap
//
// 'empty' determined when start and end indices are the same
// 'full' is determined when the end trails the start by one
class StorageController
{
public:
    StorageController(void);
    //system
    bool write(char c);
    void updateUSTime(uint32_t uSTicks);
    void dumpVars(void);
    //user
    void circularMode(bool isCircular);
    void record(bool isRecording);
    bool moveReadHead(void); //Move to oldest
    bool moveReadHead(uint32_t uSTime); //Move to first byte on or after given time
    SC_readStatus_t read(char* pC, uint32_t* time);
    void clear(void);
    
private:
    //Data array
    bool dataArrayFull(void);
    char * data;
    uint32_t dataWriteStartIndex;
    uint32_t dataWriteEndIndex;
    //Times array
    bool timesArrayFull(void);
    SC_byteTimeData_t * times;
    uint32_t timesWriteStartIndex;
    uint32_t timesWriteEndIndex;
    //Control
    uint32_t time;
    bool circularModeEnabled;
    bool recordingEnabled;
    uint32_t readIndex;

};

#endif
