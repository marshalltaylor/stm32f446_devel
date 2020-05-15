/* Includes -- STD -----------------------------------------------------------*/
#include <stdint.h>
//#include <stdbool.h>
//#include <stdarg.h>
#include <stdio.h>
//#include <string.h>
#include <stdlib.h>

/* Includes -- BSP -----------------------------------------------------------*/

/* Includes -- FreeRTOS system -----------------------------------------------*/

/* Includes -- FreeRTOS app --------------------------------------------------*/

/* Includes -- modules -------------------------------------------------------*/
#include "CRTVideo.h"

#include "game.h"
#include "videoData.h"
#include "bitmaps.h"

/* References ----------------------------------------------------------------*/
#define UNUSED(x) (void)x

game_obj ufo;

game_obj::game_obj(void)
{
	theUfo.xPos = 40;
	theUfo.yPos = 40;
	theUfo.width = 32;
	theUfo.height = 32;
	theUfo.srcFile = &game_data;//testImage;
	theUfo.index = 19;
	theUfo.prevSprite = NULL;
	theUfo.nextSprite = NULL;
}

void game_obj::tick(CRTVideo * video)
{
/***** Process game mechanics *****/
	theUfo.xPos++;
	if(theUfo.xPos >= 192 - 32) theUfo.xPos = 0;
	theUfo.yPos++;
	if(theUfo.yPos >= 144 - 32) theUfo.yPos = 0;
	
/***** Draw frame *****/
	uint8_t * buf = NULL;
	if(video->getBlank(&buf))
	{
		video->console(buf);

		video->line(buf, 94, 6, 94+17, 72, 0xA0);
		video->line(buf, 94+17, 72, 94, 138, 0xA0);
		video->line(buf, 94, 138, 94-17, 72, 0xA0);
		video->line(buf, 94-17, 72, 94, 6, 0xA0);

		video->line(buf, 94, 10, 94+15, 72, 0xFF);
		video->line(buf, 94+15, 72, 94, 134, 0xFF);
		video->line(buf, 94, 134, 94-15, 72, 0xFF);
		video->line(buf, 94-15, 72, 94, 10, 0xFF);

		layer_t myLayer;
		UNUSED(myLayer);
		myLayer.xOffset = 0;
		myLayer.yOffset = 0;
		myLayer.width = 192;
		myLayer.height = 144;
		
		//theUfo.draw(buf, &myLayer);
		video->drawSprite(buf, &theUfo, theUfo.xPos, theUfo.yPos);
		video->swap();
	}
}

bool graphics_obj::drawLayer(uint8_t * dst, layer_t * src)
{
	Sprite * pSprite = src->spriteLL;
	while(pSprite != NULL)
	{
		drawSprite(dst, pSprite, 0, 0);
		pSprite = pSprite->nextSprite;
	}
	return true;
}

//void Player::draw(uint8_t * dst, layer_t * layerInfo)
//{
//	//Scan all sprite pixels and draw into destination
//	for(int iY = 0; iY < height; iY++)
//	{
//		for(int iX = 0; iX < width; iX++)
//		{
//			dst[((iY + y) * PIXEL_WIDTH) + iX + x] = 
//					((srcFile->data[((iY + yPos) * srcFile->width) + iX + xPos])
//					 >> 2) + ASCII_BLACK_LEVEL;
//		}
//	}
//}
