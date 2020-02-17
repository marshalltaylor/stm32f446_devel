#include "globals.h"

HardwareSerial mainMidiInterface;
HardwareSerial ctrlMidiInterface;
MIDI_CREATE_INSTANCE(HardwareSerial, mainMidiInterface,  MIDI);
MIDI_CREATE_INSTANCE(HardwareSerial, ctrlMidiInterface, CtrlMIDI);

MidiClock extMidiClock;
MidiClock intMidiClock;
MidiClockSocket clockSocket;

//Note objects
SourceNoteMixer controlNoteMixer;
//Loop loops[10];

//LoopPlayer outputPlayer;
OutputNoteMixer outputNoteMixer;

SequenceTeensyView oled(0,0,0,0,0);

SequenceRegister sReg[REG_LENGTH];

SequencePlayer myPlayer;
SequenceArpeggiator myArpeggiator;
SequenceRecorder myRecorder;
SequenceDebug sDebug;