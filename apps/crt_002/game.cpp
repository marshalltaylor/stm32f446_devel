/* Includes -- STD -----------------------------------------------------------*/
#include <stdint.h>
//#include <stdbool.h>
//#include <stdarg.h>
#include <stdio.h>
//#include <string.h>
#include <stdlib.h>

/* Includes -- BSP -----------------------------------------------------------*/

/* Includes -- FreeRTOS system -----------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "event_groups.h"
#include "os.h"

/* Includes -- FreeRTOS app --------------------------------------------------*/

/* Includes -- modules -------------------------------------------------------*/
#include "CRTVideo.h"

#include "game.h"
#include "videoData.h"
#include "game_data_001.h"
#include "bitmaps.h"

/* References ----------------------------------------------------------------*/
#define UNUSED(x) (void)x

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

game_obj::game_obj(void)
{
	theUfo.xPos = 40;
	theUfo.yPos = 40;
	xScreen = 40;
	yScreen = 40;
	
	theUfo.bitmap = &(game_data.img_ufo_fly);

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
}

void game_obj::tick(CRTVideo * video)
{
	gameControlInput_t * msg = NULL;
	while(xQueueReceive( controlQueue, &msg, 0 ) == pdPASS)
	{
		//print output
		switch( msg->button )
		{
			case 1:
			theUfo.yPos -= 5;
			yScreen -= 5;
			break;
			case 2:
			theUfo.yPos += 5;
			yScreen += 5;
			break;
			case 3:
			theUfo.xPos += 5;
			xScreen += 5;
			break;
			case 4:
			theUfo.xPos -= 5;
			xScreen -= 5;
			break;
			default:
			{
				//localPrintf("msg button: 0x%X, state: %s\n", msg->button, msg->state);
				break;
			}
		}
		if(xScreen < 30)
		{
			xScreen = 30;
			scenery[0].xOffset = theUfo.xPos - 30;
		}
		else if(xScreen > 160)
		{
			xScreen = 160;
			scenery[0].xOffset = theUfo.xPos - 160;
		}
		if(yScreen < 30)
		{
			yScreen = 30;
			scenery[0].yOffset = theUfo.yPos - 30;
		}
		else if(yScreen > 100)
		{
			yScreen = 100;
			scenery[0].yOffset = theUfo.yPos - 100;
		}
		delete msg;
	}

/***** Process game mechanics *****/
	//theUfo.xPos++;
	//if(theUfo.xPos >= 192 - 32) theUfo.xPos = 0;
	//theUfo.yPos++;
	//if(theUfo.yPos >= 144 - 32) theUfo.yPos = 0;
	
	
/***** Draw frame *****/
	uint8_t * buf = NULL;
	if(video->getBlank(&buf, 0xFF))
	{
		//video->console(buf);
		//
		//video->line(buf, 94, 6, 94+17, 72, 0xA0);
		//video->line(buf, 94+17, 72, 94, 138, 0xA0);
		//video->line(buf, 94, 138, 94-17, 72, 0xA0);
		//video->line(buf, 94-17, 72, 94, 6, 0xA0);
		//
		//video->line(buf, 94, 10, 94+15, 72, 0xFF);
		//video->line(buf, 94+15, 72, 94, 134, 0xFF);
		//video->line(buf, 94, 134, 94-15, 72, 0xFF);
		//video->line(buf, 94-15, 72, 94, 10, 0xFF);

		//layer_t myLayer;
		//UNUSED(myLayer);
		//myLayer.xOffset = 0;
		//myLayer.yOffset = 0;
		//myLayer.width = 192;
		//myLayer.height = 144;
		//
		//theUfo.draw(buf, &myLayer);
		
		scenery[0].draw(video, buf);
		
		video->drawBitmap(buf, theUfo.bitmap, xScreen, yScreen);
		//video->drawBitmap(buf, theUfo.bitmap, theUfo.xPos, theUfo.yPos);
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
