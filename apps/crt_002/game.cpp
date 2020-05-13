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
game_obj ufo;

game_obj::game_obj(void)
{
}

void game_obj::tick(CRTVideo * video)
{
/***** Process game mechanics *****/
	
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

		video->swap();
	}
}

