#include "Bird.h"
#include "ofImage.h"


Bird::Bird(BirdData& bd, vector <ofImage>& vi, float speed) :
	size(bd.size), velocity(bd.velocity), _images(vi)
{
		velocity *= speed;
		setPosition(bd.position);

		color.r = ofRandom(255.0f);
		color.g = ofRandom(255.0f);
		color.b = 150.0f;
		color.a = 255.0f;

		animationFrameStart = 11;
		birth = ofGetSystemTime();
		glideFrameStart = rand();
}


Bird::~Bird(void)
{
}

const int BodyOffsetX = -138/2 - 30;
void Bird::customDraw()
{
	update();

	ofSetColor(255);
	ofPushStyle();
	ofSetColor(color);
	ofRotateZ(atan2f(velocity.y, velocity.x) * RAD_TO_DEG + 180);

	//int frameIndex = ofGetFrameNum() - animationFrameStart; //fix animation not starting in 0;
	//int animationCounter = (frameIndex + particles[i]->birth) / images.size();
	//ofDrawBitmapString(ofToString(animationCounter), 0, 0, 0);
	//int glidingFactor = 10; //flap wings each X times
	//if ((animationCounter + particles[i]->glideFrameStart) % glidingFactor > 0) frameIndex = particles[i]->animationFrameStart;

	// draw the image sequence at the new frame count
	rw ++;//= (rand() % 4) ? 1 : 0; //random walk
	//int i = ofGetFrameNum();
	
	int i = rw % _images.size();
	ofImage& img = _images[i];
	
	ofPushMatrix();

	float scl = size;

	float age = ofGetSystemTime() - birth;


	float animStart = 0;
	float animMiddle = 500;
	float animEnd = 700;

	if (age < animEnd)
	{
		float f;

		if (age < animMiddle)
		{
			f = ofMap(age, animStart, animMiddle, 0, 1.2);
		}
		else
		{
			f = ofMap(age, animMiddle, animEnd, 1.2, 1);
		}

		scl *= f;
	}

	if (age > 10000)
	{
		scl *= ofMap(age, 10000, 15000, 1, 0);
	}


	if (scl > 0.01)
	{
		ofScale(scl, scl);
		img.draw(0,0);
	}
	ofPopMatrix();

	ofPopStyle();

}


void Bird::update(){

const float SPRING_CONSTANT = 1.0f;
const float MAX_VELOCITY = 30.0f;

	// Calculate time past per frame
	float dt = ofGetLastFrameTime();

	int age = ofGetSystemTime() - birth;
	if (age < 500){
		dt *= (float(age) / 500);
	}
	move(velocity * dt);


	if (ofGetFrameNum() % int(ofGetFrameRate()) == 0)
	{
		turnTarget = ofRandom(-2, 2);
	}

	turn = (0.5 * turn) + 0.5 * turnTarget;
	velocity.rotate(turn, ofVec3f(0,0,1));

//	velocity += -SPRING_CONSTANT * (getPosition() - ofPoint(ofGetWindowWidth()/2, ofGetWindowHeight()/2, 5)) * dt;
//	velocity.limit(MAX_VELOCITY);
}
