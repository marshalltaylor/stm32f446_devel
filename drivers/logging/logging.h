#ifndef LOGGING_H
#define LOGGING_H

#include <stddef.h>

#include "bsp.h"

#define LOGGING_STAMP_MAX_LEN 10
#define LOGGING_LINE_MAX_LEN 100

typedef enum
{
	LOG_MODE_DEFAULT = 0,
	LOG_MODE_AUTO,
	LOG_MODE_MAX
} loggingMode_t;

class Logging
{
public:
	Logging(const char* name);
	void printf(const char* fmt, ...);
	void setVPrintf(bspVPrintf_t function);
	void setStamp(const char * data, uint8_t len);
	void setMode(loggingMode_t mode);
	
private:
	bspVPrintf_t bspVPrintf = NULL;
	loggingMode_t logMode = LOG_MODE_DEFAULT;
	char stamp[LOGGING_STAMP_MAX_LEN + 1] = {0};
	char line[LOGGING_STAMP_MAX_LEN + LOGGING_LINE_MAX_LEN + 1] = {0};
};

#endif
