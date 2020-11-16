#pragma once

#include "Sprite.h"

//  Manages all Sprites in a system.  You can create multiple systems
//
class SpriteSystem {
public:
	void add(Sprite);		//add sprite to system
	void remove(int i);
	void update();		//update system
	void draw();			//run system
	vector<Sprite> sprites;		//vector storing sprites - sprite system
}; 
