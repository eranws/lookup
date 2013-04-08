#pragma once
#include "ofNode.h"

class Bird : public ofNode
{
public:
	
	Bird(int positionDispersion, int velocityDispersion);
	~Bird();
	
	ofVec3f velocity;
	ofColor color;

	//animation
	int animationFrameStart;
	int glideFrameStart;
	int birth;
};

