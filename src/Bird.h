#pragma once
#include "ofMain.h"
#include "ofImage.h"

class Wing : public ofNode
{
	virtual void customDraw();
	ofImage _image;
	ofPoint _imgOffset;
	float _rotationFreq;
	float _rotationMag;

public:
	void setImage( const ofImage& image );
	void setImageOffset( const ofPoint offset);
	void setRotationFactor(float freq ,float mag);
};

class Bird : public ofNode
{
public:
	
	Bird(int positionDispersion, int velocityDispersion);
	~Bird();
	
	static void initImages();

	void setup();
	virtual void customDraw();
	void update();
	
	ofVec3f velocity;
	ofColor color;

	Wing wings[2];
	Wing tail;
	//animation
	int animationFrameStart;
	int glideFrameStart;
	int birth;

	static vector <ofImage> images;

};

