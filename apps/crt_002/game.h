#ifndef GAME_H
#define GAME_H

#include "CRTVideo.h"
#include "bitmaps.h"

/***** Graphics Classes *****/
class Sprite
{
public:
	Sprite(void){};
	uint16_t xPos;
	uint16_t yPos;
	virtual_bitmap_type_t * bitmap;
	
	Sprite * prevSprite = 0;
	Sprite * nextSprite = 0;
};

// A sprite list contains a list of xy pairs.  Set as const in table/
typedef struct xy_pair
{
	int16_t x;
	int16_t y;
} xy_pair_t;

typedef struct sprite_list
{
	Sprite * sprite;
	xy_pair_t points[31];
} sprite_list_t;

//each xy is 4 bytes, ptr is 4 bytes so sz = 4 + 4*points = 128, 124/4 = 31 points

// Layer
class Layer
{
public:
	Layer(void){};
	virtual void draw(CRTVideo * video, uint8_t * dst) = 0;

	int16_t xOffset;
	int16_t yOffset;
	uint16_t width;
	uint16_t height;
private:
	//Sprite * spriteLL;// = NULL;
};

class Background : public Layer
{
public:
//	Background(void){};
	void draw(CRTVideo * video, uint8_t * dst);
	sprite_list_t * data_pack = NULL;
private:
};

class graphics_obj : public CRTVideo
{
public:
//	bool drawLayer(uint8_t * dst, Layer * src);
};

class game_obj
{
public:
	game_obj(void);
	void tick(CRTVideo * video);
	//Sprite staticTestImage;
	Sprite theUfo;
	int16_t xScreen;
	int16_t yScreen;
	Sprite tree;
	Sprite bush;
	Sprite path[2];
	
	Sprite star[4];
	
	Background scenery[1];
private:
};

#endif
