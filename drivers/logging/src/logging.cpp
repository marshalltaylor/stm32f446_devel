#include <string.h>

#include "logging.h"

Logging::Logging(const char* name)
{
	bspVPrintf = bspGetSerialConsoleVPrintf();
	uint8_t sz = strlen(name);
	printf(" sz: %d\n", sz);
	if(sz > LOGGING_STAMP_MAX_LEN - 4)
	{
		sz = LOGGING_STAMP_MAX_LEN - 4;
	}
	
	setStamp(name, sz);
}

void Logging::printf(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	if(bspVPrintf)
	{
		if(logMode == LOG_MODE_AUTO)
		{
			bspPrintf("%s", stamp);
		}

		bspVPrintf(fmt, args);
	}

	va_end(args);
}

void Logging::setVPrintf(bspVPrintf_t function)
{
	bspVPrintf = function;
}

void Logging::setStamp(const char * data, uint8_t len)
{
	int indexOut = 0;
	stamp[indexOut++] = '[';
	stamp[indexOut++] = ' ';
	for(int indexIn = 0; indexIn < LOGGING_STAMP_MAX_LEN; indexIn++)
	{
		if(indexIn < len)
		{
			stamp[indexOut++] = data[indexIn];
		}
		else
		{
			stamp[indexOut++] = ' ';
		}
	}
	stamp[indexOut++] = ' ';
	stamp[indexOut++] = ']';
	stamp[indexOut++] = ' ';
	stamp[indexOut++] = '\0';
}

void Logging::setMode(loggingMode_t mode)
{
	logMode = mode;
}
