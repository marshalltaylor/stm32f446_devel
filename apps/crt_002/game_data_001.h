#ifndef GAME_DATA_001_H
#define GAME_DATA_001_H

#include "bitmaps.h"

#ifdef __cplusplus
 extern "C" {
#endif

class GameData
{
public:
	GameData(void);
	
	//Lets brainstorm some ideas
	//Objects
	basic_bitmap_type_t img_cow;
	basic_bitmap_type_t img_cow_eat;
	basic_bitmap_type_t img_cow_lift;
	tall_bitmap_type_t img_tree;
	basic_bitmap_type_t img_bush;
	basic_bitmap_type_t img_ufo_land;
	basic_bitmap_type_t img_ufo_fly;
	basic_bitmap_type_t img_ufo_left;
	basic_bitmap_type_t img_ufo_right;
	large_tall_bitmap_type_t img_ufo_beam;
	basic_bitmap_type_t img_star_1;
	basic_bitmap_type_t img_star_2;
	basic_bitmap_type_t img_star_3;
	basic_bitmap_type_t img_star_4;
	basic_bitmap_type_t img_path;
	basic_bitmap_type_t img_path_jog;
	//maybe these will work
	//child_name_t img_exclaim;
	//other_child_name_t img_fuel;
};

extern GameData game_data;

#ifdef __cplusplus
}

#endif
#endif
