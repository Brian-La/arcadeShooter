#include "ofApp.h"



//
// Basic Sprite Object
//
Sprite::Sprite(bool p, float id) {
	velocity = glm::vec3(0, 0, 0);		//default velocity upward
	life = 0;      // lifespan of -1 => immortal 
	birthtime = 0;	//time at creation	
	speed = 0;
	playerSprite = p;		//set to an emitter
	ID = id;
	
	if (p == true) {
		if (img.load("images/Beta_Missile.png"))
			haveImg = true;
		if (snd.load("sounds/missile.mp3"))
			haveSnd = true;
	}
	else {
		if (img.load("images/Beta_Missile_2.png"))
			haveImg = true;
		if (snd.load("sounds/missile2.mp3"))
			haveSnd = true;
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

//Return an adjusted currPos within SpriteSystem update
glm::vec3 Sprite::move(bool b, float p) {
	if (b == true) {
		return currPos + velocity / ofGetFrameRate();		//if player, continuous vertical display
	}
	else {
		if (p == 1) {
			currPos.x += speed;
			return curveEval(currPos.x, scale, cycle, yVal, p);		//else, change positions
		}
		else {
			currPos.x -= speed;
			return curveEval(currPos.x, scale, cycle, yVal, p);
		}
	}
}

//--------------------------------------------------------------
// Given x in pixel coordinates, return (x, y, z) on the sin wave
// Note that "z" is not used, so it is set to "0".
//
// Additional Parameters
//    scale - scales the curve in Y  (the amplitude)
//    cycles - number of cycles in sin wave.
//
glm::vec3 Sprite::curveEval(float x, float scale, float cycles, float y, float det)
{
	// x is in screen coordinates [0, WindowWidth]
	float u = (cycles * x * PI) / ofGetWidth();
	if (det == 1)				//adjust movement based on "det" value
		return (glm::vec3(x, -scale * sin(u) + y, 0));
	else
		return(glm::vec3(x, -scale * cos(u) + y, 0));
}


//  Render the sprite
//
void Sprite::draw() {

	ofSetColor(255, 0, 0, 255);		//lower depth

	// draw image centered and add in translation amount
	//
	if (haveImg)
		img.draw(currPos.x, currPos.y);
	else 
		// in case no image is supplied, draw something.
		// 
		ofDrawRectangle(-width / 2.0 + currPos.x, 
			-height / 2.0 + currPos.y, width, height);
}

//  Add a Sprite to the Sprite System
//
void SpriteSystem::add(Sprite s) {
	sprites.push_back(s);
}


// Remove a sprite from the sprite system. Note that this function is not currently
// used. The typical case is that sprites automatically get removed when the reach
// their lifespan.
//
void SpriteSystem::remove(int i) {
	sprites.erase(sprites.begin() + i);
}


//  Update the SpriteSystem by checking which sprites have exceeded their
//  lifespan (and deleting).  Also the sprite is moved to it's next
//  location based on velocity and direction.
//
void SpriteSystem::update() {

	if (sprites.size() == 0) return;		//exit if no sprites
	vector<Sprite>::iterator s = sprites.begin();	//create iterator
	vector<Sprite>::iterator tmp;		//temporary iterator

	// check which sprites have exceed their lifespan and delete
	// from list.  When deleting multiple objects from a vector while
	// traversing at the same time, use an iterator.
	//
	while (s != sprites.end()) {
		if (s->age() > s->life) {
			tmp = sprites.erase(s);
			s = tmp;
		}
		else s++;	
	}

	//  Move sprite
	//
	for (int i = 0; i < sprites.size(); i++) {
		sprites[i].currPos = sprites[i].move(sprites[i].playerSprite, sprites[i].ID);		//adjust currPos based on emitter
	}
}

//  Render all the sprites
//
void SpriteSystem::draw() {
	for (int i = 0; i < sprites.size(); i++) {
		sprites[i].draw();
	}
}

//  Create a new Emitter - equipped w/ SpriteSystem (PLAYER)
//
Emitter::Emitter(SpriteSystem *spriteSys) {
	isPlayer = true;
	//------------------------------------------
	sys = spriteSys;	//personal sprite system
	speed = 5;		//default speed
	lifespan = 2000;    //projectile lifespan in ms
	currPos = ofVec3f(ofGetWidth() / 2.0, ofGetHeight() / 2.0, 0);		//default center
	scale = 0;
	cycle = 0;
	
	lastSpawned = 0;
	rate = 3;    // sprites/sec
	velocity = glm::vec3(0, -500, 0);			//default projectiles

	if (image.load("images/Beta_Ship.png"))
		haveImage = true;	//return t/f based on img existence

	if (haveImage) {
		width = image.getWidth();
		height = image.getHeight();
	}
	else {
		width = 50;		//50x50 rect replacement if necc.
		height = 50;
	}
}

//Create a new Emitter - equipped w/ SpriteSystem (ENEMY)
//
Emitter::Emitter(SpriteSystem *spriteSys, float id) {
	isPlayer = false;				//SET TO FALSE TO DENOTE ENEMY
	cycle = 4;
	scale = 200;
	ID = id;
	//-----------------------------------------------------------

	sys = spriteSys;	//personal sprite system
	speed = 4;		//default speed
	lifespan = 5;    //projectile lifespan in ms
	if (ID == 1)
		currPos = ofVec3f(0, 300, 0);		//default position
	else
		currPos = ofVec3f(ofGetWidth(), 300, 0);

	lastSpawned = 0;
	rate = 3;    // sprites/sec
	velocity = glm::vec3(0, 0, 0);			//default projectiles

	//haveImage = image.load("images/Beta_Ship.png");	//return t/f based on img existence
	if (haveImage) {
		width = image.getWidth();
		height = image.getHeight();
	}
	else {
		width = 50;		//50x50 rect replacement if necc.
		height = 50;
	}
}



//  Update the Emitter. If it has been started, spawn new sprites with
//  initial velocity, lifespan, birthtime.
//
void Emitter::update() {

	//iterate through SpriteSystem, delete or move existing sprites
	sys->update();

	if(isPlayer)
		if (!ofGetKeyPressed(' ')) return;		//exit if space not pressed

	//current time-------------------
	//check elapsed time; wait until spawning next sprite based on rate
	float time = ofGetElapsedTimeMillis();

	if ((time - lastSpawned) > (1000.0 / rate)) {
		// spawn a new sprite
		Sprite sprite(isPlayer, ID);

		if (sprite.haveSnd) {
			sprite.snd.setVolume(0.3f);
			sprite.snd.play();
		}
		sprite.velocity = velocity;
		sprite.life = lifespan;
		sprite.currPos = currPos;
		sprite.yVal = yVal;
		sprite.birthtime = time;
		sprite.scale = scale;
		sprite.cycle = cycle;
		sprite.speed = speed;
		sys->add(sprite);
		lastSpawned = time;
	}

}

//  Draw the Emitter if it is drawable. In many cases you would want a hidden emitter
//
void Emitter::draw() {
	// draw sprite system
	//
	sys->draw();

	//center the image/rect around position vector
	//check if image is applicable...
	//
	if (haveImage) {
		width = image.getWidth();
		height = image.getHeight();

		//bound player movement
		if (currPos.y >= ofGetWindowHeight() - 200)
			currPos.y = ofGetWindowHeight() - 200;
		
		image.draw(currPos.x, currPos.y);
	}
	else {
		ofDrawRectangle(-width / 2.0 + currPos.x, -height / 2.0 + currPos.y, width, height);
	}
}

// Set rate of fire
void Emitter::setRate(float r) {
	rate = r;
}

//set duration of missile travel
void Emitter::setLifeSpan(float hp) {
	lifespan = hp;
}


void Emitter::setSpeed(float s) {
	speed = s;
}

/*
void Emitter::setVelocity(ofVec3f v) {
	velocity = v;
}
*/



//--------------------------------------------------------------
void ofApp::setup(){

	//ofSetVerticalSync(true);
	ofSetBackgroundColor(ofColor::black);
	
	gui.setup();
	//gui.add(mouseOn.setup("Mouse", true));		//set to use mouse first
	gui.add(yAdjust.setup("LEFT Y", 200, 0, ofGetHeight()));
	gui.add(yAdjust2.setup("RIGHT Y", 200, 0, ofGetHeight()));
	gui.add(speedMov.setup("Speed", 4, 1, 10));
	gui.add(scale.setup("Scale", 200, 1, 400));
	gui.add(cycles.setup("Cycles", 4, 1, 10));
	gui.add(rate.setup("Rate of Fire", 3, 1, 10));
	gui.add(life.setup("life", 5, .5, 10));
	//gui.add(velocity.setup("velocity", ofVec3f(0, -500, 0), ofVec3f(-1000, -1000, -1000), ofVec3f(1000, 1000, 1000)));

}

//--------------------------------------------------------------
void ofApp::update() {
	if (play) {

		//ENEMY PARAMETERS VIA GUI
		enemy->setRate(rate);
		enemy->setLifeSpan(life * 500);
		enemy->setSpeed(speedMov);
		enemy->currPos.y = yAdjust;
		enemy->yVal = yAdjust;
		enemy->scale = scale;
		enemy->cycle = cycles;

		enemy2->setRate(rate);
		enemy2->setLifeSpan(life * 500);
		enemy2->setSpeed(speedMov);
		enemy2->currPos.y = yAdjust2;
		enemy2->yVal = yAdjust2;
		enemy2->scale = scale;
		enemy2->cycle = cycles;
		

		enemy->update();
		enemy2->update();
		//player->setVelocity(ofVec3f(velocity->x, velocity->y, velocity->z));
		player->update();

		for (int i = 0; i < player->sys->sprites.size(); i++) {
			for (int j = 0; j < enemy->sys->sprites.size(); j++) {
				if (player->sys->sprites[i].inside(enemy->sys->sprites[j].currPos)) {
					player->sys->sprites[i].life = 0;			//if within boundaries, reset lifespan of sprites to 0 for deletion
					enemy->sys->sprites[j].life = 0;
					playScore++;			//update player 
					cout << "collision detected" << endl;
				}
			}
		}	//end for loop


	}
}


//--------------------------------------------------------------
void ofApp::draw(){

	//if in play state...
	if (play) {

		ofDrawBitmapStringHighlight("SCORE = " + std::to_string(playScore), ofVec3f(ofGetWidth() / 1.5, 100, 0));
		player->draw();		//draw player
		enemy->draw();		//draw enemy
		enemy2->draw();
		gui.draw();			//draw GUI
		
		ofSetColor(ofColor::red);
		//draw lines to represent boundaries
		ofDrawLine(0, ofGetWindowHeight() - 80, ofGetWidth(), ofGetWindowHeight() - 80);
	}
	else
		ofDrawBitmapString("Press Space to Start", player->currPos);		//intro prompt otherwise

}


//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	//if(mouseOn)
	player->currPos = ofVec3f(x, y, 0) - mouse_last;		//parallel player displacement
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	//if(mouseOn)
	mouse_last = ofVec3f(x, y, 0) - player->currPos;		//save mouse position

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

void ofApp::keyPressed(int key) {	
	/*if (!mouseOn) {
		switch (key) {
		case OF_KEY_UP:
			emit->currPos.y -= emit->speed;
			break;
		case OF_KEY_DOWN:
			emit->currPos.y += emit->speed;
			break;
		case OF_KEY_RIGHT:
			emit->currPos.x += emit->speed;
			break;
		case OF_KEY_LEFT:
			emit->currPos.x -= emit->speed;
			break;
		default:
			break;
		}
	} */
	
}


//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	if (!play && key == ' ') {	//if not in game state, enable 
		if (defaultSound.load("sounds/start.wav"))		//play start sound
			defaultSound.play();
		ofSetBackgroundColor(ofColor::white);		//default BG settings
		play = true;			//set play state

	}
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

