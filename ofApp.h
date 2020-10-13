#pragma once

#include "ofMain.h"
#include "ofxGui.h"

typedef enum { MoveStop, MoveLeft, MoveRight, MoveUp, MoveDown } MoveDir;

//Base object
//
class BaseObject {
public:
	BaseObject() {};	//constructor
	glm::vec3 currPos;		//current position: centered
	void setPosition(glm::vec3 position) {
		currPos = position;			//renew position
	}
};

//  General Sprite class  (similar to a Particle)
//
class Sprite : public BaseObject {
public:
	Sprite();		//constructor


	//functions-------------------------------------------------------
	void draw();		//draw override
	float age();		//elapsed time after birth
	float speed;		//speed of pixel mvmt
	glm::vec3 velocity;		//velocity vec (magnitude and direction)

	//variables-------------------------------------------------------
	ofImage image;		//imbedded image
	ofSoundPlayer spriteSound;		//sound for missile
	float birthtime; //birth time
	float lifespan;  //length of span
	bool haveImage;		//bool flag if image is available
	bool haveSound;		//bool flag for sound availability
	float width, height;		//width/height of rect
};

//  Manages all Sprites in a system.  You can create multiple systems
//
class SpriteSystem  {
public:
	void add(Sprite);		//add sprite to system
	void update();		//update system
	void draw();			//run system
	vector<Sprite> sprites;		//vector storing sprites - sprite system

};


//  General purpose Emitter class for emitting sprites
//  This works similar to a Particle emitter
//
class Emitter: public BaseObject {
public:
	Emitter(SpriteSystem *);		//constructor attached with sprite system

	
	//functions-------------------------------------------------------
	void update();			//override update
	void draw();
	void setRate(float);
	void setVelocity(ofVec3f);


	//variables-------------------------------------------------------
	SpriteSystem *sys;		//system instance
	float rate;			//float rate
	glm::vec3 velocity;			//veloc
	float lifespan;			//lifespan of entire emitter/system	
	float lastSpawned;		//last projectile spawned
	ofImage image;		//imbedded image
	bool haveImage;		//bool switch for rect or image
	float width, height;		//width/height for rect
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
		
		Emitter *emit = new Emitter(new SpriteSystem);

		ofImage defaultImage;
		ofSoundPlayer defaultSound;		//begin sound
		glm::vec3 mouse_last;		//last mouse position
		bool imageLoaded;		//image check

		bool bHide;
		bool play = false;			//bool start switch

		ofxFloatSlider rate;
		ofxFloatSlider life;
		ofxVec3Slider velocity;
		ofxLabel screenSize;


		ofxPanel gui;
};
