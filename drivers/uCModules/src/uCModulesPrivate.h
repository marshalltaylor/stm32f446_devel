#ifndef UCMODULESPRIVATE_H
#define UCMODULESPRIVATE_H

#include "bsp.h"

class DebugUtils
{
public:
	DebugUtils(void);
	void printf(const char* fmt, ...);
	void setVPrintf(bspVPrintf_t function);

private:
	bspVPrintf_t bspVPrintf = NULL;
};

#endif
