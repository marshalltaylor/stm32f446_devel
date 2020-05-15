#ifndef GAME_H
#define GAME_H

#include "CRTVideo.h"
#include "bitmaps.h"

// Layer
typedef struct layer
{
	int16_t xOffset;
	int16_t yOffset;
	uint16_t width;
	uint16_t height;
	Sprite * spriteLL;// = NULL;
} layer_t;

class graphics_obj : public CRTVideo
{
public:
	bool drawLayer(uint8_t * dst, layer_t * src);	
};

//class Player : Sprite
//{
//public:
//	void draw(uint8_t * dst, layer_t * layerInfo);
//private:
//	uint8_t index[4] = {18, 19, 20, 21}; // Could be array of indicies?
//};

class game_obj
{
public:
	game_obj(void);
	void tick(CRTVideo * video);
private:
	Sprite staticTestImage;
	Sprite theUfo;
};

#endif
