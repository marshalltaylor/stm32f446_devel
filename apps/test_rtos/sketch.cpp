#include "Arduino.h"
#include "interface.h"
#include <MIDI.h>
#include "adc_ext.h"

#include "timerModule32.h"
#include "Arduino.h"

#define Serial Serial6

uint16_t debugCounter = 0;

TimerClass32 debugTimer( 1000000 ); //1 second
TimerClass32 mainPanelTimer( 10000 );
TimerClass32 statusPanelTimer( 400 );
TimerClass32 segmentVideoTimer( 5000 );


void hwTimerCallback(void)
{
	// Things!
//	intMidiClock.incrementTime_uS(100);
//	intMidiClock.service();
}

#ifdef __cplusplus
extern "C" {
#endif

void setup(void)
{
	// Start the fake Arduino interface stuff
	interface_init();

	//pinMode(D6, OUTPUT);
	Serial2.begin(9600, 6);
	Serial6.begin(12345, 1);
	//delay(2000);
	//Serial2.println("ok");
	Serial6.println("OK");

	// Write our function address into the hw timer
	//timer3TickCallback = &MidiClock::hwTimerCallback;
	timer3TickCallback = hwTimerCallback;

	//Go to fresh state
	
}

void taskMidi(void)
{
	//MIDI.read();
}

void taskHardware(void)
{
	convertADC();
	//intMidiClock.service();
	//statusPanel.tickStateMachine(400);
	udelay(400);
}

void taskPanel(void)
{
	//mainPanel.tickStateMachine(10);
	//Segments.tickValueStateMachine();	

	//Segments.processEffects();
	//Segments.writeNextFrame();

	udelay(5000);
	
}

//#if defined(__arm__)
//extern "C" char* sbrk(int incr);
//static int FreeStack() {
//  char top = 't';
//  return &top - reinterpret_cast<char*>(sbrk(0));
//}
//#endif

void taskConsole(void)
{
	if(Serial6.available())
	{
		Serial6.println((char)Serial6.read());
	}
	//if( usTicksLocked == 0 )
	//{
	//	mainPanelTimer.update(usTicks);
	//	debugTimer.update(usTicks);
	//	statusPanelTimer.update(usTicks);
	//	segmentVideoTimer.update(usTicks);
	//	//Done?  Lock it back up
	//	usTicksLocked = 1;
	//}  //The ISR will unlock.
    //
	//if(debugTimer.flagStatus() == PENDING)
	//{
		delay(5000);
		//User code
		char buffer[200] = {0};
		//sprintf(buffer, "__DEBUG______\nintPlayState = %d, extPlayState = %d\nbeatLedState = %d, playLedState = %d\n\n", intMidiClock.getState(), extMidiClock.getState(), statusPanel.getBeatLedState(), statusPanel.getPlayLedState());
		//sprintf(buffer, "__DEBUG__\n");
		sprintf(buffer, "__DEBUG__ [%d]\n", (int16_t)millis());
		Serial6.print(buffer);
		//Serial6.println(mainPanel.getState());
		//Serial6.print("Playing: ");
	//}

	
}


#ifdef __cplusplus
}
#endif

