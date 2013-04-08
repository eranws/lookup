#pragma once
#include "ofNode.h"

class Wing : public ofNode
{
	virtual void customDraw()
	{
		ofSphere(getPosition(), 1.0);
	}
};

class Bird : public ofNode
{
public:
	
	Bird(int positionDispersion, int velocityDispersion);
	~Bird();
	
	void setup();
	virtual void customDraw();
	ofVec3f velocity;
	ofColor color;

	Wing wings[2];
	//animation
	int animationFrameStart;
	int glideFrameStart;
	int birth;
};

