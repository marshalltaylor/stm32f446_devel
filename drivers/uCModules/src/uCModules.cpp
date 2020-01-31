#include "bsp.h"
#include "../uCModules.h"
#include "uCModulesPrivate.h"

DebugUtils dbg;

DebugUtils::DebugUtils(void)
{
	bspVPrintf = bspGetSerialConsoleVPrintf();
}

void DebugUtils::printf(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	if(bspVPrintf)
	{
		bspVPrintf(fmt, args);
	}
	//else if(loggerVPrintf)
	//{
	//}
	va_end(args);
}

void DebugUtils::setVPrintf(bspVPrintf_t function)
{
}
