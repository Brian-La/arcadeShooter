#pragma once

#include "ofMain.h"
#include "ofxGui.h"

//	typedef enum { MoveStop, MoveLeft, MoveRight, MoveUp, MoveDown } MoveDir;	//unused enumeration

//Base object
//
class BaseObject {
public:
	BaseObject() {};		//constructor

	glm::vec3 currPos = ofVec3f(0, 0, 0);		//current position: centered
	//float rot;			//rotational val
	//glm::vec3 scale = glm::vec3(1, 1, 1);
};

//  General Sprite class  (similar to a Particle)
//
class Sprite : public BaseObject {
public:
	Sprite(bool, float);		//constructor


	//functions-------------------------------------------------------
	void draw();		//draw override
	float age();		//elapsed time after birth
	glm::vec3 move(bool, float);		//custom move function
	glm::vec3 curveEval(float, float, float, float, float);		//curveEval func adjusted
	bool inside(glm::vec3 pos) {
		if (pos.x >= (currPos.x - width) && pos.x <= (currPos.x + width) &&
			pos.y >= (currPos.y - height) && pos.y <= (currPos.y + height))
			return true;
		else
			return false;
	}



	//variables-------------------------------------------------------
	ofImage img;		//imbedded image
	ofSoundPlayer snd;		//sound for missile
	float birthtime; //birth time
	float life;  //length of span
	bool haveImg = false;		//bool flag if image is available
	bool haveSnd = false;		//bool flag for sound availability
	bool playerSprite;		//check player sprite
	float width, height;		//width/height of rect
	glm::vec3 velocity;		//velocity vec (magnitude and direction)


	float speed;			//speed of projectile
	float scale, cycle, yVal;		//scale, cycle, and yVal storage
	float ID;			//ID for emitter
};

//  Manages all Sprites in a system.  You can create multiple systems
//
class SpriteSystem  {
public:
	void add(Sprite);		//add sprite to system
	void remove(int i);
	void update();		//update system
	void draw();			//run system
	vector<Sprite> sprites;		//vector storing sprites - sprite system
};


//  General purpose Emitter class for emitting sprites
//  This works similar to a Particle emitter
//
class Emitter: public BaseObject {
public:
	Emitter(SpriteSystem *);		//default = player
	Emitter(SpriteSystem *, float);		//parameterized constructor bool = enemy

	
	//functions-------------------------------------------------------
	void update();			//override update
	void draw();
	void setRate(float);
	void setLifeSpan(float);
	void setSpeed(float);
	//void setVelocity(ofVec3f);


	//variables-------------------------------------------------------
	SpriteSystem *sys;		//system instance
	float rate;			//float rate
	float speed;		//speed of sprites
	glm::vec3 velocity;			//veloc
	float lifespan;			//lifespan of entire emitter/system	
	float lastSpawned;		//last projectile spawned
	ofImage image;		//imbedded image
	bool haveImage = false;		//bool switch for rect or image
	float width, height;		//width/height for rect

	float scale, cycle;			//scale cycle default
	float yVal = 0;
	float ID = 0;

	//check if player-------------------------------------------------
	bool isPlayer;			//check if player
};


class ofApp : public ofBaseApp{

	public:
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
		
		Emitter *player = new Emitter(new SpriteSystem);			//player emitter
		Emitter *enemy = new Emitter(new SpriteSystem, 1);			//enemy
		Emitter *enemy2 = new Emitter(new SpriteSystem, 2);
		int playScore = 0;					//tally score of player

		ofImage defaultImage;
		ofSoundPlayer defaultSound;		//begin sound
		glm::vec3 mouse_last;		//last mouse position
		bool imageLoaded = false;			//image check

		//bool bHide;
		bool play = false;			//bool start switch

		//sliders & buttons------------------------------------------
		ofxFloatSlider scale;		//scale of sin wave
		ofxFloatSlider cycles;		//cycles of sin wave
		ofxFloatSlider rate;
		ofxFloatSlider life;
		ofxVec3Slider velocity;
		//ofxToggle mouseOn;		//temporarily disable keystrokes
		ofxFloatSlider speedMov;
		ofxFloatSlider yAdjust;		//adjust y placement of enemy emitter spawn
		ofxFloatSlider yAdjust2;
		//ofxLabel screenSize;


		ofxPanel gui;
};