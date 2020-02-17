/* Includes -- STD -----------------------------------------------------------*/
//#include <stdint.h>
//#include <stdbool.h>
//#include <stdarg.h>
//#include <string.h>
//#include <stdio.h>

/* Includes -- BSP -----------------------------------------------------------*/
#include "bsp.h"

/* Includes -- modules -------------------------------------------------------*/
#include "logging.h"
#include "midiUtils.h"
//#include "midi47fx.h"
//#include "midiTime.h"
//#include "TeensyView.h"

/* Includes -- App -----------------------------------------------------------*/
#include "SequenceDebug.h"

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

SequenceDebug::SequenceDebug(void)
{
	mainRegister = NULL;
}

void SequenceDebug::attachMainRegister( SequenceRegister * targetRegister )
{
	// Remove old association
	if( mainRegister )
	{
		mainRegister->removeUser(this);
	}
	// Subscribe to new register
	mainRegister = targetRegister->addUser(this);
}

void SequenceDebug::printDebug( void )
{
	if( mainRegister == NULL )
	{
		return;
	}
	Sequence * seq = mainRegister->getSequence();
	
	localPrintf("SequenceDebug:\n");
	localPrintf("step note gate velo \n-----------------\n");
	for( int i = 0; i < seq->stepsUsed; i++ )
	{
		localPrintf("  %3d, %3d, %3d, %3d\n", i, seq->step[i].value, seq->step[i].gated, seq->step[i].velocity);
	}
	localPrintf("-----------------\n");
	localPrintf("Pattern length: %d\n", seq->tapeLengthInTicks);
	localPrintf("ticksPerStep: %d\n", seq->ticksPerStep);
}
