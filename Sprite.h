#pragma once
#include "ofMain.h"


class Sprite {
public:
	Sprite(bool, int);				//Sprite [PLAYER/ENEMY?, ID]


	//FUNCTIONS-------------------------------------------------------
	void draw();												
	float age();												//elapsed time after birth
	glm::vec3 move(bool, int);								//custom move function
	glm::vec3 curveEval(float, float, float, float, int);		//override curveEval function


	//check if passed position is within sprite bounds
	bool inside(glm::vec3 pos) {
		if (pos.x >= (currPos.x - width) && pos.x <= (currPos.x + width) &&
			pos.y >= (currPos.y - height) && pos.y <= (currPos.y + height))
			return true;
		else
			return false;
	}


	//VARIABLES/COMPONENTS--------------------------------------------
	ofImage img;							//sprite imbedded image
	bool haveImg = false;					//bool flag if image viable

	ofSoundPlayer snd;						//sprite sound
	bool haveSnd = false;					//bool flag if sound viable

	float birthtime;						//start time of sprite
	float life;								//lifespan
	float speed;							//speed of projectile
	glm::vec3 currPos = { 0, 0, 0 };		//position of sprite
	glm::vec3 velocity = { 0, 0, 0 };		//direction of fire

	float width = 50;						//width/height of replacement rectangle
	float height = 50;

	//ENEMY ATTRIBUTES------------------------------------------------
	int ID;								//emitter ID
	float scale, cycle, yVal;				//wave components

	bool isPlayerSprite;					//check if sprite is player's	
};
