#include "globals.h"

HardwareSerial mainMidiInterface;
//HardwareSerial ctrlMidiInterface;

MIDI_CREATE_INSTANCE(HardwareSerial, mainMidiInterface,  MIDI);
//MIDI_CREATE_INSTANCE(HardwareSerial, ctrlMidiInterface, CtrlMIDI);

//MidiClock extMidiClock;
//MidiClock intMidiClock;
//MidiClockSocket clockSocket;
//
////Note objects
//SourceNoteMixer controlNoteMixer;
////Loop loops[10];
//
////LoopPlayer outputPlayer;
//OutputNoteMixer outputNoteMixer;
//
//SequenceTeensyView oled(0,0,0,0,0);
//
//SequenceRegister sReg[REG_LENGTH];
//
//SequencePlayer myPlayer;
//SequenceArpeggiator myArpeggiator;
//SequenceRecorder myRecorder;
//SequenceDebug sDebug;

StorageController trace1;


/***** Hardware Timer Callbacks ***********************************************/
extern uint32_t usTicks;
void hwTimerCallback(void)
{
	// Things!
	//intMidiClock.incrementTime_uS(100);
    trace1.updateUSTime(usTicks);
}

//Usage:
// This function doesn't have a prototype because the header that would
// have it (globals.h) has c++ linkage which will cause problems if included
// in c code.
//
// Add 'extern void globalsInit(void);' to calling file
extern "C" void globalsInit(void)
{
	mainMidiInterface.initPort(COM1);
	//ctrlMidiInterface.initPort(COM2);

    trace1.clear();

	// Write our function address into the hw timer
	timer3TickCallback = hwTimerCallback;	

}