#pragma once
#include "ofMain.h"
#include "ofImage.h"

class Bird : public ofNode
{
public:
	
	Bird(ofVec3f velocity, const vector <ofImage>& vi);
	~Bird();

	void setup();
	virtual void customDraw();
	void update();
	
	ofVec3f velocity;
	ofColor color;

	//BirdPart bp;
	//animation
	int animationFrameStart;
	int glideFrameStart;
	int birth;

	int rw;

	const vector <ofImage>& _images;
	
};

