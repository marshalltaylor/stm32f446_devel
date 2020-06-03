/* Includes -- STD -----------------------------------------------------------*/
#include <stdint.h>
//#include <stdbool.h>
//#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Includes -- BSP -----------------------------------------------------------*/
#include "bsp.h" // For logging

/* Includes -- FreeRTOS system -----------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "event_groups.h"
#include "os.h"

/* Includes -- FreeRTOS app --------------------------------------------------*/
#include "taskLog.h"

/* Includes -- modules -------------------------------------------------------*/
#include "CRTVideo.h"

#include "game.h"
#include "videoData.h"
#include "game_data_001.h"
#include "bitmaps.h"

/* References ----------------------------------------------------------------*/
#define UNUSED(x) (void)x

//FreeRTOS queue interface for logging
static void localPrintf(const char* fmt, ...)
{
	//Setup
	va_list args;
	va_start(args, fmt);
	//Log

	strMsg_t * msg = new strMsg_t();
	msg->id = 0;
	vsprintf(msg->data, fmt, args);
	if(pdPASS != xQueueSend( logQueue, &msg, 0 ))
	{
		//TODO: error on send
		delete msg;
	}
	//Clean up
	va_end(args);
	
}

QueueHandle_t controlQueue = NULL;

game_obj ufo;

#define NUM_TEST_LAYER_ROWS 4

#define T32(x) (x * 32)
sprite_list_t myTestLayer[NUM_TEST_LAYER_ROWS] = 
{
	{&ufo.bush, {{10,95},{25,100},{120,110},{-1000,-1000}}},
	{&ufo.path[0], {{T32(0),T32(2)},{T32(1),T32(2)},{T32(3),T32(2)},{T32(6),T32(2)},{T32(7),T32(2)},{T32(8),T32(2)},{T32(11),T32(3)},{T32(13),T32(3)},{T32(14),T32(3)},{-1000,-1000}}},
	{&ufo.path[1], {{T32(2),T32(2)},{T32(4),T32(2)},{T32(5),T32(2)},{T32(12),T32(3)},{-1000,-1000}}},
	{&ufo.tree, {{120,20},{10,10},{-1000,-1000}}}
};

static float absFloat(float input)
{
	if(input < 0) input *= -1;
	return input;
}

game_obj::game_obj(void)
{
	theUfo.xPos = 40;
	theUfo.yPos = 40;
	theUfo.xScreen = 40;
	theUfo.yScreen = 40;
	theUfo.xDamping = 0.05;
	theUfo.yDamping = 0.05;
	theUfo.yGravity = 0.05;
	
	theUfo.bitmap = &(game_data.img_ufo_land);
	theUfo.frames[0] = &(game_data.img_ufo_land);
	theUfo.frames[1] = &(game_data.img_ufo_fly);
	theUfo.frames[2] = &(game_data.img_ufo_left);
	theUfo.frames[3] = &(game_data.img_ufo_right);
	
	theUfo.state = UFO_STATE_FLYING;

	beam.bitmap = &(game_data.img_ufo_beam);
	
	bush.bitmap = &(game_data.img_bush);

	tree.bitmap = &(game_data.img_tree);

	path[0].bitmap = &(game_data.img_path);
	path[1].bitmap = &(game_data.img_path_jog);

	star[0].bitmap = &(game_data.img_star_1);
	star[1].bitmap = &(game_data.img_star_2);
	star[2].bitmap = &(game_data.img_star_3);
	star[3].bitmap = &(game_data.img_star_4);

	scenery[0].data_pack = myTestLayer;
	scenery[0].xOffset = 0;
	scenery[0].yOffset = 0;
	
	//Game IO
	memset(buttonStates, 0x00, NUM_BUTTONS);
};

void game_obj::tick(CRTVideo * video)
{
	gameControlInput_t * msg = NULL;
	while(xQueueReceive( controlQueue, &msg, 0 ) == pdPASS)
	{
		buttonStates[msg->button] = msg->state;
		
		switch(msg->button)
		{
			case 0:
			case 1:
			{
				if((buttonStates[0] > 0) ^ (buttonStates[1] > 0))
				{
					if(buttonStates[0] > 0)
					{
						theUfo.yStick(-0.6);
					}
					else
					{
						theUfo.yStick(0.6);
					}
				}
				else
				{
					theUfo.yStick(0);
				}
			}
			break;
			case 2:
			case 3:
			{
				if((buttonStates[2] > 0) ^ (buttonStates[3] > 0))
				{
					if(buttonStates[2] > 0)
					{
						theUfo.xStick(0.6);
					}
					else
					{
						theUfo.xStick(-0.6);
					}
				}
				else
				{
					theUfo.xStick(0);
				}
			}
			break;
			case 4:
			{
				if( buttonStates[4] == 1 )
				{
					//cows.print();
					//beam
					if(theUfo.beamState == true)
					{
						theUfo.beamState = false;
					}
					else if(theUfo.state != UFO_STATE_LANDED)
					{
						theUfo.beamState = true;
					}
					
				}
			}
			break;
			default:
			break;
		}
		delete msg;
	}

/***** Process game mechanics *****/
	//theUfo.xPos++;
	//if(theUfo.xPos >= 192 - 32) theUfo.xPos = 0;
	//theUfo.yPos++;
	//if(theUfo.yPos >= 144 - 32) theUfo.yPos = 0;
	theUfo.tick();
	cows.tick();
	
	scenery[0].xOffset = theUfo.xView;
	scenery[0].yOffset = theUfo.yView;
	cows.xOffset = theUfo.xView;
	cows.yOffset = theUfo.yView;

	if(theUfo.beamState == true)
	{
		Cow * pCow = cows.closestCow(&theUfo);
		//UNUSED(pCow);
		//if(pCow == NULL)
		//{
		//	while(1);
		//}
		ufoBeamHitValues_t rv = cows.checkBeamHit(&theUfo, pCow);
		UNUSED(rv);
		if(pCow != lastDetectCow)
		{
			lastDetectCow = pCow;
			//localPrintf("Detect: 0x%08X\n", pCow);
		}
		if((UFO_HIT_BEAM == rv)||(UFO_HIT_TARGET == rv))
		{
			//localPrintf("%d\n", rv);
			pCow->yPos -= 1;
		}
		else if(UFO_HIT_ABDUCT == rv)
		{
			cows.removeCow(pCow);
		}
	}
	else
	{
		cows.clearTractor();
	}

/***** Draw frame *****/
	uint8_t * buf = NULL;
	if(video->getBlank(&buf, 0xFF))
	{
		scenery[0].draw(video, buf);
		
		if(theUfo.beamState)
		{
			video->drawBitmap(buf, beam.bitmap, theUfo.xScreen - 16, theUfo.yScreen + 10);
		}

		cows.draw(video, buf);

		video->drawBitmap(buf, theUfo.bitmap, theUfo.xScreen, theUfo.yScreen);

		video->swap();
	}
}

void Background::draw(CRTVideo * video, uint8_t * dst)
{
	// Go through data_pack and draw each row
	for(int row = 0; row < NUM_TEST_LAYER_ROWS; row++)
	{
		for(int i = 0; i < 31; i++)
		{
			//Test for terminator
			if((data_pack[row].points[i].x == -1000)&&(data_pack[row].points[i].y == -1000))
			{
				//Set end condition
				i = 31;
			}
			else
			{
				//Calculate position of object in relation to layer
				int16_t xRel = data_pack[row].points[i].x - xOffset;
				int16_t yRel = data_pack[row].points[i].y - yOffset;
				//if( (xRel >= 0) &&
				//	(xRel < (192-32)) &&
				//	(yRel >= 0) &&
				//	(yRel < (144-64))	)
				{
					video->drawBitmap(dst, data_pack[row].sprite->bitmap, xRel, yRel);
				}
			}
		}
	}
}

//   static members
float Cow::yGravity = 0.1;
virtual_bitmap_type_t * Cow::frames[] = {
	&(game_data.img_cow),
	&(game_data.img_cow_eat),
	&(game_data.img_cow_lift)
};
static xy_pair_t cowInitData[NUM_MAX_COWS] =
{
	{ 50, 50 },
	{ -310, 50 },
	{ 200, 50 },
	{ 291, 50 },
	{ 305, 50 },
	{ 489, 50 },
	{ 1000, 50 },
	{ -1101, 50 },
	{ -1150, 50 },
	{ 1500, 50 }
};

Cowlayer::Cowlayer(void)
{
	int i;
	for(i = 0; i < NUM_MAX_COWS - 1; i++)
	{
		cow[i].xPos = cowInitData[i].x;
		cow[i].yPos = cowInitData[i].y;
		cow[i].nextSprite = &cow[i+1];
		cow[i+1].prevSprite = &cow[i];
	}
	cow[i].xPos = cowInitData[i].x;
	cow[i].yPos = cowInitData[i].y;
	cowList = &cow[0];
}

void Cowlayer::tick(void)
{
	Cow * index = (Cow*)cowList;
	while(index != NULL)
	{
		index->tick();
		index = (Cow*)index->nextSprite;
	}
}

void Cowlayer::draw(CRTVideo * video, uint8_t * dst)
{
	Cow * index = (Cow*)cowList;
	while(index != NULL)
	{
		if(index->bitmap != NULL)
		{
			int16_t xCowOffset = index->xPos - xOffset;
			int16_t yCowOffset = index->yPos - yOffset;
			video->drawBitmap(dst, index->bitmap, xCowOffset, yCowOffset);
		}
		
		index = (Cow*)index->nextSprite;
	}

}

void Cowlayer::print(void)
{
	Cow * index = (Cow*)cowList;
	int i = 0;
	while(index != NULL)
	{
		i++;
		localPrintf("Cow[%d] adr: 0x%08X\n", i, index);
		index = (Cow*)index->nextSprite;
	}
}

bool Cowlayer::removeCow(Sprite * pCow)
{
	//if !cowExists return false
	bool cowFound = false;
	Cow * index = (Cow*)cowList;
	while(index != NULL)
	{
		if(index == pCow)
		{
			cowFound = true;
		}
		index = (Cow*)index->nextSprite;
	}
	if(!cowFound)
	{
		return false;
	}
	
	//drop cow
	if(pCow->prevSprite == NULL)
	{
		//First in list
		cowList = pCow->nextSprite;
	}
	else
	{
		pCow->prevSprite->nextSprite = pCow->nextSprite;
	}
	if(pCow->nextSprite == NULL)
	{
		//End of list
	}
	else
	{
		pCow->nextSprite->prevSprite = pCow->prevSprite;
	}
	return true;;
}

Cow * Cowlayer::closestCow(PlayerUfo * ufo)
{
	Cow * index = (Cow*)cowList;

	if(index == NULL)
	{
		return NULL;
		//localPrintf("no cow list\n");
	}
	// Start somewhere!
	Cow * targetCow = index;
	
	while(index != NULL)
	{
		if(absFloat(ufo->xPos - index->xPos) < absFloat(ufo->xPos - targetCow->xPos))
		{
			//if x is closer, cow is closest every time.
			targetCow = index;
			////Target new cow perhaps, check y axis now
			//if(absFloat(ufo->yPos - index->yPos) < absFloat(ufo->yPos - targetCow->yPos))
			//{
			//	//New cow!
			//	targetCow = index;
			//}
		}

		index = (Cow *)(index->nextSprite);
	}
	return targetCow;
}

ufoBeamHitValues_t Cowlayer::checkBeamHit(PlayerUfo * ufo, Cow * cow)
{
	if((ufo == NULL)||(cow == NULL))
	{
		return UFO_HIT_NONE;
	}
	float xDistance = cow->xPos - ufo->xPos;
	float yDistance = cow->yPos - ufo->yPos;
	if(yDistance < 0)
	{
		//Exclude above a line
		cow->inTractorBeam = false;
		return UFO_HIT_NONE;
	}
	if(xDistance > 0)
	{
		//Check right bountary
		if(xDistance > (yDistance/96) * 32)
		{
			//Out of range
			cow->inTractorBeam = false;
			return UFO_HIT_NONE;
		}
	}
	else
	{
		//check left boundary
		if((-1 * xDistance) > (yDistance/96) * 32)
		{
			//Out of range
			cow->inTractorBeam = false;
			return UFO_HIT_NONE;
		}
	}

	//We made it this far so it's in the cone, check bottom levels
	if(cow->yPos < (ufo->yPos + 96))
	{
		if(cow->yPos < (ufo->yPos + 10))
		{
			ufo->beamState = false;
			return UFO_HIT_ABDUCT;
		}
		if(cow->yPos > (ufo->yPos + 64))
		{
			cow->inTractorBeam = true;
			return UFO_HIT_TARGET;
		}
		cow->inTractorBeam = true;
		return UFO_HIT_BEAM;
	}
	cow->inTractorBeam = false;
	return UFO_HIT_NONE;
}

void Cowlayer::clearTractor(void)
{
	Cow * index = (Cow*)cowList;

	if(index == NULL)
	{
		return;
	}
	
	while(index != NULL)
	{
		index->inTractorBeam = false;
		index = (Cow*)index->nextSprite;
	}
}

void PlayerUfo::tick(void)
{
	//Apply thrust
	impulse(xThrust, yThrust);
	//   X
	if(xVelocity == 0)
	{
	}
	else if(xVelocity > 0)
	{
		xVelocity -= xDamping;
		if(xVelocity < 0) xVelocity = 0;
	}
	else if(xVelocity < 0)
	{
		xVelocity += xDamping;
		if(xVelocity > 0) xVelocity = 0;
	}
	//Process velo
	xPos += xVelocity;
	xScreen += xVelocity;
	
	//   Y
	if(yVelocity == 0)
	{
	}
	else if(yVelocity > 0)
	{
		yVelocity -= yDamping;
		if(yVelocity < 0) yVelocity = 0;
	}
	else if(yVelocity < 0)
	{
		yVelocity += yDamping;
		if(yVelocity > 0) yVelocity = 0;
	}
	//Process velo
	yPos += yVelocity;
	yScreen += yVelocity;

	switch(state)
	{
		case UFO_STATE_LANDED:
		{
			bitmap = frames[0];
			if(yVelocity < 0)
			{
				yVelocity += yGravity;
				state = UFO_STATE_FLYING;
			}
			else if(yVelocity > 0.1)
			{
				yVelocity *= -0.2;
			}
			else
			{
				yVelocity = 0;
			}
		}
		break;
		case UFO_STATE_FLYING:
		{
			if(xVelocity > 2)
			{
				bitmap = frames[3];
				//beam
				if(beamState == true)
				{
					beamState = false;
				}
			}
			else if(xVelocity < -2)
			{
				//beam
				if(beamState == true)
				{
					beamState = false;
				}
				bitmap = frames[2];
			}
			else
			{
				bitmap = frames[1];
			}
		}
		break;
		default:
		state = UFO_STATE_FLYING;
		break;
	}
	
	
	//Apply gravity
	if((state == UFO_STATE_FLYING) &&
		(yPos > 80))
	{
		yVelocity += yGravity;
	}
	
	//Ground
	if(yPos > 120)
	{
		state = UFO_STATE_LANDED;
		//beam
		if(beamState == true)
		{
			beamState = false;
		}

	}

	//Bounds
	if(xScreen < 16)
	{
		xScreen = 16;
		xView = xPos - 16;
	}
	else if(xScreen > 145)
	{
		xScreen = 145;
		xView = xPos - 145;
	}
	if(yScreen < 16)
	{
		yScreen = 16;
		yView = yPos - 16;
	}
	else if(yScreen > 100)
	{
		yScreen = 100;
		yView = yPos - 100;
	}
}

void PlayerUfo::impulse(float xMag, float yMag)
{
	xVelocity += xMag;
	yVelocity += yMag;
}

void PlayerUfo::xStick(float in)
{
	if(in > 0)
	{
		if( xVelocity < -2) xVelocity = -2;
	}
	else if(in < 0)
	{
		if( xVelocity > 2) xVelocity = 2;
	}
	xThrust = in;
}

void PlayerUfo::yStick(float in)
{
	//if(yPos < 100)
	//{
	//	if((yVelocity < 0.30)&&(yVelocity > -0.30))
	//	{
	//		state = UFO_STATE_HOVER;
	//	}
	//}
	yThrust = in;
}

void Cow::tick(void)
{
	//If cow in tractor...
	if(inTractorBeam)
	{
		state = COW_STATE_LIFTED;
	}
	else
	{
		state = COW_STATE_STANDING;
	}

	switch(state)
	{
		case COW_STATE_STANDING:
		{
			bitmap = frames[0];
			yVelocity += yGravity;
		}
		break;
		case COW_STATE_EATING:
		{
			bitmap = frames[1];
		}
		break;
		case COW_STATE_LIFTED:
		{
			bitmap = frames[2];
		}
		break;
		default:
		state = COW_STATE_STANDING;
		while(1);
		break;
	}

	//Ground
	if(yPos < 120)
	{
		yPos += yVelocity;
	}
	else
	{
		yVelocity = 0;
	}
}
