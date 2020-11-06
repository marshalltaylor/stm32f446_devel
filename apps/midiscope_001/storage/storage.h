#ifndef STORAGE_H
#define STORAGE_H

#include "src/templatePrivate.h"

typedef struct midiMetaData_t
{
    int time;
    void* data;
    int len;
    
    midiMetaData_t * next;
} midiMetaData_t;

class StorageController
{
public:
    StorageController(void);
    bool write(void* pData, int len);
    void clear(void);
    
private:
    void * data;
};

#endif
