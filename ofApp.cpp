#include "ofApp.h"


//check for collisions between two emitters
bool ofApp::collision(Emitter *a, Emitter *b, Emitter *c) {
	bool temp = false;			//local bool


	//parse through sprites 
	for (int i = 0; i < b->sys->sprites.size(); i++) {
		for (int j = 0; j < a->sys->sprites.size(); j++) {
			if (b->sys->sprites[i].inside(a->sys->sprites[j].currPos)) {
				c->currPos = b->sys->sprites[i].currPos;
				c->setGroupSize(10);
				c->setOneShot(true);
				c->start();
				a->sys->sprites[j].life = 0;			//if within boundaries (collision), reset lifespan of sprites to 0 for cache deletion
				b->sys->sprites[i].life = 0;
				//playScore++;			//update player 
				//cout << "collision detected" << endl;		//prompt
				temp = true;			//set to true
			}
		}

		if (b->sys->sprites[i].inside(a->currPos)) {
			b->sys->sprites[i].life = 0;
			playScore-=3;			//lose life upon collision
		}

	}	//end for loop

	if (temp)
		if (collideLoaded) {
			collide.play();
			collide.setVolume(0.3f);
		}
	return temp;		//return bool val
}

//--------------------------------------------------------------
void ofApp::setup(){

	ofSetVerticalSync(true);						//vertical sync
	ofSetFrameRate(60);								//set frame rate to 60
	ofSetBackgroundColor(ofColor::black);			//load screen default: black

	bgLoaded = bg.load("images/BG.png");			//load background image; set bool value
	bgSndLoaded = bgMusic.load("sounds/bgmusic.mp3");	//load background sound
	collideLoaded = collide.load("sounds/missile2.mp3");		//collision sound

	gui.setup();
	gui.add(yAdjust.setup("LEFT Y", 200, 0, ofGetHeight()));
	gui.add(yAdjust2.setup("RIGHT Y", 200, 0, ofGetHeight()));
	gui.add(speedMov.setup("Speed", 4, 1, 10));
	gui.add(scale.setup("Scale", 200, 1, 400));			
	gui.add(cycles.setup("Cycles", 4, 1, 10));			//1 cycle
	gui.add(rate.setup("Rate of Fire", 3, 1, 10));
	gui.add(life.setup("life", 5, .5, 10));

}

//--------------------------------------------------------------
void ofApp::update() {
	if (inGame) {
		//BACKGROUND ANIMATION----------------------------------
		bgPos.y += 100 * ofGetLastFrameTime();		//100 pixels per sec
		if (bgPos.y >= ofGetHeight()) bgPos.y = 0;


		//ENEMY PARAMETERS VIA GUI------------------------------
		enemy->setRate(rate);
		enemy->setLifeSpan(life * 1000);
		enemy->setSpeed(speedMov);
		enemy->yVal = yAdjust;
		enemy->scale = scale;
		enemy->cycle = cycles;

		enemy2->setRate(rate);
		enemy2->setLifeSpan(life * 1000);
		enemy2->setSpeed(speedMov);
		enemy2->yVal = yAdjust2;
		enemy2->scale = scale;
		enemy2->cycle = cycles;

		enemy3->setRate(rate);
		enemy3->setLifeSpan(life * 1000);
		enemy3->setSpeed(speedMov);

		enemy4->setRate(rate);
		enemy4->setLifeSpan(life * 1500);
		enemy4->setSpeed(speedMov);

		explode->setRate(rate);
		explode->setLifeSpan(life * 500);
		explode->setSpeed(speedMov);

		enemy->update();
		enemy2->update();
		enemy3->update();
		enemy4->update();

		//attach movement to sprite
		player->moveImage = playerMove;			
		player->update();

		if (collision(player, enemy, explode)) {
			playScore++;			//increment
		}
		if (collision(player, enemy2, explode)) {
			playScore += 2;			
		}
		if (collision(player, enemy3, explode)) {
			playScore++;			
		}
		if (collision(player, enemy4, explode)) {
			playScore++;			
		}

		explode->update();

		if (playScore < 0) {
			inGame = false;
			gameOver = true;
		}

	}
}


void ofApp::draw(){

	//if in play state...
	if (inGame) {
		//DRAW BACKGROUND-----------------------------------------------------
		bg.draw(bgPos.x, bgPos.y, ofGetWidth(), ofGetHeight());
		bg.draw(bgPos.x, bgPos.y - ofGetHeight(), ofGetWidth(), ofGetHeight());		//continuous animation

		//--------------------------------------------------------------------
		ofDrawBitmapStringHighlight("SCORE = " + std::to_string(playScore),
			ofVec3f(ofGetWidth() / 1.5, 100, 0));			//display score
		player->draw();				//draw player
		enemy->draw();		//draw enemy
		enemy2->draw();
		enemy3->draw();
		enemy4->draw();
		explode->draw();
		gui.draw();			//draw GUI
		
	}
	else {
		if(!gameOver)
			ofDrawBitmapString("Press Space to Start", ofVec2f(ofGetWidth() / 2.0, ofGetHeight() / 2.0));		//center intro
		else {
			ofSetBackgroundColor(ofColor::black);
			ofDrawBitmapString("GAME OVER", ofVec2f(ofGetWidth() / 2.0, ofGetHeight() / 2.0));		//center intro
		}
	}

}


//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	player->currPos = ofVec3f(x, y, 0) - mouse_last;		//parallel player displacement
	playerMove = true;
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	mouse_last = ofVec3f(x, y, 0) - player->currPos;		//save mouse position

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	playerMove = false;
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

void ofApp::keyPressed(int key) {	

		//assign new force values depending on which direction moved
		switch (key) {
		case OF_KEY_LEFT:   // move left
			player->force = ofVec3f(-player->fVal, 0, 0);
			playerMove = true;
			break;
		case OF_KEY_RIGHT:  // move right
			player->force = ofVec3f(player->fVal, 0, 0);
			playerMove = true;
			break;
		case OF_KEY_UP:     // move forward
			player->force = ofVec3f(0, -player->fVal, 0);
			playerMove = true;
			break;
		case OF_KEY_DOWN:   // go backward
			player->force = ofVec3f(0, player->fVal, 0);
			playerMove = true;
			break;
		}
	
}


//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

	//enable game state
	if (!inGame && key == ' ') {	 
		if (defaultSound.load("sounds/start.wav"))		//play start sound
			defaultSound.play();
		ofSetBackgroundColor(ofColor::white);		//default BG settings
		inGame = true;			//set play state
		player->start();
		playScore = 10;			//set play score

		//start music
		if (bgSndLoaded) {
			bgMusic.setLoop(true);
			bgMusic.play();
			bgMusic.setVolume(0.2f);
		}
		

	}

	//reset all forces to 0 once key is released
	switch (key) {
	case OF_KEY_LEFT:  
	case OF_KEY_RIGHT: 
	case OF_KEY_UP:
	case OF_KEY_DOWN:
		player->force = ofVec3f(0, 0, 0);
		playerMove = false;
		break;
	default:
		break;
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

