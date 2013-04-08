#include "Bird.h"
#include "ofImage.h"

vector <ofImage> Bird::images;

Bird::Bird(int positionDispersion, int velocityDispersion)
{
	//
		setPosition(ofVec3f(ofRandom(-0.5f, 0.5f), ofRandom(-0.5f, 0.5f), ofRandom(-0.5f, 0.5f)) * positionDispersion); //TODO: make member

		velocity.x = (ofRandom(1.0f) - 0.5f)  * velocityDispersion * 10; //TODO: make member
		velocity.y = (ofRandom(1.0f) - 0.5f)  * velocityDispersion * 10;
		velocity.z = (ofRandom(1.0f) - 0.5f)  * velocityDispersion;

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

void Bird::customDraw()
{
	update();

	ofPushStyle();
	ofSetColor(color);
	setGlobalOrientation(ofQuaternion(atan2f(velocity.y, velocity.x) * RAD_TO_DEG, ofVec3f(0,0,1)));

	for (int i = 0; i < 2; i++)
	{
		wings[i].draw();
	}

	ofSphere(5.0);
	ofDrawArrow(ofVec3f(), ofVec3f(1, 0, 0) * 10, 1);
	//ofDrawArrow(ofPoint(), velocity);


	//int frameIndex = ofGetFrameNum() - animationFrameStart; //fix animation not starting in 0;
	//int animationCounter = (frameIndex + particles[i]->birth) / images.size();
	//ofDrawBitmapString(ofToString(animationCounter), 0, 0, 0);
	//int glidingFactor = 10; //flap wings each X times
	//if ((animationCounter + particles[i]->glideFrameStart) % glidingFactor > 0) frameIndex = particles[i]->animationFrameStart;

	// draw the image sequence at the new frame count
	//ofSetColor(255);

	/*
	ofPushMatrix();
	ofScale(0.1, 0.1);
	images[frameIndex % images.size()].draw(0, 0);
	ofPopMatrix();
	*/


	ofPopStyle();

}


void Bird::update(){

const float SPRING_CONSTANT = 0.1f;
const float MAX_VELOCITY = 30.0f;

	// Calculate time past per frame
	float dt = ofGetLastFrameTime();
	move(velocity * dt);
	velocity += -SPRING_CONSTANT * getPosition() * dt;
	velocity.limit(MAX_VELOCITY);
}


void Bird::setup()
{
	for (int i = 0; i < 2; i++)
	{
		//wings[i].setParent(*this);
	}
	wings[0].setPosition(-2, 5, 0);
	wings[1].setPosition(-2, -5, 0);

}


void Bird::initImages()
{
	ofDirectory dir;
	//int nFiles = dir.listDir("plops");
	int nFiles = dir.listDir("PNG_yellow_new");

	if(nFiles) {        
		for(int i=0; i<dir.numFiles(); i++) { 
			// add the image to the vector
			string filePath = dir.getPath(i);
			images.push_back(ofImage());
			images.back().loadImage(filePath);
		}

		for(int i = 0; i < dir.numFiles(); i++) {
			images[i].setAnchorPercent(0.5f, 0.5f);
			images[i].rotate90(2);
		}
	}
}
