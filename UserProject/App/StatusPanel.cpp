//********************************************//
#include "StatusPanel.h"
#include "panelComponents.h"
#include "HardwareInterfaces.h"
#include <Arduino.h>
#include "midiTime.h"
#include "flagMessaging.h"
#include "timeKeeper32.h"
#include "BlinkerPanel.h"
#include "SegmentVideo.h"
#include <MIDI.h>
#include "midi_Defs.h"
extern midi::MidiInterface<HardwareSerial> MIDI;

extern MidiClock extMidiClock;
extern MidiClock intMidiClock;

extern SegmentVideo Segments;

//extern BlinkerPanel mainPanel;
extern StatusPanel statusPanel;

void StatusPanel::BeatCallback(MidiClock * caller)
{
	char buffer[4];
	// Screen logic
	switch(caller->getState())
	{
		case Stopped:
		{
			sprintf( buffer, "----" );
		}
		break;
		case OutputOff:
		{
			sprintf( buffer, "    " );
		}
		break;
		default:
		{
			sprintf( buffer, "%2d%2d", caller->ticksToMeasures(caller->ticks), caller->ticksToQuarterNotes(caller->ticks) );
		}
		break;
	}
	Segments.displayDrawClockNums(buffer);

	// LED logic
	switch(caller->getState())
	{
		case Stopped:
		case Playing:
		{
			statusPanel.beatFlag.setFlag();
			statusPanel.beatFlag.clearFlag();
			statusPanel.playFlag.setFlag();
			statusPanel.playFlag.clearFlag();
		}
		break;
		default:
		break;
	}

}

void StatusPanel::TickCallback(MidiClock * caller)
{
	//Send midi
	if(caller->outputEnabled)
	{
		MIDI.sendRealTime(midi::Clock);
	}

}

StatusPanel::StatusPanel( void )
{
	ledBeat.setHardware(new ArduinoDigitalOut( A5 ), 1);
	add( &ledBeat );
	ledPlay.setHardware(new ArduinoDigitalOut( D34 ), 1);
	add( &ledPlay );

	reset();
	//freshenComponents( 1 );
}

void StatusPanel::reset( void )
{
	//Set explicit states
	//Set all LED off
	ledBeat.setState(LEDOFF);
	ledPlay.setState(LEDOFF);
	beatLedState = BeatLedStateInit;
	ClockSocket->socketed = NULL;
	
}

void StatusPanel::tickStateMachine( int usTicksDelta )
{
	beatTimeKeeper.uIncrement( usTicksDelta );
	playTimeKeeper.uIncrement( usTicksDelta );
	
	// I don't think LEDs use the value here, normally you would pass msTicksDelta
	freshenComponents( 1 );
	
	//***** PROCESS THE LOGIC *****//
	MidiClock * clock = ClockSocket->socketed;
	
	//Now do the states.
	switch( beatLedState )
	{
	case BeatLedStateInit:
		beatLedState = BeatLedStateOff;
		break;
	case BeatLedStateOff:
		if( beatFlag.serviceRisingEdge() )
		{
			beatLedState = BeatLedStateOn;
			ledBeat.setState(LEDON);
			beatTimeKeeper.uClear();
		}
		break;
	case BeatLedStateOn:
		if( beatTimeKeeper.uGet() > 10000 )
		{
			beatLedState = BeatLedStateOff;
			ledBeat.setState(LEDOFF);
		}
		break;
	default:
		beatLedState = BeatLedStateInit;
		break;
	}

	switch( playLedState )
	{
	case PlayLedStateInit:
		playLedState = PlayLedStateOff;
		break;
	case PlayLedStateOff:
		if( clock->getState() == Playing )
		{
			playLedState = PlayLedStateOn;
			Serial6.println(" to PlayLedStateOn");
			ledPlay.setState(LEDON);
		}
		else if( playFlag.serviceRisingEdge() )
		{
			if( clock->getState() == Paused )
			{
				playLedState = PlayLedStateBlink;
				Serial6.println(" to PlayLedStateBlink");
				ledPlay.setState(LEDON);
				playTimeKeeper.uClear();
			}
		}

		break;
	case PlayLedStateOn:
		if( clock->getState() == Stopped )
		{
			playFlag.serviceRisingEdge();
			playLedState = PlayLedStateOff;
			Serial6.println(" to PlayLedStateOff");
			ledPlay.setState(LEDOFF);
		}
		break;
	case PlayLedStateBlink:
		if( playTimeKeeper.uGet() > 600 )
		{
			playLedState = PlayLedStateOff;
			Serial6.println(" to PlayLedStateOff");
			ledPlay.setState(LEDOFF);
		}
		break;
	default:
		playLedState = PlayLedStateInit;
		break;
	}

}
