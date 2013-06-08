#include "Bird.h"
#include "ofImage.h"


Bird::Bird(ofVec3f velocity, vector <ofImage>& vi) :
	velocity(velocity), _images(vi)
{

		color.r = ofRandom(255.0f);
		color.g = ofRandom(255.0f);
		color.b = 150.0f;
		color.a = 255.0f;

		animationFrameStart = 11;
		birth = rand();
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
	ofSphere(5.0);
	ofDrawArrow(ofVec3f(), ofVec3f(1, 0, 0) * 10, 1);

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

	img.draw(0,0);

	ofPopStyle();

}


void Bird::update(){

const float SPRING_CONSTANT = 0.1f;
const float MAX_VELOCITY = 30.0f;

	// Calculate time past per frame
	float dt = ofGetLastFrameTime();
	move(velocity * dt);
//	velocity += -SPRING_CONSTANT * getPosition() * dt;
//	velocity.limit(MAX_VELOCITY);
}

void Bird::setup()
{

}
