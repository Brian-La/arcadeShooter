
#include "SpriteSystem.h"

typedef enum { DirectionalEmitter, RadialEmitter } EmitterType;			//emitter type

//Emitter that shoots sprites (BASE for PLAYER/ENEMIES)
class Emitter {
public:
	Emitter(SpriteSystem *);			//default = player
	Emitter(SpriteSystem *, int);		//float ID denotes different types of enemies (mvmt)


	//FUNCTIONS-------------------------------------------------------
	void update();			
	void draw();
	void setRate(float r) { rate = r; }							//rate of fire
	void setLifeSpan(float life) { lifespan = life; }			//lifespan of missile
	void setSpeed(float s) { speed = s; }						//speed of missile
	void spawn(float time);										//spawn a sprite

	void setForce(ofVec3f f) { force.set(f); };					//set force			
	void physics(ofVec3f *, ofVec3f *, ofVec3f *);				//physics application

	void setOneShot(bool t) { oneShot = t; }					//set one shot
	void setGroupSize(int g) { groupSize = g; }					//set group size of explosion
	void start();			//start signal; started to true, get last spawned
	void stop();			//stop signal; started to false, fired to false


	//VARIABLES-------------------------------------------------------
	SpriteSystem *sys;					//individualized sprite system
	EmitterType type;					//type of emitter

	ofVec3f currPos = { 0, 0, 0 };		//position of emitter (matrix)
	float rate = 5;						//rate of emission
	float speed = 5;					//missile speed
	float lifespan = 5;						//lifespan of missile
	float lastSpawned = 0;				//save spawn time of previous missile launch
	glm::vec3 velocity = ofVec3f(0, 0, 0);		//direction of fire
	
	ofImage image;						//imbedded image
	bool haveImage = false;				//bool switch for image presence
	bool moveImage = false;				//bool switch for image movement (PLAYER)

	float width = 50;					//initialized width/height for replacement rre
	float height = 50;	

	//PHYSICS---------------------------------------------------------
	ofVec3f veloc = ofVec3f(0, 0, 0);       //velocity vecs
	ofVec3f accel = ofVec3f(0, 0, 0);        //acceleration vec
	ofVec3f force = ofVec3f(0, 0, 0);        //force
	float fVal = 50;


	float damp = 0.99;		//damping value (drag)
	float mass = 0.5;		//mass

	//EXPLOSIONS------------------------------------------------------
	bool started;			//start bool -> start sprite emission
	bool fired;				//fired bool ->	sprite spawned
	bool oneShot;			//oneShot bool ->	single spawn
	int groupSize;			//number of sprites


	//ENEMY ATTRIBUTES------------------------------------------------
	int ID = 0;							//enemy ID (default 0 = PLAYER)
	float scale, cycle;					//scale/cycle of wave movement
	//float xVal = 0;						
	float yVal = 0;						//Y component for emitter

	bool isPlayer;						//distinguishes between PLAYER & ENEMY
	bool hidden;				//hidden emitter
};
