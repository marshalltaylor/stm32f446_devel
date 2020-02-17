#include "globals.h"
#include "sketch.h"

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


SlidersPanel mainPanel;

uint16_t debugCounter = 0;

TimerClass32 debugTimer( 3000000 );
TimerClass32 mainPanelTimer( 5000 );
TimerClass32 lcdFrameTimer( 100000 );
bool lcdFrameTimerRetick = false;

int32_t executionTimes[4] = {0};

#if defined(__arm__)
extern "C" char* sbrk(int incr);
static int FreeStack() {
  char top = 't';
  return &top - reinterpret_cast<char*>(sbrk(0));
}
#endif


/* Functions -----------------------------------------------------------------*/
/***** Main MIDI Callbacks ****************************************************/
void handleClock( void )
{
	extMidiClock.incrementTick();
	//localPrintf("Clock\n");
}

void handleStart( void )
{
	extMidiClock.setTickCount(-1);
	extMidiClock.setState(Playing);
}

void handleContinue( void )
{
	extMidiClock.setState(Playing);
}

void handleStop( void )
{
	extMidiClock.setState(Stopped);
	oled.setPlayHead(0);
}
	
void handleNoteOn(byte channel, byte pitch, byte velocity)
{
	MidiMessage newMsg;
	newMsg.controlMask = NoteOn;
	newMsg.channel = channel;
	newMsg.value = pitch;
	newMsg.data = velocity;
	
	//Recorder
	if( mainPanel.isRecording() )
	{
		myRecorder.recordNote(&newMsg);
		myRecorder.printDebug();
	}
}

void handleNoteOff(byte channel, byte pitch, byte velocity)
{
	//MidiMessage newMsg;
	//newMsg.controlMask = NoteOff;
	//newMsg.channel = channel;
	//newMsg.value = pitch;
	//newMsg.data = velocity;
	////digitalWrite(D6, 1);
	////controlNoteMixer.input( 0x08, channel, pitch, velocity );
	//myArpeggiator.writeRoot(&newMsg);
	////outputNoteMixer.keyboardInput( &newMsg );
}

void handleCtrlNoteOn(byte channel, byte pitch, byte velocity)
{
	MidiMessage newMsg;
	newMsg.controlMask = NoteOn;
	newMsg.channel = channel;
	newMsg.value = pitch;
	newMsg.data = velocity;
	
	myArpeggiator.writeRoot(&newMsg);
}

void handleCtrlNoteOff(byte channel, byte pitch, byte velocity)
{
	//Should be copyish of other
	MidiMessage newMsg;
	newMsg.controlMask = NoteOff;
	newMsg.channel = channel;
	newMsg.value = pitch;
	newMsg.data = velocity;
	myArpeggiator.writeRoot(&newMsg);
}

/***** Control MIDI Callbacks *************************************************/
void handleAltClock( void )
{
	//extMidiClock.incrementTick();
	localPrintf("Alt Clock\n");
}

/***** Hardware Timer Callbacks ***********************************************/
void hwTimerCallback(void)
{
	// Things!
	intMidiClock.incrementTime_uS(100);
}

/***** Clock Socket Callbacks *************************************************/
void sketchBeatCallback(MidiClock * caller)
{
	localPrintf("....beat\n");
}

void sketchTickCallback(MidiClock * caller)
{
	//char buffer[5];
	switch(caller->getState())
	{
		case Stopped:
		{
			MIDI.sendRealTime(midi::Clock);
		}
		break;
		case OutputOff:
		{
		}
		default:
		case Playing:
			oled.setPlayHead(caller->ticks);
			//myPlayer.updateTicks(caller->ticks);
			//if(myPlayer.available())
			//{
			//	mmqObject_t readObject;
			//	myPlayer.read(&readObject);
			//	MIDI.send((midi::MidiType)readObject.controlMask, readObject.value, readObject.data, 1);
			//	localPrintf("MIDI OUT!");
			//}
			myArpeggiator.updateTicks(caller->ticks);
			if(myArpeggiator.available())
			{
				mmqObject_t readObject;
				myArpeggiator.read(&readObject);
				MIDI.send((midi::MidiType)readObject.controlMask, readObject.value, readObject.data, 1);
				localPrintf("MIDI OUT!");
			}			
		case Paused:
			localPrintf("ticks = %d\n", caller->ticks);
			MIDI.sendRealTime(midi::Clock);
		break;
		break;
	}

}

/***** Program ****************************************************************/
void setup()
{
	//Serial2.begin(9600, 6);
	//Serial6.begin(12345, 1);
	//delay(2000);

	mainMidiInterface.initPort(COM1);
	ctrlMidiInterface.initPort(COM1);
	
	localPrintf("OK\n");
	
	// Write our function address into the hw timer
	timer3TickCallback = hwTimerCallback;
	
	// Configure Clocks and Sockets
	intMidiClock.setBPM(100);
	clockSocket.SetBeatCallback(sketchBeatCallback);
	clockSocket.SetTickCallback(sketchTickCallback);
	clockSocket.SwitchMidiClock(&extMidiClock);
	
	extMidiClock.setState(Stopped);
	
	// Test config loop
	//outputPlayer.setLoop(&loops[0]);

	// Go to fresh state
	mainPanel.reset();

	// Configure MIDI objects
	MIDI.setHandleClock(handleClock);
	MIDI.setHandleStart(handleStart);
	MIDI.setHandleContinue(handleContinue);
	MIDI.setHandleStop(handleStop);
	MIDI.setHandleNoteOn(handleNoteOn);
    MIDI.setHandleNoteOff(handleNoteOff);
    MIDI.begin(MIDI_CHANNEL_OMNI);
	MIDI.turnThruOff();
	
	CtrlMIDI.setHandleClock(handleAltClock);
	CtrlMIDI.setHandleNoteOn(handleCtrlNoteOn);
    CtrlMIDI.setHandleNoteOff(handleCtrlNoteOff);
    CtrlMIDI.begin(MIDI_CHANNEL_OMNI);
	CtrlMIDI.turnThruOff();
	
	// Set up sequences
	sReg[REG_ARP].clear();
	myRecorder.attachMainRegister(&sReg[REG_ARP]);
	myRecorder.clearAndInit();
	myPlayer.attachMainRegister(&sReg[REG_ARP]);
	myArpeggiator.attachMainRegister(&sReg[REG_ARP]);
	sDebug.attachMainRegister(&sReg[REG_ARP]);
	
	// Set up LCD to segment
	oled.attachMainRegister(&sReg[REG_ARP]);
	
}

void loop()
{
	MIDI.read();
	CtrlMIDI.read();
	//TODO: Old serials
	//if(Serial6.available())
	//{
	//	Serial6.println((char)Serial6.read());
	//}
	if( usTicksLocked == 0 )
	{
		mainPanelTimer.update(usTicks);
		debugTimer.update(usTicks);
		lcdFrameTimer.update(usTicks);
		//statusPanelTimer.update(usTicks);
		//Done?  Lock it back up
		usTicksLocked = 1;
	}  //The ISR will unlock.

	if(mainPanelTimer.flagStatus() == PENDING)
	{
		int32_t time = ulGetFastRunTimeTicks();
		
		mainPanel.tickStateMachine(10);
		
		int32_t delta = ulGetFastRunTimeTicks() - time;
		if( delta > executionTimes[0] ) executionTimes[0] = delta;
	}

	if(lcdFrameTimer.flagStatus() == PENDING || oled.isProcessingFrame())
	{
		int32_t time = ulGetFastRunTimeTicks();
		
		if( !oled.isProcessingFrame() )
		{
			oled.drawFullScreen();
			//LCD was busy, frame was not processed at all.  hold this timer and keep checking.
			//lcdFrameTimerRetick = true;
		}
		oled.processDirectAccess();
		int32_t delta = ulGetFastRunTimeTicks() - time;
		if( delta > executionTimes[1] ) executionTimes[1] = delta;
	}


//	if(statusPanelTimer.flagStatus() == PENDING)
//	{
//		statusPanel.tickStateMachine(400);
//		//intMidiClock.incrementTime_uS(400);
//		intMidiClock.service();
//	}
	
	if(debugTimer.flagStatus() == PENDING)
	{
		if(1)
		{
			//User code
			//localPrintf("__DEBUG______\nintPlayState = %d, extPlayState = %d\nbeatLedState = %d, playLedState = %d\nFreeStack() = %d\n\n", intMidiClock.getState(), extMidiClock.getState(), statusPanel.getBeatLedState(), statusPanel.getPlayLedState(), FreeStack());
			localPrintf("\n\n__DEBUG__\nFreeStack() = %d\n", FreeStack());
			localPrintf("panel loop peak: %ld\n", executionTimes[0]);
			executionTimes[0] = 0;
			localPrintf(" oled loop peak: %ld\n", executionTimes[1]);
			executionTimes[1] = 0;
			
			//Object debugs -- enable as needed
			
			//mainPanel.printDebug();
			//extMidiClock.printDebug();
			//sDebug.printDebug();
			myArpeggiator.printDebug();
		
			CtrlMIDI.sendRealTime(midi::Clock);
		}
	}
	
}
