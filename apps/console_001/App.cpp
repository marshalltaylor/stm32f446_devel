/* Includes -- STD -----------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

/* Includes -- BSP -----------------------------------------------------------*/
#include "bsp.h"

/* Includes -- modules -------------------------------------------------------*/
#include "midi47fx.h"
#include "logging.h"
#include "uCModules.h"

//#include "SegmentVideo.h"

/* Includes -- App -----------------------------------------------------------*/
#include "App.h"

/* References ----------------------------------------------------------------*/
//#define USE_LOGGING
#ifdef USE_LOGGING
// Create logging object and macro for local printf
#define localPrintf dummy.printf
Logging dummy;

#else
// Connect directly to bsp.
#define localPrintf bspPrintf

#endif

#if defined(__arm__)
extern "C" char* sbrk(int incr);
__attribute__((used)) static int FreeStack() {
  char top = 't';
  return &top - reinterpret_cast<char*>(sbrk(0));
}
#endif

int AppEntry(void)
{
	localPrintf("Program.\n");
	return -1;
}
