#pragma once
#include "ofMain.h"
#include "ofImage.h"

class Wing : public ofNode
{
	virtual void customDraw()
	{
		ofSphere(1.0);
	}
};

class Bird : public ofNode
{
public:
	
	Bird(int positionDispersion, int velocityDispersion);
	~Bird();
	
	static void initImages();

	void setup();
	virtual void customDraw();
	ofVec3f velocity;
	ofColor color;

	Wing wings[2];
	//animation
	int animationFrameStart;
	int glideFrameStart;
	int birth;

	static vector <ofImage> images;

};

