#pragma once
#include "ofMain.h"
#include "ofImage.h"

#include "..\Sound3d.h"
#include "..\BirdEvents.h"

class Bird : public ofNode
{
public:
	
	Bird(BirdData& bd, vector <ofImage>& vi);
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

	vector <ofImage>& _images;
	float size;
};

