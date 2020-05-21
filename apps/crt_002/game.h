#ifndef GAME_H
#define GAME_H

#include <stdint.h>

#include "CRTVideo.h"
#include "bitmaps.h"

/***** Graphics Classes *****/
class Sprite
{
public:
	Sprite(void){};
	virtual_bitmap_type_t * bitmap = NULL;
	
	Sprite * prevSprite = NULL;
	Sprite * nextSprite = NULL;
};

typedef enum playerUfoStates
{
	UFO_STATE_LANDED,
	UFO_STATE_FLYING,
	UFO_STATE_BEAMING,
	UFO_STATE_HOVER
} playerUfoStates_t;

class PlayerUfo : public Sprite
{
public:
	void tick(void);
	void impulse(float xMag, float yMag);
	void xStick(float in);
	void yStick(float in);
	
	int16_t xView;
	int16_t yView;
	float xPos;
	float yPos;
	float xScreen;
	float yScreen;
	float xVelocity;
	float yVelocity;
	float xDamping;
	float yGravity;
	float xThrust;
	float yThrust;
	virtual_bitmap_type_t * frames[4];
	
	playerUfoStates_t state;
	bool beamState = false;
	
private:
};

typedef enum cowStates
{
	COW_STATE_VOID,
	COW_STATE_STANDING,
	COW_STATE_EATING,
	COW_STATE_LIFTED
} cowStates_t;

class Cow : public Sprite
{
public:
	void tick(void);
	
	float xPos = 0;
	float yPos = 0;
	float xVelocity = 0;
	float yVelocity = 0;
	static float yGravity;

	static virtual_bitmap_type_t * frames[3];
	
	cowStates_t state = COW_STATE_STANDING;
	
private:
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

#define NUM_MAX_COWS 10

class Cowlayer : public Layer
{
public:
	Cowlayer(void);
	void tick(void);
	void draw(CRTVideo * video, uint8_t * dst);

	Cow cow[NUM_MAX_COWS];
	Sprite * cowList;

private:
};

class graphics_obj : public CRTVideo
{
public:
//	bool drawLayer(uint8_t * dst, Layer * src);
};
#define NUM_BUTTONS 10
class game_obj
{
public:
	game_obj(void);
	void tick(CRTVideo * video);
	//Sprite staticTestImage;

	PlayerUfo theUfo;
	Sprite beam;

	Sprite tree;
	Sprite bush;
	Sprite path[2];
	
	Sprite star[4];
	
	Background scenery[1];
	Cowlayer cows;
	
private:

	int8_t buttonStates[NUM_BUTTONS];
};

#endif
