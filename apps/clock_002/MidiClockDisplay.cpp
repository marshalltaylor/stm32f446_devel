#include "display_clock.h"
#include "SegmentVideo.h"
#include "MidiClockDisplay.h"
#include "Arduino.h"
// normal_distribution
#include <iostream>
#include <string>
#include <random>

std::default_random_engine generator;
std::normal_distribution<float> distribution(5.0,2.0);

const uint8_t maskBitmaps[6][11] =
{
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF}, // Clock digits only
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0xFF}, // Most Clock digits only
	{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // All 1s
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // All 0s
	{0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
	
};

enum maskNames
{
	CLOCK_DIGITS_ONLY,
	MOST_CLOCK_DIGITS,
	ALL_ONES,
	ALL_ZEROS,
	VALUE_DIGITS_ONLY
};

const uint8_t ValueFadeMap[16][11] =
{
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x03, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x03, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x23, 0x23, 0x23, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x23, 0x23, 0x23, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x63, 0x63, 0x63, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x63, 0x63, 0x63, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x67, 0x67, 0x67, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x67, 0x67, 0x67, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x77, 0x77, 0x77, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x77, 0x77, 0x77, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x7F, 0x7F, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x7F, 0x7F, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
};

static uint8_t segmentMap[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67, 0x00, 0x00};

void MidiClockDisplay::displayDrawClockNums( const char * input )
{
	for( int i = 0; i < 4; i++ )
	{
		textBitmap[i + 7] = 0x00;
		if( *(input + i) == '-' )
		{
			textBitmap[i + 7] = 0x40;
		}
		else if((*(input + i) >= 0x30)&&(*(input + i) < 0x3A))
		{
			textBitmap[i + 7] = segmentMap[*(input + i) - 0x30];
		}
	}
}

void MidiClockDisplay::displayDrawValue( const char * input )
{
	for( int i = 0; i < 3; i++ )
	{
		textBitmap[i + 2] = 0x00;
		if((*(input + i) >= 0x30)&&(*(input + i) < 0x3A))
		{
			textBitmap[i + 2] = segmentMap[*(input + i) - 0x30];
		}
	}
}

void MidiClockDisplay::setPlayIndicator( void )
{
	textBitmap[0] |= 0x80;
}

void MidiClockDisplay::clearPlayIndicator( void )
{
	textBitmap[0] &= ~0x80;
}

void MidiClockDisplay::toggleClockColon(void)
{
	textBitmap[0] ^= 0x08;
}

void MidiClockDisplay::processEffects(void)
{
	if( debugNoise == 0 )
	{
		return;
	}
	float number = distribution(generator);
	if(number < 0.01)
	{
		noise_layer.write(maskBitmaps[3], maskBitmaps[2]);
		noise_layer.write(maskBitmaps[3], maskBitmaps[2]);
		noise_layer.write(maskBitmaps[3], maskBitmaps[2]);
		noise_layer.write(maskBitmaps[3], maskBitmaps[3]);
	}
	else if(number < 0.8)
	{
		uint8_t tempBuffer[11];
		for(int i = 0; i < 11; i++)
		{
			tempBuffer[i] = 0;
		}
		tempBuffer[2 + (rand() % 9)] = 0x01 << (rand() % 7);
		noise_layer.write(tempBuffer, maskBitmaps[3]);
		noise_layer.write(maskBitmaps[3], maskBitmaps[3]);
	}
}

void MidiClockDisplay::showNewValue( const char * input )
{
	if( input == currentValue )
	{
		displaySMTK.uClear();
		// buffer it to frames
		displayDrawValue(currentValue);
	}
	else
	{
		newValueRequested = true;
		nextValue = input;
	}		
}

void MidiClockDisplay::tickValueStateMachine( void )
{
	switch( displayState )
	{
		case init:
		{
			displayState = idle;
			valueMask_layer.write(maskBitmaps[VALUE_DIGITS_ONLY], maskBitmaps[ALL_ZEROS]);
		}
		break;
		case idle:
		{
			if(newValueRequested)
			{
				currentValue = nextValue;
				newValueRequested = false;
				displayState = fadeIn;
			}
		}
		break;
		case fadeIn:
		{
			if( valueMask_layer.ready() )
			{
				for( int i = 15; i >= 0; i--)
				{
					valueMask_layer.write(maskBitmaps[ALL_ZEROS], &ValueFadeMap[i][0]);
				}
				displaySMTK.uClear();
				displayState = waitForFadeIn;
			}
		}
		break;
		case waitForFadeIn:
		{
			if( valueMask_layer.empty() )
			{
				displayState = waitForTimeout;
			}
		}
		break;
		case waitForTimeout:
		{
			if(( displaySMTK.uGet() > 2500000 ) || newValueRequested)
			{
				displayState = fadeOut;
			}
		}
		break;
		case fadeOut:
		{
			if( valueMask_layer.ready() )
			{
				for( int i = 0; i < 16; i++)
				{
					valueMask_layer.write(maskBitmaps[ALL_ZEROS], &ValueFadeMap[i][0]);
				}
				displayState = waitForFadeOut;
				
			}
		}
		break;
		case waitForFadeOut:
		{
			if( valueMask_layer.empty() )
			{
				currentValue = blankValue;
				displayState = idle;
			}
		}
		break;
		default:
		displayState = init;
		break;
	}
}

uint8_t MidiClockDisplay::getValueState( void ){
	return (uint8_t)displayState;
}
