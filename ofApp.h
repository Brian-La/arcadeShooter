#pragma once

#include "ofMain.h"
#include "ofxGui.h"

#include "Emitter.h"

class ofApp : public ofBaseApp {

	public:
		bool collision(Emitter *a, Emitter *b, Emitter *c);
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);



		//GAME STATE---------------------------------------------------------------------------
		bool inGame = false;				//bool start switch
		bool gameOver = false;				//game over
		int playScore;					//score tracker (default 10)

		ofImage bg;							//background image
		ofVec2f bgPos = ofVec2f(0.f, 0.f);		//background image pos (topleft)
		bool bgLoaded = false;				//bool check for background image file
		ofSoundPlayer defaultSound;			//begin sound
		bool sndLoaded = false;				//bool check for sound file

		ofSoundPlayer bgMusic;				//background music
		bool bgSndLoaded = false;

		ofSoundPlayer collide;
		bool collideLoaded;	

		glm::vec3 mouse_last;				//saves last mouse position
		bool playerMove = false;			//check to see if player moves (drag)

		bool bHide = true;					//hide GUI


		//SLIDERS & BUTTONS---------------------------------------------------------------------
		ofxFloatSlider scale;		//scale of sin wave
		ofxFloatSlider cycles;		//cycles of sin wave
		ofxFloatSlider rate;
		ofxFloatSlider life;
		ofxVec3Slider velocity;
		ofxFloatSlider speedMov;
		ofxFloatSlider yAdjust;		//adjust y placement of enemy emitter spawn
		ofxFloatSlider yAdjust2;

		ofxPanel gui;

		//EMITTERS------------------------------------------------------------------------------
		Emitter *player = new Emitter(new SpriteSystem);			//player emitter
		Emitter *enemy = new Emitter(new SpriteSystem, 1);			//enemy
		Emitter *enemy2 = new Emitter(new SpriteSystem, 2);			//enemy 2
		Emitter *enemy3 = new Emitter(new SpriteSystem, 3);			//enemy 3
		Emitter *enemy4 = new Emitter(new SpriteSystem, 4);			//enemy 4
		
		Emitter *explode = new Emitter(new SpriteSystem, 7);
};