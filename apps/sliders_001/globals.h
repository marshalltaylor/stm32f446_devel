//Header
#ifndef GLOBALS_H_INCLUDED
#define GLOBALS_H_INCLUDED

/* Includes -- STD -----------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

/* Includes -- BSP -----------------------------------------------------------*/
#include "bsp.h"

/* Includes -- modules -------------------------------------------------------*/
#include "logging.h"
#include "midi47fx.h"
#include "midiTime.h"
#include "TeensyView.h"

/* Includes -- App -----------------------------------------------------------*/
// base
#include "SlidersPanel.h"
#include "sourceNoteMixer.h"
#include "outputNoteMixer.h"

// in folders
#include "sourceNoteMixer.h"
#include "outputNoteMixer.h"
#include "SequenceTeensyView.h"
#include "SequencePlayer.h"
#include "SequenceArpeggiator.h"
#include "SequenceRecorder.h"
#include "SequenceDebug.h"


/* References ----------------------------------------------------------------*/

//Defined in bspTime.h
extern uint8_t usTicksLocked;
extern uint32_t usTicks;

//LCD
extern SequenceTeensyView oled;

//MIDI hw interfaces
extern HardwareSerial mainMidiInterface;
extern HardwareSerial ctrlMidiInterface;

extern midi::MidiInterface<HardwareSerial> MIDI;
extern midi::MidiInterface<HardwareSerial> CtrlMIDI;

//MIDI clocks
extern MidiClock extMidiClock;
extern MidiClock intMidiClock;
extern MidiClockSocket clockSocket;

//Mixers
extern SourceNoteMixer controlNoteMixer;
extern OutputNoteMixer outputNoteMixer;

//Sequence
enum
{
	REG_ARP = 0,
	REG_BASS,
	REG_DRUMS,
	REG_LENGTH
};

extern SequenceRegister sReg[];

extern SequencePlayer myPlayer;
extern SequenceArpeggiator myArpeggiator;
extern SequenceRecorder myRecorder;
extern SequenceDebug sDebug;

/* Functions -----------------------------------------------------------------*/

#endif // GLOBALS_H_INCLUDED