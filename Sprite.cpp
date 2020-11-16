
#include "Sprite.h"

// Basic Sprite Object
//
Sprite::Sprite(bool p, int id) {
	//DEFINE SPRITE-----------------------------------------------
	isPlayerSprite = p;				//T if player, F if enemy
	ID = id;						//ENEMY - assign ID


	velocity = glm::vec3(0, 0, 0);		//default velocity upward
	life = 0;      // lifespan of -1 => immortal 
	birthtime = 0;	//time at creation	
	speed = 0;

	if (p == true) {
		if (img.load("images/Player_Bullet.png"))
			haveImg = true;
		if (snd.load("sounds/missile.mp3"))
			haveSnd = true;
	}
	else {

		//get sprite based on ID
		switch (ID) {
		case 1:
		case 2: 
			if (img.load("images/Minion.png")) haveImg = true;
			break;
		case 3:
		case 4: 
			if (img.load("images/Fire.png")) haveImg = true;
			ofRotateDeg(180);
			break;
		case 7:
			if (img.load("images/Explosion.png")) haveImg = true;
			break;
		default:
			break;
		}

	}

	//else load enemy sprites...

	//default parameters
	if (haveImg) {
		width = img.getWidth();
		height = img.getHeight();
	}
	else {
		width = 20;
		height = 20;
	}
}

// Return a sprite's age in milliseconds
//
float Sprite::age() {
	return (ofGetElapsedTimeMillis() - birthtime);
}

//move sprite 
glm::vec3 Sprite::move(bool b, int p) {
	glm::vec3 temp;

	if (b == true) {
		temp = currPos + velocity / ofGetFrameRate();		//if player, continuous vertical display
	}
	else {
		switch (p) {
		case 1:
			currPos.x += speed;
			temp = curveEval(currPos.x, scale, cycle, yVal, p);		//else, change positions
			break;
		case 2:
			currPos.x -= speed;
			temp = curveEval(currPos.x, scale, cycle, yVal, p);
			break;
		case 3:
			currPos.y += speed;
			temp = glm::vec3(currPos.x, currPos.y, 0);
			break;
		case 4:
			currPos.y += speed * 2;
			temp = glm::vec3(currPos.x, currPos.y, 0);
			break;
		case 7:
			currPos.x += velocity.x;
			currPos.y += velocity.y;
			currPos.z += velocity.z;
			temp = glm::vec3(currPos.x, currPos.y, currPos.z);
			break;
		default:
			break;
		}
	}
		return temp;
}

//--------------------------------------------------------------
// Given x in pixel coordinates, return (x, y, z) on the sin wave
// Note that "z" is not used, so it is set to "0".
//
// Additional Parameters
//    scale - scales the curve in Y  (the amplitude)
//    cycles - number of cycles in sin wave.
//
glm::vec3 Sprite::curveEval(float x, float scale, float cycles, float y, int t) {
	// x is in screen coordinates [0, WindowWidth]
	float u = (cycles * x * PI) / ofGetWidth();
	if (t == 1)				//adjust movement based on "det" value
		return glm::vec3(x, -scale * sin(u) + y, 0);
	else
		return glm::vec3(x, -scale * cos(u) + y, 0);
}


//  Render the sprite
//
void Sprite::draw() {


	// draw image centered and add in translation amount
	//
	if (haveImg)
		img.draw(currPos.x, currPos.y);
	else {
		// in case no image is supplied, draw something
		ofSetColor(255, 0, 0);
		ofDrawRectangle(-width / 2.0 + currPos.x,
			-height / 2.0 + currPos.y, width, height);
	}
}
