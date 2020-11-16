#include "Emitter.h"

//  Create a new Emitter - equipped w/ SpriteSystem (PLAYER)
Emitter::Emitter(SpriteSystem *spriteSys) {
	//PLAYER ATTRIBUTES-----------------------------------------
	isPlayer = true;				//player ID
	type = DirectionalEmitter;		//set type to directional
	currPos = ofVec3f(ofGetWidth() / 2.0, ofGetHeight() / 1.5, 0);		//set position
	velocity = glm::vec3(0, -750, 0);									//always upwards

	hidden = false;

	//----------------------------------------------------------
	started = false;
	oneShot = false;
	fired = false;
	sys = spriteSys;		
	lifespan = 2000;    
	scale = 0;
	cycle = 0;
}

//Create a new Emitter - equipped w/ SpriteSystem (ENEMY, IDs)
//
Emitter::Emitter(SpriteSystem *spriteSys, int id) {
	isPlayer = false;				//SET TO FALSE TO DENOTE ENEMY
	cycle = 4;
	scale = 200;
	ID = id;

	sys = spriteSys;	//personal sprite system
	speed = 4;		//default speed
	lifespan = 5;    //projectile lifespan in ms

	hidden = true;

	oneShot = false;
	fired = false;

	//determine type of emitter based on ID
	switch (ID) {
	case 1:
		currPos = ofVec3f(0, ofGetHeight() / 2.0, 0);
		type = DirectionalEmitter;
		started = true;
		break;
	case 2:
		currPos = ofVec3f(ofGetWidth(), ofGetHeight() / 2.0, 0);
		type = DirectionalEmitter;
		started = true;
		break;
	case 3:
		currPos = ofVec3f(ofGetWidth() / 1.5, 0, 0);
		type = DirectionalEmitter;
		started = true;
		break;
	case 4:
		currPos = ofVec3f(ofGetWidth() / 4.0, 0, 0);
		type = DirectionalEmitter;
		started = true;
		break;
	case 7:
		started = false;
		type = RadialEmitter;
		break;
	default:
		break;
	}
}


void Emitter::physics (ofVec3f *pos, ofVec3f *vel, ofVec3f *accel){
	//compute new position based on velocity and time step
	*pos += *vel * (1 / ofGetFrameRate());		//position = position + velocity * dt

	//compute acceleration from forces on particle using a = (1/m) * f
	//add to existing particle acceleration
	*accel = (1 / mass) * force;

	//compute new velocity based on total acceleration and time step
		//Velocity = velocity + acceleration * dt
	*vel += *accel * (1 / ofGetFrameRate());

	//damp velocity
		//Velocity = velocity * damping
	*vel *= damp;
}

void Emitter::start() {
	started = true;
	lastSpawned = ofGetElapsedTimeMillis();
}

void Emitter::stop() {
	started = false;
	fired = false;
}

// spawn a single particle.  time is current time of birth
void Emitter::spawn(float time) {

	Sprite sprite(isPlayer, ID);		//sprite instance

	if (sprite.haveSnd) {				//check for sound, play upon birth
		sprite.snd.setVolume(0.3f);
		sprite.snd.play();
	}

	// set initial velocity and position
	// based on emitter type
	//
	switch (type) {
	case RadialEmitter: {
		ofVec3f dir = ofVec3f(ofRandom(-1, 1), ofRandom(-1, 1), ofRandom(-1, 1));
		velocity = ofVec3f(1000, 0, 0);
		sprite.velocity = dir.getNormalized() * velocity.length();
		sprite.currPos = currPos;
		break;
	}
	case DirectionalEmitter:
		sprite.velocity = velocity;
		sprite.currPos = currPos;
		sprite.speed = speed;
		sprite.scale = scale;
		sprite.cycle = cycle;
		sprite.yVal = yVal;
		break;
	}

	// other particle attributes
	//
	sprite.life = lifespan;
	sprite.birthtime = time;

	// add to system
	//
	sys->add(sprite);
}



//  Update the Emitter. If it has been started, spawn new sprites with
//  initial velocity, lifespan, birthtime.
//
void Emitter::update() {

	//iterate through SpriteSystem, delete or move existing sprites
	sys->update();


	if (isPlayer) {
		haveImage = (moveImage) ? image.load("images/Player_Move.png") : image.load("images/Player.png");		//load image based on movement
		physics(&currPos, &veloc, &accel);		//activate physics
		if (!ofGetKeyPressed(' ')) return;		//exit if space not pressed
	}

	//check elapsed time; wait until spawning next sprite based on rate
	float time = ofGetElapsedTimeMillis();

	//explosions (radial emitter)----------------------------------------------
	if (oneShot && started) {
		if (!fired) {
			// spawn a new particle(s)
			for (int i = 0; i < groupSize; i++)
				spawn(time);

			lastSpawned = time;		//acquire new time spawned
		}
		fired = true;
		stop();
	}

	//regular emitters
	else if ((time - lastSpawned) > (1000.0 / rate) && started) {
		
		spawn(time);

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

	if(haveImage) {
		width = image.getWidth();
		height = image.getHeight();
		//bound player movement
		if (currPos.y >= ofGetWindowHeight() - 200)
			currPos.y = ofGetWindowHeight() - 200;

		if(!hidden)
			image.draw(currPos.x, currPos.y);
	}
	else {
		if(!hidden)
			ofDrawRectangle(-width / 2.0 + currPos.x, -height / 2.0 + currPos.y, width, height);
	}
}

