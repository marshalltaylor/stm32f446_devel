#ifndef GAME_H
#define GAME_H

#include "CRTVideo.h"
#include "bitmaps.h"

class game_obj
{
public:
	game_obj(void);
	void tick(CRTVideo * video);
private:
	Sprite staticTestImage;
};

#endif
