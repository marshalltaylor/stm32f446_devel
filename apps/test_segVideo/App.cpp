#include "App.h"
#include "Arduino.h"
#include "interface.h"
#include "sketch.h"
#include "os.h"

#include "MidiClockDisplay.h"

MidiClockDisplay Segments;

int AppEntry(void)
{
	// Arduino (serial objects) init
	interface_init();
	
	setup();
	
	//Init and run operating system.  This starts all tasks
	osInit();
	
	/* We should never get here as control is now taken by the scheduler */
	while(1)
	{
		//loop(); // Legacy execution
	}
}
