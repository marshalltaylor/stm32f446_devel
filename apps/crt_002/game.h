#ifndef GAME_H
#define GAME_H

#include "CRTVideo.h"
#include "bitmaps.h"

/***** Base Classes *****/
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

// Layer
class Layer
{
public:
	Layer(void){};
	virtual void draw(uint8_t * dst) = 0;

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
	void draw(uint8_t * dst);
private:
	Sprite * tree;
	Sprite * ground[2];
};

class graphics_obj : public CRTVideo
{
public:
	bool drawLayer(uint8_t * dst, Layer * src);
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
	//Sprite staticTestImage;
	Sprite theUfo;
};

#endif
