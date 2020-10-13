#include "ofApp.h"



//
// Basic Sprite Object
//


Sprite::Sprite() {
	speed = 0;		//default speed = 0
	velocity = glm::vec3(0, 0, 0);		//default velocity upward
	lifespan = 0;      // lifespan of -1 => immortal 
	birthtime = 0;	//time at creation	
	haveImage = false;
	haveSound = spriteSound.load("sounds/missile.wav");

	//default parameters
	width = 20;
	height = 20;
}

// Return a sprite's age in milliseconds
//
float Sprite::age() {
	return (ofGetElapsedTimeMillis() - birthtime);
}


//  Render the sprite
//
void Sprite::draw() {

	ofSetColor(255, 255, 255, 255);

	// draw image centered and add in translation amount
	//
	if (haveImage) {
		width = image.getWidth();
		height = image.getHeight();

		image.draw(-width / 2.0 + currPos.x, -height / 2.0 + currPos.y);
	}
	else {
		// in case no image is supplied, draw something.
		// 
		//ofSetColor(255, 0, 0);
		ofDrawRectangle(-width / 2.0 + currPos.x, 
			-height / 2.0 + currPos.y, width, height);
	}
}

//  Add a Sprite to the Sprite System
//
void SpriteSystem::add(Sprite s) {
	sprites.push_back(s);
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
		if (s->age() > s->lifespan) {
			tmp = sprites.erase(s);
			s = tmp;
		}
		else s++;
	}

	//  Move sprite
	//
	for (int i = 0; i < sprites.size(); i++) {
		sprites[i].currPos += sprites[i].velocity / ofGetFrameRate();
	}
}

//  Render all the sprites
//
void SpriteSystem::draw() {
	for (int i = 0; i < sprites.size(); i++) {
		sprites[i].draw();
	}
}

//  Create a new Emitter - needs a SpriteSystem
//
Emitter::Emitter(SpriteSystem *spriteSys) {
	sys = spriteSys;	//personal sprite system
	lifespan = 3000;    //projectile lifespan in ms
	currPos = ofVec3f(ofGetWidth() / 2.0, ofGetHeight() / 2.0, 0);		//default center

	lastSpawned = 0;
	rate = 1;    // sprites/sec
	haveImage = image.load("images/space-invaders-ship-scaled.png");	//return t/f based on img existence
	velocity = glm::vec3(0, 0, 0);

	width = 50;		//50x50 rect replacement if necc.
	height = 50;
}

//  Update the Emitter. If it has been started, spawn new sprites with
//  initial velocity, lifespan, birthtime.
//
void Emitter::update() {

	//iterate through SpriteSystem, delete or move existing sprites
	sys->update();

	if (!ofGetKeyPressed(' ')) return;		//exit if space not pressed

	float time = ofGetElapsedTimeMillis();	//current time
	//check elapsed time; wait until spawning next sprite
	//based on rate
	//
	if ((time - lastSpawned) > (1000.0 / rate)) {
		// spawn a new sprite
		Sprite sprite;
		if (sprite.haveSound) {
			sprite.spriteSound.setVolume(0.5f);
			sprite.spriteSound.play();
		}
		sprite.velocity = velocity;
		sprite.lifespan = lifespan;
		sprite.setPosition(ofVec3f(currPos.x, currPos.y + 100, currPos.z));
		sprite.birthtime = time;
		sys->add(sprite);
		lastSpawned = time;
	}
}

//  Draw the Emitter if it is drawable. In many cases you would want a hidden emitter
//
void Emitter::draw() {

	//center the image/rect around position vector
	//check if image is applicable...
	//
	if (haveImage) {
		width = image.getWidth();
		height = image.getHeight();

		image.draw(-width / 2.0 + currPos.x, height / 2.0 + currPos.y);
	}
	else {
		ofSetColor(0, 0, 200);
		ofDrawRectangle(-width / 2 + currPos.x, -height / 2 + currPos.y, width, height);
	}


	// draw sprite system
	//
	sys->draw();
}

// Set rate of fire
void Emitter::setRate(float r) {
	rate = r;
}

void Emitter::setVelocity(ofVec3f v) {
	velocity = v;
}

//--------------------------------------------------------------
void ofApp::setup(){

	//ofSetVerticalSync(true);
	ofSetBackgroundColor(ofColor::black);

	// create an image for sprites being spawned by emitter
	//
	if (defaultImage.load("images/space-invaders-ship-scaled.png")) {
		imageLoaded = true;
	}
	else {
		ofLogFatalError("can't load image: images/space-invaders-ship-scaled.png");
		ofExit();
	}

	
	gui.setup();
	gui.add(rate.setup("Rate of Fire", 1, 1, 10));
	//gui.add(life.setup("life", 5, .1, 10));
	gui.add(velocity.setup("velocity", ofVec3f(0, -100, 0), ofVec3f(-1000, -1000, -1000), ofVec3f(1000, 1000, 1000)));
	//bHide = false;

}

//--------------------------------------------------------------
void ofApp::update() {
	if (play) {
		emit->setRate(rate);
		emit->setVelocity(ofVec3f(velocity->x, velocity->y, velocity->z));
		emit->update();
	}
}


//--------------------------------------------------------------
void ofApp::draw(){

	//check for start prompt
	if (play) {
		emit->draw();
		gui.draw();
	}
	else
		ofDrawBitmapString("Press Space to start", emit->currPos);		//intro
}


//--------------------------------------------------------------

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	emit->currPos = ofVec3f(x, y, 0) - mouse_last;
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	mouse_last = ofVec3f(x, y, 0) - emit->currPos;

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
	
}


//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	if (!play && key == ' ') {	//if not in game state, enable 
		if (defaultSound.load("sounds/start.wav"))
			defaultSound.play();
		play = true;
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

